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

    char buf[OGS_AMFIDSTRLEN];
    amf_ue_t *amf_ue = NULL;

    OpenAPI_sm_context_create_data_t SmContextCreateData;
    OpenAPI_plmn_id_nid_t plmn_id_nid;
    OpenAPI_snssai_t s_nssai;
    OpenAPI_snssai_t hplmn_snssai;
    OpenAPI_ref_to_binary_data_t n1SmMsg;
    OpenAPI_guami_t guami;

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

    plmn_id_nid.mcc = ogs_plmn_id_mcc_string(&amf_ue->tai.plmn_id);
    plmn_id_nid.mnc = ogs_plmn_id_mnc_string(&amf_ue->tai.plmn_id);
    plmn_id_nid.nid = NULL;
    SmContextCreateData.serving_network = &plmn_id_nid;

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

    memset(&s_nssai, 0, sizeof(s_nssai));
    s_nssai.sst = sess->s_nssai.sst;
    s_nssai.sd = ogs_s_nssai_sd_to_string(sess->s_nssai.sd);
    SmContextCreateData.s_nssai = &s_nssai;

    memset(&hplmn_snssai, 0, sizeof(hplmn_snssai));
    if (sess->s_nssai.mapped_hplmn_sst) {
        hplmn_snssai.sst = sess->s_nssai.mapped_hplmn_sst;
        hplmn_snssai.sd = ogs_s_nssai_sd_to_string(
                sess->s_nssai.mapped_hplmn_sd);
        SmContextCreateData.hplmn_snssai = &hplmn_snssai;
    }

    ogs_assert(amf_ue->guami);
    guami.amf_id = ogs_amf_id_to_string(&amf_ue->guami->amf_id, buf);
    guami.plmn_id = (OpenAPI_plmn_id_t *)&plmn_id_nid;
    SmContextCreateData.guami = &guami;

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

    ogs_free(plmn_id_nid.mcc);
    ogs_free(plmn_id_nid.mnc);
    ogs_free(SmContextCreateData.sm_context_status_uri);
    ogs_free(header.resource.component[2]);
    if (s_nssai.sd)
        ogs_free(s_nssai.sd);
    if (hplmn_snssai.sd)
        ogs_free(hplmn_snssai.sd);
    if (SmContextCreateData.gpsi)
        ogs_free(SmContextCreateData.gpsi);

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

    ogs_assert(param);
    ogs_assert(sess);
    ogs_assert(sess->sm_context_ref);

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

    request = ogs_sbi_build_request(&message);
    ogs_assert(request);

    return request;
}

ogs_sbi_request_t *amf_nsmf_pdu_session_build_release_sm_context(
        amf_sess_t *sess, void *data)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    ogs_assert(sess);
    ogs_assert(sess->sm_context_ref);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_POST;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NSMF_PDUSESSION;
    message.h.api.version = (char *)OGS_SBI_API_V1;
    message.h.resource.component[0] =
        (char *)OGS_SBI_RESOURCE_NAME_SM_CONTEXTS;
    message.h.resource.component[1] = sess->sm_context_ref;
    message.h.resource.component[2] = (char *)OGS_SBI_RESOURCE_NAME_RELEASE;

    request = ogs_sbi_build_request(&message);
    ogs_assert(request);

    return request;
}
