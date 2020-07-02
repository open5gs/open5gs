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

#include "namf-build.h"
#include "gsm-build.h"
#include "ngap-build.h"

ogs_sbi_request_t *smf_namf_comm_build_n1_n2_message_transfer(
        smf_sess_t *sess, void *data)
{
    int i;
    smf_ue_t *smf_ue = NULL;

    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    OpenAPI_n1_n2_message_transfer_req_data_t N1N2MessageTransferReqData;

    OpenAPI_n1_message_container_t n1MessageContainer;
    OpenAPI_ref_to_binary_data_t n1MessageContent;

    OpenAPI_n2_info_container_t n2InfoContainer;
    OpenAPI_n2_sm_information_t smInfo;
    OpenAPI_n2_info_content_t n2InfoContent;
    OpenAPI_ref_to_binary_data_t ngapData;

    ogs_assert(sess);
    smf_ue = sess->smf_ue;
    ogs_assert(smf_ue);
    ogs_assert(smf_ue->supi);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_POST;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NAMF_COMM;
    message.h.api.version = (char *)OGS_SBI_API_V1;
    message.h.resource.component[0] = (char *)OGS_SBI_RESOURCE_NAME_UE_CONTEXTS;
    message.h.resource.component[1] = smf_ue->supi;
    message.h.resource.component[2] =
        (char *)OGS_SBI_RESOURCE_NAME_N1_N2_MESSAGES;
    message.N1N2MessageTransferReqData = &N1N2MessageTransferReqData;

    memset(&N1N2MessageTransferReqData, 0, sizeof(N1N2MessageTransferReqData));
    N1N2MessageTransferReqData.pdu_session_id = sess->psi;
    N1N2MessageTransferReqData.n1_message_container = &n1MessageContainer;
    N1N2MessageTransferReqData.n2_info_container = &n2InfoContainer;

    memset(&n1MessageContainer, 0, sizeof(n1MessageContainer));
    n1MessageContainer.n1_message_class = OpenAPI_n1_message_class_SM;
    n1MessageContainer.n1_message_content = &n1MessageContent;

    memset(&n1MessageContent, 0, sizeof(n1MessageContent));
    n1MessageContent.content_id = (char *)OGS_SBI_CONTENT_5GNAS_SM_ID;

    memset(&n2InfoContainer, 0, sizeof(n2InfoContainer));
    n2InfoContainer.n2_information_class = OpenAPI_n2_information_class_SM;
    n2InfoContainer.sm_info = &smInfo;

    memset(&smInfo, 0, sizeof(smInfo));
    smInfo.pdu_session_id = sess->psi;
    smInfo.n2_info_content = &n2InfoContent;

    memset(&n2InfoContent, 0, sizeof(n2InfoContent));
    n2InfoContent.ngap_ie_type = OpenAPI_ngap_ie_type_PDU_RES_SETUP_REQ;
    n2InfoContent.ngap_data = &ngapData;

    memset(&ngapData, 0, sizeof(ngapData));
    ngapData.content_id = (char *)OGS_SBI_CONTENT_NGAP_SM_ID;

    message.num_of_part = 0;

    message.part[message.num_of_part].pkbuf =
        gsm_build_pdu_session_establishment_accept(sess);
    if (message.part[message.num_of_part].pkbuf) {
        message.part[message.num_of_part].content_id =
            (char *)OGS_SBI_CONTENT_5GNAS_SM_ID;
        message.part[message.num_of_part].content_type =
            (char *)OGS_SBI_CONTENT_5GNAS_TYPE;
        message.num_of_part++;
    }

    message.part[message.num_of_part].pkbuf =
        ngap_build_pdu_session_resource_setup_request_transfer(sess);
    if (message.part[message.num_of_part].pkbuf) {
        message.part[message.num_of_part].content_id =
            (char *)OGS_SBI_CONTENT_NGAP_SM_ID;
        message.part[message.num_of_part].content_type =
            (char *)OGS_SBI_CONTENT_NGAP_TYPE;
        message.num_of_part++;
    }

    request = ogs_sbi_build_request(&message);
    ogs_assert(request);

    for (i = 0; i < message.num_of_part; i++)
        if (message.part[i].pkbuf)
            ogs_pkbuf_free(message.part[i].pkbuf);

    return request;
}

ogs_sbi_request_t *smf_namf_callback_build_sm_context_status(
        smf_sess_t *sess, void *data)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    OpenAPI_sm_context_status_notification_t SmContextStatusNotification;
    OpenAPI_status_info_t StatusInfo;

    ogs_assert(sess);
    ogs_assert(sess->sm_context_status_uri);

    memset(&StatusInfo, 0, sizeof(StatusInfo));
    StatusInfo.resource_status = OpenAPI_resource_status_RELEASED;

    memset(&SmContextStatusNotification, 0,
            sizeof(SmContextStatusNotification));
    SmContextStatusNotification.status_info = &StatusInfo;

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_POST;
    message.h.uri = sess->sm_context_status_uri;

    message.SmContextStatusNotification = &SmContextStatusNotification;

    request = ogs_sbi_build_request(&message);
    ogs_assert(request);

    return request;
}
