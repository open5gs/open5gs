/* Copyright (C) 2026 Open5GS contributors
 * SPDX-License-Identifier: AGPL-3.0-or-later */
#include "mme/mme-s10.h"
#include "mme/mme-as-security.h"
#include "core/abts.h"

/* Substitute only pool lookup; serialization, key selection and all TLV
 * descriptors are the production implementation. */
static sgw_ue_t snapshot_sgw;
static sgw_ue_t *snapshot_find_sgw(ogs_pool_id_t id)
{
    return id == 7 ? &snapshot_sgw : NULL;
}
#define sgw_ue_find_by_id snapshot_find_sgw
#define mme_s10_export_mm_context snapshot_export_mm
#define mme_s10_build_context_response snapshot_context_response
#define mme_s10_build_forward_relocation_request snapshot_forward_request
int snapshot_export_mm(mme_ue_t *, ogs_tlv_octet_t *, void *, size_t);
ogs_pkbuf_t *snapshot_context_response(mme_ue_t *, uint8_t);
ogs_pkbuf_t *snapshot_forward_request(mme_ue_t *,
        const S1AP_TargeteNB_ID_t *, const S1AP_Cause_t *,
        const OCTET_STRING_t *, bool);
#include "mme/mme-s10-build.c"
#undef sgw_ue_find_by_id
#undef mme_s10_export_mm_context
#undef mme_s10_build_context_response
#undef mme_s10_build_forward_relocation_request

static void target_vectors(abts_case *tc, void *data)
{
    static const struct {
        S1AP_ENB_ID_PR type;
        uint32_t id;
        uint8_t wire[10];
        size_t len;
    } cases[] = {
        {S1AP_ENB_ID_PR_macroENB_ID, 0xabcde,
            {1,0x00,0xf1,0x10,0x0a,0xbc,0xde,0x12,0x34},9},
        {S1AP_ENB_ID_PR_homeENB_ID, 0xabcdef0,
            {3,0x00,0xf1,0x10,0x0a,0xbc,0xde,0xf0,0x12,0x34},10},
        {S1AP_ENB_ID_PR_short_macroENB_ID, 0x2abcd,
            {4,0x00,0xf1,0x10,0x82,0xab,0xcd,0x12,0x34},9},
        {S1AP_ENB_ID_PR_long_macroENB_ID, 0x1abcde,
            {4,0x00,0xf1,0x10,0x1a,0xbc,0xde,0x12,0x34},9}
    };
    uint8_t plmn[] = {0x00,0xf1,0x10}, tac[] = {0x12,0x34}, out[10];
    S1AP_Global_ENB_ID_t global = {0};
    S1AP_TargeteNB_ID_t target = {0};
    S1AP_TAI_t tai = {0};
    size_t i;
    global.pLMNidentity.buf = plmn;
    global.pLMNidentity.size = sizeof(plmn);
    target.global_ENB_ID = &global;
    target.selected_TAI = &tai;
    tai.pLMNidentity.buf = plmn;
    tai.pLMNidentity.size = sizeof(plmn);
    tai.tAC.buf = tac;
    tai.tAC.size = sizeof(tac);
    for (i = 0; i < OGS_ARRAY_SIZE(cases); i++) {
        S1AP_ENB_ID_t enb = {0};
        mme_s10_target_t decoded, sentinel;
        ogs_tlv_octet_t ie = {0};
        global.eNB_ID = &enb;
        ogs_s1ap_uint32_to_ENB_ID(cases[i].type, cases[i].id, &enb);
        ABTS_INT_EQUAL(tc, OGS_OK, mme_s10_encode_target(&ie, &target, out));
        ABTS_INT_EQUAL(tc, cases[i].len, ie.len);
        ABTS_TRUE(tc, !memcmp(out, cases[i].wire, cases[i].len));
        ie.data = (void *)cases[i].wire;
        ABTS_INT_EQUAL(tc, OGS_OK, mme_s10_decode_target(&decoded, &ie));
        ABTS_INT_EQUAL(tc, cases[i].type, decoded.enb_type);
        ABTS_INT_EQUAL(tc, cases[i].id, decoded.enb_id);
        ABTS_INT_EQUAL(tc, 0x1234, decoded.tai.tac);
        ABTS_TRUE(tc, !memcmp(&decoded.enb_plmn, plmn, sizeof(plmn)));
        memset(&sentinel, 0xa5, sizeof(sentinel));
        decoded = sentinel;
        ie.len = cases[i].len - 1;
        ABTS_INT_EQUAL(tc, OGS_ERROR, mme_s10_decode_target(&decoded, &ie));
        ABTS_TRUE(tc, !memcmp(&decoded, &sentinel, sizeof(decoded)));
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1AP_ENB_ID, &enb);
    }
}

static void selected_plmn_vectors(abts_case *tc, void *data)
{
    const uint8_t nas[][3] = {{0x00,0xf1,0x10}, {0x13,0x00,0x62}};
    const uint8_t wire[][3] = {{0x00,0xf1,0x10}, {0x13,0x20,0x06}};
    ogs_plmn_id_t plmn, decoded;
    uint8_t buffer[3];
    ogs_tlv_octet_t ie = {0};
    size_t i;
    for (i = 0; i < OGS_ARRAY_SIZE(nas); i++) {
        memcpy(&plmn, nas[i], sizeof(plmn));
        ABTS_INT_EQUAL(tc, OGS_OK,
                mme_s10_encode_selected_plmn(&ie, &plmn, buffer));
        ABTS_TRUE(tc, !memcmp(buffer, wire[i], sizeof(buffer)));
        ie.data = (void *)wire[i];
        ABTS_INT_EQUAL(tc, OGS_OK, mme_s10_decode_selected_plmn(&decoded, &ie));
        ABTS_TRUE(tc, !memcmp(&decoded, nas[i], sizeof(decoded)));
    }
    ie.len = 2;
    ABTS_INT_EQUAL(tc, OGS_ERROR, mme_s10_decode_selected_plmn(&decoded, &ie));
}

static void cause_vectors(abts_case *tc, void *data)
{
    S1AP_Cause_t cause = {0}, decoded;
    uint8_t out[2], wire[] = {0xf3, 4};
    ogs_tlv_octet_t ie = {0};
    cause.present = S1AP_Cause_PR_protocol;
    cause.choice.protocol = 4;
    ABTS_INT_EQUAL(tc, OGS_OK, mme_s10_encode_cause(&ie, &cause, out));
    ABTS_INT_EQUAL(tc, 3, out[0]);
    ABTS_INT_EQUAL(tc, 4, out[1]);
    ie.data = wire;
    ABTS_INT_EQUAL(tc, OGS_OK, mme_s10_decode_cause(&decoded, &ie));
    ABTS_INT_EQUAL(tc, S1AP_Cause_PR_protocol, decoded.present);
    ABTS_INT_EQUAL(tc, 4, decoded.choice.protocol);
    wire[0] = 5;
    ABTS_INT_EQUAL(tc, OGS_ERROR, mme_s10_decode_cause(&decoded, &ie));
    ie.len = 1;
    ABTS_INT_EQUAL(tc, OGS_ERROR, mme_s10_decode_cause(&decoded, &ie));
}

static void concurrent_security_contexts(abts_case *tc, void *data)
{
    mme_ue_t *ue = ogs_calloc(1, sizeof(*ue));
    mme_ue_t *target = ogs_calloc(1, sizeof(*target));
    ogs_gtp2_eps_mm_context_t mm = {0}, parsed;
    ogs_tlv_octet_t ie = {0};
    uint8_t expected_nh[32], old_kasme[32], old_nh[32], wire[2048];
    ogs_assert(ue && target);
    memset(old_kasme, 0x11, sizeof(old_kasme));
    memset(old_nh, 0x22, sizeof(old_nh));
    memcpy(ue->kasme, old_kasme, sizeof(old_kasme));
    memcpy(ue->nh, old_nh, sizeof(old_nh));
    ue->nas_eps.mme.ksi = 1;
    ue->nhcc = 1;
    ue->enb_ue_id = 42;
    ue->security_context_available = 1;
    mme_as_security_prepare(ue, S1AP_ProcedureCode_id_InitialContextSetup);
    /* NAS changes after the radio request was built. Its response must
     * confirm the snapshot that was sent, not the current NAS key. */
    memset(ue->kasme, 0x33, sizeof(ue->kasme));
    memset(ue->nh, 0x44, sizeof(ue->nh));
    ue->nas_eps.mme.ksi = 2;
    mme_as_security_complete(ue, 43,
            S1AP_ProcedureCode_id_InitialContextSetup, true);
    mme_as_security_complete(ue, 42,
            S1AP_ProcedureCode_id_UEContextModification, true);
    ABTS_TRUE(tc, ue->as_pending.valid);
    ABTS_TRUE(tc, !ue->as_security.valid);
    mme_as_security_complete(ue, 42,
            S1AP_ProcedureCode_id_InitialContextSetup, true);
    ABTS_TRUE(tc, !ue->as_pending.valid);
    ABTS_TRUE(tc, !memcmp(ue->as_security.kasme, old_kasme, 32));
    ogs_kdf_nh_enb(old_kasme, old_nh, expected_nh);
    ABTS_INT_EQUAL(tc, OGS_OK, mme_as_security_next_hop(ue));
    ABTS_INT_EQUAL(tc, 2, ue->as_security.ncc);
    ABTS_TRUE(tc, !memcmp(ue->as_security.nh, expected_nh, 32));
    ABTS_INT_EQUAL(tc, 0x44, ue->nh[0]);
    ABTS_INT_EQUAL(tc, 1, ue->nhcc);

    mm.ksi = 2;
    mm.integrity_algorithm = 2;
    memcpy(mm.kasme, ue->kasme, 32);
    mm.nh_present = true;
    memcpy(mm.nh, ue->nh, 32);
    mm.ncc = 1;
    mme_as_security_export_old(ue, &mm);
    ABTS_TRUE(tc, mm.old_context_present && mm.old_nh_present);
    ABTS_INT_EQUAL(tc, 1, mm.old_ksi);
    ABTS_TRUE(tc, !memcmp(mm.old_kasme, old_kasme, 32));
    ABTS_INT_EQUAL(tc, OGS_OK,
            ogs_gtp2_build_eps_mm_context(&ie, &mm, wire, sizeof(wire)));
    ABTS_INT_EQUAL(tc, OGS_OK, ogs_gtp2_parse_eps_mm_context(&parsed, &ie));
    mme_as_security_import(target, &parsed);
    ABTS_TRUE(tc, !memcmp(target->as_security.kasme, old_kasme, 32));
    ABTS_TRUE(tc, !memcmp(target->as_security.nh, expected_nh, 32));
    ABTS_INT_EQUAL(tc, 2, target->as_security.ncc);
    ABTS_INT_EQUAL(tc, 0x33, parsed.kasme[0]);

    mme_as_security_prepare(ue, S1AP_ProcedureCode_id_InitialContextSetup);
    mme_as_security_complete(ue, 42,
            S1AP_ProcedureCode_id_InitialContextSetup, false);
    ABTS_TRUE(tc, !memcmp(ue->as_security.kasme, old_kasme, 32));
    mme_as_security_prepare(ue, S1AP_ProcedureCode_id_InitialContextSetup);
    mme_as_security_complete(ue, 42,
            S1AP_ProcedureCode_id_InitialContextSetup, true);
    ABTS_TRUE(tc, !memcmp(ue->as_security.kasme, ue->kasme, 32));
    ABTS_INT_EQUAL(tc, 2, ue->as_security.ksi);
    ABTS_INT_EQUAL(tc, 1, ue->as_security.ncc);
    memset(&mm, 0, sizeof(mm));
    mme_as_security_export_old(ue, &mm);
    ABTS_TRUE(tc, !mm.old_context_present);
    /* Releasing an old handover leg cannot erase the serving radio key. */
    mme_as_security_release(ue, 43);
    ABTS_TRUE(tc, ue->as_security.valid);
    mme_as_security_prepare(ue, S1AP_ProcedureCode_id_UEContextModification);
    mme_as_security_release(ue, 42);
    ABTS_TRUE(tc, !ue->as_security.valid && !ue->as_pending.valid);
    ABTS_INT_EQUAL(tc, 0, ue->as_pending_procedure);
    ogs_free(target);
    ogs_free(ue);
}

static void handover_snapshot(abts_case *tc, void *data)
{
    mme_ue_t *ue = ogs_calloc(1, sizeof(*ue));
    mme_sess_t sess = {0};
    mme_bearer_t bearer = {0};
    ogs_session_t profile = {0};
    ogs_gtp_node_t node = {0};
    ogs_sockaddr_t local = {0}, *saved_addr = ogs_gtp_self()->gtpc_addr;
    ogs_sockaddr_t *saved_addr6 = ogs_gtp_self()->gtpc_addr6;
    S1AP_Global_ENB_ID_t global = {0};
    S1AP_ENB_ID_t enb_id = {0};
    S1AP_TargeteNB_ID_t target = {0};
    S1AP_TAI_t tai = {0};
    S1AP_Cause_t cause = {0};
    uint8_t plmn[] = {0x00,0xf1,0x10}, tac[] = {0,1}, opaque[] = {1,2,3};
    OCTET_STRING_t container = {0};
    ogs_gtp2_forward_relocation_request_t *req = ogs_calloc(1, sizeof(*req));
    ogs_gtp2_eps_mm_context_t mm;
    ogs_pkbuf_t *p;
    ogs_assert(ue && req);
    local.sin.sin_family = AF_INET;
    local.sin.sin_addr.s_addr = htobe32(0x7f000002);
    ogs_gtp_self()->gtpc_addr = &local;
    ogs_gtp_self()->gtpc_addr6 = NULL;
    node.addr = local;
    snapshot_sgw.gnode = &node;
    snapshot_sgw.sgw_s11_teid = 0x12345678;
    ue->sgw_ue_id = 7;
    ue->mme_s10_teid = 0x11223344;
    ue->imsi_len = 8;
    memset(ue->imsi, 0x11, ue->imsi_len);
    ue->nas_eps.mme.ksi = 2;
    memset(ue->kasme, 0x22, sizeof(ue->kasme));
    memset(ue->nh, 0x33, sizeof(ue->nh));
    ue->nhcc = 1;
    ue->as_security.valid = true;
    ue->as_security.ksi = 1;
    memset(ue->as_security.kasme, 0x44, sizeof(ue->as_security.kasme));
    memset(ue->as_security.nh, 0x55, sizeof(ue->as_security.nh));
    ue->as_security.ncc = 6;
    ue->ue_network_capability.length = 2;
    ue->ue_network_capability.eea = ue->ue_network_capability.eia = 0x20;
    ue->selected_int_algorithm = ue->selected_enc_algorithm = 2;
    ogs_list_init(&ue->sess_list);
    ogs_list_add(&ue->sess_list, &sess);
    ogs_list_init(&sess.bearer_list);
    ogs_list_add(&sess.bearer_list, &bearer);
    profile.name = "internet";
    profile.qos.index = 9;
    profile.qos.arp.priority_level = 8;
    sess.session = &profile;
    sess.paa.session_type = OGS_PDU_SESSION_TYPE_IPV4;
    sess.paa.addr = htobe32(0x0a2d0002);
    sess.pgw_s5c_teid = 8;
    sess.pgw_s5c_ip.ipv4 = 1;
    sess.pgw_s5c_ip.len = 4;
    sess.pgw_s5c_ip.addr = htobe32(0x7f000004);
    bearer.ebi = 5;
    bearer.sgw_s1u_teid = 9;
    bearer.pgw_s5u_teid = 10;
    bearer.sgw_s1u_ip = bearer.pgw_s5u_ip = sess.pgw_s5c_ip;
    global.pLMNidentity.buf = plmn;
    global.pLMNidentity.size = sizeof(plmn);
    global.eNB_ID = &enb_id;
    ogs_s1ap_uint32_to_ENB_ID(S1AP_ENB_ID_PR_macroENB_ID, 0x12345, &enb_id);
    tai.pLMNidentity.buf = plmn;
    tai.pLMNidentity.size = sizeof(plmn);
    tai.tAC.buf = tac;
    tai.tAC.size = sizeof(tac);
    target.global_ENB_ID = &global;
    target.selected_TAI = &tai;
    cause.present = S1AP_Cause_PR_radioNetwork;
    cause.choice.radioNetwork = S1AP_CauseRadioNetwork_handover_desirable_for_radio_reason;
    container.buf = opaque;
    container.size = sizeof(opaque);
    p = snapshot_forward_request(ue, &target, &cause, &container, true);
    ABTS_PTR_NOTNULL(tc, p);
    if (p) {
        ABTS_INT_EQUAL(tc, OGS_OK, ogs_tlv_parse_msg(req,
                    &ogs_gtp2_tlv_desc_forward_relocation_request,
                    p, OGS_TLV_MODE_T1_L2_I1));
        ABTS_INT_EQUAL(tc, OGS_OK, ogs_gtp2_parse_eps_mm_context(&mm,
                    &req->mme_sgsn_amf_ue_mm_context));
        ABTS_INT_EQUAL(tc, 2, mm.ksi);
        ABTS_INT_EQUAL(tc, 0x22, mm.kasme[0]);
        ABTS_TRUE(tc, mm.old_context_present && mm.old_nh_present);
        ABTS_INT_EQUAL(tc, 1, mm.old_ksi);
        ABTS_INT_EQUAL(tc, 0x44, mm.old_kasme[0]);
        ABTS_INT_EQUAL(tc, 0x55, mm.old_nh[0]);
        ABTS_INT_EQUAL(tc, 6, mm.old_ncc);
        ABTS_INT_EQUAL(tc, sizeof(opaque) + 1, req->e_utran_transparent_container.len);
        ABTS_INT_EQUAL(tc, 3, ((uint8_t *)req->e_utran_transparent_container.data)[0]);
        ABTS_TRUE(tc, !memcmp(opaque,
                    (uint8_t *)req->e_utran_transparent_container.data + 1, sizeof(opaque)));
        ABTS_INT_EQUAL(tc, 5,
                req->mme_sgsn_amf_ue_eps_pdn_connections[0].linked_eps_bearer_id.u8);
        ogs_pkbuf_free(p);
    }
    ue->as_pending.valid = true;
    ABTS_TRUE(tc, snapshot_forward_request(ue, &target, &cause, &container, true) == NULL);
    ue->as_pending.valid = false;
    container.size = OGS_MAX_SDU_LEN;
    ABTS_TRUE(tc, snapshot_forward_request(ue, &target, &cause, &container, true) == NULL);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1AP_ENB_ID, &enb_id);
    ogs_gtp_self()->gtpc_addr = saved_addr;
    ogs_gtp_self()->gtpc_addr6 = saved_addr6;
    memset(&snapshot_sgw, 0, sizeof(snapshot_sgw));
    ogs_free(req);
    ogs_free(ue);
}

abts_suite *test_mme_s10_security(abts_suite *suite)
{
    suite = ADD_SUITE(suite);
    abts_run_test(suite, target_vectors, NULL);
    abts_run_test(suite, selected_plmn_vectors, NULL);
    abts_run_test(suite, cause_vectors, NULL);
    abts_run_test(suite, handover_snapshot, NULL);
    abts_run_test(suite, concurrent_security_contexts, NULL);
    return suite;
}
