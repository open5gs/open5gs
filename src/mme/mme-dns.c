/*
 * Copyright (C) 2024 by sysmocom - s.f.m.c. GmbH <info@sysmocom.de>
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
 * TS 29.303 DNS S-NAPTR based PGW (S5/S8) discovery for the MME.
 *
 * This is an opt-in fallback used only when the HSS provides no PGW address
 * and no static peer matches. The resolver is intentionally simple and
 * synchronous (blocking libresolv) with a TTL cache so that only the first
 * attach per APN/PLMN performs a DNS round trip; see mme-dns.h.
 *
 * Supported NAPTR terminal flags: "a" (replacement is an A/AAAA owner name)
 * and "s" (replacement is an SRV owner name). Non-terminal (empty flag)
 * recursive NAPTR chains and APN-OI-Replacement (TS 23.003 9.1.2) are out of
 * scope for this version.
 */

#include <ctype.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/nameser.h>
#include <resolv.h>

#include "mme-context.h"
#include "mme-dns.h"

#define MME_DNS_MAX_CANDIDATES  16
#define MME_DNS_NEG_TTL_SEC     30
#define MME_DNS_ANSWER_LEN      (NS_PACKETSZ * 4)

typedef struct mme_dns_cache_s {
    ogs_lnode_t     lnode;
    char            fqdn[OGS_MAX_FQDN_LEN+1];
    ogs_sockaddr_t  *sa_list;   /* NULL means a cached negative result */
    ogs_time_t      expire;     /* monotonic usec */
} mme_dns_cache_t;

static struct __res_state mme_res;
static bool initialized = false;
static ogs_list_t cache_list;

void mme_dns_init(void)
{
    if (initialized) return;

    ogs_list_init(&cache_list);

    if (res_ninit(&mme_res) != 0) {
        ogs_error("res_ninit() failed; PGW DNS discovery disabled at runtime");
        return;
    }

    initialized = true;
}

void mme_dns_final(void)
{
    mme_dns_cache_t *e = NULL, *next = NULL;

    if (!initialized) return;

    ogs_list_for_each_safe(&cache_list, next, e) {
        ogs_list_remove(&cache_list, e);
        if (e->sa_list)
            ogs_freeaddrinfo(e->sa_list);
        ogs_free(e);
    }

    res_nclose(&mme_res);
    initialized = false;
}

/* ------------------------------------------------------------------ cache */

static mme_dns_cache_t *cache_find(const char *fqdn)
{
    mme_dns_cache_t *e = NULL, *next = NULL;
    ogs_time_t now = ogs_get_monotonic_time();

    ogs_list_for_each_safe(&cache_list, next, e) {
        if (now >= e->expire) {
            ogs_list_remove(&cache_list, e);
            if (e->sa_list)
                ogs_freeaddrinfo(e->sa_list);
            ogs_free(e);
            continue;
        }
        if (strcmp(e->fqdn, fqdn) == 0)
            return e;
    }

    return NULL;
}

/* Takes ownership of sa_list (NULL for a negative entry). */
static void cache_store(const char *fqdn, ogs_sockaddr_t *sa_list,
        uint32_t ttl_sec)
{
    mme_dns_cache_t *e = ogs_calloc(1, sizeof(*e));
    if (!e) {
        if (sa_list)
            ogs_freeaddrinfo(sa_list);
        return;
    }

    ogs_cpystrn(e->fqdn, fqdn, sizeof(e->fqdn));
    e->sa_list = sa_list;
    e->expire = ogs_get_monotonic_time() +
        (ogs_time_t)ttl_sec * OGS_USEC_PER_SEC;
    ogs_list_add(&cache_list, e);
}

/* ----------------------------------------------------------- NAPTR parsing */

static void copy_charstr(char *dst, size_t dstsz,
        const unsigned char *src, uint8_t len)
{
    if (len >= dstsz)
        len = dstsz - 1;
    memcpy(dst, src, len);
    dst[len] = '\0';
}

static bool ci_contains(const char *haystack, const char *needle)
{
    size_t nlen = strlen(needle);

    if (!nlen)
        return true;
    for (; *haystack; haystack++)
        if (strncasecmp(haystack, needle, nlen) == 0)
            return true;

    return false;
}

static bool has_flag(const char *flags, char f)
{
    for (; *flags; flags++)
        if (tolower((unsigned char)*flags) == f)
            return true;

    return false;
}

/* TS 29.303: service field carries "x-3gpp-pgw" plus protocol tag(s). */
bool mme_dns_service_matches(const char *services, int iface)
{
    if (!services)
        return false;
    if (!ci_contains(services, "x-3gpp-pgw"))
        return false;

    switch (iface) {
    case MME_PGW_DISCOVERY_IFACE_S5:
        return ci_contains(services, "x-s5-gtp");
    case MME_PGW_DISCOVERY_IFACE_BOTH:
        return ci_contains(services, "x-s8-gtp") ||
               ci_contains(services, "x-s5-gtp");
    case MME_PGW_DISCOVERY_IFACE_S8:
    default:
        return ci_contains(services, "x-s8-gtp");
    }
}

static int naptr_cmp(const void *a, const void *b)
{
    const mme_dns_naptr_t *x = a, *y = b;

    if (x->order != y->order)
        return (int)x->order - (int)y->order;
    return (int)x->preference - (int)y->preference;
}

/*
 * Pure parser: decode a raw DNS NAPTR response, filter by interface mode, and
 * sort by Order/Preference. No resolver or global state; unit-tested directly.
 */
int mme_dns_parse_naptr(const unsigned char *answer, int answer_len,
        int iface, mme_dns_naptr_t *out, int max)
{
    int count, i, n = 0;
    ns_msg handle;

    if (!answer || answer_len <= 0 || !out || max <= 0)
        return 0;

    if (ns_initparse(answer, answer_len, &handle) < 0) {
        ogs_error("ns_initparse(NAPTR) failed");
        return 0;
    }

    count = ns_msg_count(handle, ns_s_an);
    for (i = 0; i < count && n < max; i++) {
        ns_rr rr;
        const unsigned char *p;
        uint8_t slen;
        int explen;
        mme_dns_naptr_t cand;

        if (ns_parserr(&handle, ns_s_an, i, &rr) < 0)
            continue;
        if (ns_rr_type(rr) != ns_t_naptr)
            continue;

        memset(&cand, 0, sizeof(cand));
        p = ns_rr_rdata(rr);

        cand.order = ns_get16(p); p += 2;
        cand.preference = ns_get16(p); p += 2;

        slen = *p++; copy_charstr(cand.flags, sizeof(cand.flags), p, slen);
        p += slen;
        slen = *p++; copy_charstr(cand.services, sizeof(cand.services), p, slen);
        p += slen;
        slen = *p++; /* regexp: unused (TS 29.303 uses replacement form) */
        p += slen;

        explen = dn_expand(ns_msg_base(handle), ns_msg_end(handle), p,
                cand.replacement, sizeof(cand.replacement));
        if (explen < 0)
            continue;

        if (!mme_dns_service_matches(cand.services, iface))
            continue;

        cand.ttl = ns_rr_ttl(rr);
        out[n++] = cand;
    }

    if (n > 1)
        qsort(out, n, sizeof(mme_dns_naptr_t), naptr_cmp);

    return n;
}

static int naptr_lookup(const char *fqdn, mme_dns_naptr_t *out, int max)
{
    unsigned char answer[MME_DNS_ANSWER_LEN];
    int len;

    len = res_nquery(&mme_res, fqdn, ns_c_in, ns_t_naptr,
            answer, sizeof(answer));
    if (len < 0) {
        ogs_debug("res_nquery(NAPTR, %s) returned no answer", fqdn);
        return 0;
    }

    return mme_dns_parse_naptr(answer, len,
            mme_self()->pgw_discovery.iface, out, max);
}

/* ------------------------------------------------------------- SRV lookup */

static ogs_sockaddr_t *srv_resolve(const char *srvname, uint32_t *ttl_out)
{
    unsigned char answer[MME_DNS_ANSWER_LEN];
    int len, count, i, explen;
    ns_msg handle;
    ogs_sockaddr_t *result = NULL;
    int best_prio = -1;
    char best_target[NS_MAXDNAME] = "";
    uint32_t best_ttl = MME_DNS_NEG_TTL_SEC;

    len = res_nquery(&mme_res, srvname, ns_c_in, ns_t_srv,
            answer, sizeof(answer));
    if (len < 0)
        return NULL;
    if (ns_initparse(answer, len, &handle) < 0)
        return NULL;

    count = ns_msg_count(handle, ns_s_an);
    for (i = 0; i < count; i++) {
        ns_rr rr;
        const unsigned char *p;
        uint16_t prio;
        char target[NS_MAXDNAME];

        if (ns_parserr(&handle, ns_s_an, i, &rr) < 0)
            continue;
        if (ns_rr_type(rr) != ns_t_srv)
            continue;

        p = ns_rr_rdata(rr);
        prio = ns_get16(p); p += 2;     /* priority */
        p += 2;                         /* weight: simplified (ignored) */
        p += 2;                         /* port: GTP-C carries IP only */

        explen = dn_expand(ns_msg_base(handle), ns_msg_end(handle), p,
                target, sizeof(target));
        if (explen < 0)
            continue;

        if (best_prio < 0 || prio < best_prio) {
            best_prio = prio;
            ogs_cpystrn(best_target, target, sizeof(best_target));
            best_ttl = ns_rr_ttl(rr);
        }
    }

    if (best_target[0] == '\0')
        return NULL;
    if (ogs_getaddrinfo(&result, AF_UNSPEC, best_target, 0, 0) != OGS_OK)
        return NULL;

    if (ttl_out)
        *ttl_out = best_ttl;

    return result;
}

/* ----------------------------------------------------------------- public */

int mme_dns_apn_fqdn(const char *apn, const ogs_plmn_id_t *home_plmn,
        char *buf, size_t sz)
{
    char *epc_domain = NULL;

    if (!apn || !home_plmn || !buf || sz == 0)
        return OGS_ERROR;

    /* APN-FQDN: <apn-NI>.apn.epc.mnc<MNC>.mcc<MCC>.3gppnetwork.org */
    epc_domain = ogs_epc_domain_from_plmn_id(home_plmn);
    if (!epc_domain)
        return OGS_ERROR;

    ogs_snprintf(buf, sz, "%s.apn.%s", apn, epc_domain);
    ogs_free(epc_domain);

    return OGS_OK;
}

int mme_dns_resolve_pgw(
        const char *apn, const ogs_plmn_id_t *home_plmn,
        ogs_sockaddr_t **sa_list)
{
    char fqdn[OGS_MAX_FQDN_LEN+1];
    mme_dns_cache_t *cached = NULL;
    mme_dns_naptr_t candidates[MME_DNS_MAX_CANDIDATES];
    int num_candidates, i;
    ogs_sockaddr_t *result = NULL;
    uint32_t result_ttl = MME_DNS_NEG_TTL_SEC;

    ogs_assert(sa_list);
    *sa_list = NULL;

    if (!mme_self()->pgw_discovery.enabled)
        return OGS_ERROR;
    if (!initialized) {
        ogs_error("PGW DNS discovery requested but resolver is unavailable");
        return OGS_ERROR;
    }
    if (!apn || !home_plmn)
        return OGS_ERROR;

    if (mme_dns_apn_fqdn(apn, home_plmn, fqdn, sizeof(fqdn)) != OGS_OK)
        return OGS_ERROR;

    cached = cache_find(fqdn);
    if (cached) {
        if (!cached->sa_list) {
            ogs_debug("PGW discovery: negative cache hit for %s", fqdn);
            return OGS_ERROR;
        }
        ogs_debug("PGW discovery: cache hit for %s", fqdn);
        ogs_copyaddrinfo(sa_list, cached->sa_list);
        return OGS_OK;
    }

    num_candidates = naptr_lookup(fqdn, candidates, MME_DNS_MAX_CANDIDATES);
    if (num_candidates <= 0) {
        ogs_warn("PGW discovery: no matching NAPTR record for %s", fqdn);
        cache_store(fqdn, NULL, MME_DNS_NEG_TTL_SEC);
        return OGS_ERROR;
    }

    for (i = 0; i < num_candidates && !result; i++) {
        mme_dns_naptr_t *c = &candidates[i];

        result_ttl = c->ttl ? c->ttl : MME_DNS_NEG_TTL_SEC;

        if (has_flag(c->flags, 'a')) {
            if (ogs_getaddrinfo(&result, AF_UNSPEC,
                        c->replacement, 0, 0) != OGS_OK)
                result = NULL;
        } else if (has_flag(c->flags, 's')) {
            result = srv_resolve(c->replacement, &result_ttl);
        } else {
            ogs_warn("PGW discovery: unsupported NAPTR flag '%s' for %s "
                    "(this version supports 'a' and 's')", c->flags, fqdn);
        }
    }

    if (!result) {
        ogs_warn("PGW discovery: could not resolve a PGW address for %s", fqdn);
        cache_store(fqdn, NULL, MME_DNS_NEG_TTL_SEC);
        return OGS_ERROR;
    }

    ogs_info("PGW discovery: resolved %s via S-NAPTR", fqdn);

    /* Cache owns 'result'; hand the caller an independent copy. */
    ogs_copyaddrinfo(sa_list, result);
    cache_store(fqdn, result, result_ttl);

    return OGS_OK;
}
