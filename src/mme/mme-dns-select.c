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

#include "mme-dns-select.h"

int mme_dns_build_apn_fqdn(char *buf, size_t sz,
        const char *apn_ni, const ogs_plmn_id_t *plmn_id)
{
    int rc;

    ogs_assert(buf);
    ogs_assert(plmn_id);

    if (!apn_ni || !apn_ni[0])
        return OGS_ERROR;

    rc = snprintf(buf, sz, "%s.apn.epc.mnc%03d.mcc%03d.3gppnetwork.org",
            apn_ni, ogs_plmn_id_mnc(plmn_id), ogs_plmn_id_mcc(plmn_id));
    if (rc <= 0 || (size_t)rc >= sz)
        return OGS_ERROR;

    return OGS_OK;
}

int mme_dns_build_tai_fqdn(char *buf, size_t sz, const ogs_eps_tai_t *tai)
{
    int rc;

    ogs_assert(buf);
    ogs_assert(tai);

    rc = snprintf(buf, sz,
            "tac-lb%02x.tac-hb%02x.tac.epc.mnc%03d.mcc%03d.3gppnetwork.org",
            tai->tac & 0xff, (tai->tac >> 8) & 0xff,
            ogs_plmn_id_mnc(&tai->plmn_id), ogs_plmn_id_mcc(&tai->plmn_id));
    if (rc <= 0 || (size_t)rc >= sz)
        return OGS_ERROR;

    return OGS_OK;
}

mme_dns_proto_e mme_dns_decide_proto(const char *imsi_bcd,
        const ogs_plmn_id_t *serving_plmn, mme_dns_proto_e override)
{
    char prefix[OGS_PLMNIDSTRLEN];
    int rc;

    if (override != MME_DNS_PROTO_AUTO)
        return override;

    ogs_assert(serving_plmn);

    if (!imsi_bcd || !imsi_bcd[0])
        return MME_DNS_PROTO_S5;

    rc = snprintf(prefix, sizeof(prefix), "%03d%0*d",
            ogs_plmn_id_mcc(serving_plmn),
            ogs_plmn_id_mnc_len(serving_plmn),
            ogs_plmn_id_mnc(serving_plmn));
    if (rc <= 0 || (size_t)rc >= sizeof(prefix))
        return MME_DNS_PROTO_S5;

    if (strncmp(imsi_bcd, prefix, strlen(prefix)) == 0)
        return MME_DNS_PROTO_S5;

    return MME_DNS_PROTO_S8;
}

static bool token_equal_nocase(const char *tok, size_t len, const char *ref)
{
    if (strlen(ref) != len)
        return false;
    return strncasecmp(tok, ref, len) == 0;
}

bool mme_dns_service_match(const char *service,
        mme_dns_service_e svc, mme_dns_proto_e proto)
{
    const char *node_tag = NULL;
    const char *p = NULL, *sep = NULL;
    size_t len;
    bool proto_found = false;

    if (!service || !service[0])
        return false;

    switch (svc) {
    case MME_DNS_SVC_SGW:
        node_tag = "x-3gpp-sgw";
        break;
    case MME_DNS_SVC_PGW:
        node_tag = "x-3gpp-pgw";
        break;
    default:
        return false;
    }

    /* First token: the node type */
    sep = strchr(service, ':');
    len = sep ? (size_t)(sep - service) : strlen(service);
    if (!token_equal_nocase(service, len, node_tag))
        return false;
    if (!sep) /* no protocol tokens at all */
        return false;

    /* Remaining tokens: protocols */
    p = sep + 1;
    while (p && *p) {
        sep = strchr(p, ':');
        len = sep ? (size_t)(sep - p) : strlen(p);

        if (proto == MME_DNS_PROTO_S5 || proto == MME_DNS_PROTO_AUTO)
            if (token_equal_nocase(p, len, "x-s5-gtp"))
                proto_found = true;
        if (proto == MME_DNS_PROTO_S8 || proto == MME_DNS_PROTO_AUTO)
            if (token_equal_nocase(p, len, "x-s8-gtp"))
                proto_found = true;

        p = sep ? sep + 1 : NULL;
    }

    return proto_found;
}

void mme_dns_canonical_name(const char *fqdn, char *canon, size_t sz)
{
    const char *rest = NULL, *second = NULL;

    ogs_assert(fqdn);
    ogs_assert(canon);
    ogs_assert(sz > 0);

    if (strncasecmp(fqdn, "topon.", 6) == 0)
        rest = fqdn + 6;
    else if (strncasecmp(fqdn, "topoff.", 7) == 0)
        rest = fqdn + 7;

    if (rest) {
        /* Skip the single interface label following the prefix */
        second = strchr(rest, '.');
        if (second && second[1])
            fqdn = second + 1;
    }

    ogs_cpystrn(canon, fqdn, sz);
}

static bool flags_terminal(const char *flags, bool *needs_srv)
{
    if (!flags)
        return false;
    if (flags[0] && !flags[1]) {
        if (flags[0] == 'a' || flags[0] == 'A') {
            *needs_srv = false;
            return true;
        }
        if (flags[0] == 's' || flags[0] == 'S') {
            *needs_srv = true;
            return true;
        }
    }
    /* Empty (non-terminal NAPTR) and any other flag are not supported */
    return false;
}

int mme_dns_candidates_from_naptr(
        const mme_dns_naptr_record_t *recs, int num_recs,
        mme_dns_service_e svc, mme_dns_proto_e proto,
        mme_dns_candidate_t *cand, int max_cand)
{
    const mme_dns_naptr_record_t *sorted[OGS_MAX_NUM_OF_HOSTNAME * 4];
    int num_sorted = 0;
    int num_cand = 0;
    int i, j, k;

    ogs_assert(cand);
    ogs_assert(max_cand > 0);

    if (!recs || num_recs <= 0)
        return 0;

    /* Filter + insertion sort: stable on (order, preference) */
    for (i = 0; i < num_recs && num_sorted < (int)OGS_ARRAY_SIZE(sorted);
            i++) {
        const mme_dns_naptr_record_t *r = &recs[i];
        bool needs_srv = false;

        if (!flags_terminal(r->flags, &needs_srv))
            continue;
        if (!mme_dns_service_match(r->service, svc, proto))
            continue;
        if (!r->replacement[0] || strcmp(r->replacement, ".") == 0)
            continue;

        for (j = 0; j < num_sorted; j++) {
            if (sorted[j]->order > r->order ||
                (sorted[j]->order == r->order &&
                 sorted[j]->preference > r->preference))
                break;
        }
        for (k = num_sorted; k > j; k--)
            sorted[k] = sorted[k-1];
        sorted[j] = r;
        num_sorted++;
    }

    /* Dedup on canonical name, best (order, preference) wins */
    for (i = 0; i < num_sorted && num_cand < max_cand; i++) {
        const mme_dns_naptr_record_t *r = sorted[i];
        char canon[MME_DNS_MAX_FQDN_LEN+1];
        bool needs_srv = false;
        bool dup = false;

        (void)flags_terminal(r->flags, &needs_srv);
        mme_dns_canonical_name(r->replacement, canon, sizeof(canon));

        for (j = 0; j < num_cand; j++) {
            if (strcasecmp(cand[j].canon, canon) == 0) {
                dup = true;
                break;
            }
        }
        if (dup)
            continue;

        memset(&cand[num_cand], 0, sizeof(cand[num_cand]));
        ogs_cpystrn(cand[num_cand].fqdn,
                r->replacement, sizeof(cand[num_cand].fqdn));
        ogs_cpystrn(cand[num_cand].canon, canon,
                sizeof(cand[num_cand].canon));
        cand[num_cand].order = r->order;
        cand[num_cand].preference = r->preference;
        cand[num_cand].needs_srv = needs_srv;
        cand[num_cand].resolved = false;
        num_cand++;
    }

    return num_cand;
}

bool mme_dns_candidate_apply_srv(mme_dns_candidate_t *cand,
        const mme_dns_srv_record_t *recs, int num_recs)
{
    const mme_dns_srv_record_t *best = NULL;
    int i;

    ogs_assert(cand);

    if (!recs || num_recs <= 0)
        return false;

    for (i = 0; i < num_recs; i++) {
        if (!recs[i].target[0] || strcmp(recs[i].target, ".") == 0)
            continue;
        if (!best || recs[i].priority < best->priority)
            best = &recs[i];
    }

    if (!best)
        return false;

    ogs_cpystrn(cand->fqdn, best->target, sizeof(cand->fqdn));
    cand->port = best->port;
    cand->needs_srv = false;

    return true;
}
