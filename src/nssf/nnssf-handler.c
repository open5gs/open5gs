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
#include "nnssf-handler.h"

bool nssf_nnrf_nsselection_handle_get(
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    int status = OGS_SBI_HTTP_STATUS_OK;
    char *strerror = NULL;

    OpenAPI_authorized_network_slice_info_t AuthorizedNetworkSliceInfo;
    OpenAPI_nsi_information_t NsiInformation;

    nssf_nsi_t *nsi = NULL;

    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;

    ogs_assert(stream);
    ogs_assert(recvmsg);

    if (!recvmsg->param.nf_id) {
        status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
        strerror = ogs_msprintf("No nf-id");
        goto cleanup;
    }

    if (!recvmsg->param.nf_type) {
        status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
        strerror = ogs_msprintf("No nf-type");
        goto cleanup;
    }

    if (!recvmsg->param.slice_info_request_for_pdu_session_presence) {
        status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
        strerror = ogs_msprintf("Not implemented except PDU session");
        goto cleanup;
    }

    nsi = nssf_nsi_find_by_s_nssai(&recvmsg->param.s_nssai);
    if (!nsi) {
        status = OGS_SBI_HTTP_STATUS_FORBIDDEN;
        strerror = ogs_msprintf("Cannot find NSI by S-NSSAI[SST:%d SD:0x%x]",
            recvmsg->param.s_nssai.sst, recvmsg->param.s_nssai.sd.v);
        goto cleanup;
    }

    memset(&NsiInformation, 0, sizeof(NsiInformation));
    NsiInformation.nrf_id = nssf_nsi_nrf_uri(nsi);
    NsiInformation.nsi_id = nsi->nsi_id;

    memset(&AuthorizedNetworkSliceInfo, 0, sizeof(AuthorizedNetworkSliceInfo));
    AuthorizedNetworkSliceInfo.nsi_information = &NsiInformation;

    memset(&sendmsg, 0, sizeof(sendmsg));
    sendmsg.AuthorizedNetworkSliceInfo = &AuthorizedNetworkSliceInfo;

    response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_OK);
    ogs_assert(response);
    ogs_assert(true == ogs_sbi_server_send_response(stream, response));

    if (NsiInformation.nrf_id)
        ogs_free(NsiInformation.nrf_id);

    return true;

cleanup:
    ogs_assert(strerror);

    ogs_error("%s", strerror);
    ogs_sbi_server_send_error(stream, status, recvmsg, strerror, NULL);
    ogs_free(strerror);

    return false;
}
