#define TRACE_MODULE _mme_s11_build

#include "core_debug.h"

#include "gtp_types.h"
#include "gtp_tlv.h"

#include "3gpp_common.h"
#include "mme_context.h"

status_t mme_s11_build_create_session_req(
        c_uint8_t *type, pkbuf_t **pkbuf, mme_ue_t *ue)
{
    status_t rv;
    gtp_message_t gtp_message;
    gtp_create_session_request_t *req = &gtp_message.create_session_request;
    *type = GTP_CREATE_SESSION_REQUEST_TYPE;

    gtp_uli_t uli;
    char uli_buf[GTP_MAX_ULI_LEN];
    plmn_id_t serving_network;
    gtp_f_teid_t s11, s5;
    gtp_paa_t paa;
    gtp_ambr_t ambr;
    pco_t pco;
    char pcobuf[MAX_PCO_LEN];
    gtp_bearer_qos_t bearer_qos;
    char bearer_qos_buf[GTP_BEARER_QOS_LEN];
    gtp_ue_timezone_t ue_timezone;

    memset(&gtp_message, 0, sizeof(gtp_message_t));

    req->imsi.presence = 1;
    req->imsi.data = (c_uint8_t *)"\x55\x15\x30\x11\x34\x00\x10\xf4";
    req->imsi.len = 8;

    req->msisdn.presence = 1;
    req->msisdn.data = (c_uint8_t *)"\x94\x71\x52\x76\x00\x41";
    req->msisdn.len = 6;

    req->me_identity.presence = 1;
    req->me_identity.data = (c_uint8_t *)"\x53\x61\x20\x00\x91\x78\x84\x00";
    req->me_identity.len = 8;

    memset(&uli, 0, sizeof(gtp_uli_t));
    uli.flags.ecgi = 1;
    uli.flags.tai = 1;
    plmn_id_build(&uli.tai.plmn_id, 555, 10, 2);
    uli.tai.tac = 4130;
    plmn_id_build(&uli.ecgi.plmn_id, 555, 10, 2);
    uli.ecgi.eci = 105729;
    req->user_location_information.presence = 1;
    gtp_build_uli(&req->user_location_information, &uli, 
            uli_buf, GTP_MAX_ULI_LEN);

    req->serving_network.presence = 1;
    req->serving_network.data = plmn_id_build(&serving_network, 555, 10, 2);
    req->serving_network.len = sizeof(serving_network);

    req->rat_type.presence = 1;
    req->rat_type.u8 = GTP_RAT_TYPE_EUTRAN;

    memset(&s11, 0, sizeof(gtp_f_teid_t));
    s11.ipv4 = 1;
    s11.interface_type = GTP_F_TEID_S11_MME_GTP_C;
#if 0
    s11.teid = htonl(0x80000084);
#else
    s11.teid = htonl(1);
#endif
    s11.ipv4_addr = inet_addr("10.50.54.10");
    req->sender_f_teid_for_control_plane.presence = 1;
    req->sender_f_teid_for_control_plane.data = &s11;
    req->sender_f_teid_for_control_plane.len = GTP_F_TEID_IPV4_LEN;

    memset(&s5, 0, sizeof(gtp_f_teid_t));
    s5.ipv4 = 1;
    s5.interface_type = GTP_F_TEID_S5_S8_PGW_GTP_C;
    s5.ipv4_addr = inet_addr("10.50.54.37");
    req->pgw_s5_s8_address_for_control_plane_or_pmip.presence = 1;
    req->pgw_s5_s8_address_for_control_plane_or_pmip.data = &s5;
    req->pgw_s5_s8_address_for_control_plane_or_pmip.len = GTP_F_TEID_IPV4_LEN;

    req->access_point_name.presence = 1;
    req->access_point_name.data = "cellwire.com";
    req->access_point_name.len = strlen(req->access_point_name.data);

    req->selection_mode.presence = 1;
    req->selection_mode.u8 = GTP_SELECTION_MODE_MS_OR_NETWORK_PROVIDED_APN | 0xfc;

    req->pdn_type.presence = 1;
    req->pdn_type.u8 = GTP_PDN_TYPE_IPV4;

    memset(&paa, 0, sizeof(gtp_paa_t));
    paa.pdn_type = GTP_PDN_TYPE_IPV4;
    req->pdn_address_allocation.presence = 1;
    req->pdn_address_allocation.data = &paa;
    req->pdn_address_allocation.len = GTP_PAA_IPV4_LEN;

    req->maximum_apn_restriction.presence = 1;
    req->maximum_apn_restriction.u8 = GTP_APN_NO_RESTRICTION;

    memset(&ambr, 0, sizeof(gtp_ambr_t));
    ambr.uplink = htonl(1000);
    ambr.downlink = htonl(2000);
    req->aggregate_maximum_bit_rate.presence = 1;
    req->aggregate_maximum_bit_rate.data = &ambr;
    req->aggregate_maximum_bit_rate.len = sizeof(ambr);

    memset(&pco, 0, sizeof(pco_t));
    pco.ext = 1;
    pco.configuration_protocol = 
        PCO_PPP_FOR_USE_WITH_IP_PDP_TYPE_OR_IP_PDN_TYPE;
    pco.num_of_id = 3;
    pco.ids[0].id = PROTOCOL_OR_CONTAINER_ID_INTERNET_PROTOCOL_CONTROL_PROTOCOL;
    pco.ids[0].contents = (c_uint8_t *)"\x01\x00\x00\x10\x81\x06\x00\x00\x00\x00\x83\x06\x00\x00\x00\x00";
    pco.ids[0].length = 16;
    pco.ids[1].id = PROTOCOL_OR_CONTAINER_ID_DNS_SERVER_IPV4_ADDRESS_REQUEST;
    pco.ids[1].length = 0;
    pco.ids[2].id = PROTOCOL_OR_CONTAINER_ID_IP_ADDRESS_ALLOCATION_VIA_NAS_SIGNALLING;
    pco.ids[2].length = 0;

    req->protocol_configuration_options.presence = 1;
    req->protocol_configuration_options.data = &pcobuf;
    req->protocol_configuration_options.len = 
        pco_build(pcobuf, MAX_PCO_LEN, &pco);

    req->bearer_contexts_to_be_created.presence = 1;
    req->bearer_contexts_to_be_created.eps_bearer_id.presence = 1;
    req->bearer_contexts_to_be_created.eps_bearer_id.u8 = 5;

    memset(&bearer_qos, 0, sizeof(bearer_qos));
    bearer_qos.pvi = 1;
    bearer_qos.pl = 1;
    bearer_qos.pci = 1;
    bearer_qos.qci = 5;
    req->bearer_contexts_to_be_created.bearer_level_qos.presence = 1;
    gtp_build_bearer_qos(&req->bearer_contexts_to_be_created.bearer_level_qos,
            &bearer_qos, bearer_qos_buf, GTP_BEARER_QOS_LEN);

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

    rv = gtp_build_msg(pkbuf, *type, &gtp_message);
    d_assert(rv == CORE_OK, return CORE_ERROR, "gtp build failed");

    return CORE_OK;
}

