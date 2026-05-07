/*
 * Copyright (C) 2026 by Rami Mohamed <ramrode@gmail.com>
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
/* nameser_compat.h provides legacy T_xxx / C_IN macros; absent on musl libc.
 * Fall back to the portable ns_* enum values when the header is missing. */
#ifdef HAVE_ARPA_NAMESER_COMPAT_H
#  include <arpa/nameser_compat.h>
#else
#  ifndef T_NAPTR
#    define T_NAPTR  ns_t_naptr
#  endif
#  ifndef T_SRV
#    define T_SRV    ns_t_srv
#  endif
#  ifndef T_A
#    define T_A      ns_t_a
#  endif
#  ifndef C_IN
#    define C_IN     ns_c_in
#  endif
#endif
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
 * Cache lifetime is governed by the minimum TTL across the full DNS chain:
 * min(NAPTR TTL, SRV TTL, A TTL).  This ensures that changes at any level
 * of the NAPTR→SRV→A hierarchy are honoured promptly.  A floor
 * (S_NAPTR_CACHE_MIN_TTL_S) prevents pathologically short TTLs from broken
 * resolvers from hammering DNS.
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
 * Minimum effective cache TTL (seconds).  If the DNS TTL is shorter than this
 * floor, the actual cache lifetime is raised to S_NAPTR_CACHE_MIN_TTL_S to
 * avoid hammering the DNS server on every session setup.
 * Set to 0 to honour the DNS TTL exactly (no floor).
 */
#define S_NAPTR_CACHE_MIN_TTL_S      30

/* Maximum effective cache TTL (seconds).  DNS TTLs longer than this are
 * clamped down so that the cached PGW pool is never used for more than one
 * day without a fresh DNS lookup.  Set to 0 to honour the DNS TTL exactly. */
#define S_NAPTR_CACHE_DEFAULT_TTL_S  86400

/*
 * Per-SRV-target descriptor stored inside the cache entry for tier0 targets.
 *
 * Each tier0 SRV target contributes one or more A records to list[].  This
 * struct records WHERE in list[] those IPs live (start..start+count-1) and
 * what SRV weight was advertised for the target.
 *
 * build_rotated_candidates() uses these descriptors to apply RFC 2782 §3
 * weight-proportional selection at the SRV-TARGET level: a target with
 * weight=99 is selected 99× more often than one with weight=1, regardless
 * of how many A records each target has.  Weight 0 is treated as 1 per
 * RFC 2782 ("a very small chance of being selected").
 */
typedef struct {
    uint16_t weight;   /* SRV weight; 0 treated as 1 per RFC 2782 §3 */
    int      start;    /* index into list[] of this target's first IP  */
    int      count;    /* number of A records resolved for this target */
} tier0_target_t;

typedef struct {
    ogs_lnode_t     lnode;
    char            fqdn[NS_MAXDNAME];   /* lookup key (APN-FQDN)              */
    ogs_sockaddr_t *list;                /* heap copy of candidate array       */
    int             count;              /* total entries in list[]            */
    int             tier0_count;        /* IPs from the lowest SRV priority   *
                                         * tier (list[0..tier0_count-1]).      *
                                         * build_rotated_candidates() applies  *
                                         * RFC 2782 weight selection here;     *
                                         * backup tiers are appended after so  *
                                         * RFC 2782 priority ordering holds.   */
    tier0_target_t  tier0_targets[MAX_SRV_CANDIDATES]; /* per-target weight+range */
    int             tier0_target_count; /* valid entries in tier0_targets[]   */
    time_t          expires_at;         /* time(NULL) value at expiry         */
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
 * Makes a deep copy of list[0..count-1].  The effective TTL is clamped to
 * [S_NAPTR_CACHE_MIN_TTL_S, S_NAPTR_CACHE_DEFAULT_TTL_S].  When the cache is
 * full the oldest entry (head of the list) is evicted before the new one is
 * appended.  If an entry for fqdn already exists it is updated in-place
 * (moved to the tail to maintain recency ordering for eviction).
 *
 * target_count == 0 is accepted: it represents a backup-only pool where every
 * primary SRV target had no A records.  build_rotated_candidates() handles
 * this with a uniform random rotation fast-path (t0_count <= 0 branch).
 */
static void cache_store(const char *fqdn,
        const ogs_sockaddr_t *list, int count,
        int tier0_count,
        const tier0_target_t *targets, int target_count,
        uint32_t ttl_s)
{
    s_naptr_cache_entry_t *e = NULL, *next = NULL;
    uint32_t effective_ttl;

    /* Programmer-error guards: invalid inputs from the caller. */
    ogs_assert(fqdn && list && count > 0);
    /*
     * target_count == 0 is legal: backup-only pool — all primary SRV targets
     * had no A records and backup IPs were promoted.  Guard against obviously
     * corrupt values only.
     */
    ogs_assert(target_count >= 0 && target_count <= MAX_SRV_CANDIDATES);
    /* tier0_count must be in (0, count].
     * Use a soft error instead of abort() so an unusual DNS topology or a
     * future bookkeeping regression doesn't crash the live MME process.
     * NOTE: the result is NOT cached — the next session for this APN will
     * re-query DNS.  This is intentional and safe; log at ERROR so operators
     * can see that caching was skipped. */
    if (tier0_count <= 0 || tier0_count > count) {
        ogs_error("S-NAPTR cache_store: invalid tier0_count=%d (count=%d) "
                  "for '%s' — result NOT cached; next session will re-query",
                  tier0_count, count, fqdn);
        return;
    }

    /*
     * Clamp the effective TTL to the range [S_NAPTR_CACHE_MIN_TTL_S,
     * S_NAPTR_CACHE_DEFAULT_TTL_S] (i.e. [30s, 86400s]).
     *
     *   Floor (30 s): prevents cache thrashing from misconfigured zones that
     *     return TTL 0 or a pathologically short value.
     *   Ceiling (86400 s): honours whatever TTL the DNS zone publishes up to
     *     a maximum of one day.  After that the cached PGW pool is always
     *     refreshed, ensuring long-lived processes do not retain arbitrarily
     *     stale topology indefinitely.
     */
    effective_ttl = ttl_s;
    if (effective_ttl < S_NAPTR_CACHE_MIN_TTL_S)
        effective_ttl = S_NAPTR_CACHE_MIN_TTL_S;
    if (effective_ttl > S_NAPTR_CACHE_DEFAULT_TTL_S)
        effective_ttl = S_NAPTR_CACHE_DEFAULT_TTL_S;

    /* Check if this FQDN is already cached — update in-place */
    ogs_list_for_each_safe(&s_naptr_cache, next, e) {
        if (strcmp(e->fqdn, fqdn) == 0) {
            /* Remove from current position; re-insert at tail below.
             * Zero the lnode after removal so ogs_list_add() starts with
             * clean prev/next pointers (defensive; OGS list_add overwrites
             * them anyway, but avoids any latent confusion). */
            ogs_list_remove(&s_naptr_cache, e);
            s_naptr_cache_entries--;
            ogs_free(e->list);
            e->list = NULL;
            memset(&e->lnode, 0, sizeof(e->lnode));
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
    e->count              = count;
    e->tier0_count        = tier0_count;
    e->tier0_target_count = target_count;
    if (target_count > 0)   /* 0 = backup-only pool; no target descriptors to copy */
        memcpy(e->tier0_targets, targets, target_count * sizeof(tier0_target_t));
    e->list               = ogs_calloc(count, sizeof(ogs_sockaddr_t));
    ogs_assert(e->list);
    memcpy(e->list, list, count * sizeof(ogs_sockaddr_t));
    e->expires_at         = time(NULL) + (time_t)effective_ttl;

    ogs_list_add(&s_naptr_cache, e);  /* newest entries at tail */
    s_naptr_cache_entries++;

    ogs_info("S-NAPTR cache: stored '%s' (%d candidates: %d primary across "
             "%d SRV target(s) / %d backup, TTL %us)",
             fqdn, count, tier0_count, target_count,
             count - tier0_count, effective_ttl);
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
 * query_naptr_all() - query NAPTR records and return ALL candidates at the
 * lowest ORDER value, sorted by preference ascending (most preferred first).
 *
 * Algorithm (TS 29.303 §5.2 / RFC 3958 §6.5):
 *   1. Filter records: service "x-3gpp-pgw:x-gtp-v2", flag "S".
 *   2. Keep only records at the lowest ORDER value.
 *   3. Sort the survivors by PREF ascending (lower pref = more preferred).
 *
 * Returns the count (≥ 1) and fills srv_names[0..count-1] on success.
 * Returns -1 on failure.
 *
 * Returning the full ordered list (rather than a single randomly-selected
 * entry) lets the caller fall back to the next candidate when a chosen
 * replacement has no usable SRV or A records, without re-querying DNS.
 */
static int query_naptr_all(
        struct __res_state *res,
        const char *fqdn,
        char srv_names[][NS_MAXDNAME],
        int max_count,
        int *count_out,
        uint32_t *min_ttl_out)
{
    unsigned char buf[DNS_BUF_LEN];
    int len, i, j, count = 0;
    uint16_t best_order = 0;
    uint32_t min_ttl = UINT32_MAX;
    ns_msg handle;
    ns_rr rr;
    naptr_rec_t candidates[MAX_NAPTR_CANDIDATES];
    naptr_rec_t cur, key;

    ogs_assert(count_out && min_ttl_out);
    *count_out   = 0;
    *min_ttl_out = 0;

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

        /* Track TTL across all qualifying records — the shortest drives expiry */
        if (ns_rr_ttl(rr) < min_ttl)
            min_ttl = ns_rr_ttl(rr);

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

    /*
     * Sort candidates by PREF ascending so the most-preferred SRV target
     * is tried first.  Falls back to the next entry when the chosen
     * replacement has no usable SRV/A data (handled in the caller).
     *
     * Insertion sort is O(n²) but n ≤ MAX_NAPTR_CANDIDATES (16).
     */
    for (i = 1; i < count; i++) {
        key = candidates[i];
        j   = i - 1;
        while (j >= 0 && candidates[j].pref > key.pref) {
            candidates[j + 1] = candidates[j];
            j--;
        }
        candidates[j + 1] = key;
    }

    /* Copy up to max_count sorted replacement strings to the output array */
    *count_out = (count < max_count) ? count : max_count;
    for (i = 0; i < *count_out; i++) {
        ogs_cpystrn(srv_names[i], candidates[i].replacement, NS_MAXDNAME);
        ogs_debug("S-NAPTR: NAPTR candidate[%d/%d] order=%u pref=%u → '%s'",
                  i + 1, *count_out,
                  candidates[i].order, candidates[i].pref, srv_names[i]);
    }

    *min_ttl_out = (min_ttl == UINT32_MAX) ? 0 : min_ttl;
    ogs_debug("S-NAPTR: %d NAPTR candidate(s) for '%s'; min TTL %us",
              *count_out, fqdn, *min_ttl_out);
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
 * query_srv_all() - query SRV records and return ALL targets across ALL
 * priority tiers, sorted by priority ascending then weight descending.
 *
 * RFC 2782 §3 mandates:
 *   1. Always try the lowest numeric priority value first.
 *   2. Within a priority tier use weight-proportional random selection.
 *   3. Only proceed to the next priority tier when every target in the
 *      current tier has been tried and failed.
 *
 * To honour rule (3) without discarding any backup targets this function:
 *   - Collects EVERY SRV record from the response, across all priorities.
 *   - Sorts them: primary sort by priority ASC (lower = preferred), secondary
 *     sort by weight DESC (higher weight = earlier within a tier).
 *   - Returns the total count and sets *tier0_count_out to the number of
 *     records at the lowest (most-preferred) priority value.
 *
 * The caller resolves A records for each target in order, so the resulting
 * IP array has all primary-tier IPs first and backup-tier IPs after them.
 * build_rotated_candidates() then applies random rotation only within the
 * primary-tier prefix, preserving the priority ordering for retries.
 *
 * Returns the total number of SRV records stored in out[] (≥1), or 0 on
 * failure.  *tier0_count_out is set to 0 on failure.
 */
static int query_srv_all(
        struct __res_state *res,
        const char *srv_name,
        srv_rec_t *out, int max_count,
        int *tier0_count_out,
        uint32_t *min_ttl_out)
{
    unsigned char buf[DNS_BUF_LEN];
    int len, i, j, count = 0;
    uint32_t min_ttl = UINT32_MAX;
    ns_msg handle;
    ns_rr rr;
    const unsigned char *rdata;
    srv_rec_t cur, key;
    uint16_t min_priority;

    ogs_assert(tier0_count_out);
    ogs_assert(min_ttl_out);
    *tier0_count_out = 0;
    *min_ttl_out     = 0;

    ogs_debug("S-NAPTR: SRV query (all tiers) for '%s'", srv_name);

    len = res_nquery(res, srv_name, C_IN, T_SRV, buf, sizeof(buf));
    if (len < 0) {
        ogs_warn("S-NAPTR: SRV query failed for '%s'", srv_name);
        return 0;
    }

    if (ns_initparse(buf, len, &handle) < 0) {
        ogs_error("S-NAPTR: ns_initparse (SRV) failed for '%s'", srv_name);
        return 0;
    }

    /* Pass 1: collect every SRV record regardless of priority */
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

        ogs_debug("S-NAPTR: SRV[%d] priority=%u weight=%u port=%u target='%s' "
                  "(TTL %us)",
                  i, cur.priority, cur.weight, cur.port, cur.target,
                  ns_rr_ttl(rr));

        /* Track minimum SRV TTL across ALL tiers — any SRV record expiring
         * early should shorten the cache lifetime for the whole pool. */
        if (ns_rr_ttl(rr) < min_ttl)
            min_ttl = ns_rr_ttl(rr);

        if (count < max_count) {
            out[count++] = cur;
        } else {
            ogs_warn("S-NAPTR: SRV record limit (%d) reached for '%s'; "
                     "extra records skipped", max_count, srv_name);
            break;
        }
    }

    if (count == 0) {
        ogs_warn("S-NAPTR: no SRV records found for '%s'", srv_name);
        return 0;
    }

    /*
     * Pass 2: insertion-sort by priority ASC, weight DESC.
     *
     * Insertion sort is O(n²) but n ≤ MAX_SRV_CANDIDATES (16) so it is
     * faster in practice than qsort() due to zero overhead and cache
     * friendliness on small arrays.
     *
     * Within the same priority, higher-weight targets sort earlier.  This
     * gives weight-influenced ordering within a tier while still exposing
     * every target to the retry mechanism.
     */
    for (i = 1; i < count; i++) {
        key = out[i];
        j   = i - 1;
        while (j >= 0 && (out[j].priority > key.priority ||
               (out[j].priority == key.priority &&
                out[j].weight   <  key.weight))) {
            out[j + 1] = out[j];
            j--;
        }
        out[j + 1] = key;
    }

    /* Pass 3: count tier0 (records at the lowest priority value) */
    min_priority = out[0].priority;
    for (i = 0; i < count; i++) {
        if (out[i].priority != min_priority)
            break;
        (*tier0_count_out)++;
    }

    *min_ttl_out = (min_ttl == UINT32_MAX) ? 0 : min_ttl;

    ogs_debug("S-NAPTR: %d SRV target(s) total; %d primary (priority=%u) "
              "+ %d backup for '%s'; SRV min TTL %us",
              count, *tier0_count_out, min_priority,
              count - *tier0_count_out, srv_name, *min_ttl_out);
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

        {
            char _addr[INET_ADDRSTRLEN];
            ogs_debug("S-NAPTR: A[%d] %s:%u (TTL %us)",
                      count,
                      inet_ntop(AF_INET, &out[count].sin.sin_addr,
                                _addr, sizeof(_addr)),
                      port, ns_rr_ttl(rr));
        }
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
 * build_rotated_candidates() - RFC 2782-compliant weighted candidate array.
 *
 * INPUT LAYOUT
 *   addrs[0 .. tier0_total-1]  Primary-tier IPs, grouped by SRV target.
 *                               targets[i] describes each group:
 *                                 .start   = first index of this group
 *                                 .count   = IPs in this group
 *                                 .weight  = SRV weight (0 → 1 per RFC 2782)
 *   addrs[tier0_total .. n-1]  Backup-tier IPs (SRV priority > min), unchanged.
 *
 * SELECTION ALGORITHM (RFC 2782 §3 at the SRV-target level)
 *   1. Compute total_weight = Σ effective_weight[i]  (0 → 1).
 *   2. Pick r = ogs_random32() % total_weight  → select SRV target group.
 *      A target with weight=99 is selected 99× more often than weight=1.
 *   3. Within the selected group, pick a random starting IP uniformly.
 *
 * OUTPUT LAYOUT
 *   result[0]                  Random IP from the weight-selected target.
 *   result[1 .. sel_count-1]   Remaining IPs of the selected target.
 *   result[sel_count ..]       IPs of OTHER primary targets in weight-DESC
 *                               order (they are already weight-sorted in
 *                               addrs[] by query_srv_all()).
 *   result[tier0_total ..]     Backup-tier IPs (unchanged order).
 *
 * This ensures:
 *   - Load balancing: high-weight PGWs receive proportionally more new
 *     sessions (DNS weight=99:1 → ~99% vs ~1% initial selection).
 *   - Failover within tier: after the preferred PGW's IPs are exhausted,
 *     retry advances to the next-highest-weight primary PGW.
 *   - Priority failover: backup tier is only reached after all primary
 *     IPs are exhausted (RFC 2782 priority semantics).
 *
 * Edge cases:
 *   t0_count == 0  → no target descriptors; uniform rotation of all IPs.
 *   n == 1         → single candidate; copy directly, no selection needed.
 *
 * The caller owns the returned array and must ogs_free() it.
 */
static ogs_sockaddr_t *build_rotated_candidates(
        const ogs_sockaddr_t *addrs, int n,
        const tier0_target_t *targets, int t0_count,
        int *count_out)
{
    ogs_sockaddr_t *result = NULL;
    int out = 0, sel = 0, start_ip = 0, tier0_total = 0, i, j;
    uint32_t total_weight = 0, pick, cum;

    ogs_assert(addrs && n > 0 && count_out);

    result = ogs_calloc(n, sizeof(ogs_sockaddr_t));
    ogs_assert(result);

    /* ------------------------------------------------------------------
     * Fast path: no target descriptors (should not normally occur, but
     * handles the edge case gracefully with uniform rotation).
     * ------------------------------------------------------------------ */
    if (t0_count <= 0) {
        int preferred = (n > 1) ? (int)(ogs_random32() % (uint32_t)n) : 0;
        for (i = 0; i < n; i++)
            result[i] = addrs[(preferred + i) % n];
        *count_out = n;
        return result;
    }

    /* Compute total primary-tier IP count from target descriptors */
    for (i = 0; i < t0_count; i++)
        tier0_total += targets[i].count;

    /* ------------------------------------------------------------------
     * Step 1: RFC 2782 §3 weighted random selection of the SRV target.
     *
     * Weight is applied at the SRV-target (hostname) level, not the IP
     * level.  A target with weight=99 is 99× more likely to be selected
     * as the preferred starting group than one with weight=1, regardless
     * of how many A records each target resolved to.
     * ------------------------------------------------------------------ */
    for (i = 0; i < t0_count; i++)
        total_weight += targets[i].weight ? (uint32_t)targets[i].weight : 1u;

    pick = ogs_random32() % total_weight;
    sel  = t0_count - 1;   /* safe default: last target */
    cum  = 0;
    for (i = 0; i < t0_count; i++) {
        cum += targets[i].weight ? (uint32_t)targets[i].weight : 1u;
        if (pick < cum) { sel = i; break; }
    }

    /* ------------------------------------------------------------------
     * Step 2: random start within the selected target's IP group.
     * ------------------------------------------------------------------ */
    start_ip = (targets[sel].count > 1)
                   ? (int)(ogs_random32() % (uint32_t)targets[sel].count)
                   : 0;

    for (j = 0; j < targets[sel].count; j++)
        result[out++] = addrs[targets[sel].start +
                               (start_ip + j) % targets[sel].count];

    /* ------------------------------------------------------------------
     * Step 3: append other primary-tier targets in their original order
     * (weight-DESC as produced by query_srv_all()'s insertion sort).
     * ------------------------------------------------------------------ */
    for (i = 0; i < t0_count; i++) {
        if (i == sel)
            continue;
        for (j = 0; j < targets[i].count; j++)
            result[out++] = addrs[targets[i].start + j];
    }

    /* ------------------------------------------------------------------
     * Step 4: append backup-tier IPs unchanged.
     * ------------------------------------------------------------------ */
    for (i = tier0_total; i < n; i++)
        result[out++] = addrs[i];

    /* Sanity: every IP must have been placed exactly once.  Use a soft error
     * rather than abort() so a bookkeeping inconsistency doesn't crash the
     * live MME during a session setup. */
    if (out != n) {
        ogs_error("S-NAPTR build_rotated: placement error (out=%d n=%d); "
                  "dropping result to avoid partial candidate array", out, n);
        ogs_free(result);
        *count_out = 0;
        return NULL;
    }

    ogs_debug("S-NAPTR: weighted selection: target[%d/%d] weight=%u "
              "(total_weight=%u) start_ip=%d",
              sel + 1, t0_count,
              targets[sel].weight ? targets[sel].weight : 1,
              total_weight, start_ip);

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
 * §19.4.2.  Cache lifetime = min(NAPTR TTL, SRV TTL, A TTL) seconds,
 *             clamped to [S_NAPTR_CACHE_MIN_TTL_S, S_NAPTR_CACHE_DEFAULT_TTL_S]
 *             i.e. [30 s, 86400 s].
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
    char naptr_srvs[MAX_NAPTR_CANDIDATES][NS_MAXDNAME]; /* ordered SRV names from NAPTR */
    int  naptr_count = 0, ni = 0;
    struct __res_state res;
    ogs_sockaddr_t tmp[MAX_A_CANDIDATES];
    ogs_sockaddr_t *result = NULL;
    srv_rec_t srvs[MAX_SRV_CANDIDATES];
    uint32_t min_ttl = UINT32_MAX;       /* minimum A-record TTL across all targets */
    uint32_t naptr_ttl = UINT32_MAX;     /* minimum NAPTR TTL from the NAPTR response */
    /* TTL of the SRV response for the successful NAPTR candidate.
     * Reset to UINT32_MAX at the start of each NAPTR loop iteration and
     * overwritten by query_srv_all() when SRV records are found.  The NAPTR
     * loop breaks on the first candidate that yields A records, so only that
     * candidate's SRV TTL contributes to the effective cache TTL. */
    uint32_t chosen_srv_ttl = UINT32_MAX;
    uint32_t a_ttl = 0;    /* per-SRV-target A-record TTL from query_a_all() */
    int n = 0, srv_count = 0, tier0_srv_count = 0, tier0_ip_count = 0;
    int s = 0, got = 0;
    tier0_target_t tier0_targets[MAX_SRV_CANDIDATES];
    int tier0_target_count = 0;
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
     * The cache stores the FULL candidate pool sorted by SRV priority:
     *   list[0 .. tier0_count-1]   primary-tier IPs  (SRV priority == min)
     *   list[tier0_count .. count-1] backup-tier IPs  (SRV priority >  min)
     *
     * build_rotated_candidates() applies random rotation only within the
     * primary prefix so load is distributed across primary PGWs while
     * backup PGWs remain at the tail for RFC 2782 priority-ordered failover.
     * ------------------------------------------------------------------ */
    cached = cache_lookup(fqdn);
    if (cached) {
        result = build_rotated_candidates(cached->list, cached->count,
                                          cached->tier0_targets,
                                          cached->tier0_target_count, count);
        if (result && *count > 0) {
            char _addr[INET_ADDRSTRLEN];
            ogs_info("S-NAPTR: [CACHE HIT] APN[%s] → %d PGW candidate(s) "
                     "(%d primary / %d backup); preferred %s",
                     apn, *count, cached->tier0_count,
                     cached->count - cached->tier0_count,
                     inet_ntop(AF_INET, &result[0].sin.sin_addr,
                               _addr, sizeof(_addr)));
        } else {
            /* build_rotated_candidates() hit a placement-error guard and
             * returned NULL.  The session will fall back to static PGW config.
             * Log so the operator knows the cache entry is effectively unusable. */
            ogs_warn("S-NAPTR: [CACHE HIT] candidate rotation failed for "
                     "APN[%s]; falling back to static PGW config", apn);
        }
        return result;
    }

    /* ------------------------------------------------------------------
     * Cache MISS — full DNS resolution: NAPTR → SRV (all tiers) → A.
     *
     * srvs[] is sorted by priority ASC so processing in order naturally
     * produces a priority-partitioned tmp[]:
     *   tmp[0 .. tier0_ip_count-1]  primary-tier IPs
     *   tmp[tier0_ip_count .. n-1]  backup-tier IPs
     * ------------------------------------------------------------------ */
    ogs_debug("S-NAPTR: [CACHE MISS] APN[%s] FQDN[%s]", apn, fqdn);

    if (init_resolver(&res) != 0)
        return NULL;

    /*
     * Step 1: get all lowest-ORDER NAPTR candidates sorted by pref ASC.
     * Returns an ordered array — most-preferred SRV name first.
     */
    if (query_naptr_all(&res, fqdn, naptr_srvs, MAX_NAPTR_CANDIDATES,
                        &naptr_count, &naptr_ttl) != 0)
        goto out;

    /*
     * Step 2: try each NAPTR candidate in preference order until one yields
     * usable A records.  A broken DNS zone on the first replacement no longer
     * makes discovery fail when another valid lowest-order NAPTR answer was
     * returned in the same response.
     *
     * For each candidate:
     *   a. Query all SRV targets across all priority tiers.
     *   b. Resolve A records for each SRV target (primary tier first).
     *   c. If at least one A record is found, commit and break.
     *   d. Otherwise warn and advance to the next NAPTR candidate.
     *
     * The priority-partitioned layout of tmp[] is the same as before:
     *   tmp[0 .. tier0_ip_count-1]  primary-tier IPs  (SRV priority == min)
     *   tmp[tier0_ip_count .. n-1]  backup-tier IPs   (SRV priority >  min)
     */
    for (ni = 0; ni < naptr_count; ni++) {
        /* Reset per-candidate accumulators before each attempt */
        n                 = 0;
        tier0_ip_count    = 0;
        tier0_target_count = 0;
        srv_count         = 0;
        tier0_srv_count   = 0;
        chosen_srv_ttl    = UINT32_MAX;   /* reset: set by query_srv_all()
                                           * on success; must not carry over
                                           * from a previous failed candidate */

        srv_count = query_srv_all(&res, naptr_srvs[ni], srvs,
                                  MAX_SRV_CANDIDATES,
                                  &tier0_srv_count, &chosen_srv_ttl);
        if (srv_count == 0) {
            ogs_warn("S-NAPTR: NAPTR[%d/%d] '%s' → no SRV records; "
                     "trying next candidate",
                     ni + 1, naptr_count, naptr_srvs[ni]);
            continue;
        }

        /*
         * Primary tier: srvs[0 .. tier0_srv_count-1], weight DESC.
         * Record tier0_target_t descriptors for build_rotated_candidates().
         */
        for (s = 0; s < tier0_srv_count && n < MAX_A_CANDIDATES; s++) {
            int before = n;
            got = query_a_all(&res, srvs[s].target, srvs[s].port,
                              tmp + n, MAX_A_CANDIDATES - n, &a_ttl);
            if (got > 0) {
                ogs_debug("S-NAPTR: primary SRV '%s:%u' weight=%u → %d addr(s)",
                          srvs[s].target, srvs[s].port, srvs[s].weight, got);
                tier0_targets[tier0_target_count].weight = srvs[s].weight;
                tier0_targets[tier0_target_count].start  = before;
                tier0_targets[tier0_target_count].count  = got;
                tier0_target_count++;
                n += got;
                if (a_ttl < min_ttl)
                    min_ttl = a_ttl;
            } else {
                ogs_warn("S-NAPTR: primary SRV target '%s' has no A records "
                         "— skipped", srvs[s].target);
            }
        }
        tier0_ip_count = n;  /* boundary: tmp[0..tier0_ip_count-1] are primary */

        /* Backup tiers: srvs[tier0_srv_count .. srv_count-1] */
        for (s = tier0_srv_count; s < srv_count && n < MAX_A_CANDIDATES; s++) {
            got = query_a_all(&res, srvs[s].target, srvs[s].port,
                              tmp + n, MAX_A_CANDIDATES - n, &a_ttl);
            if (got > 0) {
                ogs_debug("S-NAPTR: backup SRV '%s:%u' (priority=%u) → %d addr(s)",
                          srvs[s].target, srvs[s].port, srvs[s].priority, got);
                n += got;
                if (a_ttl < min_ttl)
                    min_ttl = a_ttl;
            } else {
                ogs_warn("S-NAPTR: backup SRV target '%s' has no A records "
                         "— skipped", srvs[s].target);
            }
        }

        if (n > 0) {
            if (ni > 0)
                ogs_warn("S-NAPTR: NAPTR[%d/%d] '%s' succeeded after "
                         "%d broken candidate(s)",
                         ni + 1, naptr_count, naptr_srvs[ni], ni);
            break;   /* found usable addresses — done */
        }
        ogs_warn("S-NAPTR: NAPTR[%d/%d] '%s' → SRV records present but "
                 "no A records resolved; trying next candidate",
                 ni + 1, naptr_count, naptr_srvs[ni]);
    }

    if (n == 0)
        goto out;

    /*
     * If all primary SRV targets had no A records, promote backup-tier IPs
     * to the primary tier so the session has candidates to try.
     */
    if (tier0_ip_count == 0 || tier0_target_count == 0) {
        tier0_ip_count     = n;
        tier0_target_count = 0;  /* signal fast-path in build_rotated_candidates */
    }

    /*
     * Effective cache lifetime = min(NAPTR TTL, SRV TTL, A TTL).
     *
     * Each DNS level in the NAPTR→SRV→A chain has its own TTL:
     *   - naptr_ttl     : how long the APN→SRV-name mapping is valid
     *   - chosen_srv_ttl : how long any SRV target mapping is valid
     *   - min_ttl       : the shortest A-record TTL across all targets
     *
     * If an operator reduces the NAPTR TTL to trigger rapid re-discovery
     * (e.g. during a PGW migration) we must honour that.  Using only the
     * A-record TTL would keep a stale PGW pool until the A records expire.
     *
     * Take the minimum across all three layers.  If a layer returned 0
     * (nothing captured), skip it — the cache_store() floor will apply.
     */
    {
        /* Snapshot the A-record minimum before folding in NAPTR/SRV values
         * so we can log all three contributions separately. */
        uint32_t a_chain_ttl = (min_ttl == UINT32_MAX) ? 0 : min_ttl;

        if (naptr_ttl != UINT32_MAX && naptr_ttl < min_ttl)
            min_ttl = naptr_ttl;
        if (chosen_srv_ttl != UINT32_MAX && chosen_srv_ttl < min_ttl)
            min_ttl = chosen_srv_ttl;
        if (min_ttl == UINT32_MAX)
            min_ttl = 0;   /* nothing captured at any level — use cache_store() floor */

        ogs_debug("S-NAPTR: effective cache TTL for APN[%s]: "
                  "NAPTR=%us SRV=%us A=%us → min=%us",
                  apn,
                  naptr_ttl     == UINT32_MAX ? 0 : naptr_ttl,
                  chosen_srv_ttl == UINT32_MAX ? 0 : chosen_srv_ttl,
                  a_chain_ttl,
                  min_ttl);
    }

    /*
     * Store the full priority-partitioned pool in cache together with the
     * per-target weight descriptors so cache hits reproduce the same
     * RFC 2782-weighted selection without re-querying DNS.
     */
    cache_store(fqdn, tmp, n,
                tier0_ip_count, tier0_targets, tier0_target_count,
                min_ttl);

    /* Build the caller's rotated copy using weighted target selection */
    result = build_rotated_candidates(tmp, n,
                                      tier0_targets, tier0_target_count,
                                      count);

    if (result && *count > 0) {
        char _addr[INET_ADDRSTRLEN];
        ogs_info("S-NAPTR: [DNS] resolved APN[%s] → %d PGW candidate(s) "
                 "(%d primary across %d SRV target(s) / %d backup); "
                 "preferred %s (DNS TTL %us, now cached)",
                 apn, *count, tier0_ip_count, tier0_target_count,
                 n - tier0_ip_count,
                 inet_ntop(AF_INET, &result[0].sin.sin_addr,
                           _addr, sizeof(_addr)),
                 min_ttl);
    }

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
