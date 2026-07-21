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

#include "mme/mme-dns-select.h"
#include "core/abts.h"

static ogs_plmn_id_t plmn(uint16_t mcc, uint16_t mnc, uint16_t mnc_len)
{
    ogs_plmn_id_t id;
    ogs_plmn_id_build(&id, mcc, mnc, mnc_len);
    return id;
}

static void dns_select_test_apn_fqdn(abts_case *tc, void *data)
{
    char buf[MME_DNS_MAX_FQDN_LEN+1];
    ogs_plmn_id_t id;

    id = plmn(1, 1, 2);
    ABTS_INT_EQUAL(tc, OGS_OK,
            mme_dns_build_apn_fqdn(buf, sizeof(buf), "internet", &id));
    ABTS_STR_EQUAL(tc,
            "internet.apn.epc.mnc001.mcc001.3gppnetwork.org", buf);

    id = plmn(302, 330, 3);
    ABTS_INT_EQUAL(tc, OGS_OK,
            mme_dns_build_apn_fqdn(buf, sizeof(buf), "ims", &id));
    ABTS_STR_EQUAL(tc,
            "ims.apn.epc.mnc330.mcc302.3gppnetwork.org", buf);

    ABTS_INT_EQUAL(tc, OGS_ERROR,
            mme_dns_build_apn_fqdn(buf, sizeof(buf), "", &id));
    ABTS_INT_EQUAL(tc, OGS_ERROR,
            mme_dns_build_apn_fqdn(buf, sizeof(buf), NULL, &id));
    ABTS_INT_EQUAL(tc, OGS_ERROR,
            mme_dns_build_apn_fqdn(buf, 16, "internet", &id));
}

static void dns_select_test_tai_fqdn(abts_case *tc, void *data)
{
    char buf[MME_DNS_MAX_FQDN_LEN+1];
    ogs_eps_tai_t tai;

    memset(&tai, 0, sizeof(tai));
    tai.plmn_id = plmn(1, 1, 2);
    tai.tac = 0x0001;
    ABTS_INT_EQUAL(tc, OGS_OK,
            mme_dns_build_tai_fqdn(buf, sizeof(buf), &tai));
    ABTS_STR_EQUAL(tc,
            "tac-lb01.tac-hb00.tac.epc.mnc001.mcc001.3gppnetwork.org", buf);

    tai.plmn_id = plmn(302, 330, 3);
    tai.tac = 0xabcd;
    ABTS_INT_EQUAL(tc, OGS_OK,
            mme_dns_build_tai_fqdn(buf, sizeof(buf), &tai));
    ABTS_STR_EQUAL(tc,
            "tac-lbcd.tac-hbab.tac.epc.mnc330.mcc302.3gppnetwork.org", buf);
}

static void dns_select_test_service_match(abts_case *tc, void *data)
{
    ABTS_TRUE(tc, mme_dns_service_match(
            "x-3gpp-sgw:x-s5-gtp", MME_DNS_SVC_SGW, MME_DNS_PROTO_S5));
    ABTS_TRUE(tc, mme_dns_service_match(
            "x-3gpp-pgw:x-s5-gtp:x-s8-gtp", MME_DNS_SVC_PGW,
            MME_DNS_PROTO_S5));
    ABTS_TRUE(tc, mme_dns_service_match(
            "x-3gpp-pgw:x-s5-gtp:x-s8-gtp", MME_DNS_SVC_PGW,
            MME_DNS_PROTO_S8));
    ABTS_TRUE(tc, mme_dns_service_match(
            "X-3GPP-PGW:X-S5-GTP", MME_DNS_SVC_PGW, MME_DNS_PROTO_S5));
    ABTS_TRUE(tc, mme_dns_service_match(
            "x-3gpp-pgw:x-gn:x-s5-gtp", MME_DNS_SVC_PGW, MME_DNS_PROTO_S5));

    /* Wrong node */
    ABTS_TRUE(tc, !mme_dns_service_match(
            "x-3gpp-sgw:x-s5-gtp", MME_DNS_SVC_PGW, MME_DNS_PROTO_S5));
    /* Wrong protocol */
    ABTS_TRUE(tc, !mme_dns_service_match(
            "x-3gpp-pgw:x-s8-gtp", MME_DNS_SVC_PGW, MME_DNS_PROTO_S5));
    ABTS_TRUE(tc, !mme_dns_service_match(
            "x-3gpp-pgw:x-gn:x-gp", MME_DNS_SVC_PGW, MME_DNS_PROTO_S5));
    /* Malformed */
    ABTS_TRUE(tc, !mme_dns_service_match(
            "x-3gpp-pgw", MME_DNS_SVC_PGW, MME_DNS_PROTO_S5));
    ABTS_TRUE(tc, !mme_dns_service_match(
            "", MME_DNS_SVC_PGW, MME_DNS_PROTO_S5));
    ABTS_TRUE(tc, !mme_dns_service_match(
            NULL, MME_DNS_SVC_PGW, MME_DNS_PROTO_S5));
    ABTS_TRUE(tc, !mme_dns_service_match(
            "x-3gpp-pgw-x-s5-gtp", MME_DNS_SVC_PGW, MME_DNS_PROTO_S5));
    /* Substring must not match a token */
    ABTS_TRUE(tc, !mme_dns_service_match(
            "x-3gpp-pgw:x-s5-gtp-extra", MME_DNS_SVC_PGW, MME_DNS_PROTO_S5));
}

static void dns_select_test_proto_decide(abts_case *tc, void *data)
{
    ogs_plmn_id_t serving = plmn(1, 1, 2);

    /* Home subscriber: IMSI 00101... on serving 001/01 */
    ABTS_INT_EQUAL(tc, MME_DNS_PROTO_S5, mme_dns_decide_proto(
            "001011234567890", &serving, MME_DNS_PROTO_AUTO));
    /* Roamer: IMSI 26201... on serving 001/01 */
    ABTS_INT_EQUAL(tc, MME_DNS_PROTO_S8, mme_dns_decide_proto(
            "262011234567890", &serving, MME_DNS_PROTO_AUTO));
    /* Trap: 3-digit-MNC serving PLMN must compare 6 digits */
    serving = plmn(302, 330, 3);
    ABTS_INT_EQUAL(tc, MME_DNS_PROTO_S5, mme_dns_decide_proto(
            "302330123456789", &serving, MME_DNS_PROTO_AUTO));
    ABTS_INT_EQUAL(tc, MME_DNS_PROTO_S8, mme_dns_decide_proto(
            "302331123456789", &serving, MME_DNS_PROTO_AUTO));
    /* Override always wins */
    ABTS_INT_EQUAL(tc, MME_DNS_PROTO_S8, mme_dns_decide_proto(
            "302330123456789", &serving, MME_DNS_PROTO_S8));
    ABTS_INT_EQUAL(tc, MME_DNS_PROTO_S5, mme_dns_decide_proto(
            "262011234567890", &serving, MME_DNS_PROTO_S5));
}

static void dns_select_test_canonical(abts_case *tc, void *data)
{
    char canon[MME_DNS_MAX_FQDN_LEN+1];

    mme_dns_canonical_name(
            "topon.s5.pgw1.node.epc.mnc001.mcc001.3gppnetwork.org",
            canon, sizeof(canon));
    ABTS_STR_EQUAL(tc,
            "pgw1.node.epc.mnc001.mcc001.3gppnetwork.org", canon);

    mme_dns_canonical_name(
            "topoff.eth4.gw.example.org", canon, sizeof(canon));
    ABTS_STR_EQUAL(tc, "gw.example.org", canon);

    mme_dns_canonical_name("pgw1.example.org", canon, sizeof(canon));
    ABTS_STR_EQUAL(tc, "pgw1.example.org", canon);

    /* Prefix without an interface label: kept verbatim */
    mme_dns_canonical_name("topon.", canon, sizeof(canon));
    ABTS_STR_EQUAL(tc, "topon.", canon);
}

static void dns_select_test_candidates(abts_case *tc, void *data)
{
    mme_dns_naptr_record_t recs[6];
    mme_dns_candidate_t cand[MME_DNS_MAX_CANDIDATES];
    int n;

    memset(recs, 0, sizeof(recs));

    /* order 20 - "a" flag */
    recs[0].order = 20; recs[0].preference = 10;
    strcpy(recs[0].flags, "a");
    strcpy(recs[0].service, "x-3gpp-sgw:x-s5-gtp");
    strcpy(recs[0].replacement, "sgw2.example.org");
    /* order 10 - "s" flag, must sort first */
    recs[1].order = 10; recs[1].preference = 50;
    strcpy(recs[1].flags, "S");
    strcpy(recs[1].service, "x-3gpp-sgw:x-s5-gtp:x-s8-gtp");
    strcpy(recs[1].replacement, "_gtp._udp.sgw1.example.org");
    /* Non-terminal NAPTR: skipped */
    recs[2].order = 5; recs[2].preference = 1;
    strcpy(recs[2].flags, "");
    strcpy(recs[2].service, "x-3gpp-sgw:x-s5-gtp");
    strcpy(recs[2].replacement, "other.example.org");
    /* Wrong service: skipped */
    recs[3].order = 5; recs[3].preference = 1;
    strcpy(recs[3].flags, "a");
    strcpy(recs[3].service, "x-3gpp-pgw:x-s5-gtp");
    strcpy(recs[3].replacement, "pgw.example.org");
    /* Same order 10, lower preference: must sort before recs[1] */
    recs[4].order = 10; recs[4].preference = 20;
    strcpy(recs[4].flags, "a");
    strcpy(recs[4].service, "x-3gpp-sgw:x-s5-gtp");
    strcpy(recs[4].replacement, "topon.s11.sgw3.example.org");
    /* Duplicate canonical name of recs[4] (topoff variant): deduped */
    recs[5].order = 30; recs[5].preference = 1;
    strcpy(recs[5].flags, "a");
    strcpy(recs[5].service, "x-3gpp-sgw:x-s5-gtp");
    strcpy(recs[5].replacement, "topoff.s4.sgw3.example.org");

    n = mme_dns_candidates_from_naptr(recs, 6,
            MME_DNS_SVC_SGW, MME_DNS_PROTO_S5,
            cand, MME_DNS_MAX_CANDIDATES);

    ABTS_INT_EQUAL(tc, 3, n);
    ABTS_STR_EQUAL(tc, "topon.s11.sgw3.example.org", cand[0].fqdn);
    ABTS_STR_EQUAL(tc, "sgw3.example.org", cand[0].canon);
    ABTS_TRUE(tc, !cand[0].needs_srv);
    ABTS_STR_EQUAL(tc, "_gtp._udp.sgw1.example.org", cand[1].fqdn);
    ABTS_TRUE(tc, cand[1].needs_srv);
    ABTS_STR_EQUAL(tc, "sgw2.example.org", cand[2].fqdn);
    ABTS_TRUE(tc, !cand[2].needs_srv);

    /* Candidate cap */
    n = mme_dns_candidates_from_naptr(recs, 6,
            MME_DNS_SVC_SGW, MME_DNS_PROTO_S5, cand, 1);
    ABTS_INT_EQUAL(tc, 1, n);
    ABTS_STR_EQUAL(tc, "topon.s11.sgw3.example.org", cand[0].fqdn);

    /* Empty input */
    n = mme_dns_candidates_from_naptr(NULL, 0,
            MME_DNS_SVC_SGW, MME_DNS_PROTO_S5,
            cand, MME_DNS_MAX_CANDIDATES);
    ABTS_INT_EQUAL(tc, 0, n);
}

static void dns_select_test_srv(abts_case *tc, void *data)
{
    mme_dns_candidate_t cand;
    mme_dns_srv_record_t srv[3];

    memset(&cand, 0, sizeof(cand));
    strcpy(cand.fqdn, "_gtp._udp.sgw1.example.org");
    cand.needs_srv = true;

    memset(srv, 0, sizeof(srv));
    srv[0].priority = 20; srv[0].weight = 100; srv[0].port = 2100;
    strcpy(srv[0].target, "host-b.example.org");
    srv[1].priority = 10; srv[1].weight = 0; srv[1].port = 2123;
    strcpy(srv[1].target, "host-a.example.org");
    srv[2].priority = 10; srv[2].weight = 200; /* weight ignored */
    strcpy(srv[2].target, "host-c.example.org");

    ABTS_TRUE(tc, mme_dns_candidate_apply_srv(&cand, srv, 3));
    ABTS_STR_EQUAL(tc, "host-a.example.org", cand.fqdn);
    ABTS_INT_EQUAL(tc, 2123, cand.port);
    ABTS_TRUE(tc, !cand.needs_srv);

    /* Empty SRV set: returns false, candidate untouched */
    memset(&cand, 0, sizeof(cand));
    strcpy(cand.fqdn, "unchanged");
    cand.needs_srv = true;
    ABTS_TRUE(tc, !mme_dns_candidate_apply_srv(&cand, NULL, 0));
    ABTS_STR_EQUAL(tc, "unchanged", cand.fqdn);
    ABTS_TRUE(tc, cand.needs_srv);

    /* SRV records with no usable target ("." = service not available,
     * RFC 2782): returns false so the caller knows it must SKIP this
     * candidate - retrying it would loop forever (see leg_advance in
     * mme-dns.c) */
    memset(&cand, 0, sizeof(cand));
    strcpy(cand.fqdn, "unchanged");
    cand.needs_srv = true;
    {
        mme_dns_srv_record_t dot[2];
        memset(dot, 0, sizeof(dot));
        dot[0].priority = 10; strcpy(dot[0].target, ".");
        dot[1].priority = 20; /* empty target */
        ABTS_TRUE(tc, !mme_dns_candidate_apply_srv(&cand, dot, 2));
    }
    ABTS_STR_EQUAL(tc, "unchanged", cand.fqdn);
    ABTS_TRUE(tc, cand.needs_srv);
}

abts_suite *test_mme_dns_select(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, dns_select_test_apn_fqdn, NULL);
    abts_run_test(suite, dns_select_test_tai_fqdn, NULL);
    abts_run_test(suite, dns_select_test_service_match, NULL);
    abts_run_test(suite, dns_select_test_proto_decide, NULL);
    abts_run_test(suite, dns_select_test_canonical, NULL);
    abts_run_test(suite, dns_select_test_candidates, NULL);
    abts_run_test(suite, dns_select_test_srv, NULL);

    return suite;
}
