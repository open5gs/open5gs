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

#include "ogs-sbi.h"
#include "ogs-crypt.h"
#include "yuarel.h"

#include "contrib/multipart_parser.h"

static OGS_POOL(request_pool, ogs_sbi_request_t);
static OGS_POOL(response_pool, ogs_sbi_response_t);

static char *build_json(ogs_sbi_message_t *message);
static int parse_json(ogs_sbi_message_t *message,
        char *content_type, char *json);

static void build_content(
        ogs_sbi_http_message_t *http, ogs_sbi_message_t *message);
static int parse_content(
        ogs_sbi_message_t *message, ogs_sbi_http_message_t *http);

static void build_multipart(
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
    if (message->SessionManagementSubscriptionData)
        OpenAPI_session_management_subscription_data_free(
                message->SessionManagementSubscriptionData);
    if (message->N1N2MessageTransferReqData)
        OpenAPI_n1_n2_message_transfer_req_data_free(
                message->N1N2MessageTransferReqData);
    if (message->N1N2MessageTransferRspData)
        OpenAPI_n1_n2_message_transfer_rsp_data_free(
                message->N1N2MessageTransferRspData);
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

    for (i = 0; i < message->num_of_part; i++) {
        if (message->part[i].pkbuf)
            ogs_pkbuf_free(message->part[i].pkbuf);
    }
}

ogs_sbi_request_t *ogs_sbi_request_new(void)
{
    ogs_sbi_request_t *request = NULL;

    ogs_pool_alloc(&request_pool, &request);

    ogs_assert(request);
    memset(request, 0, sizeof(ogs_sbi_request_t));

    request->http.params = ogs_hash_make();
    request->http.headers = ogs_hash_make();

    return request;
}

ogs_sbi_response_t *ogs_sbi_response_new(void)
{
    ogs_sbi_response_t *response = NULL;

    ogs_pool_alloc(&response_pool, &response);
    ogs_assert(response);
    memset(response, 0, sizeof(ogs_sbi_response_t));

    response->http.params = ogs_hash_make();
    response->http.headers = ogs_hash_make();

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
    ogs_sbi_request_t *request = NULL;

    ogs_assert(message);

    request = ogs_sbi_request_new();
    ogs_assert(request);

    ogs_assert(message->h.method);
    request->h.method = ogs_strdup(message->h.method);
    if (message->h.uri) {
        request->h.uri = ogs_strdup(message->h.uri);
    } else {
        int i;

        ogs_assert(message->h.service.name);
        request->h.service.name = ogs_strdup(message->h.service.name);
        ogs_assert(message->h.api.version);
        request->h.api.version = ogs_strdup(message->h.api.version);

        ogs_assert(message->h.resource.component[0]);
        for (i = 0; i < OGS_SBI_MAX_NUM_OF_RESOURCE_COMPONENT &&
                            message->h.resource.component[i]; i++)
            request->h.resource.component[i] = ogs_strdup(
                    message->h.resource.component[i]);
    }

    /* URL Param */
    if (message->param.nf_type) {
        char *v = OpenAPI_nf_type_ToString(message->param.nf_type);
        ogs_assert(v);
        ogs_sbi_header_set(request->http.params, OGS_SBI_PARAM_NF_TYPE, v);
    }
    if (message->param.requester_nf_type) {
        char *v = OpenAPI_nf_type_ToString(message->param.requester_nf_type);
        ogs_assert(v);
        ogs_sbi_header_set(request->http.params,
                OGS_SBI_PARAM_REQUESTER_NF_TYPE, v);
    }
    if (message->param.target_nf_type) {
        char *v = OpenAPI_nf_type_ToString(message->param.target_nf_type);
        ogs_assert(v);
        ogs_sbi_header_set(request->http.params,
                OGS_SBI_PARAM_TARGET_NF_TYPE, v);
    }
    if (message->param.limit) {
        char *v = ogs_msprintf("%d", message->param.limit);
        ogs_assert(v);
        ogs_sbi_header_set(request->http.params, OGS_SBI_PARAM_LIMIT, v);
        ogs_free(v);
    }
    if (message->param.dnn) {
        char *v = ogs_msprintf("%s", message->param.dnn);
        ogs_assert(v);
        ogs_sbi_header_set(request->http.params, OGS_SBI_PARAM_DNN, v);
        ogs_free(v);
    }
    if (message->param.plmn_id_presence) {
        OpenAPI_plmn_id_t plmn_id;

        plmn_id.mnc = ogs_plmn_id_mnc_string(&message->param.plmn_id);
        plmn_id.mcc = ogs_plmn_id_mcc_string(&message->param.plmn_id);

        if (plmn_id.mnc && plmn_id.mcc) {
            char *v = NULL;
            cJSON *item = NULL;

            item = OpenAPI_plmn_id_convertToJSON(&plmn_id);
            ogs_assert(item);
            if (plmn_id.mnc) ogs_free(plmn_id.mnc);
            if (plmn_id.mcc) ogs_free(plmn_id.mcc);

            v = cJSON_Print(item);
            ogs_assert(v);
            cJSON_Delete(item);

            ogs_sbi_header_set(request->http.params, OGS_SBI_PARAM_PLMN_ID, v);
            ogs_free(v);
        }
    }
    if (message->param.single_nssai_presence) {
        char *v = ogs_sbi_s_nssai_to_string(&message->param.single_nssai);
        ogs_sbi_header_set(request->http.params, OGS_SBI_PARAM_SINGLE_NSSAI, v);
        ogs_free(v);
    }
    if (message->param.snssai_presence) {
        char *v = ogs_sbi_s_nssai_to_string(&message->param.snssai);
        ogs_sbi_header_set(request->http.params, OGS_SBI_PARAM_SNSSAI, v);
        ogs_free(v);
    }

    build_content(&request->http, message);

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

    if (message->http.content_encoding)
        ogs_sbi_header_set(request->http.headers,
                OGS_SBI_ACCEPT_ENCODING, message->http.content_encoding);

    return request;
}

ogs_sbi_response_t *ogs_sbi_build_response(
        ogs_sbi_message_t *message, int status)
{
    ogs_sbi_response_t *response = NULL;

    ogs_assert(message);

    response = ogs_sbi_response_new();
    ogs_assert(response);

    response->status = status;

    if (response->status != OGS_SBI_HTTP_STATUS_NO_CONTENT) {
        build_content(&response->http, message);
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

    ogs_assert(request);
    ogs_assert(message);

    rv = ogs_sbi_parse_header(message, &request->h);
    if (rv != OGS_OK) {
        ogs_error("ogs_sbi_parse_header() failed");
        return OGS_ERROR;
    }

    for (hi = ogs_hash_first(request->http.params);
            hi; hi = ogs_hash_next(hi)) {
        if (!strcmp(ogs_hash_this_key(hi), OGS_SBI_PARAM_NF_TYPE)) {
            message->param.nf_type =
                OpenAPI_nf_type_FromString(ogs_hash_this_val(hi));
        } else if (!strcmp(ogs_hash_this_key(hi),
                    OGS_SBI_PARAM_TARGET_NF_TYPE)) {
            message->param.target_nf_type =
                OpenAPI_nf_type_FromString(ogs_hash_this_val(hi));
        } else if (!strcmp(ogs_hash_this_key(hi),
                    OGS_SBI_PARAM_REQUESTER_NF_TYPE)) {
            message->param.requester_nf_type =
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
                bool rc = ogs_sbi_s_nssai_from_string(
                        &message->param.single_nssai, v);
                if (rc == true)
                    message->param.single_nssai_presence = true;
            }
        } else if (!strcmp(ogs_hash_this_key(hi), OGS_SBI_PARAM_SNSSAI)) {
            char *v = ogs_hash_this_val(hi);
            if (v) {
                bool rc = ogs_sbi_s_nssai_from_string(
                        &message->param.snssai, v);
                if (rc == true)
                    message->param.snssai_presence = true;
            }
        }
    }

    for (hi = ogs_hash_first(request->http.headers);
            hi; hi = ogs_hash_next(hi)) {
        if (!ogs_strcasecmp(ogs_hash_this_key(hi), OGS_SBI_ACCEPT_ENCODING)) {
            message->http.content_encoding = ogs_hash_this_val(hi);
        } else if (!ogs_strcasecmp(
                    ogs_hash_this_key(hi), OGS_SBI_CONTENT_TYPE)) {
            message->http.content_type = ogs_hash_this_val(hi);
        } else if (!ogs_strcasecmp(ogs_hash_this_key(hi), OGS_SBI_ACCEPT)) {
            message->http.accept = ogs_hash_this_val(hi);
        }
    }

    if (parse_content(message, &request->http) != OGS_OK) {
        ogs_error("parse_content() failed");
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

void ogs_sbi_header_set(ogs_hash_t *ht, const void *key, const void *val)
{
    ogs_hash_set(ht, key, strlen(key), ogs_strdup(val));
}

void *ogs_sbi_header_get(ogs_hash_t *ht, const void *key)
{
    return ogs_hash_get(ht, key, strlen(key));
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

int ogs_sbi_parse_header(
        ogs_sbi_message_t *message, ogs_sbi_header_t *header)
{
    struct yuarel yuarel;
    char *saveptr = NULL;
    char *uri = NULL, *p = NULL;;

    char *component = NULL;
    int i = 0;

    ogs_assert(message);
    ogs_assert(header);

    memset(message, 0, sizeof(*message));

    message->h.method = header->method;
    message->h.uri = header->uri;
    ogs_assert(message->h.uri);
    ogs_debug("[%s] %s", message->h.method ? message->h.method : "Notify",
            message->h.uri);

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
    } else if (message->SessionManagementSubscriptionData) {
        item = OpenAPI_session_management_subscription_data_convertToJSON(
                message->SessionManagementSubscriptionData);
        ogs_assert(item);
    } else if (message->N1N2MessageTransferReqData) {
        item = OpenAPI_n1_n2_message_transfer_req_data_convertToJSON(
                message->N1N2MessageTransferReqData);
        ogs_assert(item);
    } else if (message->N1N2MessageTransferRspData) {
        item = OpenAPI_n1_n2_message_transfer_rsp_data_convertToJSON(
                message->N1N2MessageTransferRspData);
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
    }

    if (item) {
        content = cJSON_Print(item);
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
        ogs_error("JSON parse error");
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
                OpenAPI_list_add(message->PatchItemList, patch_item);
            }
        }
    } else {
        SWITCH(message->h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_NNRF_NFM)

            SWITCH(message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_NF_INSTANCES)
                message->NFProfile =
                    OpenAPI_nf_profile_parseFromJSON(item);
                if (!message->NFProfile) {
                    rv = OGS_ERROR;
                    ogs_error("JSON parse error");
                }
                break;

            CASE(OGS_SBI_RESOURCE_NAME_SUBSCRIPTIONS)
                message->SubscriptionData =
                    OpenAPI_subscription_data_parseFromJSON(item);
                if (!message->SubscriptionData) {
                    rv = OGS_ERROR;
                    ogs_error("JSON parse error");
                }
                break;

            CASE(OGS_SBI_RESOURCE_NAME_NF_STATUS_NOTIFY)
                message->NotificationData =
                    OpenAPI_notification_data_parseFromJSON(item);
                if (!message->NotificationData) {
                    rv = OGS_ERROR;
                    ogs_error("JSON parse error");
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
                message->SearchResult =
                    OpenAPI_search_result_parseFromJSON(item);
                if (!message->SearchResult) {
                    rv = OGS_ERROR;
                    ogs_error("JSON parse error");
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
                message->AuthEvent = OpenAPI_auth_event_parseFromJSON(item);
                if (!message->AuthEvent) {
                    rv = OGS_ERROR;
                    ogs_error("JSON parse error");
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
                    message->Amf3GppAccessRegistration =
                        OpenAPI_amf3_gpp_access_registration_parseFromJSON(
                                item);
                    if (!message->Amf3GppAccessRegistration) {
                        rv = OGS_ERROR;
                        ogs_error("JSON parse error");
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
            CASE(OGS_SBI_RESOURCE_NAME_AM_DATA)
                message->AccessAndMobilitySubscriptionData =
                    OpenAPI_access_and_mobility_subscription_data_parseFromJSON(
                            item);
                if (!message->AccessAndMobilitySubscriptionData) {
                    rv = OGS_ERROR;
                    ogs_error("JSON parse error");
                }
                break;

            CASE(OGS_SBI_RESOURCE_NAME_SMF_SELECT_DATA)
                message->SmfSelectionSubscriptionData =
                    OpenAPI_smf_selection_subscription_data_parseFromJSON(item);
                if (!message->SmfSelectionSubscriptionData) {
                    rv = OGS_ERROR;
                    ogs_error("JSON parse error");
                }
                break;

            CASE(OGS_SBI_RESOURCE_NAME_UE_CONTEXT_IN_SMF_DATA)
                message->UeContextInSmfData =
                    OpenAPI_ue_context_in_smf_data_parseFromJSON(item);
                if (!message->UeContextInSmfData) {
                    rv = OGS_ERROR;
                    ogs_error("JSON parse error");
                }
                break;

            CASE(OGS_SBI_RESOURCE_NAME_SM_DATA)
                message->SessionManagementSubscriptionData =
                    OpenAPI_session_management_subscription_data_parseFromJSON(
                            item);
                if (!message->SessionManagementSubscriptionData) {
                    rv = OGS_ERROR;
                    ogs_error("JSON parse error");
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
                        message->AuthEvent =
                            OpenAPI_auth_event_parseFromJSON(item);
                        if (!message->AuthEvent) {
                            rv = OGS_ERROR;
                            ogs_error("JSON parse error");
                        }
                        break;
                    DEFAULT
                        rv = OGS_ERROR;
                        ogs_error("Unknown resource name [%s]",
                                message->h.resource.component[3]);
                    END
                    break;

                CASE(OGS_SBI_RESOURCE_NAME_CONTEXT_DATA)
                    message->Amf3GppAccessRegistration =
                        OpenAPI_amf3_gpp_access_registration_parseFromJSON(
                                item);
                    if (!message->Amf3GppAccessRegistration) {
                        rv = OGS_ERROR;
                        ogs_error("JSON parse error");
                    }
                    break;

                DEFAULT
                    SWITCH(message->h.resource.component[3])
                    CASE(OGS_SBI_RESOURCE_NAME_PROVISIONED_DATA)
                        SWITCH(message->h.resource.component[4])
                        CASE(OGS_SBI_RESOURCE_NAME_AM_DATA)
                            message->AccessAndMobilitySubscriptionData =
                                OpenAPI_access_and_mobility_subscription_data_parseFromJSON(item);
                            if (!message->AccessAndMobilitySubscriptionData) {
                                rv = OGS_ERROR;
                                ogs_error("JSON parse error");
                            }
                            break;

                        CASE(OGS_SBI_RESOURCE_NAME_SMF_SELECT_DATA)
                            message->SmfSelectionSubscriptionData =
                                OpenAPI_smf_selection_subscription_data_parseFromJSON(item);
                            if (!message->SmfSelectionSubscriptionData) {
                                rv = OGS_ERROR;
                                ogs_error("JSON parse error");
                            }
                            break;

                        CASE(OGS_SBI_RESOURCE_NAME_UE_CONTEXT_IN_SMF_DATA)
                            message->UeContextInSmfData =
                                OpenAPI_ue_context_in_smf_data_parseFromJSON(
                                        item);
                            if (!message->UeContextInSmfData) {
                                rv = OGS_ERROR;
                                ogs_error("JSON parse error");
                            }
                            break;

                        CASE(OGS_SBI_RESOURCE_NAME_SM_DATA)
                            message->SessionManagementSubscriptionData =
                                OpenAPI_session_management_subscription_data_parseFromJSON(item);
                            if (!message->SessionManagementSubscriptionData) {
                                rv = OGS_ERROR;
                                ogs_error("JSON parse error");
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

                        message->AmPolicyData =
                            OpenAPI_am_policy_data_parseFromJSON(item);
                        if (!message->AmPolicyData) {
                            rv = OGS_ERROR;
                            ogs_error("JSON parse error");
                        }
                        break;

                    CASE(OGS_SBI_RESOURCE_NAME_SM_DATA)

                        message->SmPolicyData =
                            OpenAPI_sm_policy_data_parseFromJSON(item);
                        if (!message->SmPolicyData) {
                            rv = OGS_ERROR;
                            ogs_error("JSON parse error");
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
            SWITCH(message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_UE_CONTEXTS)
                SWITCH(message->h.resource.component[2])
                CASE(OGS_SBI_RESOURCE_NAME_N1_N2_MESSAGES)
                    if (message->res_status == 0) {
                        message->N1N2MessageTransferReqData =
                            OpenAPI_n1_n2_message_transfer_req_data_parseFromJSON(item);
                        if (!message->N1N2MessageTransferReqData) {
                            rv = OGS_ERROR;
                            ogs_error("JSON parse error");
                        }
                    } else if (message->res_status ==
                                OGS_SBI_HTTP_STATUS_OK ||
                                message->res_status ==
                                    OGS_SBI_HTTP_STATUS_ACCEPTED) {
                        message->N1N2MessageTransferRspData =
                            OpenAPI_n1_n2_message_transfer_rsp_data_parseFromJSON(item);
                        if (!message->N1N2MessageTransferRspData) {
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

            DEFAULT
                rv = OGS_ERROR;
                ogs_error("Unknown resource name [%s]",
                        message->h.resource.component[0]);
            END
            break;
        CASE(OGS_SBI_SERVICE_NAME_NPCF_AM_POLICY_CONTROL)
            SWITCH(message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_POLICIES)
                if (message->res_status == 0) {
                    message->PolicyAssociationRequest =
                        OpenAPI_policy_association_request_parseFromJSON(
                                item);
                    if (!message->PolicyAssociationRequest) {
                        rv = OGS_ERROR;
                        ogs_error("JSON parse error");
                    }
                } else if (message->res_status == OGS_SBI_HTTP_STATUS_CREATED) {
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
                if (message->res_status == 0) {
                    message->SmPolicyContextData =
                        OpenAPI_sm_policy_context_data_parseFromJSON(item);
                    if (!message->SmPolicyContextData) {
                        rv = OGS_ERROR;
                        ogs_error("JSON parse error");
                    }
                } else if (message->res_status == OGS_SBI_HTTP_STATUS_CREATED) {
                    message->SmPolicyDecision =
                        OpenAPI_sm_policy_decision_parseFromJSON(item);
                    if (!message->SmPolicyDecision) {
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

        CASE(OGS_SBI_SERVICE_NAME_NAMF_CALLBACK)
            SWITCH(message->h.resource.component[1])
            CASE(OGS_SBI_RESOURCE_NAME_SM_CONTEXT_STATUS)
                message->SmContextStatusNotification =
                    OpenAPI_sm_context_status_notification_parseFromJSON(item);
                if (!message->SmContextStatusNotification) {
                    rv = OGS_ERROR;
                    ogs_error("JSON parse error");
                }
                break;

            DEFAULT
                rv = OGS_ERROR;
                ogs_error("Unknown resource name [%s]",
                        message->h.resource.component[1]);
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

static void build_content(
        ogs_sbi_http_message_t *http, ogs_sbi_message_t *message)
{
    ogs_assert(message);
    ogs_assert(http);

    if (message->num_of_part) {
        build_multipart(http, message);
    } else {
        http->content = build_json(message);
        if (http->content) {
            http->content_length = strlen(http->content);
            if (message->http.content_type)
                ogs_sbi_header_set(http->headers,
                        OGS_SBI_CONTENT_TYPE, message->http.content_type);
            else
                ogs_sbi_header_set(http->headers,
                        OGS_SBI_CONTENT_TYPE, OGS_SBI_CONTENT_JSON_TYPE);
        }
    }
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

    if (at && length) {
        SWITCH(data->header_field)
        CASE(OGS_SBI_CONTENT_TYPE)
            if (data->part[data->num_of_part].content_type)
                ogs_free(data->part[data->num_of_part].content_type);
            data->part[data->num_of_part].content_type =
                ogs_strndup(at, length);
            break;
        CASE(OGS_SBI_CONTENT_ID)
            if (data->part[data->num_of_part].content_id)
                ogs_free(data->part[data->num_of_part].content_id);
            data->part[data->num_of_part].content_id =
                ogs_strndup(at, length);
            break;

        DEFAULT
            ogs_error("Unknown header field [%s]", data->header_field);
        END
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

    if (at && length) {
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
                        OGS_HUGE_LEN) {
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
            ogs_log_hexdump(OGS_LOG_FATAL, (unsigned char *)at, length);
            ogs_error("Unknown content_type [%s]",
                    data->part[data->num_of_part].content_type);
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

    data->num_of_part++;

    return 0;
}

static int parse_multipart(
        ogs_sbi_message_t *message, ogs_sbi_http_message_t *http)
{
    char *boundary = NULL;
    int i;

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

    for (i = 0; i < http->content_length; i++) {
        if (http->content[i] == '\r' && http->content[i+1] == '\n')
            break;
    }

    if (i >= http->content_length) {
        ogs_error("Invalid HTTP content [%d]", i);
        ogs_log_hexdump(OGS_LOG_ERROR,
                (unsigned char *)http->content, http->content_length);
        return OGS_ERROR;
    }

    boundary = ogs_strndup(http->content, i);
    ogs_assert(boundary);

    parser = multipart_parser_init(boundary, &settings);
    ogs_assert(parser);

    memset(&data, 0, sizeof(data));
    multipart_parser_set_data(parser, &data);
    multipart_parser_execute(parser, http->content, http->content_length);

    multipart_parser_free(parser);
    ogs_free(boundary);

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
            ogs_assert(http->part[http->num_of_part].pkbuf);
            ogs_pkbuf_put_data(http->part[http->num_of_part].pkbuf,
                data.part[i].content, data.part[i].content_length);

            message->part[message->num_of_part].content_id =
                http->part[http->num_of_part].content_id;
            message->part[message->num_of_part].content_type =
                http->part[http->num_of_part].content_type;
            message->part[message->num_of_part].pkbuf =
                ogs_pkbuf_copy(http->part[http->num_of_part].pkbuf);
            ogs_assert(message->part[message->num_of_part].pkbuf);

            http->num_of_part++;
            message->num_of_part++;

            if (data.part[i].content)
                ogs_free(data.part[i].content);
            break;

        DEFAULT
            ogs_error("Unknown content-type[%s]", data.part[i].content_type);
        END
    }

    if (data.part[i].content_id)
        ogs_free(data.part[i].content_id);
    if (data.part[i].content_type)
        ogs_free(data.part[i].content_type);

    if (data.header_field)
        ogs_free(data.header_field);

    return OGS_OK;
}

static void build_multipart(
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

    p = http->content = ogs_calloc(1, OGS_HUGE_LEN);
    ogs_assert(p);
    last = p + OGS_HUGE_LEN;

    /* First boundary */
    p = ogs_slprintf(p, last, "--%s\r\n", boundary);

    /* Encapsulated multipart part (application/json) */
    json = build_json(message);
    ogs_assert(json);

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
    ogs_assert(content_type);

    ogs_sbi_header_set(http->headers, OGS_SBI_CONTENT_TYPE, content_type);

    ogs_free(content_type);
}

static void http_message_free(ogs_sbi_http_message_t *http)
{
    int i;
    ogs_assert(http);

    if (http->params) {
        ogs_hash_index_t *hi;
        for (hi = ogs_hash_first(http->params); hi; hi = ogs_hash_next(hi)) {
            char *val = ogs_hash_this_val(hi);
            ogs_free(val);
        }
        ogs_hash_destroy(http->params);
    }

    if (http->headers) {
        ogs_hash_index_t *hi;
        for (hi = ogs_hash_first(http->headers); hi; hi = ogs_hash_next(hi)) {
            char *val = ogs_hash_this_val(hi);
            ogs_free(val);
        }
        ogs_hash_destroy(http->headers);
    }
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
