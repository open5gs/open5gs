/*
 * Copyright (C) 2024 by Sukchan Lee <acetcom@gmail.com>
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

#include "nudm-handler.h"

#include "sbi-path.h"

int smsf_nudm_sdm_handle_provisioned_data(
    smsf_ue_t *smsf_ue, ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    OpenAPI_sms_management_subscription_data_t *SmsManagementSubscriptionData =
            NULL;

    ogs_assert(smsf_ue);
    ogs_assert(stream);
    ogs_assert(recvmsg);

    SmsManagementSubscriptionData = recvmsg->SmsManagementSubscriptionData;

    if (!SmsManagementSubscriptionData) {
        ogs_error("[%s] No SmsManagementSubscriptionData", smsf_ue->supi);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No SmsManagementSubscriptionData", smsf_ue->supi));
        return false;
    }

    if (!SmsManagementSubscriptionData->is_mo_sms_subscribed) {
        ogs_error("[%s] No moSmsSubscribed", smsf_ue->supi);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No moSmsSubscribed", smsf_ue->supi));
        return false;
    }

    if (!SmsManagementSubscriptionData->is_mt_sms_subscribed) {
        ogs_error("[%s] No mtSmsSubscribed", smsf_ue->supi);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No mtSmsSubscribed", smsf_ue->supi));
        return false;
    }

    smsf_ue->mo_sms_subscribed =
            SmsManagementSubscriptionData->mo_sms_subscribed;
    smsf_ue->mt_sms_subscribed =
            SmsManagementSubscriptionData->mt_sms_subscribed;

    return OGS_OK;
}

int smsf_nudm_sdm_handle_subscription(
    smsf_ue_t *smsf_ue, ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    int rv;
    ogs_sbi_message_t message;
    ogs_sbi_message_t sendmsg;
    ogs_sbi_header_t header;
    ogs_sbi_response_t *response = NULL;
    ogs_sbi_server_t *server = NULL;

    ogs_assert(smsf_ue);
    ogs_assert(stream);
    ogs_assert(recvmsg);

    if (!recvmsg->http.location) {
        ogs_error("[%s] No http.location", smsf_ue->supi);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No http.location", smsf_ue->supi));
        return false;
    }

    memset(&header, 0, sizeof(header));
    header.uri = recvmsg->http.location;

    rv = ogs_sbi_parse_header(&message, &header);
    if (rv != OGS_OK) {
        ogs_error("[%s] Cannot parse http.location [%s]",
            smsf_ue->supi, recvmsg->http.location);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "Cannot parse http.location", smsf_ue->supi));
        return OGS_ERROR;
    }

    if (!message.h.resource.component[2]) {
        ogs_error("[%s] No Subscription ID [%s]",
            smsf_ue->supi, recvmsg->http.location);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No Subscription ID", smsf_ue->supi));
        return OGS_ERROR;
    }

    if (smsf_ue->data_change_subscription_id)
        ogs_free(smsf_ue->data_change_subscription_id);
    smsf_ue->data_change_subscription_id =
        ogs_strdup(message.h.resource.component[2]);

    ogs_sbi_header_free(&header);

    server = ogs_sbi_server_from_stream(stream);
    ogs_assert(server);

    OpenAPI_ue_sms_context_data_t UeSmsContextData;

    memset(&UeSmsContextData, 0, sizeof(UeSmsContextData));

    UeSmsContextData.supi = smsf_ue->supi;
    UeSmsContextData.amf_id = smsf_ue->amf_instance_id;
    UeSmsContextData.access_type = smsf_ue->access_type;

    memset(&header, 0, sizeof(header));
    header.service.name =
        (char *)OGS_SBI_SERVICE_NAME_NSMSF_SMS;
    header.api.version = (char *)OGS_SBI_API_V2;
    header.resource.component[0] = (char *)OGS_SBI_RESOURCE_NAME_UE_CONTEXTS;
    header.resource.component[1] = smsf_ue->supi;

    memset(&sendmsg, 0, sizeof(sendmsg));
    sendmsg.UeSmsContextData = &UeSmsContextData;
    sendmsg.http.location = ogs_sbi_server_uri(server, &header);

    response = ogs_sbi_build_response(&sendmsg,
        OGS_SBI_HTTP_STATUS_CREATED);
    ogs_assert(response);
    ogs_assert(true == ogs_sbi_server_send_response(stream, response));

    if (sendmsg.http.location)
        ogs_free(sendmsg.http.location);

    return OGS_OK;
}

bool smsf_nudm_uecm_handle_smsf_registration(
    smsf_ue_t *smsf_ue, ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    OpenAPI_smsf_registration_t *SmsfRegistration = NULL;

    ogs_assert(smsf_ue);
    ogs_assert(stream);
    ogs_assert(recvmsg);

    SmsfRegistration = recvmsg->SmsfRegistration;

    if (!SmsfRegistration) {
        ogs_error("[%s] No SmsfRegistration", smsf_ue->supi);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No SmsfRegistration", smsf_ue->supi));
        return false;
    }

    if (!SmsfRegistration->smsf_instance_id) {
        ogs_error("[%s] No smsfInstanceId", smsf_ue->supi);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No smsfInstanceId", smsf_ue->supi));
        return false;
    }

    if (!SmsfRegistration->plmn_id) {
        ogs_error("[%s] No PlmnId", smsf_ue->supi);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No PlmnId", smsf_ue->supi));
        return false;
    }

    if (!SmsfRegistration->plmn_id->mnc) {
        ogs_error("[%s] No PlmnId.Mnc", smsf_ue->supi);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No PlmnId.Mnc", smsf_ue->supi));
        return false;
    }

    if (!SmsfRegistration->plmn_id->mcc) {
        ogs_error("[%s] No PlmnId.Mcc", smsf_ue->supi);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No PlmnId.Mcc", smsf_ue->supi));
        return false;
    }

    return true;
}

int smsf_nudm_uecm_handle_smsf_registration_delete(
    smsf_ue_t *smsf_ue, ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;

    ogs_assert(smsf_ue);
    ogs_assert(stream);

    memset(&sendmsg, 0, sizeof(sendmsg));
    response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_NO_CONTENT);
    ogs_assert(response);
    ogs_assert(true == ogs_sbi_server_send_response(stream, response));

    return OGS_OK;
}
