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

#include "nudr-handler.h"

bool pcf_nudr_dr_handle_query_am_data(
    pcf_ue_t *pcf_ue, ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    int rv, status = 0;
    char *strerror = NULL;
    ogs_sbi_server_t *server = NULL;

    ogs_sbi_message_t sendmsg;
    ogs_sbi_header_t header;
    ogs_sbi_response_t *response = NULL;

    ogs_subscription_data_t subscription_data;

    ogs_assert(pcf_ue);
    ogs_assert(stream);
    server = ogs_sbi_server_from_stream(stream);
    ogs_assert(server);

    ogs_assert(recvmsg);

    memset(&subscription_data, 0, sizeof(ogs_subscription_data_t));

    SWITCH(recvmsg->h.resource.component[3])
    CASE(OGS_SBI_RESOURCE_NAME_AM_DATA)
        OpenAPI_policy_association_t PolicyAssociation;
        OpenAPI_ambr_t UeAmbr;
        OpenAPI_list_t *TriggerList = NULL;

        if (!recvmsg->AmPolicyData) {
            strerror = ogs_msprintf("[%s] No AmPolicyData", pcf_ue->supi);
            status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
            goto cleanup;
        }

        if (!pcf_ue->policy_association_request) {
            strerror = ogs_msprintf("[%s] No PolicyAssociationRequest",
                    pcf_ue->supi);
            status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
            goto cleanup;
        }

        rv = ogs_dbi_subscription_data(pcf_ue->supi, &subscription_data);
        if (rv != OGS_OK) {
            strerror = ogs_msprintf("[%s] Cannot find SUPI in DB",
                    pcf_ue->supi);
            status = OGS_SBI_HTTP_STATUS_NOT_FOUND;
            goto cleanup;
        }

        if (!subscription_data.ambr.uplink &&
                !subscription_data.ambr.downlink) {
            ogs_error("[%s] No UE-AMBR", pcf_ue->supi);
            status = OGS_SBI_HTTP_STATUS_NOT_FOUND;
            goto cleanup;
        }

        memset(&PolicyAssociation, 0, sizeof(PolicyAssociation));
        PolicyAssociation.request = pcf_ue->policy_association_request;
        PolicyAssociation.supp_feat =
            ogs_uint64_to_string(pcf_ue->am_policy_control_features);
        ogs_assert(PolicyAssociation.supp_feat);

        TriggerList = OpenAPI_list_create();
        ogs_assert(TriggerList);

        memset(&UeAmbr, 0, sizeof(UeAmbr));
        if (OGS_SBI_FEATURES_IS_SET(pcf_ue->am_policy_control_features,
                    OGS_SBI_NPCF_AM_POLICY_CONTROL_UE_AMBR_AUTHORIZATION)) {
            if (pcf_ue->subscribed_ue_ambr) {
                ogs_bitrate_t subscribed_ue_ambr;

                subscribed_ue_ambr.uplink = ogs_sbi_bitrate_from_string(
                        pcf_ue->subscribed_ue_ambr->uplink);
                subscribed_ue_ambr.downlink = ogs_sbi_bitrate_from_string(
                        pcf_ue->subscribed_ue_ambr->downlink);

                if (((subscribed_ue_ambr.uplink / 1024) !=
                     (subscription_data.ambr.uplink / 1024)) ||
                    ((subscribed_ue_ambr.downlink / 1024) !=
                     (subscription_data.ambr.downlink / 1024))) {

                    OpenAPI_list_add(TriggerList,
                            (void *)OpenAPI_request_trigger_UE_AMBR_CH);
                }

                UeAmbr.uplink = ogs_sbi_bitrate_to_string(
                        subscription_data.ambr.uplink, OGS_SBI_BITRATE_KBPS);
                UeAmbr.downlink = ogs_sbi_bitrate_to_string(
                        subscription_data.ambr.downlink, OGS_SBI_BITRATE_KBPS);
                PolicyAssociation.ue_ambr = &UeAmbr;
            }
        }

        if (TriggerList->count)
            PolicyAssociation.triggers = TriggerList;

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
        ogs_assert(true == ogs_sbi_server_send_response(stream, response));

        ogs_free(sendmsg.http.location);

        ogs_free(PolicyAssociation.supp_feat);

        OpenAPI_list_free(TriggerList);

        if (UeAmbr.uplink)
            ogs_free(UeAmbr.uplink);
        if (UeAmbr.downlink)
            ogs_free(UeAmbr.downlink);

        ogs_subscription_data_free(&subscription_data);

        return true;

    DEFAULT
        strerror = ogs_msprintf("[%s] Invalid resource name [%s]", 
                        pcf_ue->supi, recvmsg->h.resource.component[3]);
    END

cleanup:
    ogs_assert(strerror);
    ogs_assert(status);
    ogs_error("%s", strerror);
    ogs_assert(true ==
        ogs_sbi_server_send_error(stream, status, recvmsg, strerror, NULL));
    ogs_free(strerror);

    ogs_subscription_data_free(&subscription_data);

    return false;
}

bool pcf_nudr_dr_handle_query_sm_data(
    pcf_sess_t *sess, ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    int status = 0;
    char *strerror = NULL;
    pcf_ue_t *pcf_ue = NULL;
    ogs_sbi_server_t *server = NULL;

    ogs_assert(sess);
    pcf_ue = sess->pcf_ue;
    ogs_assert(pcf_ue);
    ogs_assert(stream);
    server = ogs_sbi_server_from_stream(stream);
    ogs_assert(server);

    ogs_assert(recvmsg);

    SWITCH(recvmsg->h.resource.component[3])
    CASE(OGS_SBI_RESOURCE_NAME_SM_DATA)
        if (!recvmsg->SmPolicyData) {
            strerror = ogs_msprintf("[%s:%d] No SmPolicyData",
                    pcf_ue->supi, sess->psi);
            status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
            goto cleanup;
        }

        ogs_assert(true ==
            pcf_sess_sbi_discover_and_send(
                OpenAPI_nf_type_BSF, sess, stream, NULL,
                pcf_nbsf_management_build_register));

        return true;

    DEFAULT
        strerror = ogs_msprintf("[%s:%d] Invalid resource name [%s]", 
                    pcf_ue->supi, sess->psi, recvmsg->h.resource.component[3]);
    END

cleanup:
    ogs_assert(strerror);
    ogs_assert(status);
    ogs_error("%s", strerror);
    ogs_assert(true ==
        ogs_sbi_server_send_error(stream, status, recvmsg, strerror, NULL));
    ogs_free(strerror);

    return false;
}
