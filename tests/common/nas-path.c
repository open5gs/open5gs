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
    case OGS_NAS_5GS_IDENTITY_REQUEST:
        testgmm_handle_identity_request(test_ue, &message.gmm.identity_request);
        break;
    case OGS_NAS_5GS_REGISTRATION_REJECT:
        break;
    case OGS_NAS_5GS_AUTHENTICATION_REQUEST:
        testgmm_handle_authentication_request(test_ue,
                &message.gmm.authentication_request);
        break;
    case OGS_NAS_5GS_AUTHENTICATION_REJECT:
        break;
    case OGS_NAS_5GS_SECURITY_MODE_COMMAND:
        break;
    case OGS_NAS_5GS_REGISTRATION_ACCEPT:
        testgmm_handle_registration_accept(test_ue,
                &message.gmm.authentication_request);
        break;
    case OGS_NAS_5GS_SERVICE_ACCEPT:
        break;
    case OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND:
        break;
    case OGS_NAS_5GS_DL_NAS_TRANSPORT:
        testgmm_handle_dl_nas_transport(test_ue, &message.gmm.dl_nas_transport);
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
    ogs_nas_5gs_message_t message;

    ogs_assert(sess);
    ogs_assert(pkbuf);

    rv = ogs_nas_5gsm_decode(&message, pkbuf);
    ogs_assert(rv == OGS_OK);

    switch (message.gsm.h.message_type) {
    case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT:
        testgsm_handle_pdu_session_establishment_accept(sess,
                &message.gsm.pdu_session_establishment_accept);
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
    ogs_pkbuf_put_data(gsmbuf,
            payload_container->buffer, payload_container->length);

    testgsm_recv(sess, gsmbuf);
}
