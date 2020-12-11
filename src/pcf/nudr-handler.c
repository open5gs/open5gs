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

#include "nudr-handler.h"

bool pcf_nudr_dr_handle_query_am_data(
    pcf_ue_t *pcf_ue, ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    char *strerror = NULL;
    ogs_sbi_server_t *server = NULL;

    ogs_sbi_message_t sendmsg;
    ogs_sbi_header_t header;
    ogs_sbi_response_t *response = NULL;

    ogs_assert(pcf_ue);
    ogs_assert(stream);
    server = ogs_sbi_server_from_stream(stream);
    ogs_assert(server);

    ogs_assert(recvmsg);

    SWITCH(recvmsg->h.resource.component[3])
    CASE(OGS_SBI_RESOURCE_NAME_AM_DATA)
        OpenAPI_policy_association_t PolicyAssociation;

        if (!recvmsg->AmPolicyData) {
            strerror = ogs_msprintf("[%s] No AmPolicyData", pcf_ue->supi);
            goto cleanup;
        }

        if (!pcf_ue->policy_association_request) {
            strerror = ogs_msprintf("[%s] No PolicyAssociationRequest",
                    pcf_ue->supi);
            goto cleanup;
        }

        memset(&PolicyAssociation, 0, sizeof(PolicyAssociation));
        PolicyAssociation.request = pcf_ue->policy_association_request;
        PolicyAssociation.supp_feat = (char *)"";

        memset(&header, 0, sizeof(header));
        header.service.name =
            (char *)OGS_SBI_SERVICE_NAME_NPCF_AM_POLICY_CONTROL;
        header.api.version = (char *)OGS_SBI_API_V1;
        header.resource.component[0] =
            (char *)OGS_SBI_RESOURCE_NAME_POLICIES;
        header.resource.component[1] = pcf_ue->association_id;

        memset(&sendmsg, 0, sizeof(sendmsg));
        sendmsg.PolicyAssociation = &PolicyAssociation;
        sendmsg.http.location = ogs_sbi_server_uri(server, &header);

        response = ogs_sbi_build_response(
                &sendmsg, OGS_SBI_HTTP_STATUS_CREATED);
        ogs_assert(response);
        ogs_sbi_server_send_response(stream, response);

        ogs_free(sendmsg.http.location);

        return true;

    DEFAULT
        strerror = ogs_msprintf("[%s] Invalid resource name [%s]", 
                        pcf_ue->supi, recvmsg->h.resource.component[3]);
    END

cleanup:
    ogs_assert(strerror);
    ogs_error("%s", strerror);
    ogs_sbi_server_send_error(
            stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST, recvmsg, strerror, NULL);
    ogs_free(strerror);

    return false;
}

bool pcf_nudr_dr_handle_query_sm_data(
    pcf_sess_t *sess, ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    char *strerror = NULL;
    pcf_ue_t *pcf_ue = NULL;
    ogs_sbi_server_t *server = NULL;

    ogs_sbi_message_t sendmsg;
    ogs_sbi_header_t header;
    ogs_sbi_response_t *response = NULL;

    ogs_assert(sess);
    pcf_ue = sess->pcf_ue;
    ogs_assert(pcf_ue);
    ogs_assert(stream);
    server = ogs_sbi_server_from_stream(stream);
    ogs_assert(server);

    ogs_assert(recvmsg);

    SWITCH(recvmsg->h.resource.component[3])
    CASE(OGS_SBI_RESOURCE_NAME_SM_DATA)
        OpenAPI_sm_policy_decision_t SmPolicyDecision;

        if (!recvmsg->SmPolicyData) {
            strerror = ogs_msprintf("[%s:%d] No SmPolicyData",
                    pcf_ue->supi, sess->psi);
            goto cleanup;
        }

        memset(&SmPolicyDecision, 0, sizeof(SmPolicyDecision));

        memset(&header, 0, sizeof(header));
        header.service.name = (char *)OGS_SBI_SERVICE_NAME_NPCF_SMPOLICYCONTROL;
        header.api.version = (char *)OGS_SBI_API_V1;
        header.resource.component[0] = (char *)OGS_SBI_RESOURCE_NAME_POLICIES;
        header.resource.component[1] = sess->sm_policy_id;

        memset(&sendmsg, 0, sizeof(sendmsg));
        sendmsg.SmPolicyDecision = &SmPolicyDecision;
        sendmsg.http.location = ogs_sbi_server_uri(server, &header);

        response = ogs_sbi_build_response(
                &sendmsg, OGS_SBI_HTTP_STATUS_CREATED);
        ogs_assert(response);
        ogs_sbi_server_send_response(stream, response);

        ogs_free(sendmsg.http.location);

        return true;

    DEFAULT
        strerror = ogs_msprintf("[%s:%d] Invalid resource name [%s]", 
                    pcf_ue->supi, sess->psi, recvmsg->h.resource.component[3]);
    END

cleanup:
    ogs_assert(strerror);
    ogs_error("%s", strerror);
    ogs_sbi_server_send_error(
            stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST, recvmsg, strerror, NULL);
    ogs_free(strerror);

    return false;
}
