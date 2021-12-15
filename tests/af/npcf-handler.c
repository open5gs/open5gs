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
#include "npcf-handler.h"

void af_npcf_policyauthorization_handle_create(
        af_sess_t *sess, ogs_sbi_message_t *recvmsg)
{
    int rv;

    ogs_sbi_message_t message;
    ogs_sbi_header_t header;

    OpenAPI_app_session_context_t *AppSessionContext = NULL;
    OpenAPI_app_session_context_req_data_t *AscReqData = NULL;

    uint64_t supported_features = 0;

    if (!recvmsg->http.location) {
        ogs_error("[%s:%s] No http.location",
                sess->ipv4addr ? sess->ipv4addr : "Unknown",
                sess->ipv6addr ? sess->ipv6addr : "Unknown");
        return;
    }

    memset(&header, 0, sizeof(header));
    header.uri = recvmsg->http.location;

    rv = ogs_sbi_parse_header(&message, &header);
    if (rv != OGS_OK) {
        ogs_error("[%s:%s] Cannot parse http.location [%s]",
                sess->ipv4addr ? sess->ipv4addr : "Unknown",
                sess->ipv6addr ? sess->ipv6addr : "Unknown",
                recvmsg->http.location);
        return;
    }

    if (!message.h.resource.component[1]) {
        ogs_error("[%s:%s] No AppSessionId[%s]",
                sess->ipv4addr ? sess->ipv4addr : "Unknown",
                sess->ipv6addr ? sess->ipv6addr : "Unknown",
                recvmsg->http.location);
        goto cleanup;
    }

    AppSessionContext = recvmsg->AppSessionContext;
    if (!AppSessionContext) {
        ogs_error("[%s:%s] No AppSessionContext",
                sess->ipv4addr ? sess->ipv4addr : "Unknown",
                sess->ipv6addr ? sess->ipv6addr : "Unknown");
        goto cleanup;
    }

    AscReqData = AppSessionContext->asc_req_data;
    if (!AscReqData) {
        ogs_error("[%s:%s] No AscReqData",
                sess->ipv4addr ? sess->ipv4addr : "Unknown",
                sess->ipv6addr ? sess->ipv6addr : "Unknown");
        goto cleanup;
    }

    if (!AscReqData->supp_feat) {
        ogs_error("[%s:%s] No AscReqData->suppFeat",
                sess->ipv4addr ? sess->ipv4addr : "Unknown",
                sess->ipv6addr ? sess->ipv6addr : "Unknown");
        goto cleanup;
    }

    supported_features = ogs_uint64_from_string(AscReqData->supp_feat);
    sess->policyauthorization_features &= supported_features;

    af_sess_set_pcf_app_session_id(sess, message.h.resource.component[1]);

cleanup:
    ogs_sbi_header_free(&header);
}

void af_npcf_policyauthorization_handle_update(
        af_sess_t *sess, ogs_sbi_message_t *recvmsg)
{
    OpenAPI_app_session_context_update_data_patch_t
        *AppSessionContextUpdateDataPatch = NULL;
    OpenAPI_app_session_context_update_data_t *AscUpdateData = NULL;

    ogs_assert(sess);
    ogs_assert(recvmsg);

    if (!recvmsg->h.resource.component[1]) {
        ogs_error("[%s:%s] No AppSessionId[%s]",
                sess->ipv4addr ? sess->ipv4addr : "Unknown",
                sess->ipv6addr ? sess->ipv6addr : "Unknown",
                recvmsg->http.location);
        return;
    }

    AppSessionContextUpdateDataPatch =
        recvmsg->AppSessionContextUpdateDataPatch;
    if (!AppSessionContextUpdateDataPatch) {
        ogs_error("[%s:%s] No AppSessionContextUpdateDataPatch",
                sess->ipv4addr ? sess->ipv4addr : "Unknown",
                sess->ipv6addr ? sess->ipv6addr : "Unknown");
        return;
    }

    AscUpdateData = AppSessionContextUpdateDataPatch->asc_req_data;
    if (!AscUpdateData) {
        ogs_error("[%s:%s] No AscUpdateData",
                sess->ipv4addr ? sess->ipv4addr : "Unknown",
                sess->ipv6addr ? sess->ipv6addr : "Unknown");
        return;
    }
}
