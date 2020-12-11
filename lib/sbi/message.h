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

#if !defined(OGS_SBI_INSIDE) && !defined(OGS_SBI_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_SBI_MESSAGE_H
#define OGS_SBI_MESSAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#define OGS_SBI_HTTP_PORT                           80
#define OGS_SBI_HTTPS_PORT                          443

#define OGS_SBI_HTTP_SCHEME                         "http"
#define OGS_SBI_HTTPS_SCHEME                        "https"

#define OGS_SBI_HTTP_STATUS_OK                      200
#define OGS_SBI_HTTP_STATUS_CREATED                 201 /* POST PUT */
#define OGS_SBI_HTTP_STATUS_ACCEPTED                202 /* DELETE PATCH
                                                           POST PUT */
#define OGS_SBI_HTTP_STATUS_NO_CONTENT              204 /* DELETE PATCH
                                                           POST PUT OPTIONS */
#define OGS_SBI_HTTP_STATUS_SEE_OTHER               303 /* DELETE GET
                                                           POST PUT */
#define OGS_SBI_HTTP_STATUS_TEMPORARY_REDIRECT      307 /* ALL */
#define OGS_SBI_HTTP_STATUS_PERMANENT_REDIRECT      308 /* ALL */
#define OGS_SBI_HTTP_STATUS_BAD_REQUEST             400 /* ALL */ 
#define OGS_SBI_HTTP_STATUS_UNAUTHORIZED            401 /* ALL */
#define OGS_SBI_HTTP_STATUS_FORBIDDEN               403 /* ALL */
#define OGS_SBI_HTTP_STATUS_NOT_FOUND               404 /* ALL */
#define OGS_SBI_HTTP_STATUS_MEHTOD_NOT_ALLOWED      405 /* ALL */
#define OGS_SBI_HTTP_STATUS_NOT_ACCEPTABLE          406 /* GET OPTIONS */
#define OGS_SBI_HTTP_STATUS_REQUEST_TIMEOUT         408 /* ALL */
#define OGS_SBI_HTTP_STATUS_CONFLICT                409 /* PATCH POST PUT */
#define OGS_SBI_HTTP_STATUS_GONE                    410 /* ALL */
#define OGS_SBI_HTTP_STATUS_LENGTH_REQUIRED         411 /* PATCH POST
                                                           PUT OPTIONS */
#define OGS_SBI_HTTP_STATUS_PRECONDITION_FAILED     412 /* DELETE GET PATCH
                                                           POST PUT */
#define OGS_SBI_HTTP_STATUS_PAYLOAD_TOO_LARGE       413 /* PATCH POST
                                                           PUT OPTIONS */
#define OGS_SBI_HTTP_STATUS_URI_TOO_LONG            414 /* GET PUT */
#define OGS_SBI_HTTP_STATUS_UNSUPPORTED_MEDIA_TYPE  415 /* PATCH POST
                                                           PUT OPTIONS */
#define OGS_SBI_HTTP_STATUS_TOO_MANY_REQUESTS       429 /* ALL */
#define OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR   500 /* ALL */
#define OGS_SBI_HTTP_STATUS_NOT_IMPLEMENTED         501 /* ALL */
#define OGS_SBI_HTTP_STATUS_SERVICE_UNAVAILABLE     503 /* ALL */
#define OGS_SBI_HTTP_STATUS_GATEWAY_TIMEOUT         504 /* ALL */

#define OGS_SBI_HTTP_METHOD_DELETE                  "DELETE"
#define OGS_SBI_HTTP_METHOD_GET                     "GET"
#define OGS_SBI_HTTP_METHOD_PATCH                   "PATCH"
#define OGS_SBI_HTTP_METHOD_POST                    "POST"
#define OGS_SBI_HTTP_METHOD_PUT                     "PUT"
#define OGS_SBI_HTTP_METHOD_OPTIONS                 "OPTIONS"

#define OGS_SBI_API_V1                              "v1"
#define OGS_SBI_API_V1_0_0                          "1.0.0"
#define OGS_SBI_API_V2                              "v2"
#define OGS_SBI_API_V2_0_0                          "2.0.0"

#define OGS_SBI_SERVICE_NAME_NNRF_NFM               "nnrf-nfm"
#define OGS_SBI_SERVICE_NAME_NNRF_DISC              "nnrf-disc"
#define OGS_SBI_RESOURCE_NAME_NF_INSTANCES          "nf-instances"
#define OGS_SBI_RESOURCE_NAME_SUBSCRIPTIONS         "subscriptions"
#define OGS_SBI_RESOURCE_NAME_NF_STATUS_NOTIFY      "nf-status-notify"

#define OGS_SBI_SERVICE_NAME_NAUSF_AUTH             "nausf-auth"
#define OGS_SBI_RESOURCE_NAME_UE_AUTHENTICATIONS    "ue-authentications"
#define OGS_SBI_RESOURCE_NAME_5G_AKA                "5g-aka"
#define OGS_SBI_RESOURCE_NAME_5G_AKA_CONFIRMATION   "5g-aka-confirmation"
#define OGS_SBI_RESOURCE_NAME_EAP_SESSION           "eap-session"

#define OGS_SBI_SERVICE_NAME_NUDM_SDM               "nudm-sdm"
#define OGS_SBI_RESOURCE_NAME_AM_DATA               "am-data"
#define OGS_SBI_RESOURCE_NAME_SM_DATA               "sm-data"
#define OGS_SBI_RESOURCE_NAME_SMF_SELECT_DATA       "smf-select-data"
#define OGS_SBI_RESOURCE_NAME_UE_CONTEXT_IN_SMF_DATA "ue-context-in-smf-data"
#define OGS_SBI_SERVICE_NAME_NUDM_UEAU              "nudm-ueau"
#define OGS_SBI_RESOURCE_NAME_SECURITY_INFORMATION  "security-information"
#define OGS_SBI_RESOURCE_NAME_GENERATE_AUTH_DATA    "generate-auth-data"
#define OGS_SBI_RESOURCE_NAME_AUTH_EVENTS           "auth-events"
#define OGS_SBI_SERVICE_NAME_NUDM_UECM              "nudm-uecm"
#define OGS_SBI_RESOURCE_NAME_REGISTRATIONS         "registrations"
#define OGS_SBI_RESOURCE_NAME_AMF_3GPP_ACCESS       "amf-3gpp-access"

#define OGS_SBI_SERVICE_NAME_NUDR_DR                "nudr-dr"
#define OGS_SBI_RESOURCE_NAME_SUBSCRIPTION_DATA     "subscription-data"
#define OGS_SBI_RESOURCE_NAME_AUTHENTICATION_DATA   "authentication-data"
#define OGS_SBI_RESOURCE_NAME_AUTHENTICATION_SUBSCRIPTION \
                                            "authentication-subscription"
#define OGS_SBI_RESOURCE_NAME_AUTHENTICATION_STATUS "authentication-status"
#define OGS_SBI_RESOURCE_NAME_CONTEXT_DATA          "context-data"
#define OGS_SBI_RESOURCE_NAME_PROVISIONED_DATA      "provisioned-data"
#define OGS_SBI_RESOURCE_NAME_POLICY_DATA           "policy-data"
#define OGS_SBI_RESOURCE_NAME_UES                   "ues"
#define OGS_SBI_RESOURCE_NAME_AM_DATA               "am-data"

#define OGS_SBI_SERVICE_NAME_NSMF_PDUSESSION        "nsmf-pdusession"
#define OGS_SBI_RESOURCE_NAME_SM_CONTEXTS           "sm-contexts"
#define OGS_SBI_SERVICE_NAME_NSMF_EVENT_EXPOSURE    "nsmf-event-exposure"
#define OGS_SBI_RESOURCE_NAME_MODIFY                "modify"
#define OGS_SBI_RESOURCE_NAME_RELEASE               "release"

#define OGS_SBI_SERVICE_NAME_NSMF_CALLBACK          "nsmf-callback"
#define OGS_SBI_RESOURCE_NAME_SM_POLICY_NOTIFY      "sm-policy-notify"

#define OGS_SBI_SERVICE_NAME_NAMF_COMM              "namf-comm"
#define OGS_SBI_RESOURCE_NAME_UE_CONTEXTS           "ue-contexts"
#define OGS_SBI_RESOURCE_NAME_N1_N2_MESSAGES        "n1-n2-messages"

#define OGS_SBI_SERVICE_NAME_NAMF_CALLBACK          "namf-callback"
#define OGS_SBI_RESOURCE_NAME_SM_CONTEXT_STATUS     "sm-context-status"
#define OGS_SBI_RESOURCE_NAME_AM_POLICY_NOTIFY      "am-policy-notify"
#define OGS_SBI_RESOURCE_NAME_DEREG_NOTIFY          "dereg-notify"

#define OGS_SBI_SERVICE_NAME_NPCF_AM_POLICY_CONTROL "npcf-am-policy-control"
#define OGS_SBI_RESOURCE_NAME_POLICIES              "policies"
#define OGS_SBI_SERVICE_NAME_NPCF_SMPOLICYCONTROL   "npcf-smpolicycontrol"
#define OGS_SBI_RESOURCE_NAME_SM_POLICIES           "sm-policies"

#define OGS_SBI_PARAM_NF_TYPE                       "nf-type"
#define OGS_SBI_PARAM_TARGET_NF_TYPE                "target-nf-type"
#define OGS_SBI_PARAM_REQUESTER_NF_TYPE             "requester-nf-type"
#define OGS_SBI_PARAM_LIMIT                         "limit"
#define OGS_SBI_PARAM_DNN                           "dnn"
#define OGS_SBI_PARAM_PLMN_ID                       "plmn-id"
#define OGS_SBI_PARAM_SINGLE_NSSAI                  "single-nssai"
#define OGS_SBI_PARAM_SNSSAI                        "snssai"

#define OGS_SBI_ACCEPT                              "Accept"
#define OGS_SBI_ACCEPT_ENCODING                     "Accept-Encoding"
#define OGS_SBI_CONTENT_TYPE                        "Content-Type"
#define OGS_SBI_LOCATION                            "Location"
#define OGS_SBI_EXPECT                              "Expect"
#define OGS_SBI_APPLICATION_TYPE                    "application"
#define OGS_SBI_APPLICATION_JSON_TYPE               "json"
#define OGS_SBI_APPLICATION_PROBLEM_TYPE            "problem+json"
#define OGS_SBI_APPLICATION_PATCH_TYPE              "json-patch+json"
#define OGS_SBI_APPLICATION_3GPPHAL_TYPE            "3gppHal+json"
#define OGS_SBI_APPLICATION_5GNAS_TYPE              "vnd.3gpp.5gnas"
#define OGS_SBI_APPLICATION_NGAP_TYPE               "vnd.3gpp.ngap"

#define OGS_SBI_CONTENT_JSON_TYPE                   \
    OGS_SBI_APPLICATION_TYPE "/" OGS_SBI_APPLICATION_JSON_TYPE
#define OGS_SBI_CONTENT_PROBLEM_TYPE                \
    OGS_SBI_APPLICATION_TYPE "/" OGS_SBI_APPLICATION_PROBLEM_TYPE
#define OGS_SBI_CONTENT_PATCH_TYPE                  \
    OGS_SBI_APPLICATION_TYPE "/" OGS_SBI_APPLICATION_PATCH_TYPE
#define OGS_SBI_CONTENT_3GPPHAL_TYPE                \
    OGS_SBI_APPLICATION_TYPE "/" OGS_SBI_APPLICATION_3GPPHAL_TYPE
#define OGS_SBI_CONTENT_5GNAS_TYPE                  \
    OGS_SBI_APPLICATION_TYPE "/" OGS_SBI_APPLICATION_5GNAS_TYPE
#define OGS_SBI_CONTENT_NGAP_TYPE                   \
    OGS_SBI_APPLICATION_TYPE "/" OGS_SBI_APPLICATION_NGAP_TYPE

#define OGS_SBI_MULTIPART_TYPE                      "multipart"
#define OGS_SBI_MULTIPART_RELATED_TYPE              "related"
#define OGS_SBI_CONTENT_MULTIPART_TYPE              \
    OGS_SBI_MULTIPART_TYPE "/" OGS_SBI_MULTIPART_RELATED_TYPE

#define OGS_SBI_CONTENT_ID                          "Content-Id"
#define OGS_SBI_CONTENT_5GNAS_SM_ID                 "5gnas-sm"
#define OGS_SBI_CONTENT_NGAP_SM_ID                  "ngap-sm"

typedef struct ogs_sbi_header_s {
    char *method;
    char *uri;

    struct {
        char *name;
    } service;

    struct {
        char *version;
    } api;

    struct {
#define OGS_SBI_MAX_NUM_OF_RESOURCE_COMPONENT 8
        char *component[OGS_SBI_MAX_NUM_OF_RESOURCE_COMPONENT];
    } resource;

} ogs_sbi_header_t;

typedef struct ogs_sbi_part_s {
    char *content_id;
    char *content_type;

    ogs_pkbuf_t *pkbuf;
} ogs_sbi_part_t;

typedef struct ogs_sbi_message_s {
    ogs_sbi_header_t h;

    struct {
        char *accept;
        char *content_encoding;
        char *content_type;
        char *location;
        char *cache_control;
    } http;

    struct {
        OpenAPI_nf_type_e target_nf_type;
        OpenAPI_nf_type_e requester_nf_type;
        OpenAPI_nf_type_e nf_type;
        int limit;
        char *dnn;
        bool plmn_id_presence;
        ogs_plmn_id_t plmn_id;

        bool single_nssai_presence;
        ogs_s_nssai_t single_nssai;
        bool snssai_presence;
        ogs_s_nssai_t snssai;
    } param;

    int res_status;

    OpenAPI_nf_profile_t *NFProfile;
    OpenAPI_problem_details_t *ProblemDetails;
    OpenAPI_list_t *PatchItemList;

    OpenAPI_subscription_data_t *SubscriptionData;
    OpenAPI_notification_data_t *NotificationData;
    OpenAPI_search_result_t *SearchResult;
    OpenAPI_authentication_info_t *AuthenticationInfo;
    OpenAPI_authentication_info_request_t *AuthenticationInfoRequest;
    OpenAPI_authentication_info_result_t *AuthenticationInfoResult;
    OpenAPI_authentication_subscription_t *AuthenticationSubscription;
    OpenAPI_ue_authentication_ctx_t *UeAuthenticationCtx;
    OpenAPI_confirmation_data_t *ConfirmationData;
    OpenAPI_confirmation_data_response_t *ConfirmationDataResponse;
    OpenAPI_auth_event_t *AuthEvent;
    OpenAPI_amf3_gpp_access_registration_t *Amf3GppAccessRegistration;
    OpenAPI_access_and_mobility_subscription_data_t
        *AccessAndMobilitySubscriptionData;
    OpenAPI_smf_selection_subscription_data_t *SmfSelectionSubscriptionData;
    OpenAPI_ue_context_in_smf_data_t *UeContextInSmfData;
    OpenAPI_sm_context_create_data_t *SmContextCreateData;
    OpenAPI_sm_context_created_data_t *SmContextCreatedData;
    OpenAPI_sm_context_create_error_t *SmContextCreateError;
    OpenAPI_sm_context_update_data_t *SmContextUpdateData;
    OpenAPI_sm_context_updated_data_t *SmContextUpdatedData;
    OpenAPI_sm_context_update_error_t *SmContextUpdateError;
    OpenAPI_sm_context_release_data_t *SmContextReleaseData;
    OpenAPI_sm_context_released_data_t *SmContextReleasedData;
    OpenAPI_session_management_subscription_data_t *
            SessionManagementSubscriptionData;
    OpenAPI_n1_n2_message_transfer_req_data_t *N1N2MessageTransferReqData;
    OpenAPI_n1_n2_message_transfer_rsp_data_t *N1N2MessageTransferRspData;
    OpenAPI_sm_context_status_notification_t *SmContextStatusNotification;
    OpenAPI_policy_association_request_t *PolicyAssociationRequest;
    OpenAPI_policy_association_t *PolicyAssociation;
    OpenAPI_am_policy_data_t *AmPolicyData;
    OpenAPI_sm_policy_context_data_t *SmPolicyContextData;
    OpenAPI_sm_policy_decision_t *SmPolicyDecision;
    OpenAPI_sm_policy_data_t *SmPolicyData;

    ogs_sbi_links_t *links;

#define OGS_SBI_MAX_NUM_OF_PART 8
    int num_of_part;
    ogs_sbi_part_t part[OGS_SBI_MAX_NUM_OF_PART];
} ogs_sbi_message_t;

typedef struct ogs_sbi_http_message_s {
    ogs_hash_t *params;
    ogs_hash_t *headers;

    char *content;
    size_t content_length;

    int num_of_part;
    ogs_sbi_part_t part[OGS_SBI_MAX_NUM_OF_PART];
} ogs_sbi_http_message_t;

typedef struct ogs_sbi_request_s {
    ogs_lnode_t lnode;

    ogs_sbi_header_t h;
    ogs_sbi_http_message_t http;

    /* Used in microhttpd */
    bool suspended;
    struct {
        ogs_poll_t *write;
    } poll;
} ogs_sbi_request_t;

typedef struct ogs_sbi_response_s {
    ogs_sbi_header_t h;
    ogs_sbi_http_message_t http;

    int status;
} ogs_sbi_response_t;

void ogs_sbi_message_init(int num_of_request_pool, int num_of_response_pool);
void ogs_sbi_message_final(void);

void ogs_sbi_message_free(ogs_sbi_message_t *message);

ogs_sbi_request_t *ogs_sbi_request_new(void);
void ogs_sbi_request_free(ogs_sbi_request_t *request);
ogs_sbi_request_t *ogs_sbi_build_request(ogs_sbi_message_t *message);
int ogs_sbi_parse_request(
        ogs_sbi_message_t *message, ogs_sbi_request_t *request);

ogs_sbi_response_t *ogs_sbi_response_new(void);
void ogs_sbi_response_free(ogs_sbi_response_t *response);
ogs_sbi_response_t *ogs_sbi_build_response(
        ogs_sbi_message_t *message, int status);
int ogs_sbi_parse_response(
        ogs_sbi_message_t *message, ogs_sbi_response_t *response);

void ogs_sbi_header_set(ogs_hash_t *ht, const void *key, const void *val);
void *ogs_sbi_header_get(ogs_hash_t *ht, const void *key);

ogs_pkbuf_t *ogs_sbi_find_part_by_content_id(
        ogs_sbi_message_t *message, char *content_id);

int ogs_sbi_parse_header(
        ogs_sbi_message_t *message, ogs_sbi_header_t *header);
void ogs_sbi_header_free(ogs_sbi_header_t *h);

#ifdef __cplusplus
}
#endif

#endif /* OGS_SBI_MESSAGE_H */
