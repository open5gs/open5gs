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

#include "naf-build.h"

ogs_sbi_request_t *pcf_naf_callback_build_policyauthorization_terminate(
        pcf_app_t *app_session, void *data)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_server_t *server = NULL;
    ogs_sbi_header_t header;
    OpenAPI_termination_info_t TerminationInfo;

    ogs_assert(app_session);

    memset(&message, 0, sizeof(message));
    memset(&TerminationInfo, 0, sizeof(TerminationInfo));

    server = ogs_sbi_server_first();
    if (!server) {
        ogs_error("No server");
        goto end;
    }

    memset(&header, 0, sizeof(header));
    header.service.name =
        OpenAPI_service_name_ToString(
                OpenAPI_service_name_npcf_policyauthorization);
    header.api.version = (char *)OGS_SBI_API_V1;
    header.resource.component[0] = (char *)OGS_SBI_RESOURCE_NAME_APP_SESSIONS;
    header.resource.component[1] = app_session->app_session_id;

    TerminationInfo.res_uri = ogs_sbi_server_uri(server, &header);
    if (!TerminationInfo.res_uri) {
        ogs_error("No TerminationInfo.res_uri");
        goto end;
    }
    TerminationInfo.term_cause = data ?
        *(OpenAPI_termination_cause_e *)data :
        OpenAPI_termination_cause_PDU_SESSION_TERMINATION;

    message.h.method = (char *)OGS_SBI_HTTP_METHOD_POST;
    message.h.uri = ogs_msprintf("%s/%s",
                app_session->notif_uri, OGS_SBI_RESOURCE_NAME_TERMINATE);
    if (!message.h.uri) {
        ogs_error("No message.h.uri");
        goto end;
    }

    message.TerminationInfo = &TerminationInfo;

    request = ogs_sbi_build_request(&message);
    ogs_assert(request);

end:
    if (TerminationInfo.res_uri)
        ogs_free(TerminationInfo.res_uri);
    if (message.h.uri)
        ogs_free(message.h.uri);

    return request;
}
