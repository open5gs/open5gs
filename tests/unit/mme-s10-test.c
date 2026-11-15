/* Copyright (C) 2026 Open5GS contributors
 * SPDX-License-Identifier: AGPL-3.0-or-later */
#include "mme/mme-s10.h"
#include "core/abts.h"

static int parse_peer(const char *text)
{
    yaml_parser_t parser;
    yaml_document_t document;
    ogs_yaml_iter_t iter;
    int rv;
    ogs_assert(yaml_parser_initialize(&parser));
    yaml_parser_set_input_string(&parser, (const unsigned char *)text,
            strlen(text));
    ogs_assert(yaml_parser_load(&parser, &document));
    yaml_parser_delete(&parser);
    ogs_yaml_iter_init(&iter, &document);
    ogs_assert(ogs_yaml_iter_next(&iter));
    rv = mme_s10_parse_peer(&iter);
    yaml_document_delete(&document);
    return rv;
}

static void routes(abts_case *tc, void *data)
{
    ogs_nas_eps_guti_t guti = {0};
    ogs_plmn_id_t plmn;
    ogs_eps_tai_t tai = {0};
    mme_s10_peer_t *peer;
    mme_s10_init();
    ABTS_INT_EQUAL(tc, OGS_OK, parse_peer(
        "mme:\n"
        "  - address: 127.0.0.3\n"
        "    port: 2123\n"
        "    tai:\n"
        "      - plmn_id: {mcc: '001', mnc: '01'}\n"
        "        tac: 1234\n"
        "    gummei:\n"
        "      plmn_id: {mcc: '001', mnc: '01'}\n"
        "      mme_gid: 65535\n"
        "      mme_code: 255\n"));
    ogs_plmn_id_build(&plmn, 1, 1, 2);
    ogs_nas_from_plmn_id(&guti.nas_plmn_id, &plmn);
    guti.mme_gid = 65535;
    guti.mme_code = 255;
    peer = mme_s10_find_by_guti(&guti);
    ABTS_PTR_NOTNULL(tc, peer);
    if (peer) ABTS_INT_EQUAL(tc, 2123, OGS_PORT(peer->gnode.sa_list));
    tai.plmn_id = plmn;
    tai.tac = 1234;
    ABTS_TRUE(tc, mme_s10_find_by_tai(&tai) == peer);
    tai.tac = 1235;
    ABTS_TRUE(tc, mme_s10_find_by_tai(&tai) == NULL);
    tai.tac = 1234;
    ogs_plmn_id_build(&tai.plmn_id, 1, 1, 3);
    ABTS_TRUE(tc, mme_s10_find_by_tai(&tai) == NULL);
    ABTS_INT_EQUAL(tc, OGS_OK, parse_peer(
        "mme: {address: 127.0.0.3, port: 2123, gummei: {plmn_id: {mcc: '001', "
        "mnc: '01'}, mme_gid: 65535, mme_code: 254}}"));
    guti.mme_code = 254;
    if (peer) {
        mme_s10_peer_t *alias = mme_s10_find_by_guti(&guti);
        ABTS_PTR_NOTNULL(tc, alias);
        if (alias) ABTS_TRUE(tc, alias->node == peer->node);
    }
    guti.mme_code = 255;
    /* Same numeric MNC, different MNC length, is a different PLMN. */
    ogs_plmn_id_build(&plmn, 1, 1, 3);
    ogs_nas_from_plmn_id(&guti.nas_plmn_id, &plmn);
    ABTS_TRUE(tc, mme_s10_find_by_guti(&guti) == NULL);
    guti.mme_gid = 1;
    ABTS_TRUE(tc, mme_s10_find_by_guti(&guti) == NULL);
    ABTS_INT_EQUAL(tc, OGS_ERROR, parse_peer(
        "mme: {address: 127.0.0.4, gummei: {plmn_id: {mcc: '001', "
        "mnc: '01'}, mme_gid: 65535, mme_code: 255}}"));
    mme_s10_final();
}

static void invalid_config(abts_case *tc, void *data)
{
    const char *cases[] = {
        "mme: {address: 127.0.0.3}",
        "mme: {address: 127.0.0.3, port: -1}",
        "mme: {address: 127.0.0.3, port: 65536}",
        "mme: {address: 127.0.0.3, gummei: {plmn_id: {mcc: '01', mnc: '01'}, mme_gid: 1, mme_code: 1}}",
        "mme: {address: 127.0.0.3, gummei: {plmn_id: {mcc: '001', mnc: '0x'}, mme_gid: 1, mme_code: 1}}",
        "mme: {address: 127.0.0.3, gummei: {plmn_id: {mcc: '001', mnc: '01'}, mme_gid: 65536, mme_code: 1}}",
        "mme: {address: 127.0.0.3, gummei: {plmn_id: {mcc: '001', mnc: '01'}, mme_gid: 1, mme_code: 256}}",
        "mme: {address: 127.0.0.3, typo: true}",
    };
    size_t i;
    mme_s10_init();
    for (i = 0; i < OGS_ARRAY_SIZE(cases); i++)
        ABTS_INT_EQUAL(tc, OGS_ERROR, parse_peer(cases[i]));
    mme_s10_final();
}

static ogs_pkbuf_t *complete_attach(mme_ue_t *ue, bool protected, uint8_t sqn)
{
    ogs_nas_eps_message_t nas = {0};
    ogs_nas_eps_attach_request_t *req = &nas.emm.attach_request;
    ogs_pkbuf_t *p;
    uint8_t esm[] = {0x02, 1, OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST};
    uint8_t mac[4];
    ogs_nas_eps_security_header_t *header;
    nas.emm.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;
    nas.emm.h.message_type = OGS_NAS_EPS_ATTACH_REQUEST;
    req->eps_attach_type.value = OGS_NAS_ATTACH_TYPE_EPS_ATTACH;
    req->eps_attach_type.nas_key_set_identifier = ue->nas_eps.mme.ksi;
    req->eps_mobile_identity.length = sizeof(req->eps_mobile_identity.guti);
    req->eps_mobile_identity.guti.spare = 15;
    req->eps_mobile_identity.guti.type = OGS_NAS_EPS_MOBILE_IDENTITY_GUTI;
    req->eps_mobile_identity.guti.nas_plmn_id = ue->current.guti.nas_plmn_id;
    req->eps_mobile_identity.guti.mme_gid = ue->current.guti.mme_gid;
    req->eps_mobile_identity.guti.mme_code = ue->current.guti.mme_code;
    req->eps_mobile_identity.guti.m_tmsi = ue->current.guti.m_tmsi;
    req->ue_network_capability.length = 2;
    req->ue_network_capability.eea = req->ue_network_capability.eia = 0x20;
    req->esm_message_container.length = sizeof(esm);
    req->esm_message_container.buffer = esm;
    p = ogs_nas_emm_encode(&nas);
    ogs_assert(p);
    if (protected) {
        *(uint8_t *)ogs_pkbuf_push(p, 1) = sqn;
        ogs_nas_mac_calculate(ue->selected_int_algorithm, ue->knas_int,
                sqn, 0, OGS_NAS_SECURITY_UPLINK_DIRECTION, p, mac);
        header = ogs_pkbuf_push(p, 5);
        header->security_header_type = OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED;
        header->protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;
        memcpy(&header->message_authentication_code, mac, sizeof(mac));
    }
    *(uint8_t *)ogs_pkbuf_push(p, 1) = 0;
    return p;
}

static void complete_request_security(abts_case *tc, void *data)
{
    mme_ue_t *ue = ogs_calloc(1, sizeof(*ue));
    ogs_hash_t *saved = mme_self()->guti_ue_hash;
    ogs_pkbuf_t *p;
    ogs_tlv_octet_t ie = {0};
    ogs_plmn_id_t plmn;
    bool verified;
    uint8_t saved_byte;
    ogs_assert(ue);
    mme_self()->guti_ue_hash = ogs_hash_make();
    ogs_plmn_id_build(&plmn, 1, 1, 2);
    ogs_nas_from_plmn_id(&ue->current.guti.nas_plmn_id, &plmn);
    ue->current.guti.mme_gid = 2;
    ue->current.guti.mme_code = 1;
    ue->current.guti.m_tmsi = 0x12345678;
    ue->current.m_tmsi = (void *)1;
    ue->nas_eps.mme.ksi = 3;
    ue->security_context_available = 1;
    ue->selected_int_algorithm = OGS_NAS_SECURITY_ALGORITHMS_128_EIA2;
    memset(ue->knas_int, 0x11, sizeof(ue->knas_int));
    ogs_hash_set(mme_self()->guti_ue_hash, &ue->current.guti,
            sizeof(ue->current.guti), ue);
    p = complete_attach(ue, true, 4);
    ie.presence = 1;
    ie.data = p->data;
    ie.len = p->len;
    /* A forged MAC must not advance COUNT or mark the real UE failed. */
    ((uint8_t *)ie.data)[2] ^= 1;
    ABTS_INT_EQUAL(tc, OGS_ERROR,
            mme_s10_verify_complete_request(ue, &ie, 0, &verified));
    ABTS_TRUE(tc, !verified);
    ABTS_INT_EQUAL(tc, 0, ue->ul_count.i32);
    ABTS_INT_EQUAL(tc, 0, ue->mac_failed);
    ((uint8_t *)ie.data)[2] ^= 1;
    ABTS_INT_EQUAL(tc, OGS_OK,
            mme_s10_verify_complete_request(ue, &ie, 0, &verified));
    ABTS_TRUE(tc, verified);
    ABTS_INT_EQUAL(tc, 4, ue->ul_count.i32);
    ABTS_TRUE(tc, ue->ul_count_accepted);
    /* A valid MAC with a reused COUNT remains a replay. */
    ABTS_INT_EQUAL(tc, OGS_ERROR,
            mme_s10_verify_complete_request(ue, &ie, 0, &verified));
    ABTS_TRUE(tc, !verified);
    ABTS_INT_EQUAL(tc, 4, ue->ul_count.i32);
    ogs_pkbuf_free(p);
    p = complete_attach(ue, false, 0);
    ie.data = p->data;
    ie.len = p->len;
    ABTS_INT_EQUAL(tc, OGS_OK,
            mme_s10_verify_complete_request(ue, &ie, 0, &verified));
    ABTS_TRUE(tc, !verified);
    /* Complete Request discriminator must agree with the NAS message. */
    ABTS_INT_EQUAL(tc, OGS_ERROR,
            mme_s10_verify_complete_request(ue, &ie, 1, &verified));
    saved_byte = ((uint8_t *)ie.data)[5];
    ((uint8_t *)ie.data)[5] ^= 1;
    ABTS_INT_EQUAL(tc, OGS_ERROR,
            mme_s10_verify_complete_request(ue, &ie, 0, &verified));
    ((uint8_t *)ie.data)[5] = saved_byte;
    ie.len = 2;
    ABTS_INT_EQUAL(tc, OGS_ERROR,
            mme_s10_verify_complete_request(ue, &ie, 0, &verified));
    ogs_pkbuf_free(p);
    ogs_hash_destroy(mme_self()->guti_ue_hash);
    mme_self()->guti_ue_hash = saved;
    ogs_free(ue);
}

abts_suite *test_mme_s10(abts_suite *suite)
{
    suite = ADD_SUITE(suite);
    abts_run_test(suite, routes, NULL);
    abts_run_test(suite, complete_request_security, NULL);
    abts_run_test(suite, invalid_config, NULL);
    return suite;
}
