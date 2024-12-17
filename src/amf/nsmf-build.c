/*
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
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

ogs_sbi_request_t *amf_nsmf_pdusession_build_create_sm_context(
        amf_sess_t *sess, void *data)
{
    amf_nsmf_pdusession_sm_context_param_t *param = data;
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    ogs_sbi_server_t *server = NULL;
    ogs_sbi_header_t header;

    amf_ue_t *amf_ue = NULL;

    OpenAPI_sm_context_create_data_t SmContextCreateData;
    OpenAPI_snssai_t sNssai;
    OpenAPI_snssai_t hplmnSnssai;
    OpenAPI_ref_to_binary_data_t n1SmMsg;
    OpenAPI_user_location_t ueLocation;
    ogs_sbi_nf_instance_t *pcf_nf_instance = NULL;

    ogs_assert(sess);
    amf_ue = amf_ue_find_by_id(sess->amf_ue_id);
    ogs_assert(amf_ue);
    ogs_assert(amf_ue->nas.access_type);
    ogs_assert(ran_ue_find_by_id(amf_ue->ran_ue_id));

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_POST;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NSMF_PDUSESSION;
    message.h.api.version = (char *)OGS_SBI_API_V1;
    message.h.resource.component[0] =
        (char *)OGS_SBI_RESOURCE_NAME_SM_CONTEXTS;

    memset(&SmContextCreateData, 0, sizeof(SmContextCreateData));
    memset(&sNssai, 0, sizeof(sNssai));
    memset(&hplmnSnssai, 0, sizeof(hplmnSnssai));
    memset(&header, 0, sizeof(header));
    memset(&ueLocation, 0, sizeof(ueLocation));

    SmContextCreateData.serving_nf_id =
        NF_INSTANCE_ID(ogs_sbi_self()->nf_instance);
    if (!SmContextCreateData.serving_nf_id) {
        ogs_error("No serving_nf_id");
        goto end;
    }

    SmContextCreateData.serving_network =
        ogs_sbi_build_plmn_id_nid(&amf_ue->nr_tai.plmn_id);
    if (!SmContextCreateData.serving_nf_id) {
        ogs_error("No serving_network");
        goto end;
    }

    SmContextCreateData.supi = amf_ue->supi;
    SmContextCreateData.pei = amf_ue->pei;
    if (amf_ue->num_of_msisdn) {
        if (amf_ue->msisdn[0]) {
            SmContextCreateData.gpsi = ogs_msprintf("%s-%s",
                        OGS_ID_GPSI_TYPE_MSISDN, amf_ue->msisdn[0]);
            if (!SmContextCreateData.gpsi) {
                ogs_error("No gpsi");
                goto end;
            }
        }
    }
    SmContextCreateData.is_pdu_session_id = true;
    SmContextCreateData.pdu_session_id = sess->psi;
    if (!SmContextCreateData.pdu_session_id) {
        ogs_error("No pdu_session_id");
        goto end;
    }

    /*
     * TS29.502
     * 6.1 Nsmf_PDUSession Service API
     * Table 6.1.6.2.2-1: Definition of type SmContextCreateData
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
    if (ogs_sbi_plmn_id_in_vplmn(&amf_ue->home_plmn_id) == true) {
        char *home_network_domain = NULL;

        home_network_domain =
            ogs_home_network_domain_from_plmn_id(&amf_ue->home_plmn_id);
        ogs_assert(home_network_domain);

        SmContextCreateData.dnn =
            ogs_msprintf("%s.%s", sess->dnn, home_network_domain);
        ogs_assert(SmContextCreateData.dnn);

        ogs_free(home_network_domain);

    } else {

        SmContextCreateData.dnn = ogs_strdup(sess->dnn);
        ogs_assert(SmContextCreateData.dnn);

    }

    sNssai.sst = sess->s_nssai.sst;
    sNssai.sd = ogs_s_nssai_sd_to_string(sess->s_nssai.sd);
    SmContextCreateData.s_nssai = &sNssai;

    if (sess->mapped_hplmn_presence) {
        hplmnSnssai.sst = sess->mapped_hplmn.sst;
        hplmnSnssai.sd = ogs_s_nssai_sd_to_string(sess->mapped_hplmn.sd);
        SmContextCreateData.hplmn_snssai = &hplmnSnssai;
    }

    SmContextCreateData.guami = ogs_sbi_build_guami(amf_ue->guami);
    if (!SmContextCreateData.guami) {
        ogs_error("No guami");
        goto end;
    }
    SmContextCreateData.an_type = amf_ue->nas.access_type; 

    header.service.name = (char *)OGS_SBI_SERVICE_NAME_NAMF_CALLBACK;
    header.api.version = (char *)OGS_SBI_API_V1;
    header.resource.component[0] = amf_ue->supi;
    header.resource.component[1] =
        (char *)OGS_SBI_RESOURCE_NAME_SM_CONTEXT_STATUS;
    header.resource.component[2] = ogs_msprintf("%d", sess->psi);
    if (!header.resource.component[2]) {
        ogs_error("No header.resource.component[2]");
        goto end;
    }

    server = ogs_sbi_server_first();
    if (!server) {
        ogs_error("No server");
        goto end;
    }
    SmContextCreateData.sm_context_status_uri =
        ogs_sbi_server_uri(server, &header);

    n1SmMsg.content_id = (char *)OGS_SBI_CONTENT_5GNAS_SM_ID;
    SmContextCreateData.n1_sm_msg = &n1SmMsg;

    SmContextCreateData.rat_type = amf_ue_rat_type(amf_ue);

    ueLocation.nr_location = ogs_sbi_build_nr_location(
            &amf_ue->nr_tai, &amf_ue->nr_cgi);
    if (!ueLocation.nr_location) {
        ogs_error("No ueLocation.nr_location");
        goto end;
    }
    ueLocation.nr_location->ue_location_timestamp =
        ogs_sbi_gmtime_string(amf_ue->ue_location_timestamp);
    if (!ueLocation.nr_location->ue_location_timestamp) {
        ogs_error("No ue_location_timestamp");
        goto end;
    }

    SmContextCreateData.ue_location = &ueLocation;
    SmContextCreateData.ue_time_zone = ogs_sbi_timezone_string(ogs_timezone());
    if (!SmContextCreateData.ue_time_zone) {
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
    pcf_nf_instance = OGS_SBI_GET_NF_INSTANCE(
            amf_ue->sbi.service_type_array[
            OGS_SBI_SERVICE_TYPE_NPCF_AM_POLICY_CONTROL]);
    if (pcf_nf_instance)
        SmContextCreateData.pcf_id = pcf_nf_instance->id;
    else
        ogs_error("No pcf_nf_instance");

    message.SmContextCreateData = &SmContextCreateData;

    message.part[message.num_of_part].pkbuf = sess->payload_container;
    if (message.part[message.num_of_part].pkbuf) {
        message.part[message.num_of_part].content_id =
            (char *)OGS_SBI_CONTENT_5GNAS_SM_ID;
        message.part[message.num_of_part].content_type =
            (char *)OGS_SBI_CONTENT_5GNAS_TYPE;
        message.num_of_part++;
    }

    message.http.accept = (char *)(OGS_SBI_CONTENT_JSON_TYPE ","
        OGS_SBI_CONTENT_NGAP_TYPE "," OGS_SBI_CONTENT_PROBLEM_TYPE);

    message.http.custom.callback =
        (char *)OGS_SBI_CALLBACK_NSMF_PDUSESSION_STATUS_NOTIFY;

    if (param && param->nrf_uri.nrf.id) {
        message.http.custom.nrf_uri =
            ogs_msprintf("%s: \"%s\"",
                    OGS_SBI_SERVICE_NAME_NNRF_DISC, param->nrf_uri.nrf.id);
    }

    request = ogs_sbi_build_request(&message);
    ogs_expect(request);

end:
    if (SmContextCreateData.serving_network)
        ogs_sbi_free_plmn_id_nid(SmContextCreateData.serving_network);
    if (SmContextCreateData.dnn)
        ogs_free(SmContextCreateData.dnn);
    if (SmContextCreateData.sm_context_status_uri)
        ogs_free(SmContextCreateData.sm_context_status_uri);
    if (header.resource.component[2])
        ogs_free(header.resource.component[2]);
    if (sNssai.sd)
        ogs_free(sNssai.sd);
    if (hplmnSnssai.sd)
        ogs_free(hplmnSnssai.sd);
    if (SmContextCreateData.guami)
        ogs_sbi_free_guami(SmContextCreateData.guami);
    if (SmContextCreateData.gpsi)
        ogs_free(SmContextCreateData.gpsi);
    if (ueLocation.nr_location) {
        if (ueLocation.nr_location->ue_location_timestamp)
            ogs_free(ueLocation.nr_location->ue_location_timestamp);
        ogs_sbi_free_nr_location(ueLocation.nr_location);
    }
    if (SmContextCreateData.ue_time_zone)
        ogs_free(SmContextCreateData.ue_time_zone);

    if (message.http.custom.nrf_uri)
        ogs_free(message.http.custom.nrf_uri);

    return request;
}

ogs_sbi_request_t *amf_nsmf_pdusession_build_update_sm_context(
        amf_sess_t *sess, void *data)
{
    amf_nsmf_pdusession_sm_context_param_t *param = data;
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    OpenAPI_sm_context_update_data_t SmContextUpdateData;
    OpenAPI_ref_to_binary_data_t n1SmMsg;
    OpenAPI_ref_to_binary_data_t n2SmInfo;
    OpenAPI_ng_ap_cause_t ngApCause;
    OpenAPI_user_location_t ueLocation;

    amf_ue_t *amf_ue = NULL;

    ogs_assert(param);
    ogs_assert(sess);
    ogs_assert(sess->sm_context.resource_uri);
    amf_ue = amf_ue_find_by_id(sess->amf_ue_id);
    ogs_assert(amf_ue);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_POST;
    message.h.uri = ogs_msprintf("%s/%s",
            sess->sm_context.resource_uri, OGS_SBI_RESOURCE_NAME_MODIFY);
    ogs_assert(message.h.uri);

    memset(&ueLocation, 0, sizeof(ueLocation));
    memset(&SmContextUpdateData, 0, sizeof(SmContextUpdateData));

    message.SmContextUpdateData = &SmContextUpdateData;

    message.num_of_part = 0;

    if (param->n1smbuf) {
        n1SmMsg.content_id = (char *)OGS_SBI_CONTENT_5GNAS_SM_ID;
        SmContextUpdateData.n1_sm_msg = &n1SmMsg;

        message.part[message.num_of_part].pkbuf = param->n1smbuf;
        message.part[message.num_of_part].content_id =
            (char *)OGS_SBI_CONTENT_5GNAS_SM_ID;
        message.part[message.num_of_part].content_type =
            (char *)OGS_SBI_CONTENT_5GNAS_TYPE;
        message.num_of_part++;
    }

    if (param->n2smbuf) {
        if (!param->n2SmInfoType) {
            ogs_error("No n2SmInfoType");
            goto end;
        }
        SmContextUpdateData.n2_sm_info_type = param->n2SmInfoType;
        SmContextUpdateData.n2_sm_info = &n2SmInfo;

        memset(&n2SmInfo, 0, sizeof(n2SmInfo));
        n2SmInfo.content_id = (char *)OGS_SBI_CONTENT_NGAP_SM_ID;

        message.part[message.num_of_part].pkbuf = param->n2smbuf;
        message.part[message.num_of_part].content_id =
            (char *)OGS_SBI_CONTENT_NGAP_SM_ID;
        message.part[message.num_of_part].content_type =
            (char *)OGS_SBI_CONTENT_NGAP_TYPE;
        message.num_of_part++;
    }

    SmContextUpdateData.up_cnx_state = param->upCnxState;

    SmContextUpdateData.ho_state = param->hoState;

    if (param->TargetID) {
        SmContextUpdateData.target_id =
            amf_nsmf_pdusession_build_target_id(param->TargetID);
        if (!SmContextUpdateData.target_id) {
            ogs_error("No target_id");
            goto end;
        }
    }

    if (param->ngApCause.group) {
        SmContextUpdateData.ng_ap_cause = &ngApCause;
        memset(&ngApCause, 0, sizeof(ngApCause));
        ngApCause.group = param->ngApCause.group;
        ngApCause.value = param->ngApCause.value;
    }

    if (param->ue_location) {
        ueLocation.nr_location = ogs_sbi_build_nr_location(
                &amf_ue->nr_tai, &amf_ue->nr_cgi);
        if (!ueLocation.nr_location) {
            ogs_error("No ueLocation.nr_location");
            goto end;
        }
        ueLocation.nr_location->ue_location_timestamp =
            ogs_sbi_gmtime_string(amf_ue->ue_location_timestamp);
        if (!ueLocation.nr_location->ue_location_timestamp) {
            ogs_error("No ueLocation.nr_location->ue_location_timestamp");
            goto end;
        }

        SmContextUpdateData.ue_location = &ueLocation;
    }
    if (param->ue_timezone) {
        SmContextUpdateData.ue_time_zone =
            ogs_sbi_timezone_string(ogs_timezone());
        if (!SmContextUpdateData.ue_time_zone) {
            ogs_error("No SmContextUpdateData.ue_time_zone");
            goto end;
        }
    }

    if (param->release) {
        SmContextUpdateData.is_release = true;
        SmContextUpdateData.release = param->release;
    }
    SmContextUpdateData.cause = param->cause;

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
    if (SmContextUpdateData.ue_time_zone)
        ogs_free(SmContextUpdateData.ue_time_zone);
    if (SmContextUpdateData.target_id)
        amf_nsmf_pdusession_free_target_id(SmContextUpdateData.target_id);

    return request;
}

ogs_sbi_request_t *amf_nsmf_pdusession_build_release_sm_context(
        amf_sess_t *sess, void *data)
{
    amf_nsmf_pdusession_sm_context_param_t *param = data;

    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    amf_ue_t *amf_ue = NULL;

    OpenAPI_sm_context_release_data_t SmContextReleaseData;
    OpenAPI_ng_ap_cause_t ngApCause;
    OpenAPI_user_location_t ueLocation;

    ogs_assert(sess);
    ogs_assert(sess->sm_context.resource_uri);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_POST;
    message.h.uri = ogs_msprintf("%s/%s",
            sess->sm_context.resource_uri, OGS_SBI_RESOURCE_NAME_RELEASE);
    ogs_assert(message.h.uri);

    memset(&SmContextReleaseData, 0, sizeof(SmContextReleaseData));

    if (param) {
        SmContextReleaseData.cause = param->cause;

        if (param->ngApCause.group) {
            SmContextReleaseData.ng_ap_cause = &ngApCause;
            memset(&ngApCause, 0, sizeof(ngApCause));
            ngApCause.group = param->ngApCause.group;
            ngApCause.value = param->ngApCause.value;
        }

        if (param->gmm_cause) {
            SmContextReleaseData._5g_mm_cause_value = param->gmm_cause;
            SmContextReleaseData.is__5g_mm_cause_value = true;
        }
    }

    memset(&ueLocation, 0, sizeof(ueLocation));

    amf_ue = amf_ue_find_by_id(sess->amf_ue_id);
    if (!amf_ue) {
        ogs_error("UE(amf_ue) Context has already been removed");
        goto end;
    }

    ueLocation.nr_location = ogs_sbi_build_nr_location(
            &amf_ue->nr_tai, &amf_ue->nr_cgi);
    if (!ueLocation.nr_location) {
        ogs_error("No ueLocation.nr_location");
        goto end;
    }
    ueLocation.nr_location->ue_location_timestamp =
        ogs_sbi_gmtime_string(amf_ue->ue_location_timestamp);
    if (!ueLocation.nr_location->ue_location_timestamp) {
        ogs_error("No ueLocation.nr_location->ue_location_timestamp");
        goto end;
    }

    SmContextReleaseData.ue_location = &ueLocation;
    SmContextReleaseData.ue_time_zone = ogs_sbi_timezone_string(ogs_timezone());
    if (!SmContextReleaseData.ue_time_zone) {
        ogs_error("No SmContextReleaseData.ue_time_zone");
        goto end;
    }

    message.SmContextReleaseData = &SmContextReleaseData;

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
    if (SmContextReleaseData.ue_time_zone)
        ogs_free(SmContextReleaseData.ue_time_zone);

    return request;
}

ogs_sbi_request_t *amf_nsmf_callback_build_n1_n2_failure_notify(
        amf_sess_t *sess, OpenAPI_n1_n2_message_transfer_cause_e cause)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    OpenAPI_n1_n2_msg_txfr_failure_notification_t
        N1N2MsgTxfrFailureNotification;

    ogs_assert(sess);
    ogs_assert(sess->paging.ongoing == true);
    ogs_assert(sess->paging.location);
    ogs_assert(sess->paging.n1n2_failure_txf_notif_uri);
    ogs_assert(cause);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_POST;
    message.h.uri = sess->paging.n1n2_failure_txf_notif_uri;

    memset(&N1N2MsgTxfrFailureNotification,
            0, sizeof(N1N2MsgTxfrFailureNotification));
    N1N2MsgTxfrFailureNotification.cause = cause;
    if (sess->paging.location) {
        N1N2MsgTxfrFailureNotification.n1n2_msg_data_uri =
            sess->paging.location;
    } else {
        /* TS29.518 6.1.6.2.30 Type: N1N2MsgTxfrFailureNotification
         *
         * If no Location header was returned when the N1/N2
         * message transfer was initiated, e.g. when a 200 OK
         * response was sent for a UE in RRC inactive state,
         * this IE shall be set to a dummy URI, i.e. an URI with
         * no authority and an empty path (e.g. "http:").
         */
        N1N2MsgTxfrFailureNotification.n1n2_msg_data_uri = (char *)"http:";
    }

    message.N1N2MsgTxfrFailureNotification = &N1N2MsgTxfrFailureNotification;

    request = ogs_sbi_build_request(&message);
    ogs_expect(request);

    return request;
}

OpenAPI_ng_ran_target_id_t *amf_nsmf_pdusession_build_target_id(
        NGAP_TargetID_t *TargetID)
{
    ogs_plmn_id_t plmn_id;
    ogs_5gs_tai_t nr_tai;

    NGAP_TargetRANNodeID_t *targetRANNodeID = NULL;
    NGAP_GlobalRANNodeID_t *globalRANNodeID = NULL;
    NGAP_GlobalGNB_ID_t *globalGNB_ID = NULL;

    OpenAPI_ng_ran_target_id_t *targetId = NULL;
    OpenAPI_global_ran_node_id_t *ranNodeId = NULL;
    OpenAPI_gnb_id_t *gNbId = NULL;
    OpenAPI_tai_t *tai = NULL;

    ogs_assert(TargetID);

    if (TargetID->present != NGAP_TargetID_PR_targetRANNodeID) {
        ogs_error("Not implemented TargetID[%d]", TargetID->present);
        return NULL;
    }
    targetRANNodeID = TargetID->choice.targetRANNodeID;
    if (!targetRANNodeID) {
        ogs_error("No targetRANNodeID");
        return NULL;
    }

    globalRANNodeID = &targetRANNodeID->globalRANNodeID;
    if (globalRANNodeID->present != NGAP_GlobalRANNodeID_PR_globalGNB_ID) {
        ogs_error("Not implemented globalRANNodeID[%d]",
                globalRANNodeID->present);
        return NULL;
    }

    globalGNB_ID = globalRANNodeID->choice.globalGNB_ID;
    if (!globalGNB_ID) {
        ogs_error("No globalGNB_ID");
        return NULL;
    }

    targetId = ogs_calloc(1, sizeof(*targetId));
    if (!targetId) {
        ogs_error("No targetId");
        return NULL;
    }

    targetId->ran_node_id = ranNodeId = ogs_calloc(1, sizeof(*ranNodeId));;
    if (!targetId->ran_node_id) {
        ogs_error("No targetId->ran_node_id");
        amf_nsmf_pdusession_free_target_id(targetId);
        return NULL;
    }

    memcpy(&plmn_id, globalGNB_ID->pLMNIdentity.buf, OGS_PLMN_ID_LEN);
    ranNodeId->plmn_id = ogs_sbi_build_plmn_id(&plmn_id);
    if (!ranNodeId->plmn_id) {
        ogs_error("No ranNodeId->plmn_id");
        amf_nsmf_pdusession_free_target_id(targetId);
        return NULL;
    }

    ranNodeId->g_nb_id = gNbId = ogs_calloc(1, sizeof(*gNbId));
    if (!ranNodeId->g_nb_id) {
        ogs_error("No ranNodeId->g_nb_id");
        amf_nsmf_pdusession_free_target_id(targetId);
        return NULL;
    }

    gNbId->g_nb_value = ogs_calloc(
            1, OGS_KEYSTRLEN(globalGNB_ID->gNB_ID.choice.gNB_ID.size));
    if (!gNbId->g_nb_value) {
        ogs_error("No gNbId->g_nb_value");
        amf_nsmf_pdusession_free_target_id(targetId);
        return NULL;
    }
    ogs_hex_to_ascii(
            globalGNB_ID->gNB_ID.choice.gNB_ID.buf,
            globalGNB_ID->gNB_ID.choice.gNB_ID.size,
            gNbId->g_nb_value,
            OGS_KEYSTRLEN(globalGNB_ID->gNB_ID.choice.gNB_ID.size));
    gNbId->bit_length = 32 - globalGNB_ID->gNB_ID.choice.gNB_ID.bits_unused;

    targetId->tai = tai = ogs_calloc(1, sizeof(*tai));;
    if (!targetId->tai) {
        ogs_error("No targetId->tai");
        amf_nsmf_pdusession_free_target_id(targetId);
        return NULL;
    }

    ogs_ngap_ASN_to_5gs_tai(&targetRANNodeID->selectedTAI, &nr_tai);
    tai->plmn_id = ogs_sbi_build_plmn_id(&nr_tai.plmn_id);
    if (!tai->plmn_id) {
        ogs_error("No tai->plmn_id");
        amf_nsmf_pdusession_free_target_id(targetId);
        return NULL;
    }
    tai->tac = ogs_uint24_to_0string(nr_tai.tac);
    if (!tai->tac) {
        ogs_error("No tai->tac");
        amf_nsmf_pdusession_free_target_id(targetId);
        return NULL;
    }

    return targetId;
}

void amf_nsmf_pdusession_free_target_id(OpenAPI_ng_ran_target_id_t *targetId)
{
    ogs_assert(targetId);

    if (targetId->ran_node_id) {
        OpenAPI_global_ran_node_id_t *ranNodeId = targetId->ran_node_id;
        OpenAPI_tai_t *tai = targetId->tai;

        if (ranNodeId) {
            if (ranNodeId->plmn_id)
                OpenAPI_plmn_id_free(ranNodeId->plmn_id);
            if (ranNodeId->g_nb_id) {
                OpenAPI_gnb_id_t *gNbId = ranNodeId->g_nb_id;
                if (gNbId->g_nb_value)
                    ogs_free(gNbId->g_nb_value);
                ogs_free(gNbId);
            }
            ogs_free(ranNodeId);
        }

        if (tai) {
            if (tai->plmn_id)
                OpenAPI_plmn_id_free(tai->plmn_id);
            if (tai->tac)
                ogs_free(tai->tac);
            ogs_free(tai);
        }
    }
    ogs_free(targetId);
}
