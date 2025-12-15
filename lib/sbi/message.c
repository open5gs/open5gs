/*
 * Copyright (C) 2019-2023 by Sukchan Lee <acetcom@gmail.com>
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

#include "ogs-sbi.h"
#include "yuarel.h"

#include "contrib/multipart_parser.h"

static OGS_POOL(request_pool, ogs_sbi_request_t);
static OGS_POOL(response_pool, ogs_sbi_response_t);

static char *build_json(ogs_sbi_message_t *message);
static int parse_json(ogs_sbi_message_t *message,
        char *content_type, char *json);

static bool build_content(
        ogs_sbi_http_message_t *http, ogs_sbi_message_t *message);
static int parse_content(
        ogs_sbi_message_t *message, ogs_sbi_http_message_t *http);

static bool build_multipart(
        ogs_sbi_http_message_t *http, ogs_sbi_message_t *message);
static int parse_multipart(
        ogs_sbi_message_t *sbi_message, ogs_sbi_http_message_t *http);

static void http_message_free(ogs_sbi_http_message_t *http);

void ogs_sbi_message_init(int num_of_request_pool, int num_of_response_pool)
{
    ogs_pool_init(&request_pool, num_of_request_pool);
    ogs_pool_init(&response_pool, num_of_response_pool);
}

void ogs_sbi_message_final(void)
{
    ogs_pool_final(&request_pool);
    ogs_pool_final(&response_pool);
}

void ogs_sbi_message_free(ogs_sbi_message_t *message)
{
    int i;

    ogs_assert(message);

    /* Discovery Option */
    if (message->param.discovery_option)
        ogs_sbi_discovery_option_free(message->param.discovery_option);

    /* Query parameters */
    for (i = 0; i < message->param.num_of_fields; i++)
        ogs_free(message->param.fields[i]);

    /* JSON Data */
    if (message->NFProfile)
        OpenAPI_nf_profile_free(message->NFProfile);
    if (message->ProblemDetails)
        OpenAPI_problem_details_free(message->ProblemDetails);
    if (message->PatchItemList) {
        OpenAPI_lnode_t *node = NULL;
        OpenAPI_list_for_each(message->PatchItemList, node)
            OpenAPI_patch_item_free(node->data);
        OpenAPI_list_free(message->PatchItemList);
    }

    if (message->SubscriptionData)
        OpenAPI_subscription_data_free(message->SubscriptionData);
    if (message->NotificationData)
        OpenAPI_notification_data_free(message->NotificationData);
    if (message->SearchResult)
        OpenAPI_search_result_free(message->SearchResult);
    if (message->AuthenticationInfo)
        OpenAPI_authentication_info_free(message->AuthenticationInfo);
    if (message->AuthenticationInfoRequest)
        OpenAPI_authentication_info_request_free(
                message->AuthenticationInfoRequest);
    if (message->AuthenticationInfoResult)
        OpenAPI_authentication_info_result_free(
                message->AuthenticationInfoResult);
    if (message->AuthenticationSubscription)
        OpenAPI_authentication_subscription_free(
                message->AuthenticationSubscription);
    if (message->UeAuthenticationCtx)
        OpenAPI_ue_authentication_ctx_free(message->UeAuthenticationCtx);
    if (message->ConfirmationData)
        OpenAPI_confirmation_data_free(message->ConfirmationData);
    if (message->ConfirmationDataResponse)
        OpenAPI_confirmation_data_response_free(
                message->ConfirmationDataResponse);
    if (message->AuthEvent)
        OpenAPI_auth_event_free(message->AuthEvent);
    if (message->Amf3GppAccessRegistration)
        OpenAPI_amf3_gpp_access_registration_free(
                message->Amf3GppAccessRegistration);
    if (message->Amf3GppAccessRegistrationModification)
        OpenAPI_amf3_gpp_access_registration_modification_free(
                message->Amf3GppAccessRegistrationModification);
    if (message->SmfRegistration)
        OpenAPI_smf_registration_free(message->SmfRegistration);
    if (message->Nssai)
        OpenAPI_nssai_free(message->Nssai);
    if (message->AccessAndMobilitySubscriptionData)
        OpenAPI_access_and_mobility_subscription_data_free(
                message->AccessAndMobilitySubscriptionData);
    if (message->SmfSelectionSubscriptionData)
        OpenAPI_smf_selection_subscription_data_free(
                message->SmfSelectionSubscriptionData);
    if (message->UeContextInSmfData)
        OpenAPI_ue_context_in_smf_data_free(message->UeContextInSmfData);
    if (message->SmContextCreateData)
        OpenAPI_sm_context_create_data_free(message->SmContextCreateData);
    if (message->SmContextCreatedData)
        OpenAPI_sm_context_created_data_free(message->SmContextCreatedData);
    if (message->SmContextCreateError)
        OpenAPI_sm_context_create_error_free(message->SmContextCreateError);
    if (message->SmContextUpdateData)
        OpenAPI_sm_context_update_data_free(message->SmContextUpdateData);
    if (message->SmContextUpdatedData)
        OpenAPI_sm_context_updated_data_free(message->SmContextUpdatedData);
    if (message->SmContextUpdateError)
        OpenAPI_sm_context_update_error_free(message->SmContextUpdateError);
    if (message->SmContextReleaseData)
        OpenAPI_sm_context_release_data_free(message->SmContextReleaseData);
    if (message->SmContextReleasedData)
        OpenAPI_sm_context_released_data_free(message->SmContextReleasedData);
    if (message->SessionManagementSubscriptionDataList) {
        OpenAPI_lnode_t *node = NULL;
        OpenAPI_list_for_each(message->SessionManagementSubscriptionDataList, node)
            OpenAPI_session_management_subscription_data_free(node->data);
        OpenAPI_list_free(message->SessionManagementSubscriptionDataList);
    }
    if (message->N1N2MessageTransferReqData)
        OpenAPI_n1_n2_message_transfer_req_data_free(
                message->N1N2MessageTransferReqData);
    if (message->N1N2MessageTransferRspData)
        OpenAPI_n1_n2_message_transfer_rsp_data_free(
                message->N1N2MessageTransferRspData);
    if (message->N1N2MsgTxfrFailureNotification)
        OpenAPI_n1_n2_msg_txfr_failure_notification_free(
                message->N1N2MsgTxfrFailureNotification);
    if (message->SmContextStatusNotification)
        OpenAPI_sm_context_status_notification_free(
                message->SmContextStatusNotification);
    if (message->PolicyAssociationRequest)
        OpenAPI_policy_association_request_free(
                message->PolicyAssociationRequest);
    if (message->PolicyAssociation)
        OpenAPI_policy_association_free(message->PolicyAssociation);
    if (message->AmPolicyData)
        OpenAPI_am_policy_data_free(message->AmPolicyData);
    if (message->SmPolicyContextData)
        OpenAPI_sm_policy_context_data_free(message->SmPolicyContextData);
    if (message->SmPolicyDecision)
        OpenAPI_sm_policy_decision_free(message->SmPolicyDecision);
    if (message->SmPolicyData)
        OpenAPI_sm_policy_data_free(message->SmPolicyData);
    if (message->SmPolicyDeleteData)
        OpenAPI_sm_policy_delete_data_free(message->SmPolicyDeleteData);
    if (message->AuthorizedNetworkSliceInfo)
        OpenAPI_authorized_network_slice_info_free(
                message->AuthorizedNetworkSliceInfo);
    if (message->PcfBinding)
        OpenAPI_pcf_binding_free(message->PcfBinding);
    if (message->AppSessionContext)
        OpenAPI_app_session_context_free(message->AppSessionContext);
    if (message->AppSessionContextUpdateDataPatch)
        OpenAPI_app_session_context_update_data_patch_free(message->AppSessionContextUpdateDataPatch);
    if (message->SmPolicyNotification)
        OpenAPI_sm_policy_notification_free(message->SmPolicyNotification);
    if (message->TerminationNotification)
        OpenAPI_termination_notification_free(message->TerminationNotification);
    if (message->DeregistrationData)
        OpenAPI_deregistration_data_free(message->DeregistrationData);
    if (message->SDMSubscription)
        OpenAPI_sdm_subscription_free(message->SDMSubscription);
    if (message->ModificationNotification)
        OpenAPI_modification_notification_free(message->ModificationNotification);
    if (message->SecNegotiateReqData)
        OpenAPI_sec_negotiate_req_data_free(message->SecNegotiateReqData);
    if (message->SecNegotiateRspData)
        OpenAPI_sec_negotiate_rsp_data_free(message->SecNegotiateRspData);
    if (message->UeContextTransferReqData)
        OpenAPI_ue_context_transfer_req_data_free(message->UeContextTransferReqData);
    if (message->UeContextTransferRspData)
        OpenAPI_ue_context_transfer_rsp_data_free(message->UeContextTransferRspData);
    if (message->UeRegStatusUpdateReqData)
        OpenAPI_ue_reg_status_update_req_data_free(message->UeRegStatusUpdateReqData);
    if (message->UeRegStatusUpdateRspData)
        OpenAPI_ue_reg_status_update_rsp_data_free(message->UeRegStatusUpdateRspData);
    if (message->N2InformationTransferReqData)
        OpenAPI_n2_information_transfer_req_data_free(
                message->N2InformationTransferReqData);
    if (message->links) {
        OpenAPI_clear_and_free_string_list(message->links->items);
        if (message->links->self)
            ogs_free(message->links->self);

        ogs_free(message->links);
    }

    /* HTTP Part */
    for (i = 0; i < message->num_of_part; i++) {
        if (message->part[i].pkbuf)
            ogs_pkbuf_free(message->part[i].pkbuf);
    }
}

ogs_sbi_request_t *ogs_sbi_request_new(void)
{
    ogs_sbi_request_t *request = NULL;

    ogs_pool_alloc(&request_pool, &request);
    if (!request) {
        ogs_error("ogs_pool_alloc() failed");
        return NULL;
    }
    memset(request, 0, sizeof(ogs_sbi_request_t));

    request->http.params = ogs_hash_make();
    if (!request->http.params) {
        ogs_error("ogs_hash_make() failed");
        ogs_sbi_request_free(request);
        return NULL;
    }
    request->http.headers = ogs_hash_make();
    if (!request->http.headers) {
        ogs_error("ogs_hash_make() failed");
        ogs_sbi_request_free(request);
        return NULL;
    }

    return request;
}

ogs_sbi_response_t *ogs_sbi_response_new(void)
{
    ogs_sbi_response_t *response = NULL;

    ogs_pool_alloc(&response_pool, &response);
    if (!response) {
        ogs_error("ogs_pool_alloc() failed");
        return NULL;
    }
    memset(response, 0, sizeof(ogs_sbi_response_t));

    response->http.params = ogs_hash_make();
    if (!response->http.params) {
        ogs_error("ogs_hash_make() failed");
        ogs_sbi_response_free(response);
        return NULL;
    }
    response->http.headers = ogs_hash_make();
    if (!response->http.headers) {
        ogs_error("ogs_hash_make() failed");
        ogs_sbi_response_free(response);
        return NULL;
    }

    return response;
}

void ogs_sbi_request_free(ogs_sbi_request_t *request)
{
    ogs_assert(request);

    if (request->h.uri)
        ogs_free(request->h.uri);

    ogs_sbi_header_free(&request->h);
    http_message_free(&request->http);

    ogs_pool_free(&request_pool, request);
}

void ogs_sbi_response_free(ogs_sbi_response_t *response)
{
    ogs_assert(response);

    if (response->h.uri)
        ogs_free(response->h.uri);

    ogs_sbi_header_free(&response->h);
    http_message_free(&response->http);

    ogs_pool_free(&response_pool, response);
}

ogs_sbi_request_t *ogs_sbi_build_request(ogs_sbi_message_t *message)
{
    int i;
    ogs_sbi_request_t *request = NULL;
    OpenAPI_nf_type_e nf_type = OpenAPI_nf_type_NULL;

    char sender_timestamp[OGS_SBI_RFC7231_DATE_LEN];
    char *max_rsp_time = NULL;

    ogs_assert(message);

    request = ogs_sbi_request_new();
    if (!request) {
        ogs_error("ogs_pool_alloc() failed");
        return NULL;
    }

    if (!message->h.method) {
        ogs_error("No Method");
        ogs_sbi_request_free(request);
        return NULL;
    }
    request->h.method = ogs_strdup(message->h.method);
    if (!request->h.method) {
        ogs_error("ogs_strdup() failed");
        ogs_sbi_request_free(request);
        return NULL;
    }
    if (message->h.uri) {
        request->h.uri = ogs_strdup(message->h.uri);
        if (!request->h.uri) {
            ogs_error("ogs_strdup() failed");
            ogs_sbi_request_free(request);
            return NULL;
        }
    } else {
        if (!message->h.service.name) {
            ogs_error("No Service Name");
            ogs_sbi_request_free(request);
            return NULL;
        }
        request->h.service.name = ogs_strdup(message->h.service.name);
        if (!request->h.service.name) {
            ogs_error("ogs_strdup() failed");
            ogs_sbi_request_free(request);
            return NULL;
        }
        if (!message->h.api.version) {
            ogs_error("No API Version");
            ogs_sbi_request_free(request);
            return NULL;
        }
        request->h.api.version = ogs_strdup(message->h.api.version);
        if (!request->h.api.version) {
            ogs_error("ogs_strdup() failed");
            ogs_sbi_request_free(request);
            return NULL;
        }

        if (!message->h.resource.component[0]) {
            ogs_error("No Resource");
            ogs_sbi_request_free(request);
            return NULL;
        }
        for (i = 0; i < OGS_SBI_MAX_NUM_OF_RESOURCE_COMPONENT &&
                            message->h.resource.component[i]; i++) {
            request->h.resource.component[i] = ogs_strdup(
                    message->h.resource.component[i]);
            if (!request->h.resource.component[i]) {
                ogs_error("ogs_strdup() failed");
                ogs_sbi_request_free(request);
                return NULL;
            }
        }
    }

    /* Discovery Parameter */
    if (message->param.target_nf_type) {
        char *v = OpenAPI_nf_type_ToString(message->param.target_nf_type);
        if (!v) {
            ogs_error("OpenAPI_nf_type_ToString() failed");
            ogs_sbi_request_free(request);
            return NULL;
        }
        ogs_sbi_header_set(request->http.params,
                OGS_SBI_PARAM_TARGET_NF_TYPE, v);
    }
    if (message->param.requester_nf_type) {
        char *v = OpenAPI_nf_type_ToString(message->param.requester_nf_type);
        if (!v) {
            ogs_error("OpenAPI_nf_type_ToString() failed");
            ogs_sbi_request_free(request);
            return NULL;
        }
        ogs_sbi_header_set(request->http.params,
                OGS_SBI_PARAM_REQUESTER_NF_TYPE, v);
    }

    /* Discovery Option Parameter */
    if (message->param.discovery_option) {
        ogs_sbi_discovery_option_t *discovery_option =
                message->param.discovery_option;

        if (discovery_option->target_nf_instance_id) {
            ogs_sbi_header_set(request->http.params,
                    OGS_SBI_PARAM_TARGET_NF_INSTANCE_ID,
                    discovery_option->target_nf_instance_id);
        }
        if (discovery_option->requester_nf_instance_id) {
            ogs_sbi_header_set(request->http.params,
                    OGS_SBI_PARAM_REQUESTER_NF_INSTANCE_ID,
                    discovery_option->requester_nf_instance_id);
        }
        if (discovery_option->guami_presence) {
            char *v = ogs_sbi_discovery_option_build_guami(discovery_option);
            if (v) {
                ogs_sbi_header_set(request->http.params,
                        OGS_SBI_PARAM_GUAMI, v);
                ogs_free(v);
            } else {
                ogs_error("build failed: guami[PLMN_ID:%06x,AMF_ID:%d]",
                            ogs_plmn_id_hexdump(
                                &discovery_option->guami.plmn_id),
                            ogs_amf_id_hexdump(
                                &discovery_option->guami.amf_id));
            }
        }
        if (discovery_option->num_of_service_names) {

    /*
     * Issues #1730
     * Send NF discovery query with service-names delimited with comma
     *
     * OpenAPI specification for sending NF discovery query with
     * "service-names" parameter is defined as folowing:
     *   name: service-names
     *   ...
     *   style: form
     *   explode: false
     *
     * According to OpenAPI specification, this means array items
     * should be delimited with a comma character (example: /users?id=3,4,5).
     *
     * See also https://swagger.io/docs/specification/serialization/
     */

            /* send array items separated by a comma */
            char *v = ogs_sbi_discovery_option_build_service_names(
                        discovery_option);
            if (v) {
                ogs_sbi_header_set(request->http.params,
                        OGS_SBI_PARAM_SERVICE_NAMES, v);
                ogs_free(v);
            } else {
                ogs_warn("build failed: service-names[%d:%s]",
                            discovery_option->num_of_service_names,
                            discovery_option->service_names[0]);
            }
        }
        if (discovery_option->num_of_snssais) {
            char *v = ogs_sbi_discovery_option_build_snssais(discovery_option);
            if (v) {
            /*
             * In http.params, the CURL library automatically encodes the URL.
             * http.headers implements open5gs to directly encode URLs.
             *
             * Since it is http.params, the result is sent as is.
             */
                ogs_sbi_header_set(request->http.params,
                        OGS_SBI_PARAM_SNSSAIS, v);
                ogs_free(v);
            } else {
                ogs_error("build failed: snssais(%d)[SST:%d SD:0x%x]",
                            discovery_option->num_of_snssais,
                            discovery_option->snssais[0].sst,
                            discovery_option->snssais[0].sd.v);
            }
        }
        if (discovery_option->dnn) {
            ogs_sbi_header_set(request->http.params,
                    OGS_SBI_PARAM_DNN, discovery_option->dnn);
        }
        if (discovery_option->tai_presence) {
            char *v = ogs_sbi_discovery_option_build_tai(discovery_option);
            if (v) {
                ogs_sbi_header_set(request->http.params, OGS_SBI_PARAM_TAI, v);
                ogs_free(v);
            } else {
                ogs_error("build failed: tai[PLMN_ID:%06x,TAC:%d]",
                            ogs_plmn_id_hexdump(
                                &discovery_option->tai.plmn_id),
                            discovery_option->tai.tac.v);
            }
        }

        if (discovery_option->num_of_target_plmn_list) {
            char *v = ogs_sbi_discovery_option_build_plmn_list(
                    discovery_option->target_plmn_list,
                    discovery_option->num_of_target_plmn_list);
            if (v) {
                ogs_sbi_header_set(request->http.params,
                        OGS_SBI_PARAM_TARGET_PLMN_LIST, v);
                ogs_free(v);
            } else {
                ogs_warn("invalid target-plmn-list failed[%d:%06x]",
                            discovery_option->num_of_target_plmn_list,
                            ogs_plmn_id_hexdump(
                                &discovery_option->target_plmn_list[0]));
            }
        }
        if (discovery_option->num_of_requester_plmn_list) {
            char *v = ogs_sbi_discovery_option_build_plmn_list(
                    discovery_option->requester_plmn_list,
                    discovery_option->num_of_requester_plmn_list);
            if (v) {
                ogs_sbi_header_set(request->http.params,
                        OGS_SBI_PARAM_REQUESTER_PLMN_LIST, v);
                ogs_free(v);
            } else {
                ogs_warn("invalid target-plmn-list failed[%d:%06x]",
                            discovery_option->num_of_requester_plmn_list,
                            ogs_plmn_id_hexdump(
                                &discovery_option->requester_plmn_list[0]));
            }
        }
        if (discovery_option->requester_features) {
            char *v = ogs_uint64_to_string(
                    discovery_option->requester_features);
            if (!v) {
                ogs_error("ogs_uint64_to_string[0x%llx] failed",
                        (long long)discovery_option->requester_features);
                ogs_sbi_request_free(request);
                return false;
            }

            ogs_sbi_header_set(request->http.params,
                    OGS_SBI_PARAM_REQUESTER_FEATURES, v);
            ogs_free(v);
        }
    }

    /* URL Query Paramemter */
    if (message->param.nf_id) {
        ogs_sbi_header_set(request->http.params,
                OGS_SBI_PARAM_NF_ID, message->param.nf_id);
    }
    if (message->param.nf_type) {
        char *v = OpenAPI_nf_type_ToString(message->param.nf_type);
        if (!v) {
            ogs_error("OpenAPI_nf_type_ToString() failed");
            ogs_sbi_request_free(request);
            return NULL;
        }
        ogs_sbi_header_set(request->http.params, OGS_SBI_PARAM_NF_TYPE, v);
    }
    if (message->param.limit) {
        char *v = ogs_msprintf("%d", message->param.limit);
        if (!v) {
            ogs_error("OpenAPI_nf_type_ToString() failed");
            ogs_sbi_request_free(request);
            return NULL;
        }
        ogs_sbi_header_set(request->http.params, OGS_SBI_PARAM_LIMIT, v);
        ogs_free(v);
    }
    if (message->param.dnn) {
        ogs_sbi_header_set(request->http.params,
                OGS_SBI_PARAM_DNN, message->param.dnn);
    }
    if (message->param.plmn_id_presence) {
        OpenAPI_plmn_id_t plmn_id;

        plmn_id.mnc = ogs_plmn_id_mnc_string(&message->param.plmn_id);
        plmn_id.mcc = ogs_plmn_id_mcc_string(&message->param.plmn_id);

        if (plmn_id.mnc && plmn_id.mcc) {
            char *v = NULL;
            cJSON *item = NULL;

            item = OpenAPI_plmn_id_convertToJSON(&plmn_id);
            if (!item) {
                ogs_error("OpenAPI_plmn_id_convertToJSON() failed");
                ogs_sbi_request_free(request);
                return NULL;
            }
            if (plmn_id.mnc) ogs_free(plmn_id.mnc);
            if (plmn_id.mcc) ogs_free(plmn_id.mcc);

            v = cJSON_PrintUnformatted(item);
            if (!v) {
                ogs_error("cJSON_PrintUnformatted() failed");
                ogs_sbi_request_free(request);
                return NULL;
            }
            cJSON_Delete(item);

            ogs_sbi_header_set(request->http.params, OGS_SBI_PARAM_PLMN_ID, v);
            ogs_free(v);
        }
    }
    if (message->param.single_nssai_presence) {
        char *v = ogs_sbi_s_nssai_to_json(&message->param.s_nssai);
        if (!v) {
            ogs_error("ogs_sbi_s_nssai_to_json() failed");
            ogs_sbi_request_free(request);
            return NULL;
        }
        ogs_sbi_header_set(request->http.params, OGS_SBI_PARAM_SINGLE_NSSAI, v);
        ogs_free(v);
    }
    if (message->param.snssai_presence) {
        char *v = ogs_sbi_s_nssai_to_json(&message->param.s_nssai);
        if (!v) {
            ogs_error("ogs_sbi_s_nssai_to_json() failed");
            ogs_sbi_request_free(request);
            return NULL;
        }
        ogs_sbi_header_set(request->http.params, OGS_SBI_PARAM_SNSSAI, v);
        ogs_free(v);
    }
    if (message->param.slice_info_request_for_pdu_session_presence) {
        OpenAPI_slice_info_for_pdu_session_t SliceInfoForPDUSession;
        OpenAPI_snssai_t sNSSAI;

        char *v = NULL;
        cJSON *item = NULL;

        if (!message->param.roaming_indication) {
            ogs_error("No Roaming Indication");
            ogs_sbi_request_free(request);
            return NULL;
        }

        memset(&sNSSAI, 0, sizeof(sNSSAI));
        sNSSAI.sst = message->param.s_nssai.sst;
        sNSSAI.sd = ogs_s_nssai_sd_to_string(message->param.s_nssai.sd);

        memset(&SliceInfoForPDUSession, 0, sizeof(SliceInfoForPDUSession));

        SliceInfoForPDUSession.s_nssai = &sNSSAI;
        SliceInfoForPDUSession.roaming_indication =
            message->param.roaming_indication;

        item = OpenAPI_slice_info_for_pdu_session_convertToJSON(
                &SliceInfoForPDUSession);
        if (!item) {
            ogs_error("OpenAPI_slice_info_for_pdu_session_convertToJSON() "
                    "failed");
            ogs_sbi_request_free(request);
            return NULL;
        }

        v = cJSON_PrintUnformatted(item);
        if (!v) {
            ogs_error("cJSON_PrintUnformatted() failed");
            ogs_sbi_request_free(request);
            return NULL;
        }
        cJSON_Delete(item);

        ogs_sbi_header_set(request->http.params,
                OGS_SBI_PARAM_SLICE_INFO_REQUEST_FOR_PDU_SESSION, v);
        ogs_free(v);

        if (sNSSAI.sd)
            ogs_free(sNSSAI.sd);
    }
    if (message->param.num_of_fields) {
        char *fields;

        fields = ogs_strdup(message->param.fields[0]);
        if (!fields) {
            ogs_error("ogs_strdup() failed");
            return NULL;
        }

        for (i = 1; i < message->param.num_of_fields; i++)
            fields = ogs_mstrcatf(
                    fields, ",%s", message->param.fields[i]);

        if (fields) {
            ogs_sbi_header_set(request->http.params,
                    OGS_SBI_PARAM_FIELDS, fields);
            ogs_free(fields);
        }

    }
    if (message->param.ipv4addr) {
        ogs_sbi_header_set(request->http.params,
                OGS_SBI_PARAM_IPV4ADDR, message->param.ipv4addr);
    }
    if (message->param.ipv6prefix) {
        ogs_sbi_header_set(request->http.params,
                OGS_SBI_PARAM_IPV6PREFIX, message->param.ipv6prefix);
    }

    if (build_content(&request->http, message) == false) {
        ogs_error("build_content() failed");
        ogs_sbi_request_free(request);
        return NULL;
    }

    if (message->http.accept) {
        ogs_sbi_header_set(request->http.headers, OGS_SBI_ACCEPT,
                message->http.accept);
    } else {
        SWITCH(message->h.method)
        CASE(OGS_SBI_HTTP_METHOD_DELETE)
            ogs_sbi_header_set(request->http.headers, OGS_SBI_ACCEPT,
                OGS_SBI_CONTENT_PROBLEM_TYPE);
            break;
        DEFAULT
            ogs_sbi_header_set(request->http.headers, OGS_SBI_ACCEPT,
                OGS_SBI_CONTENT_JSON_TYPE "," OGS_SBI_CONTENT_PROBLEM_TYPE);
            break;
        END
    }

    nf_type = NF_INSTANCE_TYPE(ogs_sbi_self()->nf_instance);
    if (nf_type) {
        char *user_agent = OpenAPI_nf_type_ToString(nf_type);
        if (user_agent)
            ogs_sbi_header_set(request->http.headers,
                    OGS_SBI_USER_AGENT, user_agent);
    }

    ogs_assert(OGS_OK ==
            ogs_sbi_rfc7231_string(sender_timestamp, ogs_time_now()));
    ogs_sbi_header_set(request->http.headers,
            OGS_SBI_OPTIONAL_CUSTOM_SENDER_TIMESTAMP, sender_timestamp);

    ogs_assert(ogs_time_to_msec(ogs_local_conf()->time.message.duration));
    max_rsp_time = ogs_msprintf("%d",
            (int)ogs_time_to_msec(ogs_local_conf()->time.message.duration));
    ogs_sbi_header_set(request->http.headers,
            OGS_SBI_OPTIONAL_CUSTOM_MAX_RSP_TIME, max_rsp_time);
    ogs_free(max_rsp_time);

    if (message->http.content_encoding)
        ogs_sbi_header_set(request->http.headers,
                OGS_SBI_ACCEPT_ENCODING, message->http.content_encoding);

    if (message->http.custom.callback)
        ogs_sbi_header_set(request->http.headers,
                OGS_SBI_CUSTOM_CALLBACK, message->http.custom.callback);

    if (message->http.custom.nrf_uri)
        ogs_sbi_header_set(request->http.headers,
                OGS_SBI_CUSTOM_NRF_URI, message->http.custom.nrf_uri);

    return request;
}

ogs_sbi_response_t *ogs_sbi_build_response(
        ogs_sbi_message_t *message, int status)
{
    ogs_sbi_response_t *response = NULL;

    ogs_assert(message);

    response = ogs_sbi_response_new();
    if (!response) {
        ogs_error("ogs_pool_alloc() failed");
        return NULL;
    }

    response->status = status;

    if (response->status != OGS_SBI_HTTP_STATUS_NO_CONTENT) {
        if (build_content(&response->http, message) == false) {
            ogs_error("build_content() failed");
            ogs_sbi_response_free(response);
            return NULL;
        }
    }

    if (message->http.location) {
        ogs_sbi_header_set(response->http.headers, "Location",
                message->http.location);
    }
    if (message->http.cache_control)
        ogs_sbi_header_set(response->http.headers, "Cache-Control",
                message->http.cache_control);

    return response;
}

int ogs_sbi_parse_request(
        ogs_sbi_message_t *message, ogs_sbi_request_t *request)
{
    int rv;
    ogs_hash_index_t *hi;
    ogs_sbi_discovery_option_t *discovery_option = NULL;
    bool discovery_option_presence = false;

    ogs_assert(request);
    ogs_assert(message);

    rv = ogs_sbi_parse_header(message, &request->h);
    if (rv != OGS_OK) {
        ogs_error("ogs_sbi_parse_header() failed");
        return OGS_ERROR;
    }

    discovery_option_presence = false;
    discovery_option = ogs_sbi_discovery_option_new();
    ogs_assert(discovery_option);

    for (hi = ogs_hash_first(request->http.params);
            hi; hi = ogs_hash_next(hi)) {
        /* Discovery Parameter */
        if (!strcmp(ogs_hash_this_key(hi),
                    OGS_SBI_PARAM_TARGET_NF_TYPE)) {
            message->param.target_nf_type =
                OpenAPI_nf_type_FromString(ogs_hash_this_val(hi));
        } else if (!strcmp(ogs_hash_this_key(hi),
                    OGS_SBI_PARAM_REQUESTER_NF_TYPE)) {
            message->param.requester_nf_type =
                OpenAPI_nf_type_FromString(ogs_hash_this_val(hi));

        /* Discovery Option Parameter */
        } else if (!strcmp(ogs_hash_this_key(hi),
                    OGS_SBI_PARAM_TARGET_NF_INSTANCE_ID)) {
            char *v = ogs_hash_this_val(hi);

            if (v) {
                ogs_sbi_discovery_option_set_target_nf_instance_id(
                        discovery_option, v);
                discovery_option_presence = true;
            }
        } else if (!strcmp(ogs_hash_this_key(hi),
                    OGS_SBI_PARAM_REQUESTER_NF_INSTANCE_ID)) {
            char *v = ogs_hash_this_val(hi);

            if (v) {
                ogs_sbi_discovery_option_set_requester_nf_instance_id(
                        discovery_option, v);
                discovery_option_presence = true;
            }
        } else if (!strcmp(ogs_hash_this_key(hi),
                    OGS_SBI_PARAM_SERVICE_NAMES)) {
    /*
     * Issues #1730
     * Send NF discovery query with service-names delimited with comma
     *
     * OpenAPI specification for sending NF discovery query with
     * "service-names" parameter is defined as folowing:
     *   name: service-names
     *   ...
     *   style: form
     *   explode: false
     *
     * According to OpenAPI specification, this means array items
     * should be delimited with a comma character (example: /users?id=3,4,5).
     *
     * See also https://swagger.io/docs/specification/serialization/
     */
            char *v = ogs_hash_this_val(hi);
            if (v) {
                ogs_sbi_discovery_option_parse_service_names(
                        discovery_option, v);
                discovery_option_presence = true;
            }
        } else if (!strcmp(ogs_hash_this_key(hi), OGS_SBI_PARAM_SNSSAIS)) {
            char *v = ogs_hash_this_val(hi);
            if (v) {
                ogs_sbi_discovery_option_parse_snssais(discovery_option, v);
                discovery_option_presence = true;
            }
        } else if (!strcmp(ogs_hash_this_key(hi), OGS_SBI_PARAM_GUAMI)) {
            char *v = ogs_hash_this_val(hi);
            if (v) {
                ogs_sbi_discovery_option_parse_guami(discovery_option, v);
                discovery_option_presence = true;
            }
        } else if (!strcmp(ogs_hash_this_key(hi), OGS_SBI_PARAM_DNN)) {
            char *v = ogs_hash_this_val(hi);
            if (v) {
                ogs_sbi_discovery_option_set_dnn(discovery_option, v);
                discovery_option_presence = true;
            }
        } else if (!strcmp(ogs_hash_this_key(hi), OGS_SBI_PARAM_TAI)) {
            char *v = ogs_hash_this_val(hi);
            if (v) {
                ogs_sbi_discovery_option_parse_tai(discovery_option, v);
                discovery_option_presence = true;
            }
        } else if (!strcmp(ogs_hash_this_key(hi),
                    OGS_SBI_PARAM_TARGET_PLMN_LIST)) {
            char *v = ogs_hash_this_val(hi);
            if (v) {
                discovery_option->num_of_target_plmn_list =
                    ogs_sbi_discovery_option_parse_plmn_list(
                        discovery_option->target_plmn_list, v);
                discovery_option_presence = true;
            }
        } else if (!strcmp(ogs_hash_this_key(hi),
                    OGS_SBI_PARAM_REQUESTER_PLMN_LIST)) {
            char *v = ogs_hash_this_val(hi);
            if (v) {
                discovery_option->num_of_requester_plmn_list =
                    ogs_sbi_discovery_option_parse_plmn_list(
                        discovery_option->requester_plmn_list, v);
                discovery_option_presence = true;
            }
        } else if (!strcmp(ogs_hash_this_key(hi),
                    OGS_SBI_PARAM_REQUESTER_FEATURES)) {
            char *v = ogs_hash_this_val(hi);
            if (v) {
                discovery_option->requester_features =
                    ogs_uint64_from_string_hexadecimal(v);
                discovery_option_presence = true;
            }
        }

        /* URL Query Parameter */
        if (!strcmp(ogs_hash_this_key(hi), OGS_SBI_PARAM_NF_ID)) {
            message->param.nf_id = ogs_hash_this_val(hi);
        } else if (!strcmp(ogs_hash_this_key(hi), OGS_SBI_PARAM_NF_TYPE)) {
            message->param.nf_type =
                OpenAPI_nf_type_FromString(ogs_hash_this_val(hi));
        } else if (!strcmp(ogs_hash_this_key(hi), OGS_SBI_PARAM_LIMIT)) {
            message->param.limit = atoi(ogs_hash_this_val(hi));
        } else if (!strcmp(ogs_hash_this_key(hi), OGS_SBI_PARAM_DNN)) {
            message->param.dnn = ogs_hash_this_val(hi);
        } else if (!strcmp(ogs_hash_this_key(hi), OGS_SBI_PARAM_PLMN_ID)) {
            char *v = NULL;
            cJSON *item = NULL;
            OpenAPI_plmn_id_t *plmn_id = NULL;

            v = ogs_hash_this_val(hi);
            if (v) {
                item = cJSON_Parse(v);
                if (item) {
                    plmn_id = OpenAPI_plmn_id_parseFromJSON(item);
                    if (plmn_id && plmn_id->mnc && plmn_id->mcc) {
                        ogs_plmn_id_build(&message->param.plmn_id,
                            atoi(plmn_id->mcc),
                            atoi(plmn_id->mnc), strlen(plmn_id->mnc));
                        message->param.plmn_id_presence = true;
                        OpenAPI_plmn_id_free(plmn_id);
                    }
                    cJSON_Delete(item);
                }
            }
        } else if (!strcmp(ogs_hash_this_key(hi), OGS_SBI_PARAM_SINGLE_NSSAI)) {
            char *v = ogs_hash_this_val(hi);
            if (v) {
                bool rc = ogs_sbi_s_nssai_from_json(&message->param.s_nssai, v);
                if (rc == true)
                    message->param.single_nssai_presence = true;
            }
        } else if (!strcmp(ogs_hash_this_key(hi), OGS_SBI_PARAM_SNSSAI)) {
            char *v = ogs_hash_this_val(hi);
            if (v) {
                bool rc = ogs_sbi_s_nssai_from_json(&message->param.s_nssai, v);
                if (rc == true)
                    message->param.snssai_presence = true;
            }
        } else if (!strcmp(ogs_hash_this_key(hi),
                    OGS_SBI_PARAM_SLICE_INFO_REQUEST_FOR_PDU_SESSION)) {
            char *v = NULL;
            cJSON *item = NULL;
            OpenAPI_slice_info_for_pdu_session_t *SliceInfoForPduSession = NULL;

            v = ogs_hash_this_val(hi);
            if (v) {
                item = cJSON_Parse(v);
                if (item) {
                    SliceInfoForPduSession =
                        OpenAPI_slice_info_for_pdu_session_parseFromJSON(item);
                    if (SliceInfoForPduSession) {
                        OpenAPI_snssai_t *s_nssai =
                            SliceInfoForPduSession->s_nssai;
                        if (s_nssai) {
                            message->param.s_nssai.sst = s_nssai->sst;
                            message->param.s_nssai.sd =
                                ogs_s_nssai_sd_from_string(s_nssai->sd);
                        }
                        message->param.roaming_indication =
                            SliceInfoForPduSession->roaming_indication;
                        message->param.
                            slice_info_request_for_pdu_session_presence = true;

                        OpenAPI_slice_info_for_pdu_session_free(
                                SliceInfoForPduSession);

                    }
                    cJSON_Delete(item);
                }
            }
        } else if (!strcmp(ogs_hash_this_key(hi), OGS_SBI_PARAM_FIELDS)) {
            char *_v = ogs_hash_this_val(hi), *v = NULL;
            char *token = NULL;
            char *saveptr = NULL;

            v = ogs_strdup(_v);
            ogs_assert(v);

            token = ogs_strtok_r(v, ",", &saveptr);
            while (token != NULL) {
                if (message->param.num_of_fields < OGS_SBI_MAX_NUM_OF_FIELDS) {
                        message->param.fields
                        [message->param.num_of_fields] = ogs_strdup(token);
                    ogs_assert(message->param.fields
                        [message->param.num_of_fields]);
                    message->param.num_of_fields++;
                    token = ogs_strtok_r(NULL, ",", &saveptr);
                } else {
                    ogs_error("Fields in query exceed MAX_NUM_OF_FIELDS");
                    break;
                }
            }

            ogs_free(v);
        } else if (!strcmp(ogs_hash_this_key(hi), OGS_SBI_PARAM_IPV4ADDR)) {
            message->param.ipv4addr = ogs_hash_this_val(hi);
        } else if (!strcmp(ogs_hash_this_key(hi), OGS_SBI_PARAM_IPV6PREFIX)) {
            message->param.ipv6prefix = ogs_hash_this_val(hi);
        }
    }

    if (discovery_option_presence == true)
        /* message->param.discovery_option will be freed()
         * in ogs_sbi_message_free() */
        message->param.discovery_option = discovery_option;
    else
        ogs_sbi_discovery_option_free(discovery_option);

    for (hi = ogs_hash_first(request->http.headers);
            hi; hi = ogs_hash_next(hi)) {
        /*
         * <RFC 2616>
         *  Each header field consists of a name followed by a colon (":")
         *  and the field value. Field names are case-insensitive.
         */
        if (!ogs_strcasecmp(ogs_hash_this_key(hi), OGS_SBI_ACCEPT_ENCODING)) {
            message->http.content_encoding = ogs_hash_this_val(hi);
        } else if (!ogs_strcasecmp(
                    ogs_hash_this_key(hi), OGS_SBI_CONTENT_TYPE)) {
            message->http.content_type = ogs_hash_this_val(hi);
        } else if (!ogs_strcasecmp(ogs_hash_this_key(hi), OGS_SBI_ACCEPT)) {
            message->http.accept = ogs_hash_this_val(hi);
        } else if (!ogs_strcasecmp(ogs_hash_this_key(hi),
                    OGS_SBI_CUSTOM_CALLBACK)) {
            message->http.custom.callback = ogs_hash_this_val(hi);
        }
    }

    if (parse_content(message, &request->http) != OGS_OK) {
        ogs_error("parse_content() failed");
        ogs_sbi_message_free(message);
        return OGS_ERROR;
    }

    return OGS_OK;
}

int ogs_sbi_parse_response(
        ogs_sbi_message_t *message, ogs_sbi_response_t *response)
{
    int rv;
    ogs_hash_index_t *hi;

    ogs_assert(response);
    ogs_assert(message);

    rv = ogs_sbi_parse_header(message, &response->h);
    if (rv != OGS_OK) {
        ogs_error("ogs_sbi_parse_header() failed");
        return OGS_ERROR;
    }

    for (hi = ogs_hash_first(response->http.headers);
            hi; hi = ogs_hash_next(hi)) {
        if (!ogs_strcasecmp(ogs_hash_this_key(hi), OGS_SBI_CONTENT_TYPE)) {
            message->http.content_type = ogs_hash_this_val(hi);
        } else if (!ogs_strcasecmp(ogs_hash_this_key(hi), OGS_SBI_LOCATION)) {
            message->http.location = ogs_hash_this_val(hi);
        }
    }

    message->res_status = response->status;

    if (parse_content(message, &response->http) != OGS_OK) {
        ogs_error("parse_content() failed");
        return OGS_ERROR;
    }

    return OGS_OK;
}

ogs_pkbuf_t *ogs_sbi_find_part_by_content_id(
        ogs_sbi_message_t *message, char *content_id)
{
    int i;

    ogs_assert(message);
    ogs_assert(content_id);

    for (i = 0; i < message->num_of_part; i++) {
        if (message->part[i].content_id &&
            strcmp(message->part[i].content_id, content_id) == 0)
            return message->part[i].pkbuf;
    }

    return NULL;
}

int ogs_sbi_parse_header(ogs_sbi_message_t *message, ogs_sbi_header_t *header)
{
    struct yuarel yuarel;
    char *saveptr = NULL;
    char *uri = NULL, *p = NULL;

    char *component = NULL;
    int i = 0;

    ogs_assert(message);
    ogs_assert(header);

    memset(message, 0, sizeof(ogs_sbi_message_t));

    message->h.method = header->method;
    message->h.uri = header->uri;
    ogs_assert(message->h.uri);

    uri = ogs_strdup(header->uri);
    ogs_assert(uri);
    p = uri;

    if (p[0] != '/') {
        int rv = yuarel_parse(&yuarel, p);
        if (rv != OGS_OK) {
            ogs_error("yuarel_parse() failed");
            ogs_free(uri);
            return OGS_ERROR;
        }

        p = yuarel.path;
    }

    header->service.name = ogs_sbi_parse_uri(p, "/", &saveptr);
    if (!header->service.name) {
        ogs_error("ogs_sbi_parse_uri() failed");
        ogs_free(uri);
        return OGS_ERROR;
    }
    message->h.service.name = header->service.name;

    header->api.version = ogs_sbi_parse_uri(NULL, "/", &saveptr);
    if (!header->api.version) {
        ogs_error("ogs_sbi_parse_uri() failed");
        ogs_free(uri);
        return OGS_ERROR;
    }
    message->h.api.version = header->api.version;

    for (i = 0; i < OGS_SBI_MAX_NUM_OF_RESOURCE_COMPONENT &&
            (component = ogs_sbi_parse_uri(NULL, "/", &saveptr)) != NULL;
         i++) {
        header->resource.component[i] = component;
        message->h.resource.component[i] = component;
    }

    ogs_free(uri);

    return OGS_OK;
}

void ogs_sbi_header_free(ogs_sbi_header_t *h)
{
    int i;
    ogs_assert(h);

    if (h->method) ogs_free(h->method);
    if (h->service.name) ogs_free(h->service.name);
    if (h->api.version) ogs_free(h->api.version);

    for (i = 0; i < OGS_SBI_MAX_NUM_OF_RESOURCE_COMPONENT &&
                        h->resource.component[i]; i++)
        ogs_free(h->resource.component[i]);
}

void ogs_sbi_http_hash_free(ogs_hash_t *hash)
{
    ogs_hash_index_t *hi;

    ogs_assert(hash);

    for (hi = ogs_hash_first(hash); hi; hi = ogs_hash_next(hi)) {
        char *key = (char *)ogs_hash_this_key(hi);
        char *val = ogs_hash_this_val(hi);
        ogs_hash_set(hash, key, strlen(key), NULL);
        ogs_free(key);
        ogs_free(val);
    }
    ogs_hash_destroy(hash);
}

static char *build_json(ogs_sbi_message_t *message)
{
    char *content = NULL;
    cJSON *item = NULL;

    ogs_assert(message);

    if (message->ProblemDetails) {
        item = OpenAPI_problem_details_convertToJSON(message->ProblemDetails);
        ogs_assert(item);
    } else if (message->NFProfile) {
        item = OpenAPI_nf_profile_convertToJSON(message->NFProfile);
        ogs_assert(item);
    } else if (message->PatchItemList) {
        OpenAPI_lnode_t *node = NULL;

        item = cJSON_CreateArray();
        ogs_assert(item);

        OpenAPI_list_for_each(message->PatchItemList, node) {
            cJSON *patchItem = OpenAPI_patch_item_convertToJSON(node->data);
            ogs_assert(patchItem);
            cJSON_AddItemToArray(item, patchItem);
        }
    } else if (message->SubscriptionData) {
        item = OpenAPI_subscription_data_convertToJSON(
                message->SubscriptionData);
        ogs_assert(item);
    } else if (message->NotificationData) {
        item = OpenAPI_notification_data_convertToJSON(
                message->NotificationData);
        ogs_assert(item);
    } else if (message->SearchResult) {
        item = OpenAPI_search_result_convertToJSON(message->SearchResult);
        ogs_assert(item);
    } else if (message->links) {
        item = ogs_sbi_links_convertToJSON(message->links);
        ogs_assert(item);
    } else if (message->AuthenticationInfo) {
        item = OpenAPI_authentication_info_convertToJSON(
                message->AuthenticationInfo);
        ogs_assert(item);
    } else if (message->AuthenticationInfoRequest) {
        item = OpenAPI_authentication_info_request_convertToJSON(
                message->AuthenticationInfoRequest);
        ogs_assert(item);
    } else if (message->AuthenticationInfoResult) {
        item = OpenAPI_authentication_info_result_convertToJSON(
                message->AuthenticationInfoResult);
        ogs_assert(item);
    } else if (message->AuthenticationSubscription) {
        item = OpenAPI_authentication_subscription_convertToJSON(
                message->AuthenticationSubscription);
        ogs_assert(item);
    } else if (message->UeAuthenticationCtx) {
        item = OpenAPI_ue_authentication_ctx_convertToJSON(
                message->UeAuthenticationCtx);
        ogs_assert(item);
    } else if (message->ConfirmationData) {
        item = OpenAPI_confirmation_data_convertToJSON(
                message->ConfirmationData);
        ogs_assert(item);
    } else if (message->ConfirmationDataResponse) {
        item = OpenAPI_confirmation_data_response_convertToJSON(
                message->ConfirmationDataResponse);
        ogs_assert(item);
    } else if (message->AuthEvent) {
        item = OpenAPI_auth_event_convertToJSON(message->AuthEvent);
        ogs_assert(item);
    } else if (message->Amf3GppAccessRegistration) {
        item = OpenAPI_amf3_gpp_access_registration_convertToJSON(
                message->Amf3GppAccessRegistration);
        ogs_assert(item);
    } else if (message->Amf3GppAccessRegistrationModification) {
        item = OpenAPI_amf3_gpp_access_registration_modification_convertToJSON(
                message->Amf3GppAccessRegistrationModification);
        ogs_assert(item);
    } else if (message->SmfRegistration) {
        item = OpenAPI_smf_registration_convertToJSON(message->SmfRegistration);
        ogs_assert(item);
    } else if (message->Nssai) {
        item = OpenAPI_nssai_convertToJSON(message->Nssai);
        ogs_assert(item);
    } else if (message->AccessAndMobilitySubscriptionData) {
        item = OpenAPI_access_and_mobility_subscription_data_convertToJSON(
                message->AccessAndMobilitySubscriptionData);
        ogs_assert(item);
    } else if (message->SmfSelectionSubscriptionData) {
        item = OpenAPI_smf_selection_subscription_data_convertToJSON(
                message->SmfSelectionSubscriptionData);
        ogs_assert(item);
    } else if (message->UeContextInSmfData) {
        item = OpenAPI_ue_context_in_smf_data_convertToJSON(
                message->UeContextInSmfData);
        ogs_assert(item);
    } else if (message->SmContextCreateData) {
        item = OpenAPI_sm_context_create_data_convertToJSON(
                message->SmContextCreateData);
        ogs_assert(item);
    } else if (message->SmContextCreatedData) {
        item = OpenAPI_sm_context_created_data_convertToJSON(
                message->SmContextCreatedData);
        ogs_assert(item);
    } else if (message->SmContextCreateError) {
        item = OpenAPI_sm_context_create_error_convertToJSON(
                message->SmContextCreateError);
        ogs_assert(item);
    } else if (message->SmContextUpdateData) {
        item = OpenAPI_sm_context_update_data_convertToJSON(
                message->SmContextUpdateData);
        ogs_assert(item);
    } else if (message->SmContextUpdatedData) {
        item = OpenAPI_sm_context_updated_data_convertToJSON(
                message->SmContextUpdatedData);
        ogs_assert(item);
    } else if (message->SmContextUpdateError) {
        item = OpenAPI_sm_context_update_error_convertToJSON(
                message->SmContextUpdateError);
        ogs_assert(item);
    } else if (message->SmContextReleaseData) {
        item = OpenAPI_sm_context_release_data_convertToJSON(
                message->SmContextReleaseData);
        ogs_assert(item);
    } else if (message->SmContextReleasedData) {
        item = OpenAPI_sm_context_released_data_convertToJSON(
                message->SmContextReleasedData);
        ogs_assert(item);
    } else if (message->SessionManagementSubscriptionDataList) {
        OpenAPI_lnode_t *node = NULL;

        item = cJSON_CreateArray();
        ogs_assert(item);

        OpenAPI_list_for_each(message->SessionManagementSubscriptionDataList, node) {
            cJSON *smSubDataItem = 
                OpenAPI_session_management_subscription_data_convertToJSON(node->data);
            ogs_assert(smSubDataItem);
            cJSON_AddItemToArray(item, smSubDataItem);
        }
    } else if (message->N1N2MessageTransferReqData) {
        item = OpenAPI_n1_n2_message_transfer_req_data_convertToJSON(
                message->N1N2MessageTransferReqData);
        ogs_assert(item);
    } else if (message->N1N2MessageTransferRspData) {
        item = OpenAPI_n1_n2_message_transfer_rsp_data_convertToJSON(
                message->N1N2MessageTransferRspData);
        ogs_assert(item);
    } else if (message->N1N2MsgTxfrFailureNotification) {
        item = OpenAPI_n1_n2_msg_txfr_failure_notification_convertToJSON(
                message->N1N2MsgTxfrFailureNotification);
        ogs_assert(item);
    } else if (message->SmContextStatusNotification) {
        item = OpenAPI_sm_context_status_notification_convertToJSON(
                message->SmContextStatusNotification);
        ogs_assert(item);
    } else if (message->PolicyAssociationRequest) {
        item = OpenAPI_policy_association_request_convertToJSON(
                message->PolicyAssociationRequest);
        ogs_assert(item);
    } else if (message->PolicyAssociation) {
        item = OpenAPI_policy_association_convertToJSON(
                message->PolicyAssociation);
        ogs_assert(item);
    } else if (message->AmPolicyData) {
        item = OpenAPI_am_policy_data_convertToJSON(message->AmPolicyData);
        ogs_assert(item);
    } else if (message->SmPolicyContextData) {
        item = OpenAPI_sm_policy_context_data_convertToJSON(
                message->SmPolicyContextData);
        ogs_assert(item);
    } else if (message->SmPolicyDecision) {
        item = OpenAPI_sm_policy_decision_convertToJSON(
                message->SmPolicyDecision);
        ogs_assert(item);
    } else if (message->SmPolicyData) {
        item = OpenAPI_sm_policy_data_convertToJSON(message->SmPolicyData);
        ogs_assert(item);
    } else if (message->SmPolicyDeleteData) {
        item = OpenAPI_sm_policy_delete_data_convertToJSON(
                message->SmPolicyDeleteData);
        ogs_assert(item);
    } else if (message->AuthorizedNetworkSliceInfo) {
        item = OpenAPI_authorized_network_slice_info_convertToJSON(
                message->AuthorizedNetworkSliceInfo);
        ogs_assert(item);
    } else if (message->PcfBinding) {
        item = OpenAPI_pcf_binding_convertToJSON(message->PcfBinding);
        ogs_assert(item);
    } else if (message->AppSessionContext) {
        item = OpenAPI_app_session_context_convertToJSON(
                message->AppSessionContext);
        ogs_assert(item);
    } else if (message->AppSessionContextUpdateDataPatch) {
        item = OpenAPI_app_session_context_update_data_patch_convertToJSON(
                message->AppSessionContextUpdateDataPatch);
        ogs_assert(item);
    } else if (message->SmPolicyNotification) {
        item = OpenAPI_sm_policy_notification_convertToJSON(
                message->SmPolicyNotification);
        ogs_assert(item);
    } else if (message->TerminationNotification) {
        item = OpenAPI_termination_notification_convertToJSON(
                message->TerminationNotification);
        ogs_assert(item);
    } else if (message->DeregistrationData) {
        item = OpenAPI_deregistration_data_convertToJSON(
                message->DeregistrationData);
        ogs_assert(item);
    } else if (message->SDMSubscription) {
        item = OpenAPI_sdm_subscription_convertToJSON(
                message->SDMSubscription);
        ogs_assert(item);
    } else if (message->ModificationNotification) {
        item = OpenAPI_modification_notification_convertToJSON(
            message->ModificationNotification);
        ogs_assert(item);
    } else if (message->SecNegotiateReqData) {
        item = OpenAPI_sec_negotiate_req_data_convertToJSON(
            message->SecNegotiateReqData);
        ogs_assert(item);
    } else if (message->SecNegotiateRspData) {
        item = OpenAPI_sec_negotiate_rsp_data_convertToJSON(
            message->SecNegotiateRspData);
        ogs_assert(item);
    } else if (message->UeContextTransferReqData) {
        item = OpenAPI_ue_context_transfer_req_data_convertToJSON(
                message->UeContextTransferReqData);
        ogs_assert(item);
    } else if (message->UeContextTransferRspData) {
        item = OpenAPI_ue_context_transfer_rsp_data_convertToJSON(
                message->UeContextTransferRspData);
        ogs_assert(item);
    } else if (message->UeRegStatusUpdateReqData) {
        item = OpenAPI_ue_reg_status_update_req_data_convertToJSON(
                message->UeRegStatusUpdateReqData);
        ogs_assert(item);
    } else if (message->UeRegStatusUpdateRspData) {
        item = OpenAPI_ue_reg_status_update_rsp_data_convertToJSON(
                message->UeRegStatusUpdateRspData);
        ogs_assert(item);
    } else if (message->N2InformationTransferReqData) {
        item = OpenAPI_n2_information_transfer_req_data_convertToJSON(
                message->N2InformationTransferReqData);
        ogs_assert(item);
    }

    if (item) {
        content = cJSON_PrintUnformatted(item);
        ogs_assert(content);
        ogs_log_print(OGS_LOG_TRACE, "%s", content);
        cJSON_Delete(item);
    }

    return content;
}

static int parse_json(ogs_sbi_message_t *message,
        char *content_type, char *json)
{
    int rv = OGS_OK;
    cJSON *item = NULL;

    ogs_assert(message);

    if (!json)
        return OGS_OK;

    if (!content_type) {
        ogs_error("No Content-type");
        return OGS_ERROR;
    }

    ogs_log_print(OGS_LOG_TRACE, "%s", json);
    item = cJSON_Parse(json);
    if (!item) {
        ogs_error("JSON parse error [%s]", json);
        return OGS_ERROR;
    }

    if (content_type &&
        !strncmp(content_type, OGS_SBI_CONTENT_PROBLEM_TYPE,
            strlen(OGS_SBI_CONTENT_PROBLEM_TYPE))) {
        message->ProblemDetails = OpenAPI_problem_details_parseFromJSON(item);
    } else if (content_type &&
                !strncmp(content_type, OGS_SBI_CONTENT_PATCH_TYPE,
                    strlen(OGS_SBI_CONTENT_PATCH_TYPE))) {
        if (item) {
            OpenAPI_patch_item_t *patch_item = NULL;
            cJSON *patchJSON = NULL;
            message->PatchItemList = OpenAPI_list_create();
            cJSON_ArrayForEach(patchJSON, item) {
                if (!cJSON_IsObject(patchJSON)) {
                    rv = OGS_ERROR;
                    ogs_error("Unknown JSON");
                    goto cleanup;
                }

                patch_item = OpenAPI_patch_item_parseFromJSON(patchJSON);
                if (!patch_item) {
                    rv = OGS_ERROR;
                    ogs_error("No PatchItem");
                    goto cleanup;
                }
                OpenAPI_list_add(message->PatchItemList, patch_item);
            }
        }
    } else {
        SWITCH(message->h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_NNRF_NFM)

            SWITCH(message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_NF_INSTANCES)
                if (message->h.resource.component[1]) {
                    if (message->res_status < 300) {
                        message->NFProfile =
                            OpenAPI_nf_profile_parseFromJSON(item);
                        if (!message->NFProfile) {
                            rv = OGS_ERROR;
                            ogs_error("JSON parse error");
                        }
                    } else {
                        ogs_error("HTTP ERROR Status : %d", message->res_status);
                    }
                } else {
                    if (message->res_status < 300) {
                        message->links = ogs_sbi_links_parseFromJSON(item);
                        if (!message->links) {
                            rv = OGS_ERROR;
                            ogs_error("JSON parse error");
                        }
                    } else {
                        ogs_error("HTTP ERROR Status : %d", message->res_status);
                    }
                }
                break;

            CASE(OGS_SBI_RESOURCE_NAME_SUBSCRIPTIONS)
                if (message->res_status < 300) {
                    message->SubscriptionData =
                        OpenAPI_subscription_data_parseFromJSON(item);
                    if (!message->SubscriptionData) {
                        rv = OGS_ERROR;
                        ogs_error("JSON parse error");
                    }
                } else {
                    ogs_error("HTTP ERROR Status : %d", message->res_status);
                }
                break;

            CASE(OGS_SBI_RESOURCE_NAME_NF_STATUS_NOTIFY)
                if (message->res_status < 300) {
                    message->NotificationData =
                        OpenAPI_notification_data_parseFromJSON(item);
                    if (!message->NotificationData) {
                        rv = OGS_ERROR;
                        ogs_error("JSON parse error");
                    }
                } else {
                    ogs_error("HTTP ERROR Status : %d", message->res_status);
                }
                break;

            DEFAULT
                rv = OGS_ERROR;
                ogs_error("Unknown resource name [%s]",
                        message->h.resource.component[0]);
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NNRF_DISC)
            SWITCH(message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_NF_INSTANCES)
                if (message->res_status < 300) {
                    message->SearchResult =
                        OpenAPI_search_result_parseFromJSON(item);
                    if (!message->SearchResult) {
                        rv = OGS_ERROR;
                        ogs_error("JSON parse error");
                    }
                } else {
                    ogs_error("HTTP ERROR Status : %d", message->res_status);
                }
                break;

            DEFAULT
                rv = OGS_ERROR;
                ogs_error("Unknown resource name [%s]",
                        message->h.resource.component[0]);
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NAUSF_AUTH)
            SWITCH(message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_UE_AUTHENTICATIONS)
                SWITCH(message->h.method)
                CASE(OGS_SBI_HTTP_METHOD_POST)
                    if (message->res_status == 0) {
                        message->AuthenticationInfo =
                            OpenAPI_authentication_info_parseFromJSON(item);
                        if (!message->AuthenticationInfo) {
                            rv = OGS_ERROR;
                            ogs_error("JSON parse error");
                        }
                    } else if (message->res_status ==
                            OGS_SBI_HTTP_STATUS_CREATED) {
                        message->UeAuthenticationCtx =
                        OpenAPI_ue_authentication_ctx_parseFromJSON(item);
                        if (!message->UeAuthenticationCtx) {
                            rv = OGS_ERROR;
                            ogs_error("JSON parse error");
                        }
                    }
                    break;
                CASE(OGS_SBI_HTTP_METHOD_PUT)
                    if (message->res_status == 0) {
                        message->ConfirmationData =
                            OpenAPI_confirmation_data_parseFromJSON(item);
                        if (!message->ConfirmationData) {
                            rv = OGS_ERROR;
                            ogs_error("JSON parse error");
                        }
                    } else if (message->res_status == OGS_SBI_HTTP_STATUS_OK) {
                        message->ConfirmationDataResponse =
                            OpenAPI_confirmation_data_response_parseFromJSON(
                                    item);
                        if (!message->ConfirmationDataResponse) {
                            rv = OGS_ERROR;
                            ogs_error("JSON parse error");
                        }
                    }
                    break;
                DEFAULT
                    rv = OGS_ERROR;
                    ogs_error("Unknown method [%s]", message->h.method);
                END
                break;

            DEFAULT
                rv = OGS_ERROR;
                ogs_error("Unknown resource name [%s]",
                        message->h.resource.component[0]);
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NUDM_UEAU)
            SWITCH(message->h.resource.component[1])
            CASE(OGS_SBI_RESOURCE_NAME_SECURITY_INFORMATION)
                SWITCH(message->h.resource.component[2])
                CASE(OGS_SBI_RESOURCE_NAME_GENERATE_AUTH_DATA)
                    if (message->res_status == 0) {
                        message->AuthenticationInfoRequest =
                        OpenAPI_authentication_info_request_parseFromJSON(
                                item);
                        if (!message->AuthenticationInfoRequest) {
                            rv = OGS_ERROR;
                            ogs_error("JSON parse error");
                        }
                    } else if (message->res_status == OGS_SBI_HTTP_STATUS_OK) {
                        message->AuthenticationInfoResult =
                        OpenAPI_authentication_info_result_parseFromJSON(
                                item);
                        if (!message->AuthenticationInfoResult) {
                            rv = OGS_ERROR;
                            ogs_error("JSON parse error");
                        }
                    }
                    break;
                DEFAULT
                    rv = OGS_ERROR;
                    ogs_error("Unknown resource name [%s]",
                            message->h.resource.component[2]);
                END
                break;

            CASE(OGS_SBI_RESOURCE_NAME_AUTH_EVENTS)
                if (message->res_status < 300) {
                    message->AuthEvent = OpenAPI_auth_event_parseFromJSON(item);
                    if (!message->AuthEvent) {
                        rv = OGS_ERROR;
                        ogs_error("JSON parse error");
                    }
                } else {
                    ogs_error("HTTP ERROR Status : %d", message->res_status);
                }
                break;

            DEFAULT
                rv = OGS_ERROR;
                ogs_error("Unknown resource name [%s]",
                        message->h.resource.component[1]);
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NUDM_UECM)
            SWITCH(message->h.resource.component[1])
            CASE(OGS_SBI_RESOURCE_NAME_REGISTRATIONS)
                SWITCH(message->h.resource.component[2])
                CASE(OGS_SBI_RESOURCE_NAME_AMF_3GPP_ACCESS)
                    SWITCH(message->h.method)
                    CASE(OGS_SBI_HTTP_METHOD_PUT)
                        if (message->res_status < 300) {
                            message->Amf3GppAccessRegistration =
                                OpenAPI_amf3_gpp_access_registration_parseFromJSON(
                                        item);
                            if (!message->Amf3GppAccessRegistration) {
                                rv = OGS_ERROR;
                                ogs_error("JSON parse error");
                            }
                        } else {
                            ogs_error("HTTP ERROR Status : %d",
                                    message->res_status);
                        }
                        break;
                    CASE(OGS_SBI_HTTP_METHOD_PATCH)
                        if (message->res_status < 300) {
                            message->Amf3GppAccessRegistrationModification =
                                OpenAPI_amf3_gpp_access_registration_modification_parseFromJSON(
                                        item);
                            if (!message->Amf3GppAccessRegistrationModification) {
                                rv = OGS_ERROR;
                                ogs_error("JSON parse error");
                            }
                        } else {
                            ogs_error("HTTP ERROR Status : %d",
                                    message->res_status);
                        }
                        break;
                    DEFAULT
                        rv = OGS_ERROR;
                        ogs_error("Unknown method [%s]", message->h.method);
                    END
                    break;

                CASE(OGS_SBI_RESOURCE_NAME_SMF_REGISTRATIONS)
                    if (message->res_status < 300) {
                        message->SmfRegistration =
                            OpenAPI_smf_registration_parseFromJSON(item);
                        if (!message->SmfRegistration) {
                            rv = OGS_ERROR;
                            ogs_error("JSON parse error");
                        }
                    } else {
                        ogs_error("HTTP ERROR Status : %d",
                                message->res_status);
                    }
                    break;

                DEFAULT
                    rv = OGS_ERROR;
                    ogs_error("Unknown resource name [%s]",
                            message->h.resource.component[2]);
                END
                break;

            DEFAULT
                rv = OGS_ERROR;
                ogs_error("Unknown resource name [%s]",
                        message->h.resource.component[1]);
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NUDM_SDM)
            SWITCH(message->h.resource.component[1])
            CASE(OGS_SBI_RESOURCE_NAME_NSSAI)
                if (message->res_status < 300) {
                    message->Nssai = OpenAPI_nssai_parseFromJSON(item);
                    if (!message->Nssai) {
                        rv = OGS_ERROR;
                        ogs_error("JSON parse error");
                    }
                } else {
                    ogs_error("HTTP ERROR Status : %d", message->res_status);
                }
                break;

            CASE(OGS_SBI_RESOURCE_NAME_AM_DATA)
                if (message->res_status < 300) {
                    message->AccessAndMobilitySubscriptionData =
                        OpenAPI_access_and_mobility_subscription_data_parseFromJSON(
                                item);
                    if (!message->AccessAndMobilitySubscriptionData) {
                        rv = OGS_ERROR;
                        ogs_error("JSON parse error");
                    }
                } else {
                    ogs_error("HTTP ERROR Status : %d", message->res_status);
                }
                break;

            CASE(OGS_SBI_RESOURCE_NAME_SMF_SELECT_DATA)
                if (message->res_status < 300) {
                    message->SmfSelectionSubscriptionData =
                        OpenAPI_smf_selection_subscription_data_parseFromJSON(
                                item);
                    if (!message->SmfSelectionSubscriptionData) {
                        rv = OGS_ERROR;
                        ogs_error("JSON parse error");
                    }
                } else {
                    ogs_error("HTTP ERROR Status : %d", message->res_status);
                }
                break;

            CASE(OGS_SBI_RESOURCE_NAME_UE_CONTEXT_IN_SMF_DATA)
                if (message->res_status < 300) {
                    message->UeContextInSmfData =
                        OpenAPI_ue_context_in_smf_data_parseFromJSON(item);
                    if (!message->UeContextInSmfData) {
                        rv = OGS_ERROR;
                        ogs_error("JSON parse error");
                    }
                } else {
                    ogs_error("HTTP ERROR Status : %d", message->res_status);
                }
                break;

            CASE(OGS_SBI_RESOURCE_NAME_SM_DATA)
                if (message->res_status < 300) {
                    if (item) {
                        OpenAPI_session_management_subscription_data_t
                            *smsub_item = NULL;
                        cJSON *smsubJSON = NULL;
                        message->SessionManagementSubscriptionDataList =
                            OpenAPI_list_create();
                        cJSON_ArrayForEach(smsubJSON, item) {
                            if (!cJSON_IsObject(smsubJSON)) {
                                rv = OGS_ERROR;
                                ogs_error("Unknown JSON");
                                goto cleanup;
                            }

                            smsub_item = OpenAPI_session_management_subscription_data_parseFromJSON(smsubJSON);
                            if (!smsub_item) {
                                rv = OGS_ERROR;
                                ogs_error("No smsub_item");
                                goto cleanup;
                            }
                            OpenAPI_list_add(message->SessionManagementSubscriptionDataList, smsub_item);
                        }
                    }
                } else {
                    ogs_error("HTTP ERROR Status : %d", message->res_status);
                }
                break;

            CASE(OGS_SBI_RESOURCE_NAME_SDM_SUBSCRIPTIONS)
                if (message->res_status < 300) {
                    message->SDMSubscription =
                        OpenAPI_sdm_subscription_parseFromJSON(item);
                    if (!message->SDMSubscription) {
                        rv = OGS_ERROR;
                        ogs_error("JSON parse error");
                    }
                } else {
                    ogs_error("HTTP ERROR Status : %d", message->res_status);
                }
                break;

            DEFAULT
                rv = OGS_ERROR;
                ogs_error("Unknown resource name [%s]",
                        message->h.resource.component[1]);
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NUDR_DR)
            SWITCH(message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_SUBSCRIPTION_DATA)
                SWITCH(message->h.resource.component[2])
                CASE(OGS_SBI_RESOURCE_NAME_AUTHENTICATION_DATA)
                    SWITCH(message->h.resource.component[3])
                    CASE(OGS_SBI_RESOURCE_NAME_AUTHENTICATION_SUBSCRIPTION)
                        if (message->res_status == OGS_SBI_HTTP_STATUS_OK) {
                            message->AuthenticationSubscription =
                                OpenAPI_authentication_subscription_parseFromJSON(item);
                            if (!message->AuthenticationSubscription) {
                                rv = OGS_ERROR;
                                ogs_error("JSON parse error");
                            }
                        }
                        break;
                    CASE(OGS_SBI_RESOURCE_NAME_AUTHENTICATION_STATUS)
                        if (message->res_status < 300) {
                            message->AuthEvent =
                                OpenAPI_auth_event_parseFromJSON(item);
                            if (!message->AuthEvent) {
                                rv = OGS_ERROR;
                                ogs_error("JSON parse error");
                            }
                        } else {
                            ogs_error("HTTP ERROR Status : %d",
                                    message->res_status);
                        }
                        break;
                    DEFAULT
                        rv = OGS_ERROR;
                        ogs_error("Unknown resource name [%s]",
                                message->h.resource.component[3]);
                    END
                    break;

                CASE(OGS_SBI_RESOURCE_NAME_CONTEXT_DATA)
                    SWITCH(message->h.resource.component[3])
                    CASE(OGS_SBI_RESOURCE_NAME_AMF_3GPP_ACCESS)
                        if (message->res_status < 300) {
                            message->Amf3GppAccessRegistration =
                                OpenAPI_amf3_gpp_access_registration_parseFromJSON(
                                        item);
                            if (!message->Amf3GppAccessRegistration) {
                                rv = OGS_ERROR;
                                ogs_error("JSON parse error");
                            }
                        } else {
                            ogs_error("HTTP ERROR Status : %d",
                                    message->res_status);
                        }
                        break;
                    CASE(OGS_SBI_RESOURCE_NAME_SMF_REGISTRATIONS)
                        if (message->res_status < 300) {
                            message->SmfRegistration =
                                OpenAPI_smf_registration_parseFromJSON(item);
                            if (!message->SmfRegistration) {
                                rv = OGS_ERROR;
                                ogs_error("JSON parse error");
                            }
                        } else {
                            ogs_error("HTTP ERROR Status : %d",
                                    message->res_status);
                        }
                        break;
                    DEFAULT
                        rv = OGS_ERROR;
                        ogs_error("Unknown resource name [%s]",
                                message->h.resource.component[3]);
                    END
                    break;

                DEFAULT
                    SWITCH(message->h.resource.component[3])
                    CASE(OGS_SBI_RESOURCE_NAME_PROVISIONED_DATA)
                        SWITCH(message->h.resource.component[4])
                        CASE(OGS_SBI_RESOURCE_NAME_AM_DATA)
                            if (message->res_status < 300) {
                                message->AccessAndMobilitySubscriptionData =
                                    OpenAPI_access_and_mobility_subscription_data_parseFromJSON(item);
                                if (!message->
                                        AccessAndMobilitySubscriptionData) {
                                    rv = OGS_ERROR;
                                    ogs_error("JSON parse error");
                                }
                            } else {
                                ogs_error("HTTP ERROR Status : %d",
                                        message->res_status);
                            }
                            break;

                        CASE(OGS_SBI_RESOURCE_NAME_SMF_SELECTION_SUBSCRIPTION_DATA)
                            if (message->res_status < 300) {
                                message->SmfSelectionSubscriptionData =
                                    OpenAPI_smf_selection_subscription_data_parseFromJSON(item);
                                if (!message->SmfSelectionSubscriptionData) {
                                    rv = OGS_ERROR;
                                    ogs_error("JSON parse error");
                                }
                            } else {
                                ogs_error("HTTP ERROR Status : %d",
                                        message->res_status);
                            }
                            break;

                        CASE(OGS_SBI_RESOURCE_NAME_UE_CONTEXT_IN_SMF_DATA)
                            if (message->res_status < 300) {
                                message->UeContextInSmfData =
                                    OpenAPI_ue_context_in_smf_data_parseFromJSON(
                                            item);
                                if (!message->UeContextInSmfData) {
                                    rv = OGS_ERROR;
                                    ogs_error("JSON parse error");
                                }
                            } else {
                                ogs_error("HTTP ERROR Status : %d",
                                        message->res_status);
                            }
                            break;

                        CASE(OGS_SBI_RESOURCE_NAME_SM_DATA)
                            if (message->res_status < 300) {
                                if (item) {
                                    OpenAPI_session_management_subscription_data_t *smsub_item = NULL;
                                    cJSON *smsubJSON = NULL;
                                    message->SessionManagementSubscriptionDataList = OpenAPI_list_create();
                                    cJSON_ArrayForEach(smsubJSON, item) {
                                        if (!cJSON_IsObject(smsubJSON)) {
                                            rv = OGS_ERROR;
                                            ogs_error("Unknown JSON");
                                            goto cleanup;
                                        }

                                        smsub_item = OpenAPI_session_management_subscription_data_parseFromJSON(smsubJSON);
                                        if (!smsub_item) {
                                            rv = OGS_ERROR;
                                            ogs_error("No smsub_item");
                                            goto cleanup;
                                        }
                                        OpenAPI_list_add(message->SessionManagementSubscriptionDataList, smsub_item);
                                    }
                                }
                            } else {
                                ogs_error("HTTP ERROR Status : %d",
                                        message->res_status);
                            }
                            break;

                        DEFAULT
                            rv = OGS_ERROR;
                            ogs_error("Unknown resource name [%s]",
                                    message->h.resource.component[4]);
                        END
                        break;

                    DEFAULT
                        rv = OGS_ERROR;
                        ogs_error("Unknown resource name [%s]",
                                message->h.resource.component[3]);
                    END
                END
                break;

            CASE(OGS_SBI_RESOURCE_NAME_POLICY_DATA)
                SWITCH(message->h.resource.component[1])
                CASE(OGS_SBI_RESOURCE_NAME_UES)
                    SWITCH(message->h.resource.component[3])
                    CASE(OGS_SBI_RESOURCE_NAME_AM_DATA)
                        if (message->res_status < 300) {
                            message->AmPolicyData =
                                OpenAPI_am_policy_data_parseFromJSON(item);
                            if (!message->AmPolicyData) {
                                rv = OGS_ERROR;
                                ogs_error("JSON parse error");
                            }
                        } else {
                            ogs_error("HTTP ERROR Status : %d",
                                    message->res_status);
                        }
                        break;

                    CASE(OGS_SBI_RESOURCE_NAME_SM_DATA)
                        if (message->res_status < 300) {
                            message->SmPolicyData =
                                OpenAPI_sm_policy_data_parseFromJSON(item);
                            if (!message->SmPolicyData) {
                                rv = OGS_ERROR;
                                ogs_error("JSON parse error");
                            }
                        } else {
                            ogs_error("HTTP ERROR Status : %d",
                                    message->res_status);
                        }
                        break;

                    DEFAULT
                        rv = OGS_ERROR;
                        ogs_error("Unknown resource name [%s]",
                                message->h.resource.component[3]);
                    END
                    break;

                DEFAULT
                    rv = OGS_ERROR;
                    ogs_error("Unknown resource name [%s]",
                            message->h.resource.component[1]);
                END
                break;

            DEFAULT
                rv = OGS_ERROR;
                ogs_error("Unknown resource name [%s]",
                        message->h.resource.component[0]);
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NSMF_PDUSESSION)
            SWITCH(message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_SM_CONTEXTS)
                SWITCH(message->h.resource.component[2])
                CASE(OGS_SBI_RESOURCE_NAME_MODIFY)
                    if (message->res_status == 0) {
                        message->SmContextUpdateData =
                            OpenAPI_sm_context_update_data_parseFromJSON(item);
                        if (!message->SmContextUpdateData) {
                            rv = OGS_ERROR;
                            ogs_error("JSON parse error");
                        }
                    } else if (message->res_status == OGS_SBI_HTTP_STATUS_OK) {
                        message->SmContextUpdatedData =
                            OpenAPI_sm_context_updated_data_parseFromJSON(item);
                        if (!message->SmContextUpdatedData) {
                            rv = OGS_ERROR;
                            ogs_error("JSON parse error");
                        }
                    } else if (message->res_status ==
                            OGS_SBI_HTTP_STATUS_BAD_REQUEST ||
                                message->res_status ==
                                    OGS_SBI_HTTP_STATUS_FORBIDDEN ||
                                message->res_status ==
                                    OGS_SBI_HTTP_STATUS_NOT_FOUND ||
                                message->res_status ==
                                    OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR ||
                                message->res_status ==
                                    OGS_SBI_HTTP_STATUS_SERVICE_UNAVAILABLE ||
                                message->res_status ==
                                    OGS_SBI_HTTP_STATUS_GATEWAY_TIMEOUT) {
                        message->SmContextUpdateError =
                            OpenAPI_sm_context_update_error_parseFromJSON(item);
                        if (!message->SmContextUpdateError) {
                            rv = OGS_ERROR;
                            ogs_error("JSON parse error");
                        }
                    }
                    break;
                CASE(OGS_SBI_RESOURCE_NAME_RELEASE)
                    if (message->res_status == 0) {
                        message->SmContextReleaseData =
                            OpenAPI_sm_context_release_data_parseFromJSON(item);
                        if (!message->SmContextReleaseData) {
                            rv = OGS_ERROR;
                            ogs_error("JSON parse error");
                        }
                    } else if (message->res_status ==
                            OGS_SBI_HTTP_STATUS_NO_CONTENT) {
                    } else if (message->res_status == OGS_SBI_HTTP_STATUS_OK) {
                        message->SmContextReleasedData =
                            OpenAPI_sm_context_released_data_parseFromJSON(
                                    item);
                        if (!message->SmContextReleasedData) {
                            rv = OGS_ERROR;
                            ogs_error("JSON parse error");
                        }
                    }
                    break;
                DEFAULT
                    if (message->res_status == 0) {
                        message->SmContextCreateData =
                            OpenAPI_sm_context_create_data_parseFromJSON(item);
                        if (!message->SmContextCreateData) {
                            rv = OGS_ERROR;
                            ogs_error("JSON parse error");
                        }
                    } else if (message->res_status ==
                            OGS_SBI_HTTP_STATUS_CREATED) {
                        message->SmContextCreatedData =
                            OpenAPI_sm_context_created_data_parseFromJSON(item);
                        if (!message->SmContextCreatedData) {
                            rv = OGS_ERROR;
                            ogs_error("JSON parse error");
                        }
                    } else if (message->res_status ==
                            OGS_SBI_HTTP_STATUS_BAD_REQUEST ||
                                message->res_status ==
                                    OGS_SBI_HTTP_STATUS_FORBIDDEN ||
                                message->res_status ==
                                    OGS_SBI_HTTP_STATUS_NOT_FOUND ||
                                message->res_status ==
                                    OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR ||
                                message->res_status ==
                                    OGS_SBI_HTTP_STATUS_SERVICE_UNAVAILABLE ||
                                message->res_status ==
                                    OGS_SBI_HTTP_STATUS_GATEWAY_TIMEOUT) {
                        message->SmContextCreateError =
                            OpenAPI_sm_context_create_error_parseFromJSON(item);
                        if (!message->SmContextCreateError) {
                            rv = OGS_ERROR;
                            ogs_error("JSON parse error");
                        }
                    }
                END
                break;

            DEFAULT
                rv = OGS_ERROR;
                ogs_error("Unknown resource name [%s]",
                        message->h.resource.component[0]);
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NAMF_COMM)
            /* Replace SWITCH/CASE with if/else if for resource.component[0] */
            if (strcmp(message->h.resource.component[0], OGS_SBI_RESOURCE_NAME_UE_CONTEXTS) == 0) {
                SWITCH(message->h.resource.component[2])
                CASE(OGS_SBI_RESOURCE_NAME_N1_N2_MESSAGES)
                    if (message->res_status == 0) {
                        message->N1N2MessageTransferReqData =
                            OpenAPI_n1_n2_message_transfer_req_data_parseFromJSON(item);
                        if (!message->N1N2MessageTransferReqData) {
                            rv = OGS_ERROR;
                            ogs_error("JSON parse error");
                        }
                    } else if (message->res_status == OGS_SBI_HTTP_STATUS_OK ||
                               message->res_status == OGS_SBI_HTTP_STATUS_ACCEPTED) {
                        message->N1N2MessageTransferRspData =
                            OpenAPI_n1_n2_message_transfer_rsp_data_parseFromJSON(item);
                        if (!message->N1N2MessageTransferRspData) {
                            rv = OGS_ERROR;
                            ogs_error("JSON parse error");
                        }
                    }
                    break;
                // ... rest of UE_CONTEXTS logic ...
                END
            } else if (strcmp(message->h.resource.component[0], OGS_SBI_RESOURCE_NAME_NON_UE_N2_MESSAGES) == 0) {
                if (message->res_status == 0) {
                    message->N2InformationTransferReqData =
                        OpenAPI_n2_information_transfer_req_data_parseFromJSON(item);
                    if (!message->N2InformationTransferReqData) {
                        rv = OGS_ERROR;
                        ogs_error("JSON parse error");
                    }
                }
            } else {
                rv = OGS_ERROR;
                ogs_error("Unknown resource name [%s]", message->h.resource.component[0]);
            }
            break;
        CASE(OGS_SBI_SERVICE_NAME_NPCF_AM_POLICY_CONTROL)
            SWITCH(message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_POLICIES)
                if (message->res_status == 0) {
                    message->PolicyAssociationRequest =
                        OpenAPI_policy_association_request_parseFromJSON(item);
                    if (!message->PolicyAssociationRequest) {
                        rv = OGS_ERROR;
                        ogs_error("JSON parse error");
                    }
                } else if (message->res_status ==
                        OGS_SBI_HTTP_STATUS_CREATED) {
                    message->PolicyAssociation =
                        OpenAPI_policy_association_parseFromJSON(item);
                    if (!message->PolicyAssociation) {
                        rv = OGS_ERROR;
                        ogs_error("JSON parse error");
                    }
                }
                break;
            DEFAULT
                rv = OGS_ERROR;
                ogs_error("Unknown resource name [%s]",
                        message->h.resource.component[0]);
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NPCF_SMPOLICYCONTROL)
            SWITCH(message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_SM_POLICIES)
                if (!message->h.resource.component[1]) {
                    if (message->res_status == 0) {
                        message->SmPolicyContextData =
                            OpenAPI_sm_policy_context_data_parseFromJSON(item);
                        if (!message->SmPolicyContextData) {
                            rv = OGS_ERROR;
                            ogs_error("JSON parse error");
                        }
                    } else if (message->res_status ==
                            OGS_SBI_HTTP_STATUS_CREATED) {
                        message->SmPolicyDecision =
                            OpenAPI_sm_policy_decision_parseFromJSON(item);
                        if (!message->SmPolicyDecision) {
                            rv = OGS_ERROR;
                            ogs_error("JSON parse error");
                        }
                    }
                } else {
                    SWITCH(message->h.resource.component[2])
                    CASE(OGS_SBI_RESOURCE_NAME_DELETE)
                        if (message->res_status == 0) {
                            message->SmPolicyDeleteData =
                                OpenAPI_sm_policy_delete_data_parseFromJSON(
                                        item);
                            if (!message->SmPolicyDeleteData) {
                                rv = OGS_ERROR;
                                ogs_error("JSON parse error");
                            }
                        }
                        break;
                    DEFAULT
                        rv = OGS_ERROR;
                        ogs_error("Unknown resource name [%s]",
                                message->h.resource.component[2]);
                    END
                }
                break;

            DEFAULT
                rv = OGS_ERROR;
                ogs_error("Unknown resource name [%s]",
                        message->h.resource.component[0]);
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NNSSF_NSSELECTION)
            SWITCH(message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_NETWORK_SLICE_INFORMATION)
                if (message->res_status == OGS_SBI_HTTP_STATUS_OK) {
                    message->AuthorizedNetworkSliceInfo =
                        OpenAPI_authorized_network_slice_info_parseFromJSON(
                                item);
                    if (!message->AuthorizedNetworkSliceInfo) {
                        rv = OGS_ERROR;
                        ogs_error("JSON parse error");
                    }
                }
                break;

            DEFAULT
                rv = OGS_ERROR;
                ogs_error("Unknown resource name [%s]",
                        message->h.resource.component[0]);
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NBSF_MANAGEMENT)
            SWITCH(message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_PCF_BINDINGS)
                if (message->h.resource.component[1]) {
                    if (message->res_status == OGS_SBI_HTTP_STATUS_OK) {
                        message->PcfBinding =
                            OpenAPI_pcf_binding_parseFromJSON(item);
                        if (!message->PcfBinding) {
                            rv = OGS_ERROR;
                            ogs_error("JSON parse error");
                        }
                    }
                    break;
                } else {
                    SWITCH(message->h.method)
                    CASE(OGS_SBI_HTTP_METHOD_POST)
                        if (message->res_status == 0 ||
                            message->res_status ==
                                OGS_SBI_HTTP_STATUS_CREATED) {
                            message->PcfBinding =
                                OpenAPI_pcf_binding_parseFromJSON(item);
                            if (!message->PcfBinding) {
                                rv = OGS_ERROR;
                                ogs_error("JSON parse error");
                            }
                        }
                        break;

                    CASE(OGS_SBI_HTTP_METHOD_GET)
                        if (message->res_status == OGS_SBI_HTTP_STATUS_OK) {
                            message->PcfBinding =
                                OpenAPI_pcf_binding_parseFromJSON(item);
                            if (!message->PcfBinding) {
                                rv = OGS_ERROR;
                                ogs_error("JSON parse error");
                            }
                        }
                        break;
                    DEFAULT
                        rv = OGS_ERROR;
                        ogs_error("Unknown method [%s]", message->h.method);
                    END
                    break;
                }

            DEFAULT
                rv = OGS_ERROR;
                ogs_error("Unknown resource name [%s]",
                        message->h.resource.component[0]);
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NPCF_POLICYAUTHORIZATION)
            SWITCH(message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_APP_SESSIONS)
                if (message->h.resource.component[1]) {
                    if (message->h.resource.component[2]) {
                        SWITCH(message->h.resource.component[2])
                        CASE(OGS_SBI_RESOURCE_NAME_DELETE)
                            /* Nothing */
                            break;
                        DEFAULT
                            rv = OGS_ERROR;
                            ogs_error("JSON parse error");
                        END
                    } else {
                        SWITCH(message->h.method)
                        CASE(OGS_SBI_HTTP_METHOD_PATCH)
                            if (message->res_status < 300) {
                                message->AppSessionContextUpdateDataPatch =
                                    OpenAPI_app_session_context_update_data_patch_parseFromJSON(item);
                                if (!message->AppSessionContextUpdateDataPatch) {
                                    rv = OGS_ERROR;
                                    ogs_error("JSON parse error");
                                }
                            } else {
                                ogs_error("HTTP ERROR Status : %d",
                                        message->res_status);
                            }
                            break;
                        DEFAULT
                            rv = OGS_ERROR;
                            ogs_error("JSON parse error");
                        END
                    }
                } else {
                    SWITCH(message->h.method)
                    CASE(OGS_SBI_HTTP_METHOD_POST)
                        if (message->res_status == 0 ||
                            message->res_status ==
                                OGS_SBI_HTTP_STATUS_CREATED) {
                            message->AppSessionContext =
                                OpenAPI_app_session_context_parseFromJSON(item);
                            if (!message->AppSessionContext) {
                                rv = OGS_ERROR;
                                ogs_error("JSON parse error");
                            }
                        }
                        break;
                    DEFAULT
                        rv = OGS_ERROR;
                        ogs_error("Unknown method [%s]", message->h.method);
                    END
                }
                break;
            DEFAULT
                rv = OGS_ERROR;
                ogs_error("Unknown resource name [%s]",
                        message->h.resource.component[0]);
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_N32C_HANDSHAKE)
            SWITCH(message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_EXCHANGE_CAPABILITY)
                SWITCH(message->h.method)
                CASE(OGS_SBI_HTTP_METHOD_POST)
                    if (message->res_status == 0) {
                        message->SecNegotiateReqData =
                            OpenAPI_sec_negotiate_req_data_parseFromJSON(item);
                        if (!message->SecNegotiateReqData) {
                            rv = OGS_ERROR;
                            ogs_error("JSON parse error");
                        }
                    } else if (message->res_status == OGS_SBI_HTTP_STATUS_OK) {
                        message->SecNegotiateRspData =
                            OpenAPI_sec_negotiate_rsp_data_parseFromJSON(item);
                        if (!message->SecNegotiateRspData) {
                            rv = OGS_ERROR;
                            ogs_error("JSON parse error");
                        }
                    }
                    break;
                DEFAULT
                    rv = OGS_ERROR;
                    ogs_error("Unknown method [%s]", message->h.method);
                END
                break;
            DEFAULT
                rv = OGS_ERROR;
                ogs_error("Unknown resource name [%s]",
                        message->h.resource.component[0]);
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NAMF_CALLBACK)
            SWITCH(message->h.resource.component[1])
            CASE(OGS_SBI_RESOURCE_NAME_SM_CONTEXT_STATUS)
                if (message->res_status < 300) {
                    message->SmContextStatusNotification =
                        OpenAPI_sm_context_status_notification_parseFromJSON(
                                item);
                    if (!message->SmContextStatusNotification) {
                        rv = OGS_ERROR;
                        ogs_error("JSON parse error");
                    }
                } else {
                    ogs_error("HTTP ERROR Status : %d", message->res_status);
                }
                break;

            CASE(OGS_SBI_RESOURCE_NAME_DEREG_NOTIFY)
                if (message->res_status < 300) {
                    message->DeregistrationData =
                        OpenAPI_deregistration_data_parseFromJSON(item);
                    if (!message->DeregistrationData) {
                        rv = OGS_ERROR;
                        ogs_error("JSON parse error");
                    }
                } else {
                    ogs_error("HTTP ERROR Status : %d", message->res_status);
                }
                break;

            CASE(OGS_SBI_RESOURCE_NAME_SDMSUBSCRIPTION_NOTIFY)
                if (message->res_status < 300) {
                    message->ModificationNotification =
                        OpenAPI_modification_notification_parseFromJSON(item);
                    if (!message->ModificationNotification) {
                        rv = OGS_ERROR;
                        ogs_error("JSON parse error");
                    }
                } else {
                    ogs_error("HTTP ERROR Status : %d", message->res_status);
                }
                break;

            DEFAULT
                rv = OGS_ERROR;
                ogs_error("Unknown resource name [%s]",
                        message->h.resource.component[1]);
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NSMF_CALLBACK)
            SWITCH(message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_N1_N2_FAILURE_NOTIFY)
                if (message->res_status < 300) {
                    message->N1N2MsgTxfrFailureNotification =
                        OpenAPI_n1_n2_msg_txfr_failure_notification_parseFromJSON(
                                item);
                    if (!message->N1N2MsgTxfrFailureNotification) {
                        rv = OGS_ERROR;
                        ogs_error("JSON parse error");
                    }
                } else {
                    ogs_error("HTTP ERROR Status : %d", message->res_status);
                }
                break;

            CASE(OGS_SBI_RESOURCE_NAME_SM_POLICY_NOTIFY)
                SWITCH(message->h.resource.component[2])
                CASE(OGS_SBI_RESOURCE_NAME_UPDATE)
                    if (message->res_status < 300) {
                        message->SmPolicyNotification =
                            OpenAPI_sm_policy_notification_parseFromJSON(item);
                        if (!message->SmPolicyNotification) {
                            rv = OGS_ERROR;
                            ogs_error("JSON parse error");
                        }
                    } else {
                        ogs_error("HTTP ERROR Status : %d",
                                message->res_status);
                    }
                    break;
                CASE(OGS_SBI_RESOURCE_NAME_TERMINATE)
                    if (message->res_status < 300) {
                        message->TerminationNotification =
                            OpenAPI_termination_notification_parseFromJSON(item);
                        if (!message->TerminationNotification) {
                            rv = OGS_ERROR;
                            ogs_error("JSON parse error");
                        }
                    } else {
                        ogs_error("HTTP ERROR Status : %d",
                                message->res_status);
                    }
                    break;

                DEFAULT
                    rv = OGS_ERROR;
                    ogs_error("Unknown resource name [%s]",
                            message->h.resource.component[2]);
                END
                break;

            CASE(OGS_SBI_RESOURCE_NAME_SDMSUBSCRIPTION_NOTIFY)
                if (message->res_status < 300) {
                    message->ModificationNotification =
                        OpenAPI_modification_notification_parseFromJSON(item);
                    if (!message->ModificationNotification) {
                        rv = OGS_ERROR;
                        ogs_error("JSON parse error");
                    }
                } else {
                    ogs_error("HTTP ERROR Status : %d", message->res_status);
                }
                break;

            DEFAULT
                rv = OGS_ERROR;
                ogs_error("Unknown resource name [%s]",
                        message->h.resource.component[0]);
            END
            break;

        DEFAULT
            rv = OGS_ERROR;
            ogs_error("Not implemented API name [%s]",
                    message->h.service.name);
        END
    }

cleanup:

    cJSON_Delete(item);
    return rv;
}

static int parse_content(
        ogs_sbi_message_t *message, ogs_sbi_http_message_t *http)
{
    ogs_assert(message);
    ogs_assert(http);

    if (message->http.content_type &&
        !strncmp(message->http.content_type, OGS_SBI_CONTENT_MULTIPART_TYPE,
            strlen(OGS_SBI_CONTENT_MULTIPART_TYPE))) {
        return parse_multipart(message, http);
    } else {
        return parse_json(message, message->http.content_type, http->content);
    }
}

static bool build_content(
        ogs_sbi_http_message_t *http, ogs_sbi_message_t *message)
{
    ogs_assert(message);
    ogs_assert(http);

    if (message->num_of_part) {
        if (build_multipart(http, message) == false) {
            ogs_error("build_multipart() failed");
            return false;
        }
    } else {
        http->content = build_json(message);
        if (http->content) {
            http->content_length = strlen(http->content);
            if (message->http.content_type) {
                ogs_sbi_header_set(http->headers,
                        OGS_SBI_CONTENT_TYPE, message->http.content_type);
            } else {
                ogs_sbi_header_set(http->headers,
                        OGS_SBI_CONTENT_TYPE, OGS_SBI_CONTENT_JSON_TYPE);
            }
        }
    }

    return true;
}

typedef struct multipart_parser_data_s {
    int num_of_part;
    struct {
        char *content_type;
        char *content_id;
        char *content;
        size_t content_length;
    } part[OGS_SBI_MAX_NUM_OF_PART];

    char *header_field;
} multipart_parser_data_t;

static int on_header_field(
        multipart_parser *parser, const char *at, size_t length)
{
    multipart_parser_data_t *data = NULL;

    ogs_assert(parser);
    data = multipart_parser_get_data(parser);
    ogs_assert(data);

    if (at && length) {
        if (data->header_field)
            ogs_free(data->header_field);
        data->header_field = ogs_strndup(at, length);
        ogs_assert(data->header_field);
    }
    return 0;
}

static int on_header_value(
        multipart_parser *parser, const char *at, size_t length)
{
    multipart_parser_data_t *data = NULL;

    ogs_assert(parser);
    data = multipart_parser_get_data(parser);
    ogs_assert(data);

    if (data->num_of_part < OGS_SBI_MAX_NUM_OF_PART && at && length) {
        if (!ogs_strcasecmp(data->header_field, OGS_SBI_CONTENT_TYPE)) {
            ogs_assert(data->part[data->num_of_part].content_type == NULL);
            data->part[data->num_of_part].content_type =
                ogs_strndup(at, length);
            ogs_assert(data->part[data->num_of_part].content_type);
        } else if (!ogs_strcasecmp(data->header_field, OGS_SBI_CONTENT_ID)) {
            ogs_assert(data->part[data->num_of_part].content_id == NULL);
            data->part[data->num_of_part].content_id =
                ogs_strndup(at, length);
            ogs_assert(data->part[data->num_of_part].content_id);
        } else {
            ogs_error("Unknown header field [%s]", data->header_field);
        }
    }

    return 0;
}

static int on_part_data(
        multipart_parser *parser, const char *at, size_t length)
{
    multipart_parser_data_t *data = NULL;

    ogs_assert(parser);
    data = multipart_parser_get_data(parser);
    ogs_assert(data);

    if (data->num_of_part < OGS_SBI_MAX_NUM_OF_PART && at && length) {
        SWITCH(data->part[data->num_of_part].content_type)
        CASE(OGS_SBI_CONTENT_JSON_TYPE)
        CASE(OGS_SBI_CONTENT_5GNAS_TYPE)
        CASE(OGS_SBI_CONTENT_NGAP_TYPE)
            size_t offset = 0;

            if (data->part[data->num_of_part].content == NULL) {
                data->part[data->num_of_part].content_length = length;
                data->part[data->num_of_part].content =
                    (char *)ogs_malloc(length + 1);
                ogs_assert(data->part[data->num_of_part].content);
            } else {
                offset = data->part[data->num_of_part].content_length;
                if ((data->part[data->num_of_part].content_length + length) >
                        OGS_MAX_SDU_LEN) {
                    ogs_error("Overflow length [%d:%d]",
                        (int)data->part[data->num_of_part].content_length,
                        (int)length);
                    ogs_assert_if_reached();
                    return 0;
                }
                data->part[data->num_of_part].content_length += length;
                data->part[data->num_of_part].content = (char *)ogs_realloc(
                    data->part[data->num_of_part].content,
                    data->part[data->num_of_part].content_length + 1);
                ogs_assert(data->part[data->num_of_part].content);
            }
            memcpy(data->part[data->num_of_part].content + offset, at, length);
            data->part[data->num_of_part].content[
                data->part[data->num_of_part].content_length] = 0;
            break;

        DEFAULT
            ogs_error("Unknown content_type [%s]",
                    data->part[data->num_of_part].content_type);
            ogs_log_hexdump(OGS_LOG_ERROR, (unsigned char *)at, length);
        END
    }
    return 0;
}

static int on_part_data_end(multipart_parser *parser)
{
    multipart_parser_data_t *data = NULL;

    ogs_assert(parser);
    data = multipart_parser_get_data(parser);
    ogs_assert(data);

    if (data->num_of_part < OGS_SBI_MAX_NUM_OF_PART) {
        data->num_of_part++;
    }

    return 0;
}

static int parse_multipart(
        ogs_sbi_message_t *message, ogs_sbi_http_message_t *http)
{
    char *boundary = NULL;
    int i, preamble;

    multipart_parser_settings settings;
    multipart_parser_data_t data;

    multipart_parser *parser = NULL;

    ogs_assert(message);
    ogs_assert(http);

    memset(&settings, 0, sizeof(settings));
    settings.on_header_field = &on_header_field;
    settings.on_header_value = &on_header_value;
    settings.on_part_data = &on_part_data;
    settings.on_part_data_end = &on_part_data_end;

    preamble = 0;
    if (http->content[0] == '\r' && http->content[1] == '\n')
        preamble = 2;

    for (i = preamble; i < (http->content_length-preamble); i++) {
        if (http->content[i] == '\r' && http->content[i+1] == '\n')
            break;
    }

    if (i >= (http->content_length-preamble)) {
        ogs_error("Invalid HTTP content [%d]", i);
        ogs_log_hexdump(OGS_LOG_ERROR,
                (unsigned char *)http->content, http->content_length);
        return OGS_ERROR;
    }

    boundary = ogs_strndup(http->content+preamble, i-preamble);
    ogs_assert(boundary);

    parser = multipart_parser_init(boundary, &settings);
    ogs_assert(parser);

    memset(&data, 0, sizeof(data));
    multipart_parser_set_data(parser, &data);
    multipart_parser_execute(parser,
            http->content+preamble, http->content_length-preamble);

    multipart_parser_free(parser);
    ogs_free(boundary);

    if (data.num_of_part > OGS_SBI_MAX_NUM_OF_PART) {
        /* Overflow Issues #1247 */
        ogs_fatal("Overflow num_of_part[%d]", data.num_of_part);
        ogs_assert_if_reached();
    }
    for (i = 0; i < data.num_of_part; i++) {
        SWITCH(data.part[i].content_type)
        CASE(OGS_SBI_CONTENT_JSON_TYPE)
            parse_json(message,
                    data.part[i].content_type, data.part[i].content);

            if (data.part[i].content_id)
                ogs_free(data.part[i].content_id);
            if (data.part[i].content_type)
                ogs_free(data.part[i].content_type);
            if (data.part[i].content)
                ogs_free(data.part[i].content);

            break;

        CASE(OGS_SBI_CONTENT_5GNAS_TYPE)
        CASE(OGS_SBI_CONTENT_NGAP_TYPE)
            http->part[http->num_of_part].content_id =
                data.part[i].content_id;
            http->part[http->num_of_part].content_type =
                data.part[i].content_type;
            http->part[http->num_of_part].pkbuf =
                ogs_pkbuf_alloc(NULL, data.part[i].content_length);
            if (!(http->part[http->num_of_part].pkbuf)) {
                ogs_error("ogs_pkbuf_copy() failed");

                if (data.part[i].content_id)
                    ogs_free(data.part[i].content_id);
                if (data.part[i].content_type)
                    ogs_free(data.part[i].content_type);
                if (data.part[i].content)
                    ogs_free(data.part[i].content);

                if (data.header_field)
                    ogs_free(data.header_field);

                return OGS_ERROR;
            }
            ogs_pkbuf_put_data(http->part[http->num_of_part].pkbuf,
                data.part[i].content, data.part[i].content_length);

            message->part[message->num_of_part].content_id =
                http->part[http->num_of_part].content_id;
            message->part[message->num_of_part].content_type =
                http->part[http->num_of_part].content_type;
            message->part[message->num_of_part].pkbuf =
                ogs_pkbuf_copy(http->part[http->num_of_part].pkbuf);
            if (!(message->part[http->num_of_part].pkbuf)) {
                ogs_error("ogs_pkbuf_copy() failed");

                if (data.part[i].content_id)
                    ogs_free(data.part[i].content_id);
                if (data.part[i].content_type)
                    ogs_free(data.part[i].content_type);
                if (data.part[i].content)
                    ogs_free(data.part[i].content);

                if (data.header_field)
                    ogs_free(data.header_field);

                if (http->part[http->num_of_part].pkbuf)
                    ogs_pkbuf_free(http->part[http->num_of_part].pkbuf);

                return OGS_ERROR;
            }

            http->num_of_part++;
            message->num_of_part++;

            if (data.part[i].content)
                ogs_free(data.part[i].content);
            break;

        DEFAULT
            ogs_error("Unknown content-type[%s]", data.part[i].content_type);

            if (data.part[i].content_id)
                ogs_free(data.part[i].content_id);
            if (data.part[i].content_type)
                ogs_free(data.part[i].content_type);
        END
    }

    if (data.header_field)
        ogs_free(data.header_field);

    return OGS_OK;
}

static bool build_multipart(
        ogs_sbi_http_message_t *http, ogs_sbi_message_t *message)
{
    int i;

    char boundary[32];
    unsigned char digest[16];
    char *p = NULL, *last;

    char *content_type = NULL;
    char *json = NULL;

    ogs_assert(message);
    ogs_assert(http);

    ogs_random(digest, 16);
    strcpy(boundary, "=-");
    ogs_base64_encode_binary(boundary + 2, digest, 16);

    p = http->content = ogs_calloc(1, OGS_MAX_SDU_LEN);
    if (!p) {
        ogs_error("ogs_calloc() failed");
        return false;
    }
    last = p + OGS_MAX_SDU_LEN;

#if SBI_MIME_PREAMBLE_CRLF /* Preamble CLRF */
    p = ogs_slprintf(p, last, "\r\n");
#endif

    /* First boundary */
    p = ogs_slprintf(p, last, "--%s\r\n", boundary);

    /* Encapsulated multipart part (application/json) */
    json = build_json(message);
    if (!json) {
        ogs_error("build_json() failed");
        return false;
    }

    p = ogs_slprintf(p, last, "%s\r\n\r\n%s",
            OGS_SBI_CONTENT_TYPE ": " OGS_SBI_CONTENT_JSON_TYPE, json);

    ogs_free(json);

    /* Add part */
    for (i = 0; i < message->num_of_part; i++) {
        p = ogs_slprintf(p, last, "\r\n--%s\r\n", boundary);
        p = ogs_slprintf(p, last, "%s: %s\r\n",
                OGS_SBI_CONTENT_ID, message->part[i].content_id);
        p = ogs_slprintf(p, last, "%s: %s\r\n\r\n",
                OGS_SBI_CONTENT_TYPE, message->part[i].content_type);
        memcpy(p, message->part[i].pkbuf->data, message->part[i].pkbuf->len);
        p += message->part[i].pkbuf->len;
    }

    /* Last boundary */
    p = ogs_slprintf(p, last, "\r\n--%s--\r\n", boundary);

    http->content_length = p - http->content;

    content_type = ogs_msprintf("%s; boundary=\"%s\"",
            OGS_SBI_CONTENT_MULTIPART_TYPE, boundary);
    if (!content_type) {
        ogs_error("ogs_msprintf() failed");
        return false;
    }

    ogs_sbi_header_set(http->headers, OGS_SBI_CONTENT_TYPE, content_type);

    ogs_free(content_type);

    return true;
}

static void http_message_free(ogs_sbi_http_message_t *http)
{
    int i;
    ogs_assert(http);

    if (http->params)
        ogs_sbi_http_hash_free(http->params);

    if (http->headers)
        ogs_sbi_http_hash_free(http->headers);

    if (http->content)
        ogs_free(http->content);

    for (i = 0; i < http->num_of_part; i++) {
        if (http->part[i].pkbuf)
            ogs_pkbuf_free(http->part[i].pkbuf);
        if (http->part[i].content_id)
            ogs_free(http->part[i].content_id);
        if (http->part[i].content_type)
            ogs_free(http->part[i].content_type);
    }
}

ogs_sbi_discovery_option_t *ogs_sbi_discovery_option_new(void)
{
    ogs_sbi_discovery_option_t *discovery_option = NULL;

    discovery_option = ogs_calloc(1, sizeof(*discovery_option));
    ogs_assert(discovery_option);

    return discovery_option;
}
void ogs_sbi_discovery_option_free(
        ogs_sbi_discovery_option_t *discovery_option)
{
    int i;

    ogs_assert(discovery_option);

    if (discovery_option->target_nf_instance_id)
        ogs_free(discovery_option->target_nf_instance_id);
    if (discovery_option->requester_nf_instance_id)
        ogs_free(discovery_option->requester_nf_instance_id);
    if (discovery_option->dnn)
        ogs_free(discovery_option->dnn);

    for (i = 0; i < discovery_option->num_of_service_names; i++)
        ogs_free(discovery_option->service_names[i]);

    ogs_free(discovery_option);
}

void ogs_sbi_discovery_option_set_target_nf_instance_id(
        ogs_sbi_discovery_option_t *discovery_option,
        char *target_nf_instance_id)
{
    ogs_assert(discovery_option);
    ogs_assert(target_nf_instance_id);

    ogs_assert(!discovery_option->target_nf_instance_id);
    discovery_option->target_nf_instance_id = ogs_strdup(target_nf_instance_id);
    ogs_assert(discovery_option->target_nf_instance_id);
}
void ogs_sbi_discovery_option_set_requester_nf_instance_id(
        ogs_sbi_discovery_option_t *discovery_option,
        char *requester_nf_instance_id)
{
    ogs_assert(discovery_option);
    ogs_assert(requester_nf_instance_id);

    ogs_assert(!discovery_option->requester_nf_instance_id);
    discovery_option->requester_nf_instance_id =
        ogs_strdup(requester_nf_instance_id);
    ogs_assert(discovery_option->requester_nf_instance_id);
}
void ogs_sbi_discovery_option_set_dnn(
        ogs_sbi_discovery_option_t *discovery_option, char *dnn)
{
    ogs_assert(discovery_option);
    ogs_assert(dnn);

    ogs_assert(!discovery_option->dnn);
    discovery_option->dnn = ogs_strdup(dnn);
    ogs_assert(discovery_option->dnn);
}

void ogs_sbi_discovery_option_add_service_names(
        ogs_sbi_discovery_option_t *discovery_option,
        char *service_name)
{
    ogs_assert(discovery_option);
    ogs_assert(service_name);

    ogs_assert(discovery_option->num_of_service_names <
                OGS_SBI_MAX_NUM_OF_SERVICE_TYPE);

    discovery_option->service_names[discovery_option->num_of_service_names] =
        ogs_strdup(service_name);
    ogs_assert(discovery_option->service_names
                [discovery_option->num_of_service_names]);
    discovery_option->num_of_service_names++;
}

char *ogs_sbi_discovery_option_build_service_names(
        ogs_sbi_discovery_option_t *discovery_option)
{
    int i;
    char *service_names = NULL;

    ogs_assert(discovery_option);

    service_names = ogs_strdup(discovery_option->service_names[0]);
    if (!service_names) {
        ogs_error("ogs_strdup() failed");
        return NULL;;
    }

    /*
     * Issues #1730
     * Send NF discovery query with service-names delimited with comma
     *
     * OpenAPI specification for sending NF discovery query with
     * "service-names" parameter is defined as folowing:
     *   name: service-names
     *   ...
     *   style: form
     *   explode: false
     *
     * According to OpenAPI specification, this means array items
     * should be delimited with a comma character (example: /users?id=3,4,5).
     *
     * See also https://swagger.io/docs/specification/serialization/
     */
    if (discovery_option->num_of_service_names > 1) {
        for (i = 1; i < discovery_option->num_of_service_names; i++)
            service_names = ogs_mstrcatf(
                    service_names, ",%s", discovery_option->service_names[i]);
    }

    return service_names;
}

void ogs_sbi_discovery_option_parse_service_names(
        ogs_sbi_discovery_option_t *discovery_option,
        char *service_names)
{
    char *v = NULL;
    char *token = NULL;
    char *saveptr = NULL;

    ogs_assert(discovery_option);
    ogs_assert(service_names);

    v = ogs_sbi_url_decode(service_names);
    if (!v) {
        ogs_error("ogs_sbi_url_decode() failed : service_names[%s]",
                service_names);
        return;
    }

    /*
     * Issues #1730
     * Send NF discovery query with service-names delimited with comma
     *
     * OpenAPI specification for sending NF discovery query with
     * "service-names" parameter is defined as folowing:
     *   name: service-names
     *   ...
     *   style: form
     *   explode: false
     *
     * According to OpenAPI specification, this means array items
     * should be delimited with a comma character (example: /users?id=3,4,5).
     *
     * See also https://swagger.io/docs/specification/serialization/
     */
    token = ogs_strtok_r(v, ",", &saveptr);
    while (token != NULL) {
        ogs_sbi_discovery_option_add_service_names(discovery_option, token);
        token = ogs_strtok_r(NULL, ",", &saveptr);
    }

    ogs_free(v);
}

void ogs_sbi_discovery_option_add_snssais(
        ogs_sbi_discovery_option_t *discovery_option, ogs_s_nssai_t *s_nssai)
{
    ogs_assert(discovery_option);
    ogs_assert(s_nssai);

    ogs_assert(discovery_option->num_of_snssais < OGS_MAX_NUM_OF_SLICE);

    memcpy(&discovery_option->snssais[discovery_option->num_of_snssais],
            s_nssai, sizeof(ogs_s_nssai_t));
    discovery_option->num_of_snssais++;
}
char *ogs_sbi_discovery_option_build_snssais(
        ogs_sbi_discovery_option_t *discovery_option)
{
    cJSON *item = NULL;
    char *v = NULL;
    int i;

    ogs_assert(discovery_option);

    item = cJSON_CreateArray();
    if (!item) {
        ogs_error("cJSON_CreateArray() failed");
        return NULL;
    }

    for (i = 0; i < discovery_option->num_of_snssais; i++) {
        OpenAPI_snssai_t sNSSAI;
        cJSON *snssaiItem = NULL;

        memset(&sNSSAI, 0, sizeof(sNSSAI));

        sNSSAI.sst = discovery_option->snssais[i].sst;
        sNSSAI.sd = ogs_s_nssai_sd_to_string(discovery_option->snssais[i].sd);

        snssaiItem = OpenAPI_snssai_convertToJSON(&sNSSAI);
        ogs_assert(snssaiItem);
        cJSON_AddItemToArray(item, snssaiItem);

        if (sNSSAI.sd)
            ogs_free(sNSSAI.sd);
    }

    v = cJSON_PrintUnformatted(item);
    ogs_expect(v);
    cJSON_Delete(item);

    return v;
}
void ogs_sbi_discovery_option_parse_snssais(
        ogs_sbi_discovery_option_t *discovery_option, char *snssais)
{
    cJSON *item = NULL;
    cJSON *snssaiItem = NULL;
    char *v = NULL;

    ogs_assert(discovery_option);
    ogs_assert(snssais);

    v = ogs_sbi_url_decode(snssais);
    if (!v) {
        ogs_error("ogs_sbi_url_decode() failed : snssais[%s]", snssais);
        return;
    }

    item = cJSON_Parse(v);
    if (!item) {
        ogs_error("Cannot parse snssais[%s]", snssais);
        ogs_free(v);
        return;
    }

    cJSON_ArrayForEach(snssaiItem, item) {
        if (cJSON_IsObject(snssaiItem)) {
            OpenAPI_snssai_t *sNSSAI = OpenAPI_snssai_parseFromJSON(snssaiItem);

            if (sNSSAI) {
                ogs_s_nssai_t s_nssai;

                s_nssai.sst = sNSSAI->sst;
                s_nssai.sd = ogs_s_nssai_sd_from_string(sNSSAI->sd);

                ogs_sbi_discovery_option_add_snssais(
                        discovery_option, &s_nssai);

                OpenAPI_snssai_free(sNSSAI);
            } else {
                ogs_error("OpenAPI_snssai_parseFromJSON() failed : snssais[%s]",
                        snssais);
            }
        } else {
            ogs_error("Invalid cJSON Type in snssias[%s]", snssais);
        }
    }
    cJSON_Delete(item);

    ogs_free(v);
}

void ogs_sbi_discovery_option_set_guami(
        ogs_sbi_discovery_option_t *discovery_option, ogs_guami_t *guami)
{
    ogs_assert(discovery_option);
    ogs_assert(guami);

    ogs_assert(discovery_option->guami_presence == false);

    memcpy(&discovery_option->guami, guami, sizeof(ogs_guami_t));
    discovery_option->guami_presence = true;
}

char *ogs_sbi_discovery_option_build_guami(
        ogs_sbi_discovery_option_t *discovery_option)
{
    OpenAPI_guami_t *Guami = NULL;
    cJSON *guamiItem = NULL;
    char *v = NULL;

    ogs_assert(discovery_option);
    ogs_assert(discovery_option->guami_presence);

    Guami = ogs_sbi_build_guami(&discovery_option->guami);
    ogs_assert(Guami);
    guamiItem = OpenAPI_guami_convertToJSON(Guami);
    ogs_assert(guamiItem);
    ogs_sbi_free_guami(Guami);

    v = cJSON_PrintUnformatted(guamiItem);
    ogs_expect(v);
    cJSON_Delete(guamiItem);

    return v;
}

void ogs_sbi_discovery_option_parse_guami(
        ogs_sbi_discovery_option_t *discovery_option, char *guami)
{
    OpenAPI_guami_t *Guami = NULL;
    cJSON *guamItem = NULL;
    char *v = NULL;

    ogs_assert(discovery_option);
    ogs_assert(guami);

    v = ogs_sbi_url_decode(guami);
    if (!v) {
        ogs_error("ogs_sbi_url_decode() failed : guami[%s]", guami);
        return;
    }

    guamItem = cJSON_Parse(v);
    if (!guamItem) {
        ogs_error("Cannot parse guami[%s]", guami);
        ogs_free(v);
        return;
    }

    Guami = OpenAPI_guami_parseFromJSON(guamItem);

    if (Guami) {
        ogs_guami_t guami;
        ogs_sbi_parse_guami(&guami, Guami);
        ogs_sbi_discovery_option_set_guami(discovery_option, &guami);
        OpenAPI_guami_free(Guami);
    } else {
        ogs_error("OpenAPI_guami_parseFromJSON() failed : guami[%s]",
                guami);
    }
    cJSON_Delete(guamItem);

    ogs_free(v);
}

void ogs_sbi_discovery_option_set_tai(
        ogs_sbi_discovery_option_t *discovery_option, ogs_5gs_tai_t *tai)
{
    ogs_assert(discovery_option);
    ogs_assert(tai);

    ogs_assert(discovery_option->tai_presence == false);

    memcpy(&discovery_option->tai, tai, sizeof(ogs_5gs_tai_t));
    discovery_option->tai_presence = true;
}
char *ogs_sbi_discovery_option_build_tai(
        ogs_sbi_discovery_option_t *discovery_option)
{
    OpenAPI_tai_t Tai;
    cJSON *taiItem = NULL;
    char *v = NULL;

    ogs_assert(discovery_option);
    ogs_assert(discovery_option->tai_presence);

    memset(&Tai, 0, sizeof(Tai));

    Tai.plmn_id = ogs_sbi_build_plmn_id(&discovery_option->tai.plmn_id);
    ogs_assert(Tai.plmn_id);
    Tai.tac = ogs_uint24_to_0string(discovery_option->tai.tac);
    ogs_assert(Tai.tac);

    taiItem = OpenAPI_tai_convertToJSON(&Tai);
    ogs_assert(taiItem);

    ogs_sbi_free_plmn_id(Tai.plmn_id);
    ogs_free(Tai.tac);

    v = cJSON_PrintUnformatted(taiItem);
    ogs_expect(v);
    cJSON_Delete(taiItem);

    return v;
}
void ogs_sbi_discovery_option_parse_tai(
        ogs_sbi_discovery_option_t *discovery_option, char *tai)
{
    cJSON *taiItem = NULL;
    char *v = NULL;

    ogs_assert(discovery_option);
    ogs_assert(tai);

    v = ogs_sbi_url_decode(tai);
    if (!v) {
        ogs_error("ogs_sbi_url_decode() failed : tai[%s]", tai);
        return;
    }

    taiItem = cJSON_Parse(v);
    if (!taiItem) {
        ogs_error("Cannot parse tai[%s]", tai);
        ogs_free(v);
        return;
    }

    if (cJSON_IsObject(taiItem)) {
        OpenAPI_tai_t *Tai = OpenAPI_tai_parseFromJSON(taiItem);

        if (Tai) {
            ogs_5gs_tai_t tai;

            memset(&tai, 0, sizeof(tai));

            if (Tai->plmn_id)
                ogs_sbi_parse_plmn_id(&tai.plmn_id, Tai->plmn_id);
            if (Tai->tac)
                tai.tac = ogs_uint24_from_string_hexadecimal(Tai->tac);

            ogs_sbi_discovery_option_set_tai(discovery_option, &tai);

            OpenAPI_tai_free(Tai);
        } else {
            ogs_error("OpenAPI_snssai_parseFromJSON() failed : tai[%s]",
                    tai);
        }
    } else {
        ogs_error("Invalid cJSON Type in snssias[%s]", tai);
    }
    cJSON_Delete(taiItem);

    ogs_free(v);
}

void ogs_sbi_discovery_option_add_target_plmn_list(
        ogs_sbi_discovery_option_t *discovery_option,
        ogs_plmn_id_t *target_plmn_id)
{
    ogs_assert(discovery_option);
    ogs_assert(target_plmn_id);

    ogs_assert(discovery_option->num_of_target_plmn_list <
                OGS_MAX_NUM_OF_PLMN);

    memcpy(&discovery_option->target_plmn_list[
        discovery_option->num_of_target_plmn_list],
        target_plmn_id, OGS_PLMN_ID_LEN);
    discovery_option->num_of_target_plmn_list++;
}

void ogs_sbi_discovery_option_add_requester_plmn_list(
        ogs_sbi_discovery_option_t *discovery_option,
        ogs_plmn_id_t *requester_plmn_id)
{
    ogs_assert(discovery_option);
    ogs_assert(requester_plmn_id);

    ogs_assert(discovery_option->num_of_requester_plmn_list <
                OGS_MAX_NUM_OF_PLMN);

    memcpy(&discovery_option->requester_plmn_list[
        discovery_option->num_of_requester_plmn_list],
        requester_plmn_id, OGS_PLMN_ID_LEN);
    discovery_option->num_of_requester_plmn_list++;
}

char *ogs_sbi_discovery_option_build_plmn_list(
        ogs_plmn_id_t *plmn_list, int num_of_plmn_list)
{
    OpenAPI_list_t *PlmnList = NULL;
    OpenAPI_plmn_id_t *PlmnId = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *item = NULL;

    char *v = NULL;

    ogs_assert(plmn_list);
    ogs_assert(num_of_plmn_list);

    PlmnList = ogs_sbi_build_plmn_list(plmn_list, num_of_plmn_list);
    ogs_assert(PlmnList);

    item = cJSON_CreateArray();
    ogs_assert(item);

    OpenAPI_list_for_each(PlmnList, node) {
        PlmnId = node->data;
        if (PlmnId) {
            cJSON *PlmnIdItem = OpenAPI_plmn_id_convertToJSON(PlmnId);
            ogs_assert(PlmnIdItem);
            cJSON_AddItemToArray(item, PlmnIdItem);
        }
    }

    v = cJSON_PrintUnformatted(item);
    ogs_assert(v);

    cJSON_Delete(item);

    ogs_sbi_free_plmn_list(PlmnList);

    return v;
}

int ogs_sbi_discovery_option_parse_plmn_list(
        ogs_plmn_id_t *plmn_list, char *v)
{
    cJSON *item = NULL;
    cJSON *PlmnIdJSON = NULL;
    OpenAPI_list_t *PlmnList = NULL;
    OpenAPI_plmn_id_t *PlmnId = NULL;
    int num_of_plmn_list = 0;

    ogs_assert(v);
    ogs_assert(plmn_list);

    item = cJSON_Parse(v);
    if (item) {
        PlmnList = OpenAPI_list_create();
        ogs_assert(PlmnList);
        cJSON_ArrayForEach(PlmnIdJSON, item) {
            if (!cJSON_IsObject(PlmnIdJSON)) {
                ogs_error("Unknown JSON");
                goto cleanup;
            }

            PlmnId = OpenAPI_plmn_id_parseFromJSON(PlmnIdJSON);
            if (!PlmnId) {
                ogs_error("No PlmnId");
                goto cleanup;
            }

            OpenAPI_list_add(PlmnList, PlmnId);
        }
    }

    num_of_plmn_list = ogs_sbi_parse_plmn_list(plmn_list, PlmnList);

cleanup:
    ogs_sbi_free_plmn_list(PlmnList);
    cJSON_Delete(item);

    return num_of_plmn_list;
}
