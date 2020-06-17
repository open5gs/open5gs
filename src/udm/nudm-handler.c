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

#include "sbi-path.h"
#include "nnrf-handler.h"
#include "nudm-handler.h"

bool udm_nudm_ueau_handle_get(udm_ue_t *udm_ue, ogs_sbi_message_t *message)
{
    ogs_sbi_session_t *session = NULL;

    OpenAPI_authentication_info_request_t *AuthenticationInfoRequest = NULL;
    char *serving_network_name = NULL;
    char *ausf_instance_id = NULL;

    ogs_assert(udm_ue);
    session = udm_ue->sbi.session;
    ogs_assert(session);

    ogs_assert(message);

    AuthenticationInfoRequest = message->AuthenticationInfoRequest;
    if (!AuthenticationInfoRequest) {
        ogs_error("[%s] No AuthenticationInfoRequest", udm_ue->suci);
        ogs_sbi_server_send_error(session, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No AuthenticationInfoRequest", udm_ue->suci);
        return false;
    }

    serving_network_name = AuthenticationInfoRequest->serving_network_name;
    if (!AuthenticationInfoRequest) {
        ogs_error("[%s] No servingNetworkName", udm_ue->suci);
        ogs_sbi_server_send_error(session, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No servingNetworkName", udm_ue->suci);
        return false;
    }

    ausf_instance_id = AuthenticationInfoRequest->ausf_instance_id;
    if (!AuthenticationInfoRequest) {
        ogs_error("[%s] No ausfInstanceId", udm_ue->suci);
        ogs_sbi_server_send_error(session, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No ausfInstanceId", udm_ue->suci);
        return false;
    }

    if (udm_ue->serving_network_name)
        ogs_free(udm_ue->serving_network_name);
    udm_ue->serving_network_name = ogs_strdup(serving_network_name);

    if (udm_ue->ausf_instance_id)
        ogs_free(udm_ue->ausf_instance_id);
    udm_ue->ausf_instance_id = ogs_strdup(ausf_instance_id);

    udm_sbi_discover_and_send(OpenAPI_nf_type_UDR, udm_ue, NULL,
            udm_nudr_dr_build_query_authentication);

    return true;
}

bool udm_nudm_ueau_handle_result_confirmation_inform(
        udm_ue_t *udm_ue, ogs_sbi_message_t *message)
{
    ogs_sbi_session_t *session = NULL;

    ogs_assert(udm_ue);
    session = udm_ue->sbi.session;
    ogs_assert(session);

    ogs_assert(message);

    if (!message->AuthEvent) {
        ogs_error("[%s] No AuthEvent", udm_ue->suci);
        ogs_sbi_server_send_error(session, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No AuthEvent", udm_ue->suci);
        return false;
    }

    udm_ue->auth_event = OpenAPI_auth_event_copy(
            udm_ue->auth_event, message->AuthEvent);

    udm_sbi_discover_and_send(OpenAPI_nf_type_UDR, udm_ue, NULL,
            udm_nudr_dr_build_update_authentication);

    return true;
}

bool udm_nudm_uecm_handle_registration(
        udm_ue_t *udm_ue, ogs_sbi_message_t *message)
{
    ogs_sbi_session_t *session = NULL;

    OpenAPI_amf3_gpp_access_registration_t *Amf3GppAccessRegistration = NULL;
    OpenAPI_guami_t *guami = NULL;
    OpenAPI_plmn_id_t *serving_plmn_id = NULL;

    ogs_assert(udm_ue);
    session = udm_ue->sbi.session;
    ogs_assert(session);

    ogs_assert(message);

    Amf3GppAccessRegistration = message->Amf3GppAccessRegistration;
    if (!Amf3GppAccessRegistration) {
        ogs_error("[%s] No Amf3GppAccessRegistration", udm_ue->supi);
        ogs_sbi_server_send_error(session, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No Amf3GppAccessRegistration", udm_ue->supi);
        return false;
    }

    if (!Amf3GppAccessRegistration->dereg_callback_uri) {
        ogs_error("[%s] No dregCallbackUri", udm_ue->supi);
        ogs_sbi_server_send_error(session, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No dregCallbackUri", udm_ue->supi);
        return false;
    }

    guami = Amf3GppAccessRegistration->guami;
    if (!guami) {
        ogs_error("[%s] No Guami", udm_ue->supi);
        ogs_sbi_server_send_error(session, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No Guami", udm_ue->supi);
        return false;
    }

    if (!guami->amf_id) {
        ogs_error("[%s] No Guami.AmfId", udm_ue->supi);
        ogs_sbi_server_send_error(session, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No Guami.AmfId", udm_ue->supi);
        return false;
    }

    serving_plmn_id = guami->plmn_id;
    if (!serving_plmn_id) {
        ogs_error("[%s] No PlmnId", udm_ue->supi);
        ogs_sbi_server_send_error(session, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No PlmnId", udm_ue->supi);
        return false;
    }

    if (!serving_plmn_id->mnc) {
        ogs_error("[%s] No PlmnId.Mnc", udm_ue->supi);
        ogs_sbi_server_send_error(session, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No PlmnId.Mnc", udm_ue->supi);
        return false;
    }

    if (!serving_plmn_id->mcc) {
        ogs_error("[%s] No PlmnId.Mcc", udm_ue->supi);
        ogs_sbi_server_send_error(session, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No PlmnId.Mcc", udm_ue->supi);
        return false;
    }

    if (udm_ue->dereg_callback_uri)
        ogs_free(udm_ue->dereg_callback_uri);
    udm_ue->dereg_callback_uri = ogs_strdup(
            Amf3GppAccessRegistration->dereg_callback_uri);

    ogs_amf_id_from_string(&udm_ue->amf_id, guami->amf_id);
    ogs_plmn_id_build(&udm_ue->serving_plmn_id, 
        atoi(serving_plmn_id->mcc), atoi(serving_plmn_id->mnc),
        strlen(serving_plmn_id->mnc));

    udm_ue->amf_3gpp_access_registration =
        OpenAPI_amf3_gpp_access_registration_copy(
            udm_ue->amf_3gpp_access_registration,
                message->Amf3GppAccessRegistration);

    udm_sbi_discover_and_send(OpenAPI_nf_type_UDR, udm_ue, NULL,
            udm_nudr_dr_build_update_context);

    return true;
}

bool udm_nudm_sdm_handle_subscription_provisioned(
        udm_ue_t *udm_ue, ogs_sbi_message_t *recvmsg)
{
    ogs_sbi_session_t *session = NULL;

    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;

    ogs_assert(udm_ue);
    session = udm_ue->sbi.session;
    ogs_assert(session);

    ogs_assert(recvmsg);

    SWITCH(recvmsg->h.resource.component[1])
    CASE(OGS_SBI_RESOURCE_NAME_UE_CONTEXT_IN_SMF_DATA)
        memset(&sendmsg, 0, sizeof(sendmsg));

        response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_OK);
        ogs_assert(response);
        ogs_sbi_server_send_response(session, response);

        break;

    DEFAULT
        ogs_error("Invalid resource name [%s]",
                recvmsg->h.resource.component[3]);
        return false;
    END

    return true;
}
