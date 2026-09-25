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
#include "nausf-handler.h"
#include "eap-aka-prime.h"

bool ausf_nausf_auth_handle_authenticate(ausf_ue_t *ausf_ue,
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    OpenAPI_authentication_info_t *AuthenticationInfo = NULL;
    char *serving_network_name = NULL;
    int r;

    ogs_assert(ausf_ue);
    ogs_assert(stream);
    ogs_assert(recvmsg);

    AuthenticationInfo = recvmsg->AuthenticationInfo;
    if (!AuthenticationInfo) {
        ogs_error("[%s] No AuthenticationInfo", ausf_ue->suci);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No AuthenticationInfo", ausf_ue->suci, NULL));
        return false;
    }

    serving_network_name = AuthenticationInfo->serving_network_name;
    if (!serving_network_name) {
        ogs_error("[%s] No servingNetworkName", ausf_ue->suci);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No servingNetworkName", ausf_ue->suci, NULL));
        return false;
    }
    if (!serving_network_name[0]) {
        ogs_error("[%s] Empty servingNetworkName", ausf_ue->suci);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "Empty servingNetworkName", ausf_ue->suci, NULL));
        return false;
    }

    if (ausf_ue->serving_network_name)
        ogs_free(ausf_ue->serving_network_name);
    ausf_ue->serving_network_name = ogs_strdup(serving_network_name);
    ogs_assert(ausf_ue->serving_network_name);

    r = ausf_sbi_discover_and_send(
            OpenAPI_service_name_nudm_ueau, NULL,
            ausf_nudm_ueau_build_get,
            ausf_ue, stream, AuthenticationInfo->resynchronization_info);
    ogs_expect(r == OGS_OK);
    ogs_assert(r != OGS_ERROR);

    return true;
}

bool ausf_nausf_auth_handle_authenticate_confirmation(ausf_ue_t *ausf_ue,
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    OpenAPI_confirmation_data_t *ConfirmationData = NULL;
    char *res_star_string = NULL;
    uint8_t res_star[OGS_KEYSTRLEN(OGS_MAX_RES_LEN)];
    int r;

    ogs_assert(ausf_ue);
    ogs_assert(stream);
    ogs_assert(recvmsg);

    ConfirmationData = recvmsg->ConfirmationData;
    if (!ConfirmationData) {
        ogs_error("[%s] No ConfirmationData", ausf_ue->suci);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No ConfirmationData", ausf_ue->suci, NULL));
        return false;
    }

    res_star_string = ConfirmationData->res_star;
    if (!res_star_string) {
        ogs_error("[%s] No ConfirmationData.resStar", ausf_ue->suci);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No ConfirmationData.resStar", ausf_ue->suci, NULL));
        return false;
    }

    ogs_ascii_to_hex(res_star_string, strlen(res_star_string),
            res_star, sizeof(res_star));

    if (memcmp(res_star, ausf_ue->xres_star, OGS_MAX_RES_LEN) != 0) {
        ogs_log_hexdump(OGS_LOG_WARN, res_star, OGS_MAX_RES_LEN);
        ogs_log_hexdump(OGS_LOG_WARN, ausf_ue->xres_star, OGS_MAX_RES_LEN);

        ausf_ue->auth_result = OpenAPI_auth_result_AUTHENTICATION_FAILURE;
    } else {
        ausf_ue->auth_result = OpenAPI_auth_result_AUTHENTICATION_SUCCESS;
    }

    r = ausf_sbi_discover_and_send(
            OpenAPI_service_name_nudm_ueau, NULL,
            ausf_nudm_ueau_build_result_confirmation_inform,
            ausf_ue, stream, NULL);
    ogs_expect(r == OGS_OK);
    ogs_assert(r != OGS_ERROR);

    return true;
}

/* Maximum consecutive AKA'-Synchronization-Failures before giving up */
#define AUSF_EAP_MAX_SYNC_FAILURES 2

/* Respond to the held eap-session request with a terminating EAP-Failure. */
static bool send_eap_aka_prime_failure(ausf_ue_t *ausf_ue,
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;
    OpenAPI_eap_session_t EapSession;
    char *eap_payload;

    ausf_ue->auth_result = OpenAPI_auth_result_AUTHENTICATION_FAILURE;

    eap_payload = ausf_eap_aka_prime_build_result(
            AUSF_EAP_CODE_FAILURE, ausf_ue->eap_id);

    memset(&EapSession, 0, sizeof(EapSession));
    EapSession.eap_payload = eap_payload;
    EapSession.auth_result = OpenAPI_auth_result_AUTHENTICATION_FAILURE;
    EapSession.supi = ausf_ue->supi;

    memset(&sendmsg, 0, sizeof(sendmsg));
    sendmsg.EapSession = &EapSession;

    response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_OK);
    ogs_assert(response);
    ogs_assert(true == ogs_sbi_server_send_response(stream, response));

    if (eap_payload)
        ogs_free(eap_payload);

    return true;
}

bool ausf_nausf_auth_handle_authenticate_eap_session(ausf_ue_t *ausf_ue,
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    OpenAPI_eap_session_t *EapSession = NULL;
    bool success = false;
    uint8_t auts[OGS_AUTS_LEN];
    int subtype, r;

    ogs_assert(ausf_ue);
    ogs_assert(stream);
    ogs_assert(recvmsg);

    EapSession = recvmsg->EapSession;
    if (!EapSession || !EapSession->eap_payload) {
        ogs_error("[%s] No EapSession.eapPayload", ausf_ue->suci);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No EapSession.eapPayload", ausf_ue->suci, NULL));
        return false;
    }

    subtype = ausf_eap_aka_prime_handle_response(
            ausf_ue, EapSession->eap_payload, &success, auts);
    if (subtype < 0) {
        ogs_error("[%s] Malformed EAP-Response", ausf_ue->suci);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "Malformed EAP-Response", ausf_ue->suci, NULL));
        return false;
    }

    if (subtype == AUSF_EAP_AKA_SUBTYPE_SYNCHRONIZATION_FAILURE) {
        /*
         * The UE reported an SQN synchronization failure. Bounded by
         * AUSF_EAP_MAX_SYNC_FAILURES to avoid an endless re-challenge loop
         * (TS 33.501 6.1.3.3); otherwise fetch a fresh authentication
         * vector from the UDM with the AUTS and re-challenge (the UDM
         * response handler answers this request with authResult ONGOING).
         */
        ausf_ue->eap_sync_count++;
        if (ausf_ue->eap_sync_count > AUSF_EAP_MAX_SYNC_FAILURES) {
            ogs_error("[%s] Too many EAP-AKA' synchronization failures",
                    ausf_ue->suci);
            return send_eap_aka_prime_failure(ausf_ue, stream, recvmsg);
        }

        ogs_info("[%s] EAP-AKA' synchronization failure, resyncing",
                ausf_ue->suci);
        memcpy(ausf_ue->eap_auts, auts, OGS_AUTS_LEN);
        ausf_ue->eap_resync = true;

        r = ausf_sbi_discover_and_send(
                OpenAPI_service_name_nudm_ueau, NULL,
                ausf_nudm_ueau_build_get,
                ausf_ue, stream, NULL);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);

        return true;
    }

    /* Any non-sync response clears the synchronization-failure counter. */
    ausf_ue->eap_sync_count = 0;

    if (!success) {
        /*
         * AKA'-Challenge with a bad AT_MAC/AT_RES, or an
         * Authentication-Reject / Client-Error: terminate with EAP-Failure.
         */
        ogs_warn("[%s] EAP-AKA' authentication failed (subtype %d)",
                ausf_ue->suci, subtype);
        return send_eap_aka_prime_failure(ausf_ue, stream, recvmsg);
    }

    ausf_ue->auth_result = OpenAPI_auth_result_AUTHENTICATION_SUCCESS;

    r = ausf_sbi_discover_and_send(
            OpenAPI_service_name_nudm_ueau, NULL,
            ausf_nudm_ueau_build_result_confirmation_inform,
            ausf_ue, stream, NULL);
    ogs_expect(r == OGS_OK);
    ogs_assert(r != OGS_ERROR);

    return true;
}

bool ausf_nausf_auth_handle_authenticate_delete(ausf_ue_t *ausf_ue,
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    int r;

    ogs_assert(ausf_ue);
    ogs_assert(stream);
    ogs_assert(recvmsg);

    r = ausf_sbi_discover_and_send(
            OpenAPI_service_name_nudm_ueau, NULL,
            ausf_nudm_ueau_build_auth_removal_ind,
            ausf_ue, stream, NULL);
    ogs_expect(r == OGS_OK);
    ogs_assert(r != OGS_ERROR);

    return true;
}
