/*
 * Copyright (C) 2026 by Evgenii Grigorev <tothe8c@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/*
 * Asynchronous DNS-based (3GPP TS 29.303 S-NAPTR) SGW/PGW selection.
 *
 * All functions here run on the MME main thread: queries are kicked from
 * FSM handlers, c-ares sockets are polled by ogs_app()->pollset and query
 * timeouts by ogs_app()->timer_mgr, both pumped in mme_main(). Completion
 * is still marshalled through the event queue (MME_EVENT_DNS_RESOLVED) to
 * keep FSM discipline.
 */

#include <ares.h>

#include "mme-dns.h"
#include "mme-dns-select.h"
#include "mme-event.h"
#include "mme-gtp-path.h"
#include "metrics.h"

/* DNS wire constants (avoid a resolv.h dependency) */
#define MME_DNS_C_IN        1
#define MME_DNS_T_A         1
#define MME_DNS_T_SRV       33
#define MME_DNS_T_NAPTR     35

#define MME_DNS_MAX_RR      8
#define MME_DNS_MAX_CSR_ATTEMPTS 3
#define MME_DNS_NEGATIVE_TTL     5  /* seconds, capped by dns.cache_ttl */

static int negative_ttl(void)
{
    if (mme_self()->dns.cache_ttl < MME_DNS_NEGATIVE_TTL)
        return mme_self()->dns.cache_ttl;
    return MME_DNS_NEGATIVE_TTL;
}

typedef enum {
    MME_DNS_ST_PENDING = 0,
    MME_DNS_ST_DONE,        /* event posted, not yet consumed */
    MME_DNS_ST_CONSUMED,    /* result applied, CSR sent */
} mme_dns_state_e;

typedef struct mme_dns_leg_s {
    bool active;
    mme_dns_service_e svc;
    char fqdn[MME_DNS_MAX_FQDN_LEN+1];      /* NAPTR query name */
    bool naptr_fetched;
    int num_cand;
    mme_dns_candidate_t cand[MME_DNS_MAX_CANDIDATES];
    int cursor;
    bool query_pending;
    /* set when the entry now in cache was fetched by our own query,
     * so the immediate cache read is not counted as a cache hit */
    bool just_answered;
    bool done;
    bool fallback;
} mme_dns_leg_t;

typedef struct mme_dns_resolution_s {
    ogs_lnode_t lnode;
    ogs_pool_id_t id;

    ogs_pool_id_t sess_id;
    ogs_pool_id_t enb_ue_id;
    int create_action;

    mme_dns_state_e state;
    ogs_timer_t *t_guard;

    mme_dns_proto_e proto;
    mme_dns_leg_t sgw;
    mme_dns_leg_t pgw;

    int csr_attempts;
} mme_dns_resolution_t;

typedef struct mme_dns_query_ctx_s {
    ogs_pool_id_t res_id;
    uint8_t leg_idx;                        /* 0 = sgw, 1 = pgw */
    int qtype;
    char qname[MME_DNS_MAX_FQDN_LEN+1];
} mme_dns_query_ctx_t;

typedef struct mme_dns_cache_entry_s {
    char key[MME_DNS_MAX_FQDN_LEN+4];
    ogs_time_t expire;
    int num;                                /* 0 = negative entry */
    union {
        mme_dns_naptr_record_t naptr[MME_DNS_MAX_RR];
        mme_dns_srv_record_t srv[MME_DNS_MAX_RR];
        struct {
            struct in_addr addr;
            uint16_t port;                  /* unused, alignment only */
        } a[MME_DNS_MAX_RR];
    } u;
} mme_dns_cache_entry_t;

static OGS_POOL(mme_dns_resolution_pool, mme_dns_resolution_t);

static bool g_enabled = false;
static bool g_closing = false;
static ares_channel g_channel = NULL;
static bool g_channel_up = false;
static ogs_timer_t *g_t_process = NULL;
static ogs_hash_t *g_cache = NULL;

static struct {
    ares_socket_t fd;
    ogs_poll_t *rpoll;
    ogs_poll_t *wpoll;
} g_fdmap[ARES_GETSOCK_MAXNUM * 2];

static void leg_advance(mme_dns_resolution_t *res, mme_dns_leg_t *leg);
static void check_completion(mme_dns_resolution_t *res);

/*****************************************************************
 * c-ares <-> ogs_pollset / ogs_timer glue
 *****************************************************************/

static void dns_rearm_process_timer(void)
{
    struct timeval tv, *tvp = NULL;

    if (!g_channel_up || g_closing)
        return;

    tvp = ares_timeout(g_channel, NULL, &tv);
    if (tvp) {
        ogs_time_t duration =
            (ogs_time_t)tvp->tv_sec * OGS_USEC_PER_SEC + tvp->tv_usec;
        if (duration < ogs_time_from_msec(1))
            duration = ogs_time_from_msec(1);
        ogs_timer_start(g_t_process, duration);
    } else {
        ogs_timer_stop(g_t_process);
    }
}

static void dns_poll_cb(short when, ogs_socket_t fd, void *data)
{
    ares_process_fd(g_channel,
            (when & OGS_POLLIN) ? fd : ARES_SOCKET_BAD,
            (when & OGS_POLLOUT) ? fd : ARES_SOCKET_BAD);
    dns_rearm_process_timer();
}

static void dns_process_timer_cb(void *data)
{
    ares_process_fd(g_channel, ARES_SOCKET_BAD, ARES_SOCKET_BAD);
    dns_rearm_process_timer();
}

static void dns_sock_state_cb(
        void *data, ares_socket_t socket_fd, int readable, int writable)
{
    int i, free_slot = -1;

    for (i = 0; i < (int)OGS_ARRAY_SIZE(g_fdmap); i++) {
        if (g_fdmap[i].rpoll || g_fdmap[i].wpoll) {
            if (g_fdmap[i].fd == socket_fd)
                break;
        } else if (free_slot < 0) {
            free_slot = i;
        }
    }
    if (i == (int)OGS_ARRAY_SIZE(g_fdmap)) {
        if (!readable && !writable)
            return;
        if (free_slot < 0) {
            ogs_error("mme-dns: no free fd slot for c-ares socket %d",
                    (int)socket_fd);
            return;
        }
        i = free_slot;
        g_fdmap[i].fd = socket_fd;
    }

    if (readable && !g_fdmap[i].rpoll) {
        g_fdmap[i].rpoll = ogs_pollset_add(ogs_app()->pollset,
                OGS_POLLIN, socket_fd, dns_poll_cb, NULL);
        ogs_assert(g_fdmap[i].rpoll);
    } else if (!readable && g_fdmap[i].rpoll) {
        ogs_pollset_remove(g_fdmap[i].rpoll);
        g_fdmap[i].rpoll = NULL;
    }

    if (writable && !g_fdmap[i].wpoll) {
        g_fdmap[i].wpoll = ogs_pollset_add(ogs_app()->pollset,
                OGS_POLLOUT, socket_fd, dns_poll_cb, NULL);
        ogs_assert(g_fdmap[i].wpoll);
    } else if (!writable && g_fdmap[i].wpoll) {
        ogs_pollset_remove(g_fdmap[i].wpoll);
        g_fdmap[i].wpoll = NULL;
    }
}

/*****************************************************************
 * Cache
 *****************************************************************/

static mme_dns_cache_entry_t *cache_get(int qtype, const char *fqdn)
{
    char key[sizeof(((mme_dns_cache_entry_t *)0)->key)];
    mme_dns_cache_entry_t *entry = NULL;

    ogs_snprintf(key, sizeof(key), "%d:%s", qtype, fqdn);
    entry = ogs_hash_get(g_cache, key, OGS_HASH_KEY_STRING);
    if (!entry)
        return NULL;

    if (entry->expire < ogs_time_now()) {
        ogs_hash_set(g_cache, entry->key, OGS_HASH_KEY_STRING, NULL);
        ogs_free(entry);
        return NULL;
    }

    return entry;
}

static mme_dns_cache_entry_t *cache_put(
        int qtype, const char *fqdn, int ttl_sec)
{
    mme_dns_cache_entry_t *entry = NULL, *old = NULL;

    entry = ogs_calloc(1, sizeof(*entry));
    ogs_assert(entry);
    ogs_snprintf(entry->key, sizeof(entry->key), "%d:%s", qtype, fqdn);
    entry->expire = ogs_time_now() + ogs_time_from_sec(ttl_sec);

    old = ogs_hash_get(g_cache, entry->key, OGS_HASH_KEY_STRING);
    if (old) {
        ogs_hash_set(g_cache, old->key, OGS_HASH_KEY_STRING, NULL);
        ogs_free(old);
    }
    ogs_hash_set(g_cache, entry->key, OGS_HASH_KEY_STRING, entry);

    return entry;
}

static void cache_flush(void)
{
    ogs_hash_index_t *hi = NULL;

    if (!g_cache)
        return;

    for (hi = ogs_hash_first(g_cache); hi; hi = ogs_hash_next(hi)) {
        mme_dns_cache_entry_t *entry = ogs_hash_this_val(hi);
        ogs_hash_set(g_cache, entry->key, OGS_HASH_KEY_STRING, NULL);
        ogs_free(entry);
    }
}

/*****************************************************************
 * Resolution lifecycle
 *****************************************************************/

static mme_dns_resolution_t *resolution_find_by_sess(mme_sess_t *sess)
{
    if (!sess || sess->dns_id == OGS_INVALID_POOL_ID)
        return NULL;
    return ogs_pool_find_by_id(&mme_dns_resolution_pool, sess->dns_id);
}

static void resolution_free(mme_dns_resolution_t *res)
{
    ogs_assert(res);
    if (res->t_guard)
        ogs_timer_delete(res->t_guard);
    ogs_pool_id_free(&mme_dns_resolution_pool, res);
}

void mme_dns_sess_clear(mme_sess_t *sess)
{
    mme_dns_resolution_t *res = NULL;

    ogs_assert(sess);
    res = resolution_find_by_sess(sess);
    if (res)
        resolution_free(res);
    sess->dns_id = OGS_INVALID_POOL_ID;
}

static void post_resolved(mme_dns_resolution_t *res)
{
    int rv;
    mme_event_t *e = NULL;

    ogs_assert(res);
    ogs_assert(res->state == MME_DNS_ST_PENDING);

    res->state = MME_DNS_ST_DONE;
    if (res->t_guard)
        ogs_timer_stop(res->t_guard);

    if (g_closing)
        return;

    e = mme_event_new(MME_EVENT_DNS_RESOLVED);
    ogs_assert(e);
    e->sess_id = res->sess_id;
    e->enb_ue_id = res->enb_ue_id;
    e->create_action = res->create_action;

    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_error("mme-dns: ogs_queue_push() failed:%d", (int)rv);
        mme_event_free(e);
        return;
    }
    ogs_pollset_notify(ogs_app()->pollset);
}

static void check_completion(mme_dns_resolution_t *res)
{
    if (res->state != MME_DNS_ST_PENDING)
        return;
    if (res->sgw.active && !res->sgw.done)
        return;
    if (res->pgw.active && !res->pgw.done)
        return;
    post_resolved(res);
}

static void leg_fail(mme_dns_resolution_t *res, mme_dns_leg_t *leg,
        const char *reason)
{
    ogs_warn("mme-dns: %s selection via [%s] failed (%s), "
            "falling back to static configuration",
            leg->svc == MME_DNS_SVC_SGW ? "SGW" : "PGW",
            leg->fqdn, reason);
    leg->done = true;
    leg->fallback = true;
    mme_metrics_inst_global_inc(MME_METR_GLOB_CTR_DNS_QUERY_FAILED);
}

static void guard_expire_cb(void *data)
{
    ogs_pool_id_t res_id = OGS_POINTER_TO_UINT(data);
    mme_dns_resolution_t *res = NULL;

    res = ogs_pool_find_by_id(&mme_dns_resolution_pool, res_id);
    if (!res || res->state != MME_DNS_ST_PENDING)
        return;

    if (res->sgw.active && !res->sgw.done)
        leg_fail(res, &res->sgw, "timeout");
    if (res->pgw.active && !res->pgw.done)
        leg_fail(res, &res->pgw, "timeout");

    check_completion(res);
}

/*****************************************************************
 * Query issue & callbacks
 *****************************************************************/

static mme_dns_leg_t *leg_ptr(mme_dns_resolution_t *res, uint8_t leg_idx)
{
    return leg_idx == 0 ? &res->sgw : &res->pgw;
}

static void dns_query_cb(
        void *arg, int status, int timeouts, unsigned char *abuf, int alen);

static void issue_query(mme_dns_resolution_t *res, mme_dns_leg_t *leg,
        int qtype, const char *qname)
{
    mme_dns_query_ctx_t *ctx = NULL;

    ctx = ogs_calloc(1, sizeof(*ctx));
    ogs_assert(ctx);
    ctx->res_id = res->id;
    ctx->leg_idx = (leg == &res->sgw) ? 0 : 1;
    ctx->qtype = qtype;
    ogs_cpystrn(ctx->qname, qname, sizeof(ctx->qname));

    leg->query_pending = true;
    mme_metrics_inst_global_inc(MME_METR_GLOB_CTR_DNS_QUERY_TOTAL);

    /* NOTE: on immediate failure c-ares invokes the callback synchronously
     * from inside ares_query(); dns_query_cb tolerates that (it re-enters
     * leg_advance which sees the failure already recorded). */
    ares_query(g_channel, qname, MME_DNS_C_IN, qtype, dns_query_cb, ctx);
    dns_rearm_process_timer();
}

static void handle_naptr_answer(mme_dns_query_ctx_t *ctx,
        int status, unsigned char *abuf, int alen)
{
    struct ares_naptr_reply *reply = NULL, *r = NULL;
    mme_dns_cache_entry_t *entry = NULL;
    int num = 0;

    if (status == ARES_SUCCESS &&
        ares_parse_naptr_reply(abuf, alen, &reply) == ARES_SUCCESS) {
        entry = cache_put(MME_DNS_T_NAPTR, ctx->qname,
                mme_self()->dns.cache_ttl);
        for (r = reply; r && num < MME_DNS_MAX_RR; r = r->next) {
            mme_dns_naptr_record_t *rec = &entry->u.naptr[num];
            rec->order = r->order;
            rec->preference = r->preference;
            ogs_cpystrn(rec->flags,
                    r->flags ? (const char *)r->flags : "",
                    sizeof(rec->flags));
            ogs_cpystrn(rec->service,
                    r->service ? (const char *)r->service : "",
                    sizeof(rec->service));
            ogs_cpystrn(rec->replacement,
                    r->replacement ? r->replacement : "",
                    sizeof(rec->replacement));
            num++;
        }
        entry->num = num;
        ares_free_data(reply);
    } else {
        entry = cache_put(MME_DNS_T_NAPTR, ctx->qname, negative_ttl());
        entry->num = 0;
    }
}

static void handle_srv_answer(mme_dns_query_ctx_t *ctx,
        int status, unsigned char *abuf, int alen)
{
    struct ares_srv_reply *reply = NULL, *r = NULL;
    mme_dns_cache_entry_t *entry = NULL;
    int num = 0;

    if (status == ARES_SUCCESS &&
        ares_parse_srv_reply(abuf, alen, &reply) == ARES_SUCCESS) {
        entry = cache_put(MME_DNS_T_SRV, ctx->qname,
                mme_self()->dns.cache_ttl);
        for (r = reply; r && num < MME_DNS_MAX_RR; r = r->next) {
            mme_dns_srv_record_t *rec = &entry->u.srv[num];
            rec->priority = r->priority;
            rec->weight = r->weight;
            rec->port = r->port;
            ogs_cpystrn(rec->target,
                    r->host ? r->host : "", sizeof(rec->target));
            num++;
        }
        entry->num = num;
        ares_free_data(reply);
    } else {
        entry = cache_put(MME_DNS_T_SRV, ctx->qname, negative_ttl());
        entry->num = 0;
    }
}

static void handle_a_answer(mme_dns_query_ctx_t *ctx,
        int status, unsigned char *abuf, int alen)
{
    struct hostent *host = NULL;
    struct ares_addrttl addrttls[MME_DNS_MAX_RR];
    int naddrttls = MME_DNS_MAX_RR;
    mme_dns_cache_entry_t *entry = NULL;
    int i, num = 0, ttl = -1;

    if (status == ARES_SUCCESS &&
        ares_parse_a_reply(abuf, alen, &host, addrttls, &naddrttls) ==
            ARES_SUCCESS) {
        for (i = 0; i < naddrttls; i++) {
            if (ttl < 0 || addrttls[i].ttl < ttl)
                ttl = addrttls[i].ttl;
        }
        if (ttl <= 0)
            ttl = mme_self()->dns.cache_ttl;
        entry = cache_put(MME_DNS_T_A, ctx->qname, ttl);
        for (i = 0; i < naddrttls && num < MME_DNS_MAX_RR; i++) {
            entry->u.a[num].addr = addrttls[i].ipaddr;
            num++;
        }
        entry->num = num;
        if (host)
            ares_free_hostent(host);
    } else {
        entry = cache_put(MME_DNS_T_A, ctx->qname, negative_ttl());
        entry->num = 0;
    }
}

static void dns_query_cb(
        void *arg, int status, int timeouts, unsigned char *abuf, int alen)
{
    mme_dns_query_ctx_t *ctx = arg;
    mme_dns_resolution_t *res = NULL;
    mme_dns_leg_t *leg = NULL;

    ogs_assert(ctx);

    if (status == ARES_EDESTRUCTION || g_closing) {
        ogs_free(ctx);
        return;
    }

    /* Populate the cache regardless of resolution liveness */
    switch (ctx->qtype) {
    case MME_DNS_T_NAPTR:
        handle_naptr_answer(ctx, status, abuf, alen);
        break;
    case MME_DNS_T_SRV:
        handle_srv_answer(ctx, status, abuf, alen);
        break;
    case MME_DNS_T_A:
        handle_a_answer(ctx, status, abuf, alen);
        break;
    default:
        ogs_assert_if_reached();
    }

    res = ogs_pool_find_by_id(&mme_dns_resolution_pool, ctx->res_id);
    if (res && res->state == MME_DNS_ST_PENDING) {
        leg = leg_ptr(res, ctx->leg_idx);
        leg->query_pending = false;
        leg->just_answered = true;
        leg_advance(res, leg);
        check_completion(res);
    }

    ogs_free(ctx);
}

/*****************************************************************
 * Per-leg resolution state machine (cache-driven)
 *****************************************************************/

static void count_cache_hit(mme_dns_leg_t *leg)
{
    if (leg->just_answered) {
        leg->just_answered = false;
        return;
    }
    mme_metrics_inst_global_inc(MME_METR_GLOB_CTR_DNS_CACHE_HIT);
}

static void leg_advance(mme_dns_resolution_t *res, mme_dns_leg_t *leg)
{
    mme_dns_cache_entry_t *entry = NULL;

    while (leg->active && !leg->done && !leg->query_pending) {
        if (!leg->naptr_fetched) {
            entry = cache_get(MME_DNS_T_NAPTR, leg->fqdn);
            if (!entry) {
                issue_query(res, leg, MME_DNS_T_NAPTR, leg->fqdn);
                return;
            }
            count_cache_hit(leg);
            leg->naptr_fetched = true;
            leg->num_cand = mme_dns_candidates_from_naptr(
                    entry->u.naptr, entry->num,
                    leg->svc, res->proto,
                    leg->cand, MME_DNS_MAX_CANDIDATES);
            leg->cursor = 0;
            if (leg->num_cand == 0) {
                leg_fail(res, leg, "no matching NAPTR records");
                return;
            }
            continue;
        }

        if (leg->cursor >= leg->num_cand) {
            leg_fail(res, leg, "candidates exhausted");
            return;
        }

        if (leg->cand[leg->cursor].needs_srv) {
            entry = cache_get(MME_DNS_T_SRV, leg->cand[leg->cursor].fqdn);
            if (!entry) {
                issue_query(res, leg, MME_DNS_T_SRV,
                        leg->cand[leg->cursor].fqdn);
                return;
            }
            count_cache_hit(leg);
            if (entry->num == 0) {
                /* No SRV target: skip this candidate */
                leg->cursor++;
                continue;
            }
            mme_dns_candidate_apply_srv(&leg->cand[leg->cursor],
                    entry->u.srv, entry->num);
            continue;
        }

        if (!leg->cand[leg->cursor].resolved) {
            mme_dns_candidate_t *cand = &leg->cand[leg->cursor];
            uint16_t port;

            entry = cache_get(MME_DNS_T_A, cand->fqdn);
            if (!entry) {
                issue_query(res, leg, MME_DNS_T_A, cand->fqdn);
                return;
            }
            count_cache_hit(leg);
            if (entry->num == 0) {
                leg->cursor++;
                continue;
            }
            port = cand->port ? cand->port : ogs_gtp_self()->gtpc_port;
            memset(&cand->addr, 0, sizeof(cand->addr));
            cand->addr.ogs_sa_family = AF_INET;
            cand->addr.sin.sin_addr = entry->u.a[0].addr;
            cand->addr.ogs_sin_port = htobe16(port);
            cand->resolved = true;
        }

        /* Candidate at cursor is fully resolved */
        leg->done = true;
        leg->fallback = false;
    }
}

/*****************************************************************
 * Applying results
 *****************************************************************/

static mme_dns_candidate_t *leg_result(mme_dns_leg_t *leg)
{
    if (!leg->active || !leg->done || leg->fallback)
        return NULL;
    if (leg->cursor >= leg->num_cand)
        return NULL;
    if (!leg->cand[leg->cursor].resolved)
        return NULL;
    return &leg->cand[leg->cursor];
}

static void apply_sgw(mme_dns_resolution_t *res, mme_sess_t *sess)
{
    mme_dns_candidate_t *cand = NULL;
    mme_ue_t *mme_ue = NULL;
    sgw_ue_t *sgw_ue = NULL;
    mme_sgw_t *sgw = NULL;
    char buf[OGS_ADDRSTRLEN];
    int rv;

    cand = leg_result(&res->sgw);
    if (!cand)
        return;

    mme_ue = mme_ue_find_by_id(sess->mme_ue_id);
    if (!mme_ue)
        return;
    sgw_ue = sgw_ue_find_by_id(mme_ue->sgw_ue_id);
    if (!sgw_ue)
        return;

    sgw = mme_sgw_find_by_addr(&cand->addr);
    if (!sgw) {
        ogs_sockaddr_t *addr = NULL;

        addr = ogs_calloc(1, sizeof(*addr));
        ogs_assert(addr);
        memcpy(addr, &cand->addr, sizeof(*addr));

        sgw = mme_sgw_add(addr);
        if (!sgw) {
            ogs_error("mme-dns: mme_sgw_add() failed, "
                    "using statically selected SGW");
            ogs_free(addr);
            return;
        }
        sgw->dns_origin = true;

        rv = ogs_gtp_connect(
                ogs_gtp_self()->gtpc_sock, ogs_gtp_self()->gtpc_sock6,
                &sgw->gnode);
        if (rv != OGS_OK) {
            ogs_error("mme-dns: ogs_gtp_connect() to [%s] failed, "
                    "using statically selected SGW",
                    OGS_ADDR(&cand->addr, buf));
            mme_sgw_remove(sgw);
            return;
        }
    }

    if (sgw_ue->sgw != sgw) {
        ogs_debug("mme-dns: switching UE to DNS-selected SGW [%s]",
                OGS_ADDR(&cand->addr, buf));
        sgw_ue_switch_to_sgw(sgw_ue, sgw);
    }
}

ogs_sockaddr_t *mme_dns_sess_pgw_addr(mme_sess_t *sess)
{
    mme_dns_resolution_t *res = NULL;
    mme_dns_candidate_t *cand = NULL;

    res = resolution_find_by_sess(sess);
    if (!res)
        return NULL;

    cand = leg_result(&res->pgw);
    if (!cand)
        return NULL;

    return &cand->addr;
}

/*****************************************************************
 * Entry points
 *****************************************************************/

bool mme_dns_enabled(void)
{
    return g_enabled;
}

int mme_dns_prepare(enb_ue_t *enb_ue, mme_sess_t *sess, int create_action)
{
    mme_dns_resolution_t *res = NULL;
    mme_ue_t *mme_ue = NULL;

    ogs_assert(sess);

    if (!g_enabled)
        return MME_DNS_SEND_NOW;

    /* DNS-based selection applies to initial session creation only;
     * TAU / Path-Switch keep both the statically relocated SGW and the
     * existing PGW. */
    if (create_action != OGS_GTP_CREATE_IN_ATTACH_REQUEST &&
        create_action != OGS_GTP_CREATE_IN_UPLINK_NAS_TRANSPORT)
        return MME_DNS_SEND_NOW;

    mme_ue = mme_ue_find_by_id(sess->mme_ue_id);
    if (!mme_ue || !sess->session || !sess->session->name)
        return MME_DNS_SEND_NOW;

    res = resolution_find_by_sess(sess);
    if (res) {
        if (res->state == MME_DNS_ST_PENDING)
            return MME_DNS_DEFERRED;    /* already in flight */
        return MME_DNS_SEND_NOW;        /* result already applied */
    }

    ogs_pool_id_calloc(&mme_dns_resolution_pool, &res);
    if (!res) {
        ogs_error("mme-dns: resolution pool exhausted, "
                "using static selection");
        return MME_DNS_SEND_NOW;
    }
    sess->dns_id = res->id;

    res->sess_id = sess->id;
    res->enb_ue_id = enb_ue ? enb_ue->id : OGS_INVALID_POOL_ID;
    res->create_action = create_action;
    res->state = MME_DNS_ST_PENDING;
    res->csr_attempts = 0;

    res->proto = mme_dns_decide_proto(mme_ue->imsi_bcd,
            &mme_ue->tai.plmn_id,
            (mme_dns_proto_e)mme_self()->dns.protocol);

    /* SGW leg: only when the UE is not yet anchored on an SGW session
     * (initial attach). Additional PDN connections must stay on the
     * UE's current SGW. */
    if (create_action == OGS_GTP_CREATE_IN_ATTACH_REQUEST) {
        res->sgw.svc = MME_DNS_SVC_SGW;
        if (mme_dns_build_tai_fqdn(res->sgw.fqdn, sizeof(res->sgw.fqdn),
                    &mme_ue->tai) == OGS_OK)
            res->sgw.active = true;
    }

    /* PGW leg: skipped when the HSS provided a PGW address
     * (MIP6-Agent-Info), which always takes precedence. */
    if (!sess->session->smf_ip.ipv4 && !sess->session->smf_ip.ipv6) {
        res->pgw.svc = MME_DNS_SVC_PGW;
        if (mme_dns_build_apn_fqdn(res->pgw.fqdn, sizeof(res->pgw.fqdn),
                    sess->session->name, &mme_ue->tai.plmn_id) == OGS_OK)
            res->pgw.active = true;
    }

    if (!res->sgw.active && !res->pgw.active) {
        resolution_free(res);
        sess->dns_id = OGS_INVALID_POOL_ID;
        return MME_DNS_SEND_NOW;
    }

    res->t_guard = ogs_timer_add(ogs_app()->timer_mgr,
            guard_expire_cb, OGS_UINT_TO_POINTER(res->id));
    ogs_assert(res->t_guard);
    ogs_timer_start(res->t_guard,
            ogs_time_from_msec(mme_self()->dns.guard_timeout));

    if (res->sgw.active)
        leg_advance(res, &res->sgw);
    if (res->pgw.active)
        leg_advance(res, &res->pgw);
    check_completion(res);

    return MME_DNS_DEFERRED;
}

void mme_dns_handle_resolved(mme_event_t *e)
{
    mme_sess_t *sess = NULL;
    enb_ue_t *enb_ue = NULL;
    mme_dns_resolution_t *res = NULL;
    int rv;

    ogs_assert(e);

    sess = mme_sess_find_by_id(e->sess_id);
    if (!sess) {
        ogs_warn("mme-dns: session has already been removed");
        return;
    }

    res = resolution_find_by_sess(sess);
    if (!res || res->state != MME_DNS_ST_DONE)
        return;    /* stale or duplicate event */

    enb_ue = enb_ue_find_by_id(res->enb_ue_id);

    apply_sgw(res, sess);

    res->state = MME_DNS_ST_CONSUMED;
    res->csr_attempts++;

    rv = mme_gtp_send_create_session_request_now(
            enb_ue, sess, res->create_action);
    ogs_expect(rv == OGS_OK);
}

static bool retry_leg(mme_sess_t *sess, mme_dns_leg_t *leg)
{
    mme_dns_resolution_t *res = NULL;

    res = resolution_find_by_sess(sess);
    if (!res || res->state != MME_DNS_ST_CONSUMED)
        return false;
    if (res->csr_attempts >= MME_DNS_MAX_CSR_ATTEMPTS)
        return false;

    if (!leg->active || leg->fallback)
        return false;
    if (leg->cursor + 1 >= leg->num_cand)
        return false;

    leg->cursor++;
    leg->done = false;

    res->state = MME_DNS_ST_PENDING;
    ogs_timer_start(res->t_guard,
            ogs_time_from_msec(mme_self()->dns.guard_timeout));

    leg_advance(res, leg);
    check_completion(res);

    return true;
}

bool mme_dns_retry_on_csr_failure(mme_sess_t *sess)
{
    mme_dns_resolution_t *res = NULL;
    bool armed;

    if (!g_enabled || !sess)
        return false;

    res = resolution_find_by_sess(sess);
    if (!res)
        return false;

    /* The SGW answered (with a reject), so blame the PGW leg */
    armed = retry_leg(sess, &res->pgw);
    if (armed)
        ogs_warn("mme-dns: Create Session rejected, "
                "retrying with next PGW candidate");
    return armed;
}

bool mme_dns_retry_on_gtp_timeout(mme_sess_t *sess)
{
    mme_dns_resolution_t *res = NULL;
    bool armed;

    if (!g_enabled || !sess)
        return false;

    res = resolution_find_by_sess(sess);
    if (!res)
        return false;

    /* No answer at all: blame the SGW leg */
    armed = retry_leg(sess, &res->sgw);
    if (armed)
        ogs_warn("mme-dns: Create Session timed out, "
                "retrying with next SGW candidate");
    return armed;
}

/*****************************************************************
 * Init / final
 *****************************************************************/

int mme_dns_open(void)
{
    int i, rc, optmask = 0;
    struct ares_options opt;
    char csv[MME_DNS_MAX_SERVER * (OGS_ADDRSTRLEN + 8)];
    size_t used = 0;

    if (!mme_self()->dns.enabled)
        return OGS_OK;

    if (!ogs_gtp_self()->gtpc_sock) {
        ogs_error("mme-dns: no IPv4 GTP-C socket; DNS-based selection "
                "supports A records only and cannot be enabled");
        return OGS_ERROR;
    }

    rc = ares_library_init(ARES_LIB_INIT_ALL);
    if (rc != ARES_SUCCESS) {
        ogs_error("ares_library_init() failed: %s", ares_strerror(rc));
        return OGS_ERROR;
    }

    memset(&opt, 0, sizeof(opt));
    opt.timeout = mme_self()->dns.timeout * 1000;
    optmask |= ARES_OPT_TIMEOUTMS;
    opt.tries = mme_self()->dns.retries;
    optmask |= ARES_OPT_TRIES;
    opt.flags = ARES_FLAG_NOSEARCH | ARES_FLAG_NOALIASES;
    optmask |= ARES_OPT_FLAGS;
    opt.sock_state_cb = dns_sock_state_cb;
    optmask |= ARES_OPT_SOCK_STATE_CB;

    rc = ares_init_options(&g_channel, &opt, optmask);
    if (rc != ARES_SUCCESS) {
        ogs_error("ares_init_options() failed: %s", ares_strerror(rc));
        ares_library_cleanup();
        return OGS_ERROR;
    }

    csv[0] = 0;
    for (i = 0; i < mme_self()->dns.num_of_server; i++) {
        used += ogs_snprintf(csv + used, sizeof(csv) - used, "%s%s:%d",
                i ? "," : "",
                mme_self()->dns.server[i].address,
                mme_self()->dns.server[i].port);
    }
    rc = ares_set_servers_ports_csv(g_channel, csv);
    if (rc != ARES_SUCCESS) {
        ogs_error("ares_set_servers_ports_csv(%s) failed: %s",
                csv, ares_strerror(rc));
        ares_destroy(g_channel);
        ares_library_cleanup();
        return OGS_ERROR;
    }

    g_t_process = ogs_timer_add(ogs_app()->timer_mgr,
            dns_process_timer_cb, NULL);
    ogs_assert(g_t_process);

    g_cache = ogs_hash_make();
    ogs_assert(g_cache);

    ogs_pool_init(&mme_dns_resolution_pool, ogs_global_conf()->max.ue);

    g_channel_up = true;
    g_enabled = true;
    g_closing = false;

    ogs_info("mme-dns: DNS-based SGW/PGW selection enabled [%s]", csv);

    return OGS_OK;
}

void mme_dns_close(void)
{
    int i;

    if (!g_channel_up)
        return;

    g_closing = true;
    g_enabled = false;

    ares_destroy(g_channel);
    g_channel = NULL;
    g_channel_up = false;
    ares_library_cleanup();

    for (i = 0; i < (int)OGS_ARRAY_SIZE(g_fdmap); i++) {
        if (g_fdmap[i].rpoll) {
            ogs_pollset_remove(g_fdmap[i].rpoll);
            g_fdmap[i].rpoll = NULL;
        }
        if (g_fdmap[i].wpoll) {
            ogs_pollset_remove(g_fdmap[i].wpoll);
            g_fdmap[i].wpoll = NULL;
        }
    }

    if (g_t_process) {
        ogs_timer_delete(g_t_process);
        g_t_process = NULL;
    }

    cache_flush();
    if (g_cache) {
        ogs_hash_destroy(g_cache);
        g_cache = NULL;
    }

    ogs_pool_final(&mme_dns_resolution_pool);
}
