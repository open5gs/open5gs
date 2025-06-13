/*
 * Copyright (C) 2019-2024 by Sukchan Lee <acetcom@gmail.com>
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

bool nssf_nnrf_nsselection_handle_get_from_amf_or_vnssf(
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

    if (!recvmsg->param.slice_info_for_pdu_session_presence) {
        status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
        strerror = ogs_msprintf("Not implemented except PDU session");
        goto cleanup;
    }

    if (!recvmsg->param.snssai_presence) {
        status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
        strerror = ogs_msprintf("No sNssai");
        goto cleanup;
    }

    if (!recvmsg->param.roaming_indication) {
        status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
        strerror = ogs_msprintf("No roamingIndication");
        goto cleanup;
    }

    nsi = nssf_nsi_find_by_s_nssai(&recvmsg->param.s_nssai);
    if (!nsi) {
        status = OGS_SBI_HTTP_STATUS_FORBIDDEN;
        strerror = ogs_msprintf("Cannot find NSI by S-NSSAI[SST:%d SD:0x%x]",
            recvmsg->param.s_nssai.sst, recvmsg->param.s_nssai.sd.v);
        goto cleanup;
    }

    nsi->roaming_indication = recvmsg->param.roaming_indication;

    if (recvmsg->param.tai_presence) {
        nsi->tai_presence = true;
        memcpy(&nsi->tai, &recvmsg->param.tai, sizeof(nsi->tai));
    }

    memset(&NsiInformation, 0, sizeof(NsiInformation));

    if (recvmsg->param.home_plmn_id_presence &&
            recvmsg->param.home_snssai_presence) {
        nssf_home_t *home = nssf_home_find(
                &recvmsg->param.home_plmn_id, &recvmsg->param.home_snssai);
        if (!home) {
            home = nssf_home_add(
                    &recvmsg->param.home_plmn_id, &recvmsg->param.home_snssai);
            if (!home) {
                status = OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR;
                strerror = ogs_msprintf("Cannot allocate Home Network "
                        "by PLMN-ID(0x%x) S-NSSAI[SST:%d SD:0x%x]",
                        ogs_plmn_id_hexdump(&recvmsg->param.home_plmn_id),
                        recvmsg->param.home_snssai.sst,
                        recvmsg->param.home_snssai.sd.v);
                goto cleanup;
            }
        }

        if (!home->nrf_id || !home->nsi_id) {
            int r, i;
            nssf_nnssf_nsselection_param_t param;
            ogs_sbi_discovery_option_t *h_discovery_option = NULL;

            h_discovery_option = ogs_sbi_discovery_option_new();
            ogs_assert(h_discovery_option);

            ogs_sbi_discovery_option_add_snssais(
                    h_discovery_option, &home->s_nssai);
            ogs_sbi_discovery_option_add_target_plmn_list(
                    h_discovery_option, &home->plmn_id);
            ogs_assert(ogs_local_conf()->num_of_serving_plmn_id);
            for (i = 0; i < ogs_local_conf()->num_of_serving_plmn_id; i++) {
                ogs_sbi_discovery_option_add_requester_plmn_list(
                        h_discovery_option,
                        &ogs_local_conf()->serving_plmn_id[i]);
            }

            memset(&param, 0, sizeof(param));
            param.slice_info_for_pdu_session.presence = true;
            param.slice_info_for_pdu_session.snssai = &home->s_nssai;
            param.slice_info_for_pdu_session.roaming_indication =
                nsi->roaming_indication;

            if (nsi->tai_presence) param.tai = &nsi->tai;

            r = nssf_sbi_discover_and_send(
                    OGS_SBI_SERVICE_TYPE_NNSSF_NSSELECTION, h_discovery_option,
                    nssf_nnssf_nsselection_build_get,
                    home, stream, &param);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);

            return true;
        }

        /* Home Network */
        NsiInformation.nrf_id = home->nrf_id;
        NsiInformation.nsi_id = home->nsi_id;
    } else {
        /* Serving Network */
        NsiInformation.nrf_id = nsi->nrf_id;
        NsiInformation.nsi_id = nsi->nsi_id;
    }

    memset(&AuthorizedNetworkSliceInfo, 0, sizeof(AuthorizedNetworkSliceInfo));
    AuthorizedNetworkSliceInfo.nsi_information = &NsiInformation;

    memset(&sendmsg, 0, sizeof(sendmsg));
    sendmsg.AuthorizedNetworkSliceInfo = &AuthorizedNetworkSliceInfo;

    response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_OK);
    ogs_assert(response);
    ogs_assert(true == ogs_sbi_server_send_response(stream, response));

    return true;

cleanup:
    ogs_assert(strerror);

    ogs_error("%s", strerror);
    ogs_sbi_server_send_error(stream, status, recvmsg, strerror, NULL,
            NULL);
    ogs_free(strerror);

    return false;
}

bool nssf_nnrf_nsselection_handle_get_from_hnssf(
        nssf_home_t *home,
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    int status = OGS_SBI_HTTP_STATUS_OK;
    char *strerror = NULL;

    OpenAPI_authorized_network_slice_info_t *AuthorizedNetworkSliceInfo = NULL;
    OpenAPI_nsi_information_t *NsiInformation = NULL;

    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;

    ogs_assert(home);
    ogs_assert(stream);
    ogs_assert(recvmsg);

    if (recvmsg->res_status != OGS_SBI_HTTP_STATUS_OK) {
        status = recvmsg->res_status;
        strerror = ogs_msprintf("HTTP response error [%d]",
                recvmsg->res_status);
        goto cleanup;
    }

    AuthorizedNetworkSliceInfo = recvmsg->AuthorizedNetworkSliceInfo;
    if (!AuthorizedNetworkSliceInfo) {
        status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
        strerror = ogs_msprintf("No AuthorizedNetworkSliceInfo");
        goto cleanup;
    }

    NsiInformation = AuthorizedNetworkSliceInfo->nsi_information;
    if (!NsiInformation) {
        status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
        strerror = ogs_msprintf("No NsiInformation");
        goto cleanup;
    }

    if (!NsiInformation->nrf_id) {
        status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
        strerror = ogs_msprintf("No nrfId");
        goto cleanup;
    }

    if (!NsiInformation->nsi_id) {
        status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
        strerror = ogs_msprintf("No nsiId");
        goto cleanup;
    }

    home->nrf_id = ogs_strdup(NsiInformation->nrf_id);
    ogs_assert(home->nrf_id);
    home->nsi_id = ogs_strdup(NsiInformation->nsi_id);
    ogs_assert(home->nsi_id);

    memset(&sendmsg, 0, sizeof(sendmsg));
    sendmsg.AuthorizedNetworkSliceInfo = AuthorizedNetworkSliceInfo;

    response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_OK);
    ogs_assert(response);
    ogs_assert(true == ogs_sbi_server_send_response(stream, response));

    return true;

cleanup:
    ogs_assert(strerror);

    ogs_error("%s", strerror);
    ogs_sbi_server_send_error(stream, status, recvmsg, strerror, NULL,
            NULL);
    ogs_free(strerror);

    return false;
}
