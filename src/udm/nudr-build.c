/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
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

#include "nudr-build.h"

ogs_sbi_request_t *udm_nudr_dr_build_authentication_subscription(
        udm_ue_t *udm_ue, void *data)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    OpenAPI_list_t *PatchItemList = NULL;
    OpenAPI_patch_item_t item, *pitem = NULL;
    OpenAPI_lnode_t *node = NULL;
    uint8_t *sqn = data;
    char sqn_string[OGS_KEYSTRLEN(OGS_SQN_LEN)];

    ogs_assert(udm_ue);

    memset(&message, 0, sizeof(message));
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NUDR_DR;
    message.h.api.version = (char *)OGS_SBI_API_V1;
    message.h.resource.component[0] =
        (char *)OGS_SBI_RESOURCE_NAME_SUBSCRIPTION_DATA;
    message.h.resource.component[1] = udm_ue->supi;
    message.h.resource.component[2] =
        (char *)OGS_SBI_RESOURCE_NAME_AUTHENTICATION_DATA;
    message.h.resource.component[3] =
        (char *)OGS_SBI_RESOURCE_NAME_AUTHENTICATION_SUBSCRIPTION;

    if (!sqn) {
        message.h.method = (char *)OGS_SBI_HTTP_METHOD_GET;

    } else {
        message.h.method = (char *)OGS_SBI_HTTP_METHOD_PATCH;
        message.http.content_type = (char *)OGS_SBI_CONTENT_PATCH_TYPE;

        PatchItemList = OpenAPI_list_create();
        ogs_assert(PatchItemList);

        ogs_hex_to_ascii(sqn, OGS_SQN_LEN, sqn_string, sizeof(sqn_string));

        memset(&item, 0, sizeof(item));
        item.op = OpenAPI_patch_operation_replace;
        item.path = (char *)"/sequenceNumber/sqn";
        item.value = OpenAPI_any_type_create_string(sqn_string);
        ogs_assert(item.value);

        OpenAPI_list_add(PatchItemList, &item);

        message.PatchItemList = PatchItemList;
    }

    request = ogs_sbi_build_request(&message);
    ogs_assert(request);

    if (PatchItemList) {
        OpenAPI_list_for_each(PatchItemList, node) {
            pitem = node->data;
            if (pitem)
                OpenAPI_any_type_free(pitem->value);
        }
        OpenAPI_list_free(PatchItemList);
    }

    return request;
}

ogs_sbi_request_t *udm_nudr_dr_build_update_authentication_status(
        udm_ue_t *udm_ue, void *data)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    ogs_assert(udm_ue);
    ogs_assert(udm_ue->auth_event);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_PUT;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NUDR_DR;
    message.h.api.version = (char *)OGS_SBI_API_V1;
    message.h.resource.component[0] =
        (char *)OGS_SBI_RESOURCE_NAME_SUBSCRIPTION_DATA;
    message.h.resource.component[1] = udm_ue->supi;
    message.h.resource.component[2] =
        (char *)OGS_SBI_RESOURCE_NAME_AUTHENTICATION_DATA;
    message.h.resource.component[3] =
        (char *)OGS_SBI_RESOURCE_NAME_AUTHENTICATION_STATUS;

    message.AuthEvent = OpenAPI_auth_event_copy(
            message.AuthEvent, udm_ue->auth_event);

    request = ogs_sbi_build_request(&message);
    ogs_assert(request);

    OpenAPI_auth_event_free(message.AuthEvent);

    return request;
}

ogs_sbi_request_t *udm_nudr_dr_build_update_amf_context(
        udm_ue_t *udm_ue, void *data)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    ogs_assert(udm_ue);
    ogs_assert(udm_ue->amf_3gpp_access_registration);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_PUT;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NUDR_DR;
    message.h.api.version = (char *)OGS_SBI_API_V1;
    message.h.resource.component[0] =
        (char *)OGS_SBI_RESOURCE_NAME_SUBSCRIPTION_DATA;
    message.h.resource.component[1] = udm_ue->supi;
    message.h.resource.component[2] =
        (char *)OGS_SBI_RESOURCE_NAME_CONTEXT_DATA;
    message.h.resource.component[3] =
        (char *)OGS_SBI_RESOURCE_NAME_AMF_3GPP_ACCESS;

    message.Amf3GppAccessRegistration =
        OpenAPI_amf3_gpp_access_registration_copy(
            message.Amf3GppAccessRegistration,
                udm_ue->amf_3gpp_access_registration);

    request = ogs_sbi_build_request(&message);
    ogs_assert(request);

    OpenAPI_amf3_gpp_access_registration_free(
            message.Amf3GppAccessRegistration);

    return request;
}

ogs_sbi_request_t *udm_nudr_dr_build_patch_amf_context(
        udm_ue_t *udm_ue, void *data)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    OpenAPI_patch_item_t *pitem = NULL;
    OpenAPI_lnode_t *node = NULL;

    OpenAPI_list_t *PatchItemList = (OpenAPI_list_t *)data;

    ogs_assert(udm_ue);
    ogs_assert(PatchItemList);

    memset(&message, 0, sizeof(message));
    message.http.content_type = (char *)OGS_SBI_CONTENT_PATCH_TYPE;
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_PATCH;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NUDR_DR;
    message.h.api.version = (char *)OGS_SBI_API_V1;
    message.h.resource.component[0] =
        (char *)OGS_SBI_RESOURCE_NAME_SUBSCRIPTION_DATA;
    message.h.resource.component[1] = (char *)udm_ue->supi;
    message.h.resource.component[2] =
        (char *)OGS_SBI_RESOURCE_NAME_CONTEXT_DATA;
    message.h.resource.component[3] =
        (char *)OGS_SBI_RESOURCE_NAME_AMF_3GPP_ACCESS;

    message.PatchItemList = PatchItemList;

    request = ogs_sbi_build_request(&message);
    ogs_assert(request);

    if (PatchItemList) {
        OpenAPI_list_for_each(PatchItemList, node) {
            pitem = node->data;
            if (pitem)
                OpenAPI_any_type_free(pitem->value);
        }
        OpenAPI_list_free(PatchItemList);
    }

    return request;
}

ogs_sbi_request_t *udm_nudr_dr_build_query_subscription_provisioned(
        udm_ue_t *udm_ue, void *data)
{
    char buf[OGS_PLMNIDSTRLEN];

    ogs_sbi_message_t *recvmsg = NULL;
    ogs_sbi_message_t sendmsg;
    ogs_sbi_request_t *request = NULL;

    ogs_assert(udm_ue);
    recvmsg = data;
    ogs_assert(recvmsg);

    memset(&sendmsg, 0, sizeof(sendmsg));
    sendmsg.h.method = (char *)OGS_SBI_HTTP_METHOD_GET;
    sendmsg.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NUDR_DR;
    sendmsg.h.api.version = (char *)OGS_SBI_API_V1;
    sendmsg.h.resource.component[0] =
        (char *)OGS_SBI_RESOURCE_NAME_SUBSCRIPTION_DATA;
    sendmsg.h.resource.component[1] = udm_ue->supi;
    sendmsg.h.resource.component[2] =
        (char *)ogs_plmn_id_to_string(&udm_ue->guami.plmn_id, buf);
    sendmsg.h.resource.component[3] =
        (char *)OGS_SBI_RESOURCE_NAME_PROVISIONED_DATA;
    sendmsg.h.resource.component[4] = recvmsg->h.resource.component[1];

    SWITCH(recvmsg->h.resource.component[1])
    CASE(OGS_SBI_RESOURCE_NAME_SM_DATA)
        if (recvmsg->param.dnn)
            sendmsg.param.dnn = recvmsg->param.dnn;
        if (recvmsg->param.single_nssai_presence) {
            sendmsg.param.single_nssai_presence = true;
            memcpy(&sendmsg.param.s_nssai, &recvmsg->param.s_nssai,
                    sizeof(sendmsg.param.s_nssai));
        }
        break;

    CASE(OGS_SBI_RESOURCE_NAME_SMF_SELECT_DATA)
        sendmsg.h.resource.component[4] =
            (char *)OGS_SBI_RESOURCE_NAME_SMF_SELECTION_SUBSCRIPTION_DATA;
        break;

    DEFAULT
    END

    request = ogs_sbi_build_request(&sendmsg);
    ogs_assert(request);

    return request;
}
