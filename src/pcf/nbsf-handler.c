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

#include "nbsf-handler.h"

bool pcf_nbsf_management_handle_register(
    pcf_sess_t *sess, ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    int rv, status = 0;
    char *strerror = NULL;
    pcf_ue_t *pcf_ue = NULL;
    ogs_sbi_server_t *server = NULL;

    ogs_sbi_header_t header;
    ogs_sbi_message_t message;

    OpenAPI_pcf_binding_t *PcfBinding = NULL;

    bool rc;
    ogs_sbi_client_t *client = NULL;
    OpenAPI_uri_scheme_e scheme = OpenAPI_uri_scheme_NULL;
    char *fqdn = NULL;
    uint16_t fqdn_port = 0;
    ogs_sockaddr_t *addr = NULL, *addr6 = NULL;

    ogs_assert(sess);
    pcf_ue = pcf_ue_find_by_id(sess->pcf_ue_id);
    ogs_assert(pcf_ue);
    ogs_assert(stream);
    server = ogs_sbi_server_from_stream(stream);
    ogs_assert(server);

    ogs_assert(recvmsg);

    ogs_assert(pcf_ue->supi);
    ogs_assert(sess->dnn);

    if (!recvmsg->http.location) {
        strerror = ogs_msprintf("[%s:%d] No http.location",
                pcf_ue->supi, sess->psi);
        status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
        goto cleanup;
    }

    if (!recvmsg->PcfBinding) {
        strerror = ogs_msprintf("[%s:%d] No PcfBinding",
                pcf_ue->supi, sess->psi);
        status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
        goto cleanup;
    }

    PcfBinding = recvmsg->PcfBinding;
    if (PcfBinding->supp_feat) {
        uint64_t supported_features =
            ogs_uint64_from_string(PcfBinding->supp_feat);
        sess->management_features &= supported_features;
    }

    memset(&header, 0, sizeof(header));
    header.uri = recvmsg->http.location;

    rv = ogs_sbi_parse_header(&message, &header);
    if (rv != OGS_OK) {
        strerror = ogs_msprintf("[%s:%d] Cannot parse http.location [%s]",
                pcf_ue->supi, sess->psi, recvmsg->http.location);
        goto cleanup;
    }

    if (!message.h.resource.component[1]) {
        strerror = ogs_msprintf("[%s:%d] No Binding ID [%s]",
                pcf_ue->supi, sess->psi, recvmsg->http.location);

        ogs_sbi_header_free(&header);
        goto cleanup;
    }

    rc = ogs_sbi_getaddr_from_uri(
            &scheme, &fqdn, &fqdn_port, &addr, &addr6, header.uri);
    if (rc == false || scheme == OpenAPI_uri_scheme_NULL) {
        strerror = ogs_msprintf("[%s:%d] Invalid URI [%s]",
                pcf_ue->supi, sess->psi, header.uri);
        ogs_sbi_header_free(&header);
        goto cleanup;
    }

    client = ogs_sbi_client_find(scheme, fqdn, fqdn_port, addr, addr6);
    if (!client) {
        ogs_debug("[%s:%d] ogs_sbi_client_add()", pcf_ue->supi, sess->psi);
        client = ogs_sbi_client_add(scheme, fqdn, fqdn_port, addr, addr6);
        if (!client) {
            strerror = ogs_msprintf("[%s:%d] ogs_sbi_client_add() failed",
                    pcf_ue->supi, sess->psi);

            ogs_sbi_header_free(&header);
            ogs_free(fqdn);
            ogs_freeaddrinfo(addr);
            ogs_freeaddrinfo(addr6);

            goto cleanup;
        }
    }

    OGS_SBI_SETUP_CLIENT(&sess->binding, client);

    ogs_free(fqdn);
    ogs_freeaddrinfo(addr);
    ogs_freeaddrinfo(addr6);

    PCF_BINDING_STORE(sess, header.uri, message.h.resource.component[1]);

    ogs_sbi_header_free(&header);

    /* Send Response for SM Policy Association establishment */
    rc = pcf_sbi_send_smpolicycontrol_create_response(sess, stream);
    ogs_expect(rc == true);

    return rc;

cleanup:
    ogs_assert(strerror);
    ogs_assert(status);
    ogs_error("%s", strerror);
    ogs_assert(true == ogs_sbi_server_send_error(
                stream, status, recvmsg, strerror, NULL, NULL));
    ogs_free(strerror);

    return false;
}

bool pcf_nbsf_management_handle_de_register(
    pcf_sess_t *sess, ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    bool rc;

    ogs_assert(sess);
    ogs_assert(stream);

    rc = ogs_sbi_send_response(stream, OGS_SBI_HTTP_STATUS_NO_CONTENT);
    ogs_expect(rc == true);

    return rc;
}
