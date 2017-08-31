#define TRACE_MODULE _mme_s11_build

#include "core_debug.h"

#include "gtp_types.h"
#include "gtp_message.h"

#include "types.h"
#include "mme_context.h"

status_t mme_s11_build_create_session_request(pkbuf_t **pkbuf, mme_sess_t *sess)
{
    status_t rv;
    pdn_t *pdn = NULL;
    mme_sgw_t *sgw = NULL;
    mme_ue_t *mme_ue = NULL;
    gtp_message_t gtp_message;
    gtp_create_session_request_t *req = &gtp_message.create_session_request;

    gtp_uli_t uli;
    char uli_buf[GTP_MAX_ULI_LEN];
    gtp_f_teid_t mme_s11_teid, pgw_s5c_teid;
    gtp_ambr_t ambr;
    gtp_bearer_qos_t bearer_qos;
    char bearer_qos_buf[GTP_BEARER_QOS_LEN];
    gtp_ue_timezone_t ue_timezone;
    c_int8_t apn[MAX_APN_LEN];
    c_uint32_t pgw_ipv4_addr = 0;

    d_assert(sess, return CORE_ERROR, "Null param");
    sgw = sess->sgw;
    d_assert(sgw, return CORE_ERROR, "Null param");
    pdn = sess->pdn;
    d_assert(pdn, return CORE_ERROR, "Null param");
    pgw_ipv4_addr = MME_SESSION_GET_PGW_IPV4_ADDR(sess);
    d_assert(pgw_ipv4_addr, return CORE_ERROR, "Null param");
    mme_ue = sess->mme_ue;
    d_assert(mme_ue, return CORE_ERROR, "Null param");
    d_assert(mme_ue->enb_ue, return CORE_ERROR, "Null param");

    memset(&gtp_message, 0, sizeof(gtp_message_t));

    d_assert(mme_ue->imsi_len, return CORE_ERROR, "Null param");
    req->imsi.presence = 1;
    req->imsi.data = mme_ue->imsi;
    req->imsi.len = mme_ue->imsi_len;

    memset(&uli, 0, sizeof(gtp_uli_t));
    uli.flags.e_cgi = 1;
    uli.flags.tai = 1;
    memcpy(&uli.tai.plmn_id, &mme_ue->enb_ue->tai.plmn_id, 
            sizeof(uli.tai.plmn_id));
    uli.tai.tac = mme_ue->enb_ue->tai.tac;
    memcpy(&uli.e_cgi.plmn_id, &mme_ue->enb_ue->e_cgi.plmn_id, 
            sizeof(uli.e_cgi.plmn_id));
    uli.e_cgi.cell_id = mme_ue->enb_ue->e_cgi.cell_id;
    req->user_location_information.presence = 1;
    gtp_build_uli(&req->user_location_information, &uli, 
            uli_buf, GTP_MAX_ULI_LEN);

    req->serving_network.presence = 1;
    req->serving_network.data = &mme_ue->visited_plmn_id;
    req->serving_network.len = sizeof(mme_ue->visited_plmn_id);

    req->rat_type.presence = 1;
    req->rat_type.u8 = GTP_RAT_TYPE_EUTRAN;

    memset(&mme_s11_teid, 0, sizeof(gtp_f_teid_t));
    mme_s11_teid.ipv4 = 1;
    mme_s11_teid.interface_type = GTP_F_TEID_S11_MME_GTP_C;
    mme_s11_teid.teid = htonl(sess->mme_s11_teid);
    mme_s11_teid.ipv4_addr = sess->mme_s11_addr;
    req->sender_f_teid_for_control_plane.presence = 1;
    req->sender_f_teid_for_control_plane.data = &mme_s11_teid;
    req->sender_f_teid_for_control_plane.len = GTP_F_TEID_IPV4_LEN;

    memset(&pgw_s5c_teid, 0, sizeof(gtp_f_teid_t));
    pgw_s5c_teid.ipv4 = 1;
    pgw_s5c_teid.interface_type = GTP_F_TEID_S5_S8_PGW_GTP_C;
    pgw_s5c_teid.ipv4_addr = pgw_ipv4_addr;
    req->pgw_s5_s8_address_for_control_plane_or_pmip.presence = 1;
    req->pgw_s5_s8_address_for_control_plane_or_pmip.data = &pgw_s5c_teid;
    req->pgw_s5_s8_address_for_control_plane_or_pmip.len = GTP_F_TEID_IPV4_LEN;

    req->access_point_name.presence = 1;
    req->access_point_name.len = apn_build(apn, pdn->apn, strlen(pdn->apn));
    req->access_point_name.data = apn;

    req->selection_mode.presence = 1;
    req->selection_mode.u8 = 
        GTP_SELECTION_MODE_MS_OR_NETWORK_PROVIDED_APN | 0xfc;

    req->pdn_type.presence = 1;
    req->pdn_type.u8 = GTP_PDN_TYPE_IPV4;

    pdn->paa.pdn_type = GTP_PDN_TYPE_IPV4;
    req->pdn_address_allocation.presence = 1;
    req->pdn_address_allocation.data = &pdn->paa;
    req->pdn_address_allocation.len = PAA_IPV4_LEN;

    req->maximum_apn_restriction.presence = 1;
    req->maximum_apn_restriction.u8 = GTP_APN_NO_RESTRICTION;

    if (pdn->ambr.uplink || pdn->ambr.downlink)
    {
        memset(&ambr, 0, sizeof(gtp_ambr_t));
        ambr.uplink = htonl(pdn->ambr.uplink);
        ambr.downlink = htonl(pdn->ambr.downlink);
        req->aggregate_maximum_bit_rate.presence = 1;
        req->aggregate_maximum_bit_rate.data = &ambr;
        req->aggregate_maximum_bit_rate.len = sizeof(ambr);
    }

    if (sess->ue_pco_len)
    {
        req->protocol_configuration_options.presence = 1;
        req->protocol_configuration_options.data = sess->ue_pco;
        req->protocol_configuration_options.len = sess->ue_pco_len;
    }

    req->bearer_contexts_to_be_created.presence = 1;
    req->bearer_contexts_to_be_created.eps_bearer_id.presence = 1;
    req->bearer_contexts_to_be_created.eps_bearer_id.u8 = sess->ebi;

    memset(&bearer_qos, 0, sizeof(bearer_qos));
    bearer_qos.qci = pdn->qos.qci;
    bearer_qos.priority_level = pdn->qos.arp.priority_level;
    bearer_qos.pre_emption_capability = pdn->qos.arp.pre_emption_capability;
    bearer_qos.pre_emption_vulnerability =
        pdn->qos.arp.pre_emption_vulnerability;
    req->bearer_contexts_to_be_created.bearer_level_qos.presence = 1;
    gtp_build_bearer_qos(&req->bearer_contexts_to_be_created.bearer_level_qos,
            &bearer_qos, bearer_qos_buf, GTP_BEARER_QOS_LEN);

    /* FIXME : where did we receive this information from MS */
    memset(&ue_timezone, 0, sizeof(ue_timezone));
    ue_timezone.timezone = 0x40;
    ue_timezone.daylight_saving_time = 
        GTP_UE_TIME_ZONE_NO_ADJUSTMENT_FOR_DAYLIGHT_SAVING_TIME;
    req->ue_time_zone.presence = 1;
    req->ue_time_zone.data = &ue_timezone;
    req->ue_time_zone.len = sizeof(ue_timezone);

    req->charging_characteristics.presence = 1;
    req->charging_characteristics.data = (c_uint8_t *)"\x54\x00";
    req->charging_characteristics.len = 2;

    rv = gtp_build_msg(pkbuf, GTP_CREATE_SESSION_REQUEST_TYPE, &gtp_message);
    d_assert(rv == CORE_OK, return CORE_ERROR, "gtp build failed");

    return CORE_OK;
}

status_t mme_s11_build_modify_bearer_request(
            pkbuf_t **pkbuf, mme_bearer_t *bearer)
{
    status_t rv;
    gtp_message_t gtp_message;
    gtp_modify_bearer_request_t *req = &gtp_message.modify_bearer_request;

    mme_sess_t *sess = NULL;

    gtp_f_teid_t enb_s1u_teid;

    d_assert(bearer, return CORE_ERROR, "Null param");
    sess = bearer->sess;
    d_assert(sess, return CORE_ERROR, "Null param");

    memset(&gtp_message, 0, sizeof(gtp_message_t));

    req->bearer_contexts_to_be_modified.presence = 1;
    req->bearer_contexts_to_be_modified.eps_bearer_id.presence = 1;
    req->bearer_contexts_to_be_modified.eps_bearer_id.u8 = sess->ebi;

    /* Send Data Plane(DL) : ENB-S1U */
    memset(&enb_s1u_teid, 0, sizeof(gtp_f_teid_t));
    enb_s1u_teid.ipv4 = 1;
    enb_s1u_teid.interface_type = GTP_F_TEID_S1_U_ENODEB_GTP_U;
    enb_s1u_teid.ipv4_addr = bearer->enb_s1u_addr;
    enb_s1u_teid.teid = htonl(bearer->enb_s1u_teid);
    req->bearer_contexts_to_be_modified.s1_u_enodeb_f_teid.presence = 1;
    req->bearer_contexts_to_be_modified.s1_u_enodeb_f_teid.data = &enb_s1u_teid;
    req->bearer_contexts_to_be_modified.s1_u_enodeb_f_teid.len = 
        GTP_F_TEID_IPV4_LEN;

    rv = gtp_build_msg(pkbuf, GTP_MODIFY_BEARER_REQUEST_TYPE, &gtp_message);
    d_assert(rv == CORE_OK, return CORE_ERROR, "gtp build failed");

    return CORE_OK;
}

status_t mme_s11_build_delete_session_request(pkbuf_t **pkbuf, mme_sess_t *sess)
{
    status_t rv;
    mme_ue_t *mme_ue = NULL;
    gtp_message_t gtp_message;
    gtp_delete_session_request_t *req = &gtp_message.delete_session_request;

    gtp_uli_t uli;
    char uli_buf[GTP_MAX_ULI_LEN];
    gtp_indication_t indication;

    d_assert(sess, return CORE_ERROR, "Null param");
    mme_ue = sess->mme_ue;
    d_assert(mme_ue, return CORE_ERROR, "Null param");

    memset(&gtp_message, 0, sizeof(gtp_message_t));

    req->linked_eps_bearer_id.presence = 1;
    req->linked_eps_bearer_id.u8 = sess->ebi;

    memset(&uli, 0, sizeof(gtp_uli_t));
    uli.flags.e_cgi = 1;
    uli.flags.tai = 1;
    memcpy(&uli.tai.plmn_id, &mme_ue->enb_ue->tai.plmn_id,
            sizeof(uli.tai.plmn_id));
    uli.tai.tac = mme_ue->enb_ue->tai.tac;
    memcpy(&uli.e_cgi.plmn_id, &mme_ue->enb_ue->e_cgi.plmn_id,
            sizeof(uli.tai.plmn_id));
    uli.e_cgi.cell_id = mme_ue->enb_ue->e_cgi.cell_id;
    req->user_location_information.presence = 1;
    gtp_build_uli(&req->user_location_information, &uli,
            uli_buf, GTP_MAX_ULI_LEN);

    memset(&indication, 0, sizeof(gtp_indication_t));
    indication.oi = 1;
    req->indication_flags.presence = 1;
    req->indication_flags.data = &indication;
    req->indication_flags.len = sizeof(gtp_indication_t);

    rv = gtp_build_msg(pkbuf, GTP_DELETE_SESSION_REQUEST_TYPE, &gtp_message);
    d_assert(rv == CORE_OK, return CORE_ERROR, "gtp build failed");

    return CORE_OK;
}

status_t mme_s11_build_release_access_bearers_request(pkbuf_t **pkbuf)
{
    status_t rv;
    gtp_message_t gtp_message;
    gtp_release_access_bearers_request_t *req = 
        &gtp_message.release_access_bearers_request;

    memset(&gtp_message, 0, sizeof(gtp_message_t));

    req->originating_node.presence = 1;
    req->originating_node.u8 = GTP_NODE_TYPE_MME;

    rv = gtp_build_msg(pkbuf, 
            GTP_RELEASE_ACCESS_BEARERS_REQUEST_TYPE, &gtp_message);
    d_assert(rv == CORE_OK, return CORE_ERROR, "gtp build failed");

    return CORE_OK;
}

status_t mme_s11_build_downlink_data_notification_ack(pkbuf_t **pkbuf, 
        mme_sess_t *sess)
{
    status_t rv;
    mme_ue_t *mme_ue = NULL;
    gtp_message_t gtp_message;
    gtp_downlink_data_notification_acknowledge_t *ack = 
        &gtp_message.downlink_data_notification_acknowledge;

    gtp_cause_t cause;

    d_assert(sess, return CORE_ERROR, "Null param");
    mme_ue = sess->mme_ue;
    d_assert(mme_ue, return CORE_ERROR, "Null param");

    memset(&gtp_message, 0, sizeof(gtp_message_t));

    memset(&cause, 0, sizeof(cause));
    cause.value = GTP_CAUSE_REQUEST_ACCEPTED;

    ack->cause.presence = 1;
    ack->cause.data = &cause;
    ack->cause.len = sizeof(cause);

    rv = gtp_build_msg(pkbuf, GTP_DOWNLINK_DATA_NOTIFICATION_ACKNOWLEDGE_TYPE, 
            &gtp_message);
    d_assert(rv == CORE_OK, return CORE_ERROR, "gtp build failed");

    return CORE_OK;
}
