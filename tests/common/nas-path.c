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

#include "test-common.h"

void testgmm_recv(test_ue_t *test_ue, ogs_pkbuf_t *pkbuf)
{
    int rv;
    ogs_nas_5gs_message_t message;

    ogs_assert(test_ue);
    ogs_assert(pkbuf);

    rv = ogs_nas_5gmm_decode(&message, pkbuf);
    ogs_assert(rv == OGS_OK);

    test_ue->gmm_message_type = message.gmm.h.message_type;
    switch (message.gmm.h.message_type) {
    case OGS_NAS_5GS_REGISTRATION_ACCEPT:
        testgmm_handle_registration_accept(test_ue,
                &message.gmm.registration_request);
        break;
    case OGS_NAS_5GS_REGISTRATION_REJECT:
        break;
    case OGS_NAS_5GS_SERVICE_ACCEPT:
        testgmm_handle_service_accept(test_ue, &message.gmm.service_accept);
        break;
    case OGS_NAS_5GS_SERVICE_REJECT:
        testgmm_handle_service_reject(test_ue, &message.gmm.service_reject);
        break;
    case OGS_NAS_5GS_IDENTITY_REQUEST:
        testgmm_handle_identity_request(test_ue, &message.gmm.identity_request);
        break;
    case OGS_NAS_5GS_AUTHENTICATION_REQUEST:
        testgmm_handle_authentication_request(test_ue,
                &message.gmm.authentication_request);
        break;
    case OGS_NAS_5GS_AUTHENTICATION_REJECT:
        break;
    case OGS_NAS_5GS_SECURITY_MODE_COMMAND:
        break;
    case OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND:
        testgmm_handle_configuration_update_command(
                test_ue, &message.gmm.configuration_update_command);
        break;
    case OGS_NAS_5GS_DL_NAS_TRANSPORT:
        testgmm_handle_dl_nas_transport(test_ue, &message.gmm.dl_nas_transport);
        break;
    case OGS_NAS_5GS_5GMM_STATUS:
        break;
    default:
        ogs_error("Unknown message[%d]", message.gmm.h.message_type);
        break;
    }

    ogs_pkbuf_free(pkbuf);
}

void testgsm_recv(test_sess_t *sess, ogs_pkbuf_t *pkbuf)
{
    int rv;
    test_ue_t *test_ue = NULL;
    ogs_nas_5gs_message_t message;

    ogs_assert(sess);
    test_ue = sess->test_ue;
    ogs_assert(test_ue);
    ogs_assert(pkbuf);

    rv = ogs_nas_5gsm_decode(&message, pkbuf);
    ogs_assert(rv == OGS_OK);

    sess->pti = message.gsm.h.procedure_transaction_identity;

    test_ue->gsm_message_type = message.gsm.h.message_type;
    switch (message.gsm.h.message_type) {
    case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT:
        testgsm_handle_pdu_session_establishment_accept(sess,
                &message.gsm.pdu_session_establishment_accept);
        break;
    case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT:
        break;
    case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND:
        testgsm_handle_pdu_session_modification_command(sess,
                &message.gsm.pdu_session_modification_command);
        break;
    case OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND:
        break;
    case OGS_NAS_5GS_PDU_SESSION_RELEASE_REJECT:
        break;
    default:
        ogs_error("Unknown message[%d]", message.gsm.h.message_type);
        break;
    }

    ogs_pkbuf_free(pkbuf);
}

void testgmm_send_to_gsm(test_sess_t *sess,
        ogs_nas_payload_container_t *payload_container)
{
    ogs_pkbuf_t *gsmbuf = NULL;

    ogs_assert(sess);
    ogs_assert(payload_container);
    ogs_assert(payload_container->buffer);
    ogs_assert(payload_container->length);

    gsmbuf = ogs_pkbuf_alloc(NULL, payload_container->length);
    ogs_assert(gsmbuf);
    ogs_pkbuf_put_data(gsmbuf,
            payload_container->buffer, payload_container->length);

    testgsm_recv(sess, gsmbuf);
}

void testemm_recv(test_ue_t *test_ue, ogs_pkbuf_t *pkbuf)
{
    int rv;
    ogs_nas_eps_message_t message;

    ogs_assert(test_ue);
    ogs_assert(pkbuf);

    rv = ogs_nas_emm_decode(&message, pkbuf);
    ogs_assert(rv == OGS_OK);

    test_ue->emm_message_type = message.emm.h.message_type;
    switch (message.emm.h.message_type) {
    case OGS_NAS_EPS_IDENTITY_REQUEST:
        testemm_handle_identity_request(test_ue, &message.emm.identity_request);
        break;
    case OGS_NAS_EPS_AUTHENTICATION_REQUEST:
        testemm_handle_authentication_request(test_ue,
                &message.emm.authentication_request);
        break;
    case OGS_NAS_EPS_AUTHENTICATION_REJECT:
        break;
    case OGS_NAS_EPS_SECURITY_MODE_COMMAND:
        break;
    case OGS_NAS_EPS_ATTACH_ACCEPT:
        testemm_handle_attach_accept(test_ue, &message.emm.attach_accept);
        break;
    case OGS_NAS_EPS_ATTACH_REJECT:
        break;
    case OGS_NAS_EPS_SERVICE_REJECT:
        break;
    case OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT:
        testemm_handle_tau_accept(test_ue,
                &message.emm.tracking_area_update_accept);
        break;
    case OGS_NAS_EPS_TRACKING_AREA_UPDATE_REJECT:
        break;
    case OGS_NAS_EPS_EMM_INFORMATION:
        break;
    case OGS_NAS_EPS_CS_SERVICE_NOTIFICATION:
        break;
    case OGS_NAS_EPS_DOWNLINK_NAS_TRANSPORT:
        break;
    default:
        ogs_error("Unknown message[%d]", message.emm.h.message_type);
        break;
    }

    ogs_pkbuf_free(pkbuf);
}

void testesm_recv(test_ue_t *test_ue, ogs_pkbuf_t *pkbuf)
{
    int rv;
    test_bearer_t *bearer = NULL;
    ogs_nas_eps_message_t message;

    ogs_assert(test_ue);
    ogs_assert(pkbuf);

    rv = ogs_nas_esm_decode(&message, pkbuf);
    ogs_assert(rv == OGS_OK);

    test_ue->esm_message_type = message.esm.h.message_type;
    switch (message.esm.h.message_type) {
    case OGS_NAS_EPS_ESM_INFORMATION_REQUEST:
        testesm_handle_esm_information_request(test_ue, &message);
        break;
    case OGS_NAS_EPS_PDN_CONNECTIVITY_REJECT:
        testesm_handle_pdn_connectivity_reject(test_ue, &message);
        break;
    case OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST:
        testesm_handle_activate_default_eps_bearer_context_request(
                test_ue, &message);
        break;
    case OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST:
        testesm_handle_activate_dedicated_eps_bearer_context_request(
                test_ue, &message);
        break;
    case OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST:
        testesm_handle_modify_eps_bearer_context_request(test_ue, &message);
        break;
    case OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST:
        testesm_handle_deactivate_eps_bearer_context_request(test_ue, &message);
        break;
    case OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REJECT:
        testesm_handle_bearer_resource_allocation(test_ue, &message);
        break;
    case OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REJECT:
        testesm_handle_bearer_resource_modification(test_ue, &message);
        break;
    default:
        ogs_error("Unknown message[%d]", message.esm.h.message_type);
        break;
    }

    ogs_pkbuf_free(pkbuf);
}

void testemm_send_to_esm(test_ue_t *test_ue,
    ogs_nas_esm_message_container_t *esm_message_container)
{
    ogs_pkbuf_t *esmbuf = NULL;

    ogs_assert(test_ue);
    ogs_assert(esm_message_container);
    ogs_assert(esm_message_container->buffer);

    esmbuf = ogs_pkbuf_alloc(NULL, esm_message_container->length);
    ogs_assert(esmbuf);
    ogs_pkbuf_put_data(esmbuf,
            esm_message_container->buffer, esm_message_container->length);

    testesm_recv(test_ue, esmbuf);
}
