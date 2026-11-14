/* SPDX-License-Identifier: AGPL-3.0-or-later */
#include "mme/mme-s10.h"
#include "core/abts.h"
static mme_ue_t *wire_ue;
static sgw_ue_t wire_sgw;
static mme_sess_t wire_sess;
static mme_ue_t *wire_find_ue(ogs_pool_id_t id) { return id == 11 ? wire_ue : NULL; }
static sgw_ue_t *wire_find_sgw(ogs_pool_id_t id) { return id == 7 ? &wire_sgw : NULL; }
#define mme_ue_find_by_id wire_find_ue
#define sgw_ue_find_by_id wire_find_sgw
#define mme_s11_build_create_session_request wire_mme_s11_build_create_session_request
#define mme_s11_build_modify_bearer_request wire_mme_s11_build_modify_bearer_request
#define mme_s11_build_delete_session_request wire_mme_s11_build_delete_session_request
#define mme_s11_build_create_bearer_response wire_mme_s11_build_create_bearer_response
#define mme_s11_build_update_bearer_response wire_mme_s11_build_update_bearer_response
#define mme_s11_build_delete_bearer_response wire_mme_s11_build_delete_bearer_response
#define mme_s11_build_release_access_bearers_request wire_mme_s11_build_release_access_bearers_request
#define mme_s11_build_downlink_data_notification_ack wire_mme_s11_build_downlink_data_notification_ack
#define mme_s11_build_create_indirect_data_forwarding_tunnel_request wire_mme_s11_build_create_indirect_data_forwarding_tunnel_request
#define mme_s11_build_bearer_resource_command wire_mme_s11_build_bearer_resource_command
#include "mme/mme-s11-build.c"
#undef mme_s11_build_create_session_request
#undef mme_s11_build_modify_bearer_request
#undef mme_s11_build_delete_session_request
#undef mme_s11_build_create_bearer_response
#undef mme_s11_build_update_bearer_response
#undef mme_s11_build_delete_bearer_response
#undef mme_s11_build_release_access_bearers_request
#undef mme_s11_build_downlink_data_notification_ack
#undef mme_s11_build_create_indirect_data_forwarding_tunnel_request
#undef mme_s11_build_bearer_resource_command
#undef mme_ue_find_by_id
#undef sgw_ue_find_by_id
static void relocation_wire(abts_case *tc, void *data)
{
    ogs_session_t profile = {0};
    mme_bearer_t def = {0}, dedicated = {0};
    ogs_sockaddr_t local = {0};
    ogs_sockaddr_t *saved = ogs_gtp_self()->gtpc_addr;
    ogs_sockaddr_t *saved6 = ogs_gtp_self()->gtpc_addr6;
    ogs_gtp2_create_session_request_t *req = ogs_calloc(1, sizeof(*req));
    ogs_pkbuf_t *p;
    ogs_gtp2_bearer_qos_t qos;
    uint8_t tft[] = {0x21,0x31,0,2,0x30,17}; /* One bidirectional UDP filter. */
    static const uint8_t pgw[] = {0x87,0x11,0x22,0x33,0x44,192,0,2,21};
    static const uint8_t user[] = {0x85,0x55,0x66,0x77,0x88,192,0,2,22};
    int action;
    wire_ue = ogs_calloc(1, sizeof(*wire_ue));
    ogs_assert(wire_ue && req);
    memset(&wire_sgw, 0, sizeof(wire_sgw));
    memset(&wire_sess, 0, sizeof(wire_sess));
    local.ogs_sa_family = AF_INET;
    local.sin.sin_addr.s_addr = inet_addr("192.0.2.10");
    ogs_gtp_self()->gtpc_addr = &local;
    ogs_gtp_self()->gtpc_addr6 = NULL;
    wire_ue->id = 11;
    wire_ue->sgw_ue_id = 7;
    wire_ue->imsi_len = 8;
    wire_ue->mme_s11_teid = 0x10203040;
    ogs_plmn_id_build(&wire_ue->tai.plmn_id, 1, 1, 2);
    wire_ue->e_cgi.plmn_id = wire_ue->tai.plmn_id;
    wire_sess.mme_ue_id = 11;
    wire_sess.session = &profile;
    wire_sess.ue_request_type.type = OGS_PDU_SESSION_TYPE_IPV4;
    wire_sess.paa.session_type = OGS_PDU_SESSION_TYPE_IPV4;
    wire_sess.paa.addr = inet_addr("198.51.100.10");
    profile.name = "internet";
    profile.session_type = OGS_PDU_SESSION_TYPE_IPV4;
    profile.ue_ip.ipv4 = 1;
    profile.ue_ip.addr = inet_addr("198.51.100.99");
    profile.smf_ip.ipv4 = 1;
    profile.smf_ip.addr = inet_addr("192.0.2.99");
    wire_sess.pgw_s5c_ip.ipv4 = 1;
    wire_sess.pgw_s5c_ip.addr = inet_addr("192.0.2.21");
    wire_sess.pgw_s5c_teid = 0x11223344;
    ogs_list_init(&wire_sess.bearer_list);
    def.ebi = 5;
    def.qos.index = 9;
    def.qos.arp.priority_level = 8;
    dedicated.ebi = 6;
    dedicated.qos.index = 1;
    dedicated.qos.arp.priority_level = 2;
    dedicated.qos.gbr.uplink = 64000;
    dedicated.qos.gbr.downlink = 128000;
    dedicated.qos.mbr.uplink = 256000;
    dedicated.qos.mbr.downlink = 512000;
    dedicated.tft.presence = 1;
    dedicated.tft.data = tft;
    dedicated.tft.len = sizeof(tft);
    profile.qos = def.qos;
    def.pgw_s5u_ip.ipv4 = dedicated.pgw_s5u_ip.ipv4 = 1;
    def.pgw_s5u_ip.addr = dedicated.pgw_s5u_ip.addr = inet_addr("192.0.2.22");
    def.pgw_s5u_teid = dedicated.pgw_s5u_teid = 0x55667788;
    ogs_list_add(&wire_sess.bearer_list, &def);
    ogs_list_add(&wire_sess.bearer_list, &dedicated);
    for (action = OGS_GTP_CREATE_IN_S10_RELOCATION;
            action <= OGS_GTP_CREATE_IN_S10_TAU_RELOCATION; action++) {
        memset(req, 0, sizeof(*req));
        p = wire_mme_s11_build_create_session_request(
                OGS_GTP2_CREATE_SESSION_REQUEST_TYPE, &wire_sess, action);
        ABTS_PTR_NOTNULL(tc, p);
        if (!p) continue;
        ABTS_INT_EQUAL(tc, OGS_OK, ogs_tlv_parse_msg(req,
                    &ogs_gtp2_tlv_desc_create_session_request, p, OGS_TLV_MODE_T1_L2_I1));
        ABTS_TRUE(tc, req->indication_flags.presence && req->indication_flags.len >= 2);
        ABTS_INT_EQUAL(tc, action == OGS_GTP_CREATE_IN_S10_TAU_RELOCATION ? 8 : 0,
                ((uint8_t *)req->indication_flags.data)[0] & 8);
        ABTS_INT_EQUAL(tc, sizeof(pgw), req->pgw_s5_s8_address_for_control_plane_or_pmip.len);
        ABTS_TRUE(tc, !memcmp(pgw, req->pgw_s5_s8_address_for_control_plane_or_pmip.data, sizeof(pgw)));
        ABTS_INT_EQUAL(tc, sizeof(user), req->bearer_contexts_to_be_created[1].s5_s8_u_sgw_f_teid.len);
        ABTS_TRUE(tc, !memcmp(user, req->bearer_contexts_to_be_created[1].s5_s8_u_sgw_f_teid.data, sizeof(user)));
        ABTS_INT_EQUAL(tc, 6, req->bearer_contexts_to_be_created[1].eps_bearer_id.u8);
        ABTS_INT_EQUAL(tc, GTP2_BEARER_QOS_LEN, ogs_gtp2_parse_bearer_qos(&qos,
                    &req->bearer_contexts_to_be_created[1].bearer_level_qos));
        ABTS_INT_EQUAL(tc, 1, qos.qci);
        ABTS_INT_EQUAL(tc, 2, qos.priority_level);
        ABTS_INT_EQUAL(tc, 64000, qos.ul_gbr);
        ABTS_INT_EQUAL(tc, 512000, qos.dl_mbr);
        ABTS_INT_EQUAL(tc, sizeof(tft), req->bearer_contexts_to_be_created[1].tft.len);
        ABTS_TRUE(tc, !memcmp(tft, req->bearer_contexts_to_be_created[1].tft.data, sizeof(tft)));
        ABTS_INT_EQUAL(tc, inet_addr("198.51.100.10"),
                ((ogs_paa_t *)req->pdn_address_allocation.data)->addr);
        ABTS_INT_EQUAL(tc, inet_addr("198.51.100.10"), wire_sess.paa.addr);
        ogs_pkbuf_free(p);
    }
    ogs_gtp_self()->gtpc_addr = saved;
    ogs_gtp_self()->gtpc_addr6 = saved6;
    ogs_free(req);
    ogs_free(wire_ue);
    wire_ue = NULL;
}

abts_suite *test_mme_s10_sgw(abts_suite *suite)
{
    suite = ADD_SUITE(suite);
    abts_run_test(suite, relocation_wire, NULL);
    return suite;
}
