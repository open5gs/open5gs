/*
 * Copyright (C) 2025 by Rami Mohamed <ramrode@gmail.com>
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

#include <resolv.h>
#include <arpa/nameser.h>
#include <arpa/nameser_compat.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time.h>

#include "mme-context.h"
#include "mme-s-naptr.h"

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __mme_log_domain

/* -------------------------------------------------------------------------
 * Constants
 * ---------------------------------------------------------------------- */

/* 3GPP TS 29.303 §5.2: S-NAPTR service tag for PGW over GTPv2 (S5/S8) */
#define S_NAPTR_SERVICE_PGW_GTP  "x-3gpp-pgw:x-gtp-v2"

/*
 * DNS response buffer.  4096 bytes is the maximum DNS UDP payload size
 * (RFC 6891 §6.2.5 hard cap) and is sufficient for any well-formed response.
 */
#define DNS_BUF_LEN  4096

/*
 * Maximum candidate records collected at each resolution stage.
 * Records beyond this limit are skipped with a warning.
 */
#define MAX_NAPTR_CANDIDATES  16
#define MAX_SRV_CANDIDATES    16
#define MAX_A_CANDIDATES      16

/* -------------------------------------------------------------------------
 * DNS candidate cache
 *
 * Caches the PGW candidate list keyed by APN-FQDN so that repeated
 * Create Session Requests for the same APN avoid a full DNS
 * NAPTR→SRV→A round-trip on every call.
 *
 * Cache lifetime is governed by the minimum TTL returned by the DNS A
 * records.  A floor (S_NAPTR_CACHE_MIN_TTL_S) prevents caching with an
 * unreasonably short TTL from broken resolvers.
 *
 * The cache is keyed on the full APN-FQDN which already encodes the
 * APN name, MCC, and MNC per TS 23.003 §19.4.2, so no separate PLMN
 * field is needed in the key.
 *
 * Thread safety: the open5gs MME is single-threaded (event-driven), so no
 * locking is required.
 *
 * Load balancing: on every cache hit, mme_s_naptr_resolve_candidates()
 * returns a freshly rotated copy of the cached list so that successive
 * sessions for the same APN still receive a random initial PGW selection.
 * ---------------------------------------------------------------------- */

/* Maximum distinct APN-FQDNs in cache; oldest entry evicted when full */
#define S_NAPTR_CACHE_MAX_ENTRIES    64

/*
 * Minimum effective cache TTL (seconds).  If the DNS TTL is shorter, we
 * cache for S_NAPTR_CACHE_DEFAULT_TTL_S instead to avoid hammering the DNS
 * server on every session.  Set to 0 to honour the DNS TTL exactly.
 */
#define S_NAPTR_CACHE_MIN_TTL_S      30

/* Fallback TTL used when the DNS server returns TTL 0 */
#define S_NAPTR_CACHE_DEFAULT_TTL_S  300

typedef struct {
    ogs_lnode_t     lnode;
    char            fqdn[NS_MAXDNAME];   /* lookup key (APN-FQDN)         */
    ogs_sockaddr_t *list;                /* heap copy of candidate array  */
    int             count;              /* entries in list[]             */
    time_t          expires_at;         /* time(NULL) value at expiry    */
} s_naptr_cache_entry_t;

/* Entries are appended at the tail; oldest is at the head (FIFO eviction) */
static OGS_LIST(s_naptr_cache);
static int s_naptr_cache_entries = 0;

/* Release one cache entry and update the occupancy counter */
static void cache_entry_free(s_naptr_cache_entry_t *e)
{
    ogs_assert(e);
    ogs_list_remove(&s_naptr_cache, e);
    s_naptr_cache_entries--;
    ogs_free(e->list);
    ogs_free(e);
}

/*
 * cache_lookup() - find a live (non-expired) entry by FQDN.
 *
 * Expired entries encountered during the scan are collected and freed after
 * the iteration completes.  This avoids calling cache_entry_free() (which
 * calls ogs_list_remove()) from inside ogs_list_for_each_safe, making the
 * removal explicit and easy to reason about.
 *
 * Returns the entry on cache hit, NULL on miss or expiry.
 */
static s_naptr_cache_entry_t *cache_lookup(const char *fqdn)
{
    s_naptr_cache_entry_t *e = NULL, *next = NULL;
    s_naptr_cache_entry_t *found = NULL, *expired = NULL;
    time_t now = time(NULL);

    ogs_list_for_each_safe(&s_naptr_cache, next, e) {
        if (strcmp(e->fqdn, fqdn) != 0)
            continue;

        if (now >= e->expires_at) {
            ogs_debug("S-NAPTR cache: expired entry for '%s'", fqdn);
            expired = e;   /* free after loop exits */
        } else {
            ogs_debug("S-NAPTR cache: HIT '%s' (%d candidates, %lds remaining)",
                      fqdn, e->count, (long)(e->expires_at - now));
            found = e;
        }
        break;  /* FQDN is unique in the cache — stop scanning */
    }

    if (expired)
        cache_entry_free(expired);  /* safe: loop already finished */

    return found;
}

/*
 * cache_store() - insert or refresh a cache entry for fqdn.
 *
 * Makes a deep copy of list[0..count-1].  The effective TTL is clamped to at
 * least S_NAPTR_CACHE_MIN_TTL_S.  When the cache is full the oldest entry
 * (head of the list) is evicted before the new entry is appended.
 *
 * If an entry for fqdn already exists it is updated in-place (moved to
 * the tail to maintain recency ordering for eviction).
 */
static void cache_store(const char *fqdn,
        const ogs_sockaddr_t *list, int count, uint32_t ttl_s)
{
    s_naptr_cache_entry_t *e = NULL, *next = NULL;
    uint32_t effective_ttl;

    ogs_assert(fqdn && list && count > 0);

    /*
     * Clamp short TTLs to the floor value so a misconfigured DNS zone that
     * returns TTL 0 or a very small value does not cause cache thrashing.
     * Use S_NAPTR_CACHE_MIN_TTL_S (not the 300-second default) so that the
     * floor is the 30-second minimum advertised in the comment above.
     */
    effective_ttl = (ttl_s < S_NAPTR_CACHE_MIN_TTL_S)
                        ? S_NAPTR_CACHE_MIN_TTL_S : ttl_s;

    /* Check if this FQDN is already cached — update in-place */
    ogs_list_for_each_safe(&s_naptr_cache, next, e) {
        if (strcmp(e->fqdn, fqdn) == 0) {
            /* Remove from current position; re-insert at tail below */
            ogs_list_remove(&s_naptr_cache, e);
            s_naptr_cache_entries--;
            ogs_free(e->list);
            e->list = NULL;
            goto fill;
        }
    }

    /* New entry — evict the oldest (head) if the cache is full */
    if (s_naptr_cache_entries >= S_NAPTR_CACHE_MAX_ENTRIES) {
        e = ogs_list_first(&s_naptr_cache);
        ogs_assert(e);
        ogs_debug("S-NAPTR cache: evicting oldest entry '%s'", e->fqdn);
        ogs_list_remove(&s_naptr_cache, e);
        s_naptr_cache_entries--;
        ogs_free(e->list);
        memset(e, 0, sizeof(*e));   /* reuse the allocation */
    } else {
        e = ogs_calloc(1, sizeof(s_naptr_cache_entry_t));
        ogs_assert(e);
    }

fill:
    ogs_cpystrn(e->fqdn, fqdn, sizeof(e->fqdn));
    e->count      = count;
    e->list       = ogs_calloc(count, sizeof(ogs_sockaddr_t));
    ogs_assert(e->list);
    memcpy(e->list, list, count * sizeof(ogs_sockaddr_t));
    e->expires_at = time(NULL) + (time_t)effective_ttl;

    ogs_list_add(&s_naptr_cache, e);  /* newest entries at tail */
    s_naptr_cache_entries++;

    ogs_info("S-NAPTR cache: stored '%s' (%d candidates, TTL %us)",
             fqdn, count, effective_ttl);
}

/*
 * mme_s_naptr_cache_flush() - release all cached entries.
 *
 * Called from mme_context_final() on shutdown to prevent leak reports from
 * tools like valgrind.  Not required for correctness; the OS reclaims all
 * memory on process exit regardless.
 */
void mme_s_naptr_cache_flush(void)
{
    s_naptr_cache_entry_t *e = NULL, *next = NULL;

    ogs_list_for_each_safe(&s_naptr_cache, next, e)
        cache_entry_free(e);

    ogs_assert(s_naptr_cache_entries == 0);
    ogs_debug("S-NAPTR cache: flushed");
}

/* -------------------------------------------------------------------------
 * Internal helpers
 * ---------------------------------------------------------------------- */

/*
 * build_apn_fqdn() - construct the APN FQDN per 3GPP TS 23.003 §19.4.2.
 *
 * Format: <apn-ni>.apn.epc.mnc<MNC>.mcc<MCC>.3gppnetwork.org
 *
 * MNC is zero-padded to 2 or 3 digits; MCC is always 3 digits.
 * Returns the number of characters written (excluding NUL), or -1 if the
 * buffer is too small.
 */
static int build_apn_fqdn(
        char *buf, size_t buflen,
        const char *apn, const ogs_plmn_id_t *plmn_id)
{
    int n;
    uint16_t mcc     = ogs_plmn_id_mcc(plmn_id);
    uint16_t mnc     = ogs_plmn_id_mnc(plmn_id);
    uint16_t mnc_len = ogs_plmn_id_mnc_len(plmn_id);

    ogs_assert(buf && buflen > 0 && apn && plmn_id);

    if (mnc_len == 2)
        n = ogs_snprintf(buf, buflen,
                "%s.apn.epc.mnc%02d.mcc%03d.3gppnetwork.org",
                apn, mnc, mcc);
    else
        n = ogs_snprintf(buf, buflen,
                "%s.apn.epc.mnc%03d.mcc%03d.3gppnetwork.org",
                apn, mnc, mcc);

    if (n < 0 || (size_t)n >= buflen) {
        ogs_error("APN FQDN too long for APN[%s]", apn);
        return -1;
    }
    return n;
}

/*
 * init_resolver() - initialise a res_state pointing at the configured
 * s5s8_dns servers, bypassing /etc/resolv.conf.
 *
 * Ownership / cleanup contract:
 *   - Returns 0 on success.  Caller MUST call res_nclose() when done.
 *   - Returns -1 on failure.  res_nclose() must NOT be called.
 */
static int init_resolver(struct __res_state *res)
{
    int i;
    const char *dns[2];

    ogs_assert(res);

    dns[0] = mme_self()->s5s8_dns[0];
    dns[1] = mme_self()->s5s8_dns[1];

    if (!dns[0]) {
        ogs_debug("S-NAPTR: s5s8.dns not configured, skipping DNS discovery");
        return -1;
    }

    if (res_ninit(res) != 0) {
        ogs_error("res_ninit() failed");
        return -1;
    }

    res->nscount = 0;
    for (i = 0; i < 2 && dns[i]; i++) {
        struct in_addr addr;
        if (inet_pton(AF_INET, dns[i], &addr) != 1) {
            ogs_warn("Invalid s5s8.dns[%d] address '%s', skipping", i, dns[i]);
            continue;
        }
        memset(&res->nsaddr_list[res->nscount], 0,
                sizeof(res->nsaddr_list[res->nscount]));
        res->nsaddr_list[res->nscount].sin_family = AF_INET;
        res->nsaddr_list[res->nscount].sin_port   = htons(NS_DEFAULTPORT);
        res->nsaddr_list[res->nscount].sin_addr   = addr;
        res->nscount++;
    }

    if (res->nscount == 0) {
        ogs_error("No valid s5s8.dns addresses available");
        res_nclose(res);
        return -1;
    }

    /*
     * Fast failover to the secondary DNS server.
     * retrans=1: 1 s per-server timeout.
     * retry=2:   2 attempts per server before trying the next.
     * Worst-case total (both servers down): 2 × 2 = 4 s.
     */
    res->retrans = 1;
    res->retry   = 2;

    return 0;
}

/* -------------------------------------------------------------------------
 * NAPTR record parsing and selection
 * ---------------------------------------------------------------------- */

typedef struct {
    uint16_t order;
    uint16_t pref;
    char     flags[8];
    char     service[64];
    char     replacement[NS_MAXDNAME];
} naptr_rec_t;

/*
 * parse_naptr_rdata() - decode a NAPTR RDATA field into naptr_rec_t.
 * Wire format per RFC 3403 §4.
 * Returns 0 on success, -1 on malformed input.
 */
static int parse_naptr_rdata(
        const unsigned char *msg, int msglen,
        const unsigned char *rdata, int rdlen,
        naptr_rec_t *out)
{
    const unsigned char *p   = rdata;
    const unsigned char *end = rdata + rdlen;
    int slen;

    if (p + 4 > end) return -1;
    out->order = ns_get16(p);  p += 2;
    out->pref  = ns_get16(p);  p += 2;

    if (p >= end) return -1;
    slen = *p++;
    if (p + slen > end || (size_t)slen >= sizeof(out->flags)) return -1;
    memcpy(out->flags, p, slen);
    out->flags[slen] = '\0';
    p += slen;

    if (p >= end) return -1;
    slen = *p++;
    if (p + slen > end || (size_t)slen >= sizeof(out->service)) return -1;
    memcpy(out->service, p, slen);
    out->service[slen] = '\0';
    p += slen;

    /* REGEXP — skip (not used for S-NAPTR per RFC 3958 §6.5) */
    if (p >= end) return -1;
    slen = *p++;
    if (p + slen > end) return -1;
    p += slen;

    if (ns_name_uncompress(msg, msg + msglen, p,
                out->replacement, NS_MAXDNAME) < 0)
        return -1;

    return 0;
}

/*
 * query_naptr() - query NAPTR records and select one SRV target name.
 *
 * Algorithm (TS 29.303 §5.2):
 *   1. Filter records by service "x-3gpp-pgw:x-gtp-v2" and flag "S".
 *   2. Keep only records at the lowest ORDER value.
 *   3. Inverse-preference weighted random selection:
 *        weight_i = max_pref + 1 - pref_i
 *
 * Returns 0 and fills srv_name on success; -1 on failure.
 */
static int query_naptr(
        struct __res_state *res,
        const char *fqdn,
        char *srv_name)
{
    unsigned char buf[DNS_BUF_LEN];
    int len, i, count = 0;
    uint16_t best_order = 0;
    ns_msg handle;
    ns_rr rr;
    naptr_rec_t candidates[MAX_NAPTR_CANDIDATES];
    naptr_rec_t cur;

    ogs_debug("S-NAPTR: NAPTR query for '%s'", fqdn);

    len = res_nquery(res, fqdn, C_IN, T_NAPTR, buf, sizeof(buf));
    if (len < 0) {
        ogs_warn("S-NAPTR: NAPTR query failed for '%s'", fqdn);
        return -1;
    }

    if (ns_initparse(buf, len, &handle) < 0) {
        ogs_error("S-NAPTR: ns_initparse failed for '%s'", fqdn);
        return -1;
    }

    for (i = 0; i < ns_msg_count(handle, ns_s_an); i++) {
        if (ns_parserr(&handle, ns_s_an, i, &rr) < 0)
            continue;
        if (ns_rr_type(rr) != T_NAPTR)
            continue;
        if (parse_naptr_rdata(buf, len,
                ns_rr_rdata(rr), ns_rr_rdlen(rr), &cur) < 0)
            continue;

        ogs_debug("S-NAPTR: NAPTR[%d] order=%u pref=%u flags='%s' "
                  "service='%s' replacement='%s'",
                  i, cur.order, cur.pref, cur.flags,
                  cur.service, cur.replacement);

        if (strcasecmp(cur.service, S_NAPTR_SERVICE_PGW_GTP) != 0)
            continue;
        if (strcasecmp(cur.flags, "s") != 0)
            continue;

        if (count == 0 || cur.order < best_order) {
            best_order    = cur.order;
            candidates[0] = cur;
            count         = 1;
        } else if (cur.order == best_order) {
            if (count < MAX_NAPTR_CANDIDATES) {
                candidates[count++] = cur;
            } else {
                ogs_warn("S-NAPTR: NAPTR candidate limit (%d) reached for "
                         "'%s'; extra records at order=%u skipped",
                         MAX_NAPTR_CANDIDATES, fqdn, best_order);
            }
        }
    }

    if (count == 0) {
        ogs_warn("S-NAPTR: no matching NAPTR record for '%s' "
                 "(service=" S_NAPTR_SERVICE_PGW_GTP ", flag=S)", fqdn);
        return -1;
    }

    if (count == 1) {
        ogs_cpystrn(srv_name, candidates[0].replacement, NS_MAXDNAME);
    } else {
        /* Inverse-preference weighted selection; uint32_t guards overflow */
        uint32_t total_weight = 0;
        uint32_t weights[MAX_NAPTR_CANDIDATES];
        uint32_t max_pref = 0;
        uint32_t pick, cum;
        int selected = 0;

        for (i = 0; i < count; i++)
            if ((uint32_t)candidates[i].pref > max_pref)
                max_pref = candidates[i].pref;

        for (i = 0; i < count; i++) {
            weights[i]    = max_pref + 1 - (uint32_t)candidates[i].pref;
            total_weight += weights[i];
        }

        pick = ogs_random32() % total_weight;
        cum  = 0;
        for (i = 0; i < count; i++) {
            cum += weights[i];
            if (pick < cum) { selected = i; break; }
        }

        ogs_debug("S-NAPTR: selected NAPTR [%d/%d] order=%u pref=%u '%s'",
                  selected + 1, count,
                  candidates[selected].order, candidates[selected].pref,
                  candidates[selected].replacement);
        ogs_cpystrn(srv_name, candidates[selected].replacement, NS_MAXDNAME);
    }

    return 0;
}

/* -------------------------------------------------------------------------
 * SRV record querying and selection
 * ---------------------------------------------------------------------- */

typedef struct {
    uint16_t priority;
    uint16_t weight;
    uint16_t port;
    char     target[NS_MAXDNAME];
} srv_rec_t;

/*
 * query_srv_all() - query SRV records and return ALL targets at the lowest
 * priority level per RFC 2782 §3.
 *
 * Unlike a single-selection query, this function collects every SRV record
 * at the lowest priority value so that the caller can resolve A records for
 * each target and build a complete PGW candidate pool.  Records at higher
 * priority values are discarded — they are only relevant when all
 * lower-priority targets are unreachable, which is handled at the session
 * retry layer (pgw_candidates.index in mme-gtp-path.c / mme-s11-handler.c).
 *
 * SRV weight is preserved in out[].weight for callers that wish to apply
 * weight-ordered iteration; it is not used to make a selection here.
 *
 * Why NOT select one target by weight here:
 *   If only one SRV target is selected, only its A records enter the DNS
 *   cache.  All other PGW IPs are permanently excluded from load balancing
 *   and failover for the entire cache lifetime — defeating the purpose of
 *   maintaining a candidate pool in mme_sess_t.pgw_candidates.
 *
 * Returns the number of SRV records stored in out[] (≥1), or 0 on failure.
 */
static int query_srv_all(
        struct __res_state *res,
        const char *srv_name,
        srv_rec_t *out, int max_count)
{
    unsigned char buf[DNS_BUF_LEN];
    int len, i, count = 0;
    uint16_t best_priority = 0;
    ns_msg handle;
    ns_rr rr;
    const unsigned char *rdata;
    srv_rec_t cur;

    ogs_debug("S-NAPTR: SRV query (all targets) for '%s'", srv_name);

    len = res_nquery(res, srv_name, C_IN, T_SRV, buf, sizeof(buf));
    if (len < 0) {
        ogs_warn("S-NAPTR: SRV query failed for '%s'", srv_name);
        return 0;
    }

    if (ns_initparse(buf, len, &handle) < 0) {
        ogs_error("S-NAPTR: ns_initparse (SRV) failed for '%s'", srv_name);
        return 0;
    }

    for (i = 0; i < ns_msg_count(handle, ns_s_an); i++) {
        if (ns_parserr(&handle, ns_s_an, i, &rr) < 0)
            continue;
        if (ns_rr_type(rr) != T_SRV)
            continue;

        rdata = ns_rr_rdata(rr);
        if (ns_rr_rdlen(rr) < 6)
            continue;

        cur.priority = ns_get16(rdata);  rdata += 2;
        cur.weight   = ns_get16(rdata);  rdata += 2;
        cur.port     = ns_get16(rdata);  rdata += 2;

        if (ns_name_uncompress(buf, buf + len, rdata,
                cur.target, NS_MAXDNAME) < 0)
            continue;

        ogs_debug("S-NAPTR: SRV[%d] priority=%u weight=%u port=%u target='%s'",
                  i, cur.priority, cur.weight, cur.port, cur.target);

        if (count == 0 || cur.priority < best_priority) {
            /* New lowest priority — discard any higher-priority records seen
             * so far and start fresh with this one. */
            best_priority = cur.priority;
            out[0]        = cur;
            count         = 1;
        } else if (cur.priority == best_priority) {
            if (count < max_count) {
                out[count++] = cur;
            } else {
                ogs_warn("S-NAPTR: SRV candidate limit (%d) reached for "
                         "'%s'; extra records at priority=%u skipped",
                         max_count, srv_name, best_priority);
            }
        }
        /* Records at a higher priority value than best_priority are skipped */
    }

    if (count == 0) {
        ogs_warn("S-NAPTR: no SRV records found for '%s'", srv_name);
        return 0;
    }

    ogs_debug("S-NAPTR: %d SRV target(s) at priority=%u for '%s'",
              count, best_priority, srv_name);
    return count;
}

/* -------------------------------------------------------------------------
 * A record resolution — all addresses for retry support
 * ---------------------------------------------------------------------- */

/*
 * query_a_all() - resolve hostname to ALL IPv4 addresses.
 *
 * Fills out[0..max_count-1] with all A records found, each with the given
 * port.  *min_ttl_out receives the minimum TTL across all A records; this
 * drives the DNS cache expiry.  Returns the count stored, or 0 on failure.
 */
static int query_a_all(
        struct __res_state *res,
        const char *hostname, uint16_t port,
        ogs_sockaddr_t *out, int max_count,
        uint32_t *min_ttl_out)
{
    unsigned char buf[DNS_BUF_LEN];
    int len, i, count = 0;
    uint32_t min_ttl = UINT32_MAX;
    ns_msg handle;
    ns_rr rr;

    ogs_assert(res && hostname && out && max_count > 0 && min_ttl_out);

    ogs_debug("S-NAPTR: A query for '%s'", hostname);

    len = res_nquery(res, hostname, C_IN, T_A, buf, sizeof(buf));
    if (len < 0) {
        ogs_warn("S-NAPTR: A query failed for '%s'", hostname);
        *min_ttl_out = 0;
        return 0;
    }

    if (ns_initparse(buf, len, &handle) < 0) {
        ogs_error("S-NAPTR: ns_initparse (A) failed for '%s'", hostname);
        *min_ttl_out = 0;
        return 0;
    }

    for (i = 0; i < ns_msg_count(handle, ns_s_an); i++) {
        if (ns_parserr(&handle, ns_s_an, i, &rr) < 0)
            continue;
        if (ns_rr_type(rr) != T_A)
            continue;
        if (ns_rr_rdlen(rr) != NS_INADDRSZ)
            continue;

        if (count >= max_count) {
            ogs_warn("S-NAPTR: A record limit (%d) reached for '%s'; "
                     "extra records skipped", max_count, hostname);
            break;
        }

        memset(&out[count], 0, sizeof(ogs_sockaddr_t));
        out[count].sin.sin_family = AF_INET;
        out[count].sin.sin_port   = htons(port);
        memcpy(&out[count].sin.sin_addr, ns_rr_rdata(rr), NS_INADDRSZ);

        /* Track minimum TTL for cache expiry calculation */
        if (ns_rr_ttl(rr) < min_ttl)
            min_ttl = ns_rr_ttl(rr);

        ogs_debug("S-NAPTR: A[%d] %s:%u (TTL %us)",
                  count, inet_ntoa(out[count].sin.sin_addr),
                  port, ns_rr_ttl(rr));
        count++;
    }

    *min_ttl_out = (min_ttl == UINT32_MAX) ? 0 : min_ttl;

    if (count == 0)
        ogs_warn("S-NAPTR: no A records found for '%s'", hostname);

    return count;
}

/* -------------------------------------------------------------------------
 * Internal: build a rotated candidate array
 * ---------------------------------------------------------------------- */

/*
 * build_rotated_candidates() - allocate and return a rotated copy of addrs[].
 *
 * Picks one address at random as the preferred candidate (index 0) and
 * rotates the remaining addresses after it.  This implements the random
 * first-choice required by TS 23.401 §5.3.2.1 while keeping all candidates
 * available for retry in order.
 *
 * The caller owns the returned array and must ogs_free() it.
 */
static ogs_sockaddr_t *build_rotated_candidates(
        const ogs_sockaddr_t *addrs, int n, int *count_out)
{
    ogs_sockaddr_t *result = NULL;
    int preferred, i;

    ogs_assert(addrs && n > 0 && count_out);

    result = ogs_calloc(n, sizeof(ogs_sockaddr_t));
    ogs_assert(result);

    if (n == 1) {
        result[0]  = addrs[0];
    } else {
        preferred = (int)(ogs_random32() % (uint32_t)n);
        for (i = 0; i < n; i++)
            result[i] = addrs[(preferred + i) % n];
    }

    *count_out = n;
    return result;
}

/* -------------------------------------------------------------------------
 * Public API
 * ---------------------------------------------------------------------- */

/*
 * mme_s_naptr_resolve_candidates() - S-NAPTR PGW discovery with caching.
 *
 * On cache miss, runs the full NAPTR→SRV→A DNS chain and stores the result.
 * On cache hit, returns a freshly rotated copy of the cached candidates.
 *
 * The cache key is the APN-FQDN derived from apn + plmn_id per TS 23.003
 * §19.4.2.  Cache lifetime = min(DNS A TTL, S_NAPTR_CACHE_DEFAULT_TTL_S)
 * seconds, floored at S_NAPTR_CACHE_MIN_TTL_S.
 *
 * Returns a heap-allocated array (caller ogs_free()s in mme_sess_remove())
 * and sets *count.  Returns NULL / *count=0 on failure.
 *
 * All returned addresses carry port OGS_GTPV2_C_UDP_PORT (2123).
 */
ogs_sockaddr_t *mme_s_naptr_resolve_candidates(
        const char *apn, const ogs_plmn_id_t *plmn_id, int *count)
{
    char fqdn[NS_MAXDNAME];
    char srv_name[NS_MAXDNAME];
    struct __res_state res;
    ogs_sockaddr_t tmp[MAX_A_CANDIDATES];
    ogs_sockaddr_t *result = NULL;
    srv_rec_t srvs[MAX_SRV_CANDIDATES];
    uint32_t min_ttl = UINT32_MAX;
    uint32_t srv_ttl = 0;
    int n = 0, srv_count = 0, s = 0, got = 0;
    s_naptr_cache_entry_t *cached = NULL;

    ogs_assert(apn);
    ogs_assert(plmn_id);
    ogs_assert(count);

    *count = 0;

    /* Step 1: build APN-FQDN — serves as both the DNS query name and cache key */
    if (build_apn_fqdn(fqdn, sizeof(fqdn), apn, plmn_id) < 0)
        return NULL;

    /* ------------------------------------------------------------------
     * Cache lookup — skip the DNS round-trip when we have a fresh result.
     *
     * The cache stores the FULL unrotated candidate pool (all PGW IPs from
     * all SRV targets).  build_rotated_candidates() then picks a random
     * start so each session gets a different preferred PGW while still
     * having access to every other PGW for load balancing and failover.
     * ------------------------------------------------------------------ */
    cached = cache_lookup(fqdn);
    if (cached) {
        result = build_rotated_candidates(cached->list, cached->count, count);
        ogs_info("S-NAPTR: [CACHE HIT] APN[%s] → %d PGW candidate(s); "
                 "preferred %s",
                 apn, *count, inet_ntoa(result[0].sin.sin_addr));
        return result;
    }

    /* ------------------------------------------------------------------
     * Cache MISS — perform full DNS resolution: NAPTR → SRV → A.
     *
     * All SRV targets at the lowest priority are resolved and their A
     * records are accumulated into tmp[] so the full PGW pool is cached.
     * ------------------------------------------------------------------ */
    ogs_debug("S-NAPTR: [CACHE MISS] APN[%s] FQDN[%s]", apn, fqdn);

    if (init_resolver(&res) != 0)
        return NULL;

    if (query_naptr(&res, fqdn, srv_name) != 0)
        goto out;

    /* Collect ALL SRV targets at the lowest priority (not just one) */
    srv_count = query_srv_all(&res, srv_name, srvs, MAX_SRV_CANDIDATES);
    if (srv_count == 0)
        goto out;

    /*
     * Resolve A records for every SRV target and accumulate into tmp[].
     *
     * This is the critical step that ensures the cache contains the FULL
     * PGW candidate pool:
     *
     *   SRV → pgw1.example.com  → A: 10.0.1.1          ┐
     *   SRV → pgw2.example.com  → A: 10.0.1.2, 10.0.1.3 ├─ all stored
     *   SRV → pgw3.example.com  → A: 10.0.1.4          ┘
     *
     * Without this loop, only pgw1 OR pgw2 OR pgw3 would be selected by
     * query_srv() and only that one PGW's IPs would be cached — the others
     * would be permanently excluded from load balancing and failover.
     *
     * min_ttl tracks the shortest TTL across all A records so the cache
     * entry expires before any individual address becomes stale.
     */
    for (s = 0; s < srv_count && n < MAX_A_CANDIDATES; s++) {
        got = query_a_all(&res, srvs[s].target, srvs[s].port,
                          tmp + n, MAX_A_CANDIDATES - n, &srv_ttl);
        if (got > 0) {
            ogs_debug("S-NAPTR: SRV target '%s:%u' → %d address(es)",
                      srvs[s].target, srvs[s].port, got);
            n += got;
            if (srv_ttl < min_ttl)
                min_ttl = srv_ttl;
        } else {
            ogs_warn("S-NAPTR: SRV target '%s' has no A records — skipped",
                     srvs[s].target);
        }
    }

    if (n == 0)
        goto out;

    if (min_ttl == UINT32_MAX)
        min_ttl = 0;   /* no A record TTL captured — use cache_store() floor */

    /*
     * Store the full unrotated candidate pool in cache.
     * Every subsequent session for this APN gets a freshly rotated copy
     * from build_rotated_candidates(), distributing load without DNS I/O.
     */
    cache_store(fqdn, tmp, n, min_ttl);

    /* Build the caller's rotated copy */
    result = build_rotated_candidates(tmp, n, count);

    ogs_info("S-NAPTR: [DNS] resolved APN[%s] → %d PGW candidate(s) from "
             "%d SRV target(s); preferred %s (DNS TTL %us, now cached)",
             apn, *count, srv_count,
             inet_ntoa(result[0].sin.sin_addr), min_ttl);

out:
    res_nclose(&res);

    if (*count == 0 && result) {
        ogs_free(result);
        result = NULL;
    }

    if (!result)
        ogs_warn("S-NAPTR: DNS discovery failed for APN[%s]; "
                 "will fall back to static PGW config", apn);

    return result;
}
