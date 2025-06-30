/*
 * Copyright (C) 2024-2025 by Sukchan Lee <acetcom@gmail.com>
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

#include "nsmf-build.h"
#include "gsm-build.h"

ogs_sbi_request_t *smf_nsmf_pdusession_build_create_data(
        smf_sess_t *sess, void *data)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    ogs_sbi_server_t *server = NULL;
    ogs_sbi_header_t header;

    smf_ue_t *smf_ue = NULL;

    OpenAPI_pdu_session_create_data_t PduSessionCreateData;
    OpenAPI_snssai_t sNssai;
    OpenAPI_snssai_t hplmnSnssai;
    OpenAPI_tunnel_info_t vcnTunnelInfo;
    OpenAPI_ref_to_binary_data_t n1SmInfoFromUe;
    OpenAPI_user_location_t ueLocation;

#if 0 /* Needs to be checked against AMF's nsmf-build.c */
    ogs_sbi_nf_instance_t *pcf_nf_instance = NULL;
#endif

    int rv;
    ogs_nas_5gs_message_t nas_message;
    ogs_nas_5gs_pdu_session_establishment_request_t
        *pdu_session_establishment_request = NULL;
    ogs_nas_integrity_protection_maximum_data_rate_t
        *integrity_protection_maximum_data_rate = NULL;
    ogs_pkbuf_t *n1SmBufFromUe = NULL;

    ogs_assert(sess);
    smf_ue = smf_ue_find_by_id(sess->smf_ue_id);
    ogs_assert(smf_ue);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_POST;
    ogs_assert(sess->h_smf_uri);
    message.h.uri = sess->h_smf_uri;

    memset(&header, 0, sizeof(header));
    memset(&PduSessionCreateData, 0, sizeof(PduSessionCreateData));
    memset(&sNssai, 0, sizeof(sNssai));
    memset(&hplmnSnssai, 0, sizeof(hplmnSnssai));
    memset(&vcnTunnelInfo, 0, sizeof(vcnTunnelInfo));
    memset(&ueLocation, 0, sizeof(ueLocation));

    PduSessionCreateData.supi = smf_ue->supi;
    if (smf_ue->imeisv_len) {
        PduSessionCreateData.pei = ogs_msprintf("%s-%s",
                OGS_ID_SUPI_TYPE_IMEISV, smf_ue->imeisv_bcd);
    }
    if (smf_ue->msisdn_len) {
        PduSessionCreateData.gpsi = ogs_msprintf("%s-%s",
                OGS_ID_GPSI_TYPE_MSISDN, smf_ue->msisdn_bcd);
    }
    PduSessionCreateData.is_pdu_session_id = true;
    PduSessionCreateData.pdu_session_id = sess->psi;
    if (!PduSessionCreateData.pdu_session_id) {
        ogs_error("No pdu_session_id");
        goto end;
    }

    /*
     * TS29.502
     * 6.1 Nsmf_PDUSession Service API
     * Table 6.1.6.2.2-1: Definition of type PduSessionCreateData
     *
     * NAME: dnn
     * Data type: Dnn
     * P: C
     * Cardinality: 0..1
     *
     * This IE shall be present, except during an EPS to 5GS Idle mode mobility
     * or handover using the N26 interface.
     *
     * When present, it shall contain the requested DNN; the DNN shall
     * be the full DNN (i.e. with both the Network Identifier and
     * Operator Identifier) for a HR PDU session, and it should be
     * the full DNN in LBO and non-roaming scenarios. If the Operator Identifier
     * is absent, the serving core network operator shall be assumed.
     */
    PduSessionCreateData.dnn = ogs_strdup(sess->full_dnn);
    ogs_assert(PduSessionCreateData.dnn);

    sNssai.sst = sess->s_nssai.sst;
    sNssai.sd = ogs_s_nssai_sd_to_string(sess->s_nssai.sd);
    PduSessionCreateData.s_nssai = &sNssai;

    if (sess->mapped_hplmn.sst) {
        hplmnSnssai.sst = sess->mapped_hplmn.sst;
        hplmnSnssai.sd = ogs_s_nssai_sd_to_string(sess->mapped_hplmn.sd);
        PduSessionCreateData.hplmn_snssai = &hplmnSnssai;
    }

    PduSessionCreateData.vsmf_id = NF_INSTANCE_ID(ogs_sbi_self()->nf_instance);
    if (!PduSessionCreateData.vsmf_id) {
        ogs_error("No vsmfId");
        goto end;
    }

    PduSessionCreateData.serving_network =
        ogs_sbi_build_plmn_id_nid(&sess->nr_tai.plmn_id);
    if (!PduSessionCreateData.serving_network) {
        ogs_error("No serving_network");
        goto end;
    }

    if (sess->request_type >= OpenAPI_request_type_INITIAL_REQUEST &&
            sess->request_type <=
            OpenAPI_request_type_EXISTING_EMERGENCY_PDU_SESSION)
        PduSessionCreateData.request_type = sess->request_type;

    header.service.name = (char *)OGS_SBI_SERVICE_NAME_NSMF_PDUSESSION;
    header.api.version = (char *)OGS_SBI_API_V1;
    header.resource.component[0] =
        (char *)OGS_SBI_RESOURCE_NAME_VSMF_PDU_SESSIONS;
    header.resource.component[1] = sess->sm_context_ref;
    if (!header.resource.component[1]) {
        ogs_error("No header.resource.component[1]");
        goto end;
    }

    server = ogs_sbi_server_first();
    if (!server) {
        ogs_error("No server");
        goto end;
    }
    PduSessionCreateData.vsmf_pdu_session_uri =
        ogs_sbi_server_uri(server, &header);

    if (sess->local_dl_addr)
        vcnTunnelInfo.ipv4_addr = ogs_ipstrdup(sess->local_dl_addr);
    if (sess->local_dl_addr6)
        vcnTunnelInfo.ipv6_addr = ogs_ipstrdup(sess->local_dl_addr6);
    vcnTunnelInfo.gtp_teid = ogs_uint32_to_0string(sess->local_dl_teid);
    PduSessionCreateData.vcn_tunnel_info = &vcnTunnelInfo;

    PduSessionCreateData.an_type = sess->an_type;
    PduSessionCreateData.rat_type = sess->sbi_rat_type;

    ueLocation.nr_location = ogs_sbi_build_nr_location(
            &sess->nr_tai, &sess->nr_cgi);
    if (!ueLocation.nr_location) {
        ogs_error("No ueLocation.nr_location");
        goto end;
    }
    if (sess->ue_location_timestamp)
        ueLocation.nr_location->ue_location_timestamp =
            ogs_sbi_gmtime_string(sess->ue_location_timestamp);

    PduSessionCreateData.ue_location = &ueLocation;
    PduSessionCreateData.ue_time_zone = ogs_sbi_timezone_string(ogs_timezone());
    if (!PduSessionCreateData.ue_time_zone) {
        ogs_error("No ue_time_zone");
        goto end;
    }

    /*
     * We're experiencing an issue after changing SearchResult.validityTime
     * from 3600 seconds to 30 seconds. (#3210)
     *
     * When AMF finds a PCF through Discovery, it can be deleted
     * after 30 seconds by ValidityTime.
     *
     * We have changed our implementation to not send the PCF-ID in this case.
     *
     * What we need to do is proactively add a part that will re-discover
     * the PCF when a situation arises where we really need the PCF-ID.
     */
#if 0 /* Needs to be checked against AMF's nsmf-build.c */
    pcf_nf_instance = OGS_SBI_GET_NF_INSTANCE(
            amf_ue->sbi.service_type_array[
            OGS_SBI_SERVICE_TYPE_NPCF_AM_POLICY_CONTROL]);
    if (pcf_nf_instance)
        PduSessionCreateData.pcf_id = pcf_nf_instance->id;
    else
        ogs_error("No pcf_nf_instance");
#endif

    ogs_assert(sess->amf_nf_id);
    PduSessionCreateData.amf_nf_id = sess->amf_nf_id;

    PduSessionCreateData.guami = ogs_sbi_build_guami(&sess->guami);
    if (!PduSessionCreateData.guami) {
        ogs_error("No guami");
        goto end;
    }

    ogs_assert(sess->n1SmBufFromUe);
    rv = ogs_nas_5gsm_decode(&nas_message, sess->n1SmBufFromUe);

    if (rv == OGS_OK) {
        n1SmBufFromUe = gsmue_encode_n1_sm_info(&nas_message);
        message.part[message.num_of_part].pkbuf = n1SmBufFromUe;
        if (message.part[message.num_of_part].pkbuf) {
            message.part[message.num_of_part].content_id =
                (char *)OGS_SBI_CONTENT_5GNAS_SM_ID;
            message.part[message.num_of_part].content_type =
                (char *)OGS_SBI_CONTENT_5GNAS_TYPE;
            message.num_of_part++;

            n1SmInfoFromUe.content_id = (char *)OGS_SBI_CONTENT_5GNAS_SM_ID;
            PduSessionCreateData.n1_sm_info_from_ue = &n1SmInfoFromUe;

            pdu_session_establishment_request =
                &nas_message.gsm.pdu_session_establishment_request;
            integrity_protection_maximum_data_rate =
                &pdu_session_establishment_request->
                integrity_protection_maximum_data_rate;

            switch (integrity_protection_maximum_data_rate->ul) {
            case OGS_NAS_INTEGRITY_PROTECTION_MAXIMUM_DATA_RATE_64KBPS:
                PduSessionCreateData.max_integrity_protected_data_rate_ul =
                    OpenAPI_max_integrity_protected_data_rate__64_KBPS;
                break;
            case OGS_NAS_INTEGRITY_PROTECTION_MAXIMUM_DATA_RATE_NULL:
                PduSessionCreateData.max_integrity_protected_data_rate_ul =
                    OpenAPI_max_integrity_protected_data_rate_NULL;
                break;
            case OGS_NAS_INTEGRITY_PROTECTION_MAXIMUM_DATA_RATE_FULL:
                PduSessionCreateData.max_integrity_protected_data_rate_ul =
                    OpenAPI_max_integrity_protected_data_rate_MAX_UE_RATE;
                break;
            default:
                ogs_error("Unknown Integrity Protection "
                        "Maximum Data Rate [UL:%d]",
                        integrity_protection_maximum_data_rate->ul);
                break;
            }

            switch (integrity_protection_maximum_data_rate->dl) {
            case OGS_NAS_INTEGRITY_PROTECTION_MAXIMUM_DATA_RATE_64KBPS:
                PduSessionCreateData.max_integrity_protected_data_rate_dl =
                    OpenAPI_max_integrity_protected_data_rate__64_KBPS;
                break;
            case OGS_NAS_INTEGRITY_PROTECTION_MAXIMUM_DATA_RATE_NULL:
                PduSessionCreateData.max_integrity_protected_data_rate_dl =
                    OpenAPI_max_integrity_protected_data_rate_NULL;
                break;
            case OGS_NAS_INTEGRITY_PROTECTION_MAXIMUM_DATA_RATE_FULL:
                PduSessionCreateData.max_integrity_protected_data_rate_dl =
                    OpenAPI_max_integrity_protected_data_rate_MAX_UE_RATE;
                break;
            defadlt:
                ogs_error("Unknown Integrity Protection "
                        "Maximum Data Rate [DL:%d]",
                        integrity_protection_maximum_data_rate->dl);
                break;
            }
        } else {
            ogs_error("gsm_encode_n1_sm_info() failed [%d]", rv);
            ogs_log_hexdump(OGS_LOG_ERROR,
                    sess->n1SmBufFromUe->data, sess->n1SmBufFromUe->len);
        }
    } else {
        ogs_error("ogs_nas_5gsm_decode() failed [%d]", rv);
        ogs_log_hexdump(OGS_LOG_ERROR, sess->n1SmBufFromUe->data, sess->n1SmBufFromUe->len);
    }

    message.PduSessionCreateData = &PduSessionCreateData;

    message.http.accept = (char *)(OGS_SBI_CONTENT_JSON_TYPE ","
        OGS_SBI_CONTENT_NGAP_TYPE "," OGS_SBI_CONTENT_PROBLEM_TYPE);

    request = ogs_sbi_build_request(&message);
    ogs_expect(request);

end:
    if (header.resource.component[2])
        ogs_free(header.resource.component[2]);

    if (PduSessionCreateData.serving_network)
        ogs_sbi_free_plmn_id_nid(PduSessionCreateData.serving_network);
    if (PduSessionCreateData.dnn)
        ogs_free(PduSessionCreateData.dnn);
    if (PduSessionCreateData.vsmf_pdu_session_uri)
        ogs_free(PduSessionCreateData.vsmf_pdu_session_uri);
    if (vcnTunnelInfo.ipv4_addr)
        ogs_free(vcnTunnelInfo.ipv4_addr);
    if (vcnTunnelInfo.ipv6_addr)
        ogs_free(vcnTunnelInfo.ipv6_addr);
    if (vcnTunnelInfo.gtp_teid)
        ogs_free(vcnTunnelInfo.gtp_teid);
    if (sNssai.sd)
        ogs_free(sNssai.sd);
    if (hplmnSnssai.sd)
        ogs_free(hplmnSnssai.sd);
    if (PduSessionCreateData.guami)
        ogs_sbi_free_guami(PduSessionCreateData.guami);
    if (PduSessionCreateData.pei)
        ogs_free(PduSessionCreateData.pei);
    if (PduSessionCreateData.gpsi)
        ogs_free(PduSessionCreateData.gpsi);
    if (ueLocation.nr_location) {
        if (ueLocation.nr_location->ue_location_timestamp)
            ogs_free(ueLocation.nr_location->ue_location_timestamp);
        ogs_sbi_free_nr_location(ueLocation.nr_location);
    }
    if (PduSessionCreateData.ue_time_zone)
        ogs_free(PduSessionCreateData.ue_time_zone);

    if (sess->n1SmBufFromUe) {
        ogs_pkbuf_free(sess->n1SmBufFromUe);
        sess->n1SmBufFromUe = NULL;
    }

    if (n1SmBufFromUe)
        ogs_pkbuf_free(n1SmBufFromUe);

    if (message.http.custom.nrf_uri)
        ogs_free(message.http.custom.nrf_uri);

    return request;
}

ogs_sbi_request_t *smf_nsmf_pdusession_build_hsmf_update_data(
        smf_sess_t *sess, void *data)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    smf_ue_t *smf_ue = NULL;

    OpenAPI_hsmf_update_data_t HsmfUpdateData;
    OpenAPI_ng_ap_cause_t ngApCause;
    OpenAPI_tunnel_info_t vcnTunnelInfo;
    OpenAPI_user_location_t ueLocation;

    int rv;
    ogs_nas_5gs_message_t nas_message;
    ogs_pkbuf_t *n1SmBufFromUe = NULL;
    OpenAPI_ref_to_binary_data_t n1SmInfoFromUe;

    ogs_assert(sess);
    smf_ue = smf_ue_find_by_id(sess->smf_ue_id);
    ogs_assert(smf_ue);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_POST;
    ogs_assert(sess->pdu_session_resource_uri);
    message.h.uri = ogs_msprintf("%s/%s",
            sess->pdu_session_resource_uri, OGS_SBI_RESOURCE_NAME_MODIFY);
    ogs_assert(message.h.uri);

    memset(&HsmfUpdateData, 0, sizeof(HsmfUpdateData));
    memset(&ngApCause, 0, sizeof(ngApCause));
    memset(&vcnTunnelInfo, 0, sizeof(vcnTunnelInfo));
    memset(&ueLocation, 0, sizeof(ueLocation));

    HsmfUpdateData.request_indication = sess->nsmf_param.request_indication;
    ogs_assert(HsmfUpdateData.request_indication);

    HsmfUpdateData.cause = sess->nsmf_param.cause;

    HsmfUpdateData.up_cnx_state = sess->nsmf_param.up_cnx_state;

    if (sess->nsmf_param.ngap_cause.group) {
        HsmfUpdateData.ng_ap_cause = &ngApCause;
        ngApCause.group = sess->nsmf_param.ngap_cause.group;
        ngApCause.value = sess->nsmf_param.ngap_cause.value;
    }

    if (sess->nsmf_param.gmm_cause) {
        HsmfUpdateData.is__5g_mm_cause_value = true;
        HsmfUpdateData._5g_mm_cause_value = sess->nsmf_param.gmm_cause;
    }

    if (sess->nsmf_param.serving_network) {
        HsmfUpdateData.serving_network =
            ogs_sbi_build_plmn_id_nid(&sess->nr_tai.plmn_id);
        if (!HsmfUpdateData.serving_network) {
            ogs_error("No serving_network");
            goto end;
        }
    }

    if (sess->nsmf_param.dl_ip.ipv4)
        vcnTunnelInfo.ipv4_addr = ogs_ipv4_to_string(
                sess->nsmf_param.dl_ip.addr);

    if (sess->nsmf_param.dl_ip.ipv6)
        vcnTunnelInfo.ipv6_addr = ogs_ipv6addr_to_string(
                sess->nsmf_param.dl_ip.addr6);

    if (vcnTunnelInfo.ipv4_addr || vcnTunnelInfo.ipv6_addr) {
        vcnTunnelInfo.gtp_teid = ogs_uint32_to_0string(
                sess->nsmf_param.dl_teid);
        HsmfUpdateData.vcn_tunnel_info = &vcnTunnelInfo;
    }

    HsmfUpdateData.an_type = sess->nsmf_param.an_type;
    HsmfUpdateData.rat_type = sess->nsmf_param.rat_type;

    if (sess->nsmf_param.ue_location) {
        ueLocation.nr_location = ogs_sbi_build_nr_location(
                &sess->nr_tai, &sess->nr_cgi);
        if (!ueLocation.nr_location) {
            ogs_error("No ueLocation.nr_location");
            goto end;
        }
        if (sess->ue_location_timestamp)
            ueLocation.nr_location->ue_location_timestamp =
                ogs_sbi_gmtime_string(sess->ue_location_timestamp);

        HsmfUpdateData.ue_location = &ueLocation;
    }
    if (sess->nsmf_param.ue_timezone) {
        HsmfUpdateData.ue_time_zone = ogs_sbi_timezone_string(ogs_timezone());
        if (!HsmfUpdateData.ue_time_zone) {
            ogs_error("No ue_time_zone");
            goto end;
        }
    }

    if (sess->n1SmBufFromUe) {
        rv = ogs_nas_5gsm_decode(&nas_message, sess->n1SmBufFromUe);

        if (rv == OGS_OK) {
            n1SmBufFromUe = gsmue_encode_n1_sm_info(&nas_message);
            message.part[message.num_of_part].pkbuf = n1SmBufFromUe;
            if (message.part[message.num_of_part].pkbuf) {
                message.part[message.num_of_part].content_id =
                    (char *)OGS_SBI_CONTENT_5GNAS_SM_ID;
                message.part[message.num_of_part].content_type =
                    (char *)OGS_SBI_CONTENT_5GNAS_TYPE;
                message.num_of_part++;

                n1SmInfoFromUe.content_id = (char *)OGS_SBI_CONTENT_5GNAS_SM_ID;
                HsmfUpdateData.n1_sm_info_from_ue = &n1SmInfoFromUe;
            } else {
                ogs_error("gsm_encode_n1_sm_info() failed [%d]", rv);
                ogs_log_hexdump(OGS_LOG_ERROR,
                        sess->n1SmBufFromUe->data, sess->n1SmBufFromUe->len);
            }
        } else {
            ogs_error("ogs_nas_5gsm_decode() failed [%d]", rv);
            ogs_log_hexdump(OGS_LOG_ERROR,
                    sess->n1SmBufFromUe->data, sess->n1SmBufFromUe->len);
        }
    }

    message.HsmfUpdateData = &HsmfUpdateData;

    request = ogs_sbi_build_request(&message);
    ogs_expect(request);

end:
    if (message.h.uri)
        ogs_free(message.h.uri);

    if (HsmfUpdateData.serving_network)
        ogs_sbi_free_plmn_id_nid(HsmfUpdateData.serving_network);

    if (vcnTunnelInfo.ipv4_addr)
        ogs_free(vcnTunnelInfo.ipv4_addr);
    if (vcnTunnelInfo.ipv6_addr)
        ogs_free(vcnTunnelInfo.ipv6_addr);
    if (vcnTunnelInfo.gtp_teid)
        ogs_free(vcnTunnelInfo.gtp_teid);

    if (ueLocation.nr_location) {
        if (ueLocation.nr_location->ue_location_timestamp)
            ogs_free(ueLocation.nr_location->ue_location_timestamp);
        ogs_sbi_free_nr_location(ueLocation.nr_location);
    }
    if (HsmfUpdateData.ue_time_zone)
        ogs_free(HsmfUpdateData.ue_time_zone);

    if (sess->n1SmBufFromUe) {
        ogs_pkbuf_free(sess->n1SmBufFromUe);
        sess->n1SmBufFromUe = NULL;
    }

    return request;
}

ogs_sbi_request_t *smf_nsmf_pdusession_build_vsmf_update_data(
        smf_sess_t *sess, void *data)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    OpenAPI_vsmf_update_data_t VsmfUpdateData;

    OpenAPI_ref_to_binary_data_t n1SmInfoToUe;
    ogs_pkbuf_t *n1SmBufToUe = NULL;

    OpenAPI_list_t *qosFlowsAddModRequestList = NULL;
    OpenAPI_qos_flow_add_modify_request_item_t *qosFlowAddModRequestItem =
        NULL;
    OpenAPI_list_t *qosFlowsRelRequestList = NULL;
    OpenAPI_qos_flow_release_request_item_t *qosFlowRelRequestItem = NULL;

    smf_bearer_t *qos_flow = NULL;

    ogs_assert(sess);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_POST;
    ogs_assert(sess->vsmf_pdu_session_uri);
    message.h.uri = ogs_msprintf("%s/%s",
            sess->vsmf_pdu_session_uri, OGS_SBI_RESOURCE_NAME_MODIFY);
    ogs_assert(message.h.uri);

    memset(&VsmfUpdateData, 0, sizeof(VsmfUpdateData));

    VsmfUpdateData.request_indication = sess->nsmf_param.request_indication;
    ogs_assert(VsmfUpdateData.request_indication);

    switch (VsmfUpdateData.request_indication) {
    case OpenAPI_request_indication_UE_REQ_PDU_SES_MOD:
    case OpenAPI_request_indication_NW_REQ_PDU_SES_MOD:
        n1SmBufToUe = gsmue_build_pdu_session_modification_command(sess);
        ogs_assert(n1SmBufToUe);

        qosFlowsAddModRequestList = OpenAPI_list_create();
        ogs_assert(qosFlowsAddModRequestList);
        qosFlowsRelRequestList = OpenAPI_list_create();
        ogs_assert(qosFlowsRelRequestList);

        int i = 0;

        ogs_list_for_each_entry(
                &sess->qos_flow_to_modify_list, qos_flow, to_modify_node) {

            int rv, enc_len = 0;

            OpenAPI_qos_flow_profile_t *qosFlowProfile = NULL;
            OpenAPI_arp_t *Arp = NULL;
            OpenAPI_gbr_qos_flow_information_t *gbrQosFlowInfo = NULL;

            char *encoded_qos_rules = NULL;
            char *encoded_qos_flow_description = NULL;

            ogs_assert(i < OGS_MAX_NUM_OF_BEARER);

            if (sess->nsmf_param.qos_rule_code) {
                ogs_nas_qos_rules_t authorized_qos_rules;
                ogs_nas_qos_rule_t qos_rule;

                memset(&qos_rule, 0, sizeof(qos_rule));
                gsm_encode_qos_rule(
                        &qos_rule, qos_flow, sess->nsmf_param.qos_rule_code);

                rv = ogs_nas_build_qos_rules(
                        &authorized_qos_rules, &qos_rule, 1);
                if (rv != OGS_OK) {
                    ogs_error("ogs_nas_build_qos_rules() failed");
                    goto end;
                }
                if (!authorized_qos_rules.length) {
                    ogs_error("No length");
                    goto end;
                }

                enc_len = ogs_base64_encode_len(authorized_qos_rules.length);
                encoded_qos_rules = ogs_calloc(1, enc_len);
                ogs_assert(encoded_qos_rules);
                ogs_base64_encode(
                        encoded_qos_rules,
                        authorized_qos_rules.buffer,
                        authorized_qos_rules.length);

                ogs_free(authorized_qos_rules.buffer);
            }

            if (sess->nsmf_param.qos_flow_description_code) {
                ogs_nas_qos_flow_descriptions_t
                    authorized_qos_flow_descriptions;
                ogs_nas_qos_flow_description_t qos_flow_description;

                memset(&qos_flow_description, 0, sizeof(qos_flow_description));
                gsm_encode_qos_flow_description(
                        &qos_flow_description, qos_flow,
                        sess->nsmf_param.qos_flow_description_code);
                rv = ogs_nas_build_qos_flow_descriptions(
                        &authorized_qos_flow_descriptions,
                        &qos_flow_description, 1);
                if (rv != OGS_OK) {
                    ogs_error("ogs_nas_build_qos_flow_descriptions() failed");
                    goto end;
                }
                if (!authorized_qos_flow_descriptions.length) {
                    ogs_error("No length");
                    goto end;
                }

                enc_len = ogs_base64_encode_len(
                        authorized_qos_flow_descriptions.length);
                encoded_qos_flow_description = ogs_calloc(1, enc_len);
                ogs_assert(encoded_qos_flow_description);
                ogs_base64_encode(
                        encoded_qos_flow_description,
                        authorized_qos_flow_descriptions.buffer,
                        authorized_qos_flow_descriptions.length);

                ogs_free(authorized_qos_flow_descriptions.buffer);
            }

            if (sess->nsmf_param.qos_rule_code ==
                    OGS_NAS_QOS_CODE_DELETE_EXISTING_QOS_RULE ||
                sess->nsmf_param.qos_flow_description_code ==
                    OGS_NAS_DELETE_NEW_QOS_FLOW_DESCRIPTION) {

                if (sess->nsmf_param.qos_rule_code !=
                        OGS_NAS_QOS_CODE_DELETE_EXISTING_QOS_RULE ||
                    sess->nsmf_param.qos_flow_description_code !=
                        OGS_NAS_DELETE_NEW_QOS_FLOW_DESCRIPTION)
                    ogs_error("Invalid qosRule[%d]/qosFlowDesc[%d]",
                        sess->nsmf_param.qos_rule_code,
                        sess->nsmf_param.qos_flow_description_code);

                qosFlowRelRequestItem =
                    ogs_calloc(1, sizeof(*qosFlowRelRequestItem));
                ogs_assert(qosFlowRelRequestItem);

                qosFlowRelRequestItem->qfi = qos_flow->qfi;

                qosFlowRelRequestItem->qos_rules = encoded_qos_rules;
                qosFlowRelRequestItem->qos_flow_description =
                    encoded_qos_flow_description;

                OpenAPI_list_add(qosFlowsRelRequestList, qosFlowRelRequestItem);

            } else if (sess->nsmf_param.qos_rule_code ||
                    sess->nsmf_param.qos_flow_description_code) {
                Arp = ogs_calloc(1, sizeof(*Arp));
                ogs_assert(Arp);
                if (qos_flow->qos.arp.pre_emption_capability ==
                        OGS_5GC_PRE_EMPTION_ENABLED)
                    Arp->preempt_cap =
                        OpenAPI_preemption_capability_MAY_PREEMPT;
                else if (qos_flow->qos.arp.pre_emption_capability ==
                        OGS_5GC_PRE_EMPTION_DISABLED)
                    Arp->preempt_cap =
                        OpenAPI_preemption_capability_NOT_PREEMPT;
                else {
                    ogs_error("No Arp->preempt_cap");
                    goto end;
                }

                if (qos_flow->qos.arp.pre_emption_vulnerability ==
                        OGS_5GC_PRE_EMPTION_ENABLED)
                    Arp->preempt_vuln =
                        OpenAPI_preemption_vulnerability_PREEMPTABLE;
                else if (qos_flow->qos.arp.pre_emption_vulnerability ==
                        OGS_5GC_PRE_EMPTION_DISABLED)
                    Arp->preempt_vuln =
                        OpenAPI_preemption_vulnerability_NOT_PREEMPTABLE;
                else {
                    ogs_error("No Arp->preempt_vuln");
                    goto end;
                }
                Arp->priority_level = qos_flow->qos.arp.priority_level;

                if (qos_flow->qos.mbr.downlink && qos_flow->qos.mbr.uplink &&
                    qos_flow->qos.gbr.downlink && qos_flow->qos.gbr.uplink) {
                    gbrQosFlowInfo = ogs_calloc(1, sizeof(*gbrQosFlowInfo));
                    ogs_assert(gbrQosFlowInfo);

                    gbrQosFlowInfo->max_fbr_ul = ogs_sbi_bitrate_to_string(
                            qos_flow->qos.mbr.uplink, OGS_SBI_BITRATE_BPS);
                    gbrQosFlowInfo->max_fbr_dl = ogs_sbi_bitrate_to_string(
                            qos_flow->qos.mbr.downlink, OGS_SBI_BITRATE_BPS);

                    gbrQosFlowInfo->gua_fbr_ul = ogs_sbi_bitrate_to_string(
                            qos_flow->qos.gbr.uplink, OGS_SBI_BITRATE_BPS);
                    gbrQosFlowInfo->gua_fbr_dl = ogs_sbi_bitrate_to_string(
                            qos_flow->qos.gbr.downlink, OGS_SBI_BITRATE_BPS);
                }

                qosFlowProfile = ogs_calloc(1, sizeof(*qosFlowProfile));
                ogs_assert(qosFlowProfile);
                qosFlowProfile->arp = Arp;
                qosFlowProfile->_5qi = qos_flow->qos.index;
                qosFlowProfile->gbr_qos_flow_info = gbrQosFlowInfo;

                qosFlowAddModRequestItem =
                    ogs_calloc(1, sizeof(*qosFlowAddModRequestItem));
                ogs_assert(qosFlowAddModRequestItem);

                qosFlowAddModRequestItem->qfi = qos_flow->qfi;

                qosFlowAddModRequestItem->qos_rules = encoded_qos_rules;
                qosFlowAddModRequestItem->qos_flow_description =
                    encoded_qos_flow_description;

                qosFlowAddModRequestItem->qos_flow_profile = qosFlowProfile;

                OpenAPI_list_add(qosFlowsAddModRequestList,
                        qosFlowAddModRequestItem);
            } else {
                ogs_error("Invalid qosRule[%d]/qosFlowDesc[%d]",
                    sess->nsmf_param.qos_rule_code,
                    sess->nsmf_param.qos_flow_description_code);
            }

            i++;
        }

        if (qosFlowsAddModRequestList->count)
            VsmfUpdateData.qos_flows_add_mod_request_list =
                qosFlowsAddModRequestList;
        else
            OpenAPI_list_free(qosFlowsAddModRequestList);

        if (qosFlowsRelRequestList->count)
            VsmfUpdateData.qos_flows_rel_request_list = qosFlowsRelRequestList;
        else
            OpenAPI_list_free(qosFlowsRelRequestList);

        break;
    case OpenAPI_request_indication_NW_REQ_PDU_SES_REL:
    case OpenAPI_request_indication_UE_REQ_PDU_SES_REL:
        n1SmBufToUe = gsmue_build_pdu_session_release_command(sess);
        ogs_assert(n1SmBufToUe);
        break;
    default:
        ogs_fatal("Not implemented [request_indication:%d]",
                VsmfUpdateData.request_indication);

    }

    n1SmInfoToUe.content_id = (char *)OGS_SBI_CONTENT_5GNAS_SM_ID;
    VsmfUpdateData.n1_sm_info_to_ue = &n1SmInfoToUe;

    message.part[message.num_of_part].pkbuf = n1SmBufToUe;
    message.part[message.num_of_part].content_id =
        (char *)OGS_SBI_CONTENT_5GNAS_SM_ID;
    message.part[message.num_of_part].content_type =
        (char *)OGS_SBI_CONTENT_5GNAS_TYPE;
    message.num_of_part++;

    message.VsmfUpdateData = &VsmfUpdateData;

    request = ogs_sbi_build_request(&message);
    ogs_expect(request);

end:
    if (message.h.uri)
        ogs_free(message.h.uri);

    CLEAR_QOS_FLOWS_ADD_MOD_REQUEST_LIST(
            VsmfUpdateData.qos_flows_add_mod_request_list);
    CLEAR_QOS_FLOWS_REL_REQUEST_LIST(VsmfUpdateData.qos_flows_rel_request_list);

    return request;
}

ogs_sbi_request_t *smf_nsmf_pdusession_build_release_data(
        smf_sess_t *sess, void *data)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    smf_ue_t *smf_ue = NULL;

    OpenAPI_release_data_t ReleaseData;
    OpenAPI_ng_ap_cause_t ngApCause;
    OpenAPI_user_location_t ueLocation;

    ogs_assert(sess);
    smf_ue = smf_ue_find_by_id(sess->smf_ue_id);
    ogs_assert(smf_ue);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_POST;
    ogs_assert(sess->pdu_session_resource_uri);
    message.h.uri = ogs_msprintf("%s/%s",
            sess->pdu_session_resource_uri, OGS_SBI_RESOURCE_NAME_RELEASE);
    ogs_assert(message.h.uri);

    memset(&ReleaseData, 0, sizeof(ReleaseData));
    memset(&ngApCause, 0, sizeof(ngApCause));
    memset(&ueLocation, 0, sizeof(ueLocation));

    ReleaseData.cause = sess->nsmf_param.cause;

    if (sess->nsmf_param.ngap_cause.group) {
        ReleaseData.ng_ap_cause = &ngApCause;
        ngApCause.group = sess->nsmf_param.ngap_cause.group;
        ngApCause.value = sess->nsmf_param.ngap_cause.value;
    }

    if (sess->nsmf_param.gmm_cause) {
        ReleaseData.is__5g_mm_cause_value = true;
        ReleaseData._5g_mm_cause_value = sess->nsmf_param.gmm_cause;
    }

    if (sess->nsmf_param.ue_location) {
        ueLocation.nr_location = ogs_sbi_build_nr_location(
                &sess->nr_tai, &sess->nr_cgi);
        if (!ueLocation.nr_location) {
            ogs_error("No ueLocation.nr_location");
            goto end;
        }
        if (sess->ue_location_timestamp)
            ueLocation.nr_location->ue_location_timestamp =
                ogs_sbi_gmtime_string(sess->ue_location_timestamp);

        ReleaseData.ue_location = &ueLocation;
    }
    if (sess->nsmf_param.ue_timezone) {
        ReleaseData.ue_time_zone = ogs_sbi_timezone_string(ogs_timezone());
        if (!ReleaseData.ue_time_zone) {
            ogs_error("No ue_time_zone");
            goto end;
        }
    }

    message.ReleaseData = &ReleaseData;

    request = ogs_sbi_build_request(&message);
    ogs_expect(request);

end:
    if (message.h.uri)
        ogs_free(message.h.uri);

    if (ueLocation.nr_location) {
        if (ueLocation.nr_location->ue_location_timestamp)
            ogs_free(ueLocation.nr_location->ue_location_timestamp);
        ogs_sbi_free_nr_location(ueLocation.nr_location);
    }
    if (ReleaseData.ue_time_zone)
        ogs_free(ReleaseData.ue_time_zone);

    return request;
}

ogs_sbi_request_t *smf_nsmf_pdusession_build_status(
        smf_sess_t *sess, void *data)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    OpenAPI_status_notification_t StatusNotification;
    OpenAPI_status_info_t StatusInfo;

    ogs_assert(sess);
    ogs_assert(sess->vsmf_pdu_session_uri);

    memset(&StatusInfo, 0, sizeof(StatusInfo));
    StatusInfo.resource_status = OpenAPI_resource_status_RELEASED;

    memset(&StatusNotification, 0, sizeof(StatusNotification));
    StatusNotification.status_info = &StatusInfo;

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_POST;
    message.h.uri = sess->vsmf_pdu_session_uri;

    message.StatusNotification = &StatusNotification;

    request = ogs_sbi_build_request(&message);
    ogs_expect(request);

    return request;
}
