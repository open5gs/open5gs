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
    bool rc;
    int r;
    OpenAPI_uri_scheme_e scheme = OpenAPI_uri_scheme_NULL;
    ogs_sbi_client_t *client = NULL, *scp_client = NULL;
    ogs_sockaddr_t *addr = NULL;

    OpenAPI_authorized_network_slice_info_t *AuthorizedNetworkSliceInfo = NULL;
    OpenAPI_nsi_information_t *NsiInformation = NULL;

    amf_ue_t *amf_ue = NULL;
    ran_ue_t *ran_ue = NULL;

    ogs_assert(sess);
    amf_ue = sess->amf_ue;
    ogs_assert(amf_ue);
    ogs_assert(recvmsg);

    ogs_assert(!SESSION_CONTEXT_IN_SMF(sess));

    ran_ue = ran_ue_cycle(amf_ue->ran_ue);
    if (!ran_ue) {
        ogs_error("NG context has already been removed");
        return OGS_ERROR;
    }

    if (recvmsg->res_status != OGS_SBI_HTTP_STATUS_OK) {
        ogs_error("[%s] HTTP response error [%d]",
                amf_ue->supi, recvmsg->res_status);
        r = nas_5gs_send_gmm_status(amf_ue, recvmsg->res_status);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return OGS_ERROR;
    }

    AuthorizedNetworkSliceInfo = recvmsg->AuthorizedNetworkSliceInfo;
    if (!AuthorizedNetworkSliceInfo) {
        ogs_error("No AuthorizedNetworkSliceInfo");
        r = nas_5gs_send_gmm_reject_from_sbi(
                amf_ue, OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return OGS_ERROR;
    }

    NsiInformation = AuthorizedNetworkSliceInfo->nsi_information;
    if (!NsiInformation) {
        ogs_error("No NsiInformation");
        r = nas_5gs_send_gmm_reject_from_sbi(
                amf_ue, OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return OGS_ERROR;
    }

    if (!NsiInformation->nrf_id) {
        ogs_error("No nrfId");
        r = nas_5gs_send_gmm_reject_from_sbi(
                amf_ue, OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return OGS_ERROR;
    }

    if (sess->nssf.nrf.id)
        ogs_free(sess->nssf.nrf.id);
    sess->nssf.nrf.id = ogs_strdup(NsiInformation->nrf_id);
    ogs_assert(sess->nssf.nrf.id);

    scp_client = NF_INSTANCE_CLIENT(ogs_sbi_self()->scp_instance);

    if (scp_client) {
        amf_nsmf_pdusession_sm_context_param_t param;

        memset(&param, 0, sizeof(param));
        param.nrf_uri.nrf.id = sess->nssf.nrf.id;

        r = amf_sess_sbi_discover_and_send(
                OGS_SBI_SERVICE_TYPE_NSMF_PDUSESSION, NULL,
                amf_nsmf_pdusession_build_create_sm_context,
                sess, AMF_CREATE_SM_CONTEXT_NO_STATE, &param);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
    } else {
        rc = ogs_sbi_getaddr_from_uri(&scheme, &addr, NsiInformation->nrf_id);
        if (rc == false || scheme == OpenAPI_uri_scheme_NULL) {
            ogs_error("[%s:%d] Invalid URI [%s]",
                    amf_ue->supi, sess->psi, NsiInformation->nrf_id);
            r = nas_5gs_send_gmm_reject_from_sbi(
                    amf_ue, OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return OGS_ERROR;;
        }

        client = ogs_sbi_client_find(scheme, addr);
        if (!client) {
            client = ogs_sbi_client_add(scheme, addr);
            ogs_assert(client);
        }

        OGS_SBI_SETUP_CLIENT(&sess->nssf.nrf, client);
        ogs_freeaddrinfo(addr);

        r = amf_sess_sbi_discover_by_nsi(
                sess, OGS_SBI_SERVICE_TYPE_NSMF_PDUSESSION, NULL);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
    }

    return OGS_OK;
}
