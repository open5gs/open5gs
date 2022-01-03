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

#include "nnssf-handler.h"
#include "nas-path.h"
#include "sbi-path.h"

int amf_nnssf_nsselection_handle_get(
        amf_sess_t *sess, ogs_sbi_message_t *recvmsg)
{
    amf_ue_t *amf_ue = NULL;

    ogs_sbi_client_t *client = NULL;
    ogs_sockaddr_t *addr = NULL;

    OpenAPI_authorized_network_slice_info_t *AuthorizedNetworkSliceInfo = NULL;
    OpenAPI_nsi_information_t *NsiInformation = NULL;

    ogs_assert(sess);
    amf_ue = sess->amf_ue;
    ogs_assert(amf_ue);
    ogs_assert(recvmsg);

    ogs_assert(!SESSION_CONTEXT_IN_SMF(sess));

    if (recvmsg->res_status != OGS_SBI_HTTP_STATUS_OK) {
        ogs_error("[%s] HTTP response error [%d]",
                amf_ue->supi, recvmsg->res_status);
        ogs_assert(OGS_OK ==
            nas_5gs_send_gmm_status(amf_ue, recvmsg->res_status));
        return OGS_ERROR;
    }

    AuthorizedNetworkSliceInfo = recvmsg->AuthorizedNetworkSliceInfo;
    if (!AuthorizedNetworkSliceInfo) {
        ogs_error("No AuthorizedNetworkSliceInfo");
        ogs_assert(OGS_OK ==
            nas_5gs_send_gmm_reject_from_sbi(
                amf_ue, OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR));
        return OGS_ERROR;
    }

    NsiInformation = AuthorizedNetworkSliceInfo->nsi_information;
    if (!NsiInformation) {
        ogs_error("No NsiInformation");
        ogs_assert(OGS_OK ==
            nas_5gs_send_gmm_reject_from_sbi(
                amf_ue, OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR));
        return OGS_ERROR;
    }

    if (!NsiInformation->nrf_id) {
        ogs_error("No nrfId");
        ogs_assert(OGS_OK ==
            nas_5gs_send_gmm_reject_from_sbi(
                amf_ue, OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR));
        return OGS_ERROR;
    }

    if (sess->nssf.nrf.id)
        ogs_free(sess->nssf.nrf.id);
    sess->nssf.nrf.id = ogs_strdup(NsiInformation->nrf_id);
    ogs_assert(sess->nssf.nrf.id);

    addr = ogs_sbi_getaddr_from_uri(NsiInformation->nrf_id);
    if (!addr) {
        ogs_error("[%s:%d] Invalid URI [%s]",
                amf_ue->supi, sess->psi, NsiInformation->nrf_id);
        ogs_assert(OGS_OK ==
            nas_5gs_send_gmm_reject_from_sbi(
                amf_ue, OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR));
        return OGS_ERROR;;
    }

    client = ogs_sbi_client_find(addr);
    if (!client) {
        client = ogs_sbi_client_add(addr);
        ogs_assert(client);
    }

    if (sess->nssf.nrf.client && sess->nssf.nrf.client != client) {
        ogs_warn("NSSF NRF URI Updated [%s]", sess->nssf.nrf.id);
        ogs_sbi_client_remove(sess->nssf.nrf.client);
    }
    OGS_SETUP_SBI_CLIENT(&sess->nssf.nrf, client);

    ogs_freeaddrinfo(addr);

    ogs_assert(true == amf_sess_sbi_discover_by_nsi(OpenAPI_nf_type_SMF, sess));

    return OGS_OK;
}
