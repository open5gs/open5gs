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

ogs_sbi_request_t *amf_nsmf_pdu_session_build_create_sm_context(
        amf_sess_t *sess, void *data)
{
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

    ogs_assert(sess);
    amf_ue = sess->amf_ue;
    ogs_assert(amf_ue);
    ogs_assert(amf_ue->nas.access_type);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_POST;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NSMF_PDUSESSION;
    message.h.api.version = (char *)OGS_SBI_API_V1;
    message.h.resource.component[0] =
        (char *)OGS_SBI_RESOURCE_NAME_SM_CONTEXTS;

    memset(&SmContextCreateData, 0, sizeof(SmContextCreateData));

    SmContextCreateData.serving_nf_id = ogs_sbi_self()->nf_instance_id;

    SmContextCreateData.serving_network =
        ogs_sbi_build_plmn_id_nid(&amf_ue->tai.plmn_id);

    SmContextCreateData.supi = amf_ue->supi;
    SmContextCreateData.pei = amf_ue->pei;
    if (amf_ue->num_of_msisdn) {
        if (amf_ue->msisdn[0]) {
            SmContextCreateData.gpsi = ogs_msprintf("%s-%s",
                        OGS_ID_GPSI_TYPE_MSISDN, amf_ue->msisdn[0]);
        }
    }
    SmContextCreateData.pdu_session_id = sess->psi;
    SmContextCreateData.dnn = sess->dnn;

    memset(&sNssai, 0, sizeof(sNssai));
    sNssai.sst = sess->s_nssai.sst;
    sNssai.sd = ogs_s_nssai_sd_to_string(sess->s_nssai.sd);
    SmContextCreateData.s_nssai = &sNssai;

    memset(&hplmnSnssai, 0, sizeof(hplmnSnssai));
    if (sess->s_nssai.mapped_hplmn_sst) {
        hplmnSnssai.sst = sess->s_nssai.mapped_hplmn_sst;
        hplmnSnssai.sd = ogs_s_nssai_sd_to_string(
                sess->s_nssai.mapped_hplmn_sd);
        SmContextCreateData.hplmn_snssai = &hplmnSnssai;
    }

    SmContextCreateData.guami = ogs_sbi_build_guami(amf_ue->guami);
    SmContextCreateData.an_type = amf_ue->nas.access_type; 

    memset(&header, 0, sizeof(header));
    header.service.name = (char *)OGS_SBI_SERVICE_NAME_NAMF_CALLBACK;
    header.api.version = (char *)OGS_SBI_API_V1;
    header.resource.component[0] = amf_ue->supi;
    header.resource.component[1] =
        (char *)OGS_SBI_RESOURCE_NAME_SM_CONTEXT_STATUS;
    header.resource.component[2] = ogs_msprintf("%d", sess->psi);

    server = ogs_list_first(&ogs_sbi_self()->server_list);
    ogs_assert(server);
    SmContextCreateData.sm_context_status_uri =
        ogs_sbi_server_uri(server, &header);

    n1SmMsg.content_id = (char *)OGS_SBI_CONTENT_5GNAS_SM_ID;
    SmContextCreateData.n1_sm_msg = &n1SmMsg;

    memset(&ueLocation, 0, sizeof(ueLocation));
    ueLocation.nr_location = ogs_sbi_build_nr_location(
            &amf_ue->tai, &amf_ue->nr_cgi);
    ogs_assert(ueLocation.nr_location);
    ueLocation.nr_location->ue_location_timestamp =
        ogs_sbi_gmtime_string(amf_ue->ue_location_timestamp);

    SmContextCreateData.ue_location = &ueLocation;
    SmContextCreateData.ue_time_zone = ogs_sbi_timezone_string(ogs_timezone());

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

    request = ogs_sbi_build_request(&message);
    ogs_assert(request);

    if (SmContextCreateData.serving_network)
        ogs_sbi_free_plmn_id_nid(SmContextCreateData.serving_network);
    ogs_free(SmContextCreateData.sm_context_status_uri);
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

    return request;
}

ogs_sbi_request_t *amf_nsmf_pdu_session_build_update_sm_context(
        amf_sess_t *sess, void *data)
{
    amf_nsmf_pdu_session_update_sm_context_param_t *param = data;
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
    ogs_assert(sess->sm_context_ref);
    amf_ue = sess->amf_ue;
    ogs_assert(amf_ue);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_POST;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NSMF_PDUSESSION;
    message.h.api.version = (char *)OGS_SBI_API_V1;
    message.h.resource.component[0] =
        (char *)OGS_SBI_RESOURCE_NAME_SM_CONTEXTS;
    message.h.resource.component[1] = sess->sm_context_ref;
    message.h.resource.component[2] = (char *)OGS_SBI_RESOURCE_NAME_MODIFY;

    memset(&SmContextUpdateData, 0, sizeof(SmContextUpdateData));

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

        message.SmContextUpdateData = &SmContextUpdateData;
    }

    if (param->n2smbuf) {
        ogs_assert(param->n2SmInfoType);
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

        message.SmContextUpdateData = &SmContextUpdateData;
    }

    if (param->upCnxState) {
        SmContextUpdateData.up_cnx_state = param->upCnxState;

        if (param->ngApCause.group) {
            SmContextUpdateData.ng_ap_cause = &ngApCause;
            memset(&ngApCause, 0, sizeof(ngApCause));
            ngApCause.group = param->ngApCause.group;
            ngApCause.value = param->ngApCause.value;
        }

        message.SmContextUpdateData = &SmContextUpdateData;
    }

    memset(&ueLocation, 0, sizeof(ueLocation));
    if (param->ue_location) {
        ueLocation.nr_location = ogs_sbi_build_nr_location(
                &amf_ue->tai, &amf_ue->nr_cgi);
        ogs_assert(ueLocation.nr_location);
        ueLocation.nr_location->ue_location_timestamp =
            ogs_sbi_gmtime_string(amf_ue->ue_location_timestamp);

        SmContextUpdateData.ue_location = &ueLocation;
    }
    if (param->ue_timezone) {
        SmContextUpdateData.ue_time_zone =
            ogs_sbi_timezone_string(ogs_timezone());
    }

    request = ogs_sbi_build_request(&message);
    ogs_assert(request);

    if (ueLocation.nr_location) {
        if (ueLocation.nr_location->ue_location_timestamp)
            ogs_free(ueLocation.nr_location->ue_location_timestamp);
        ogs_sbi_free_nr_location(ueLocation.nr_location);
    }
    if (SmContextUpdateData.ue_time_zone)
        ogs_free(SmContextUpdateData.ue_time_zone);

    return request;
}

ogs_sbi_request_t *amf_nsmf_pdu_session_build_release_sm_context(
        amf_sess_t *sess, void *data)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    amf_ue_t *amf_ue = NULL;

    OpenAPI_sm_context_release_data_t SmContextReleaseData;
    OpenAPI_user_location_t ueLocation;

    ogs_assert(sess);
    ogs_assert(sess->sm_context_ref);
    amf_ue = sess->amf_ue;
    ogs_assert(amf_ue);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_POST;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NSMF_PDUSESSION;
    message.h.api.version = (char *)OGS_SBI_API_V1;
    message.h.resource.component[0] =
        (char *)OGS_SBI_RESOURCE_NAME_SM_CONTEXTS;
    message.h.resource.component[1] = sess->sm_context_ref;
    message.h.resource.component[2] = (char *)OGS_SBI_RESOURCE_NAME_RELEASE;

    memset(&SmContextReleaseData, 0, sizeof(SmContextReleaseData));

    memset(&ueLocation, 0, sizeof(ueLocation));
    ueLocation.nr_location = ogs_sbi_build_nr_location(
            &amf_ue->tai, &amf_ue->nr_cgi);
    ogs_assert(ueLocation.nr_location);
    ueLocation.nr_location->ue_location_timestamp =
        ogs_sbi_gmtime_string(amf_ue->ue_location_timestamp);

    SmContextReleaseData.ue_location = &ueLocation;
    SmContextReleaseData.ue_time_zone = ogs_sbi_timezone_string(ogs_timezone());

    message.SmContextReleaseData = &SmContextReleaseData;

    request = ogs_sbi_build_request(&message);
    ogs_assert(request);

    if (ueLocation.nr_location) {
        if (ueLocation.nr_location->ue_location_timestamp)
            ogs_free(ueLocation.nr_location->ue_location_timestamp);
        ogs_sbi_free_nr_location(ueLocation.nr_location);
    }
    if (SmContextReleaseData.ue_time_zone)
        ogs_free(SmContextReleaseData.ue_time_zone);

    return request;
}
