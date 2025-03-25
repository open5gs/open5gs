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
#define OGS_SBI_HTTP_STATUS_METHOD_NOT_ALLOWED      405 /* ALL */
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

#define OGS_SBI_RESOURCE_NAME_NF_INSTANCES          "nf-instances"
#define OGS_SBI_RESOURCE_NAME_SUBSCRIPTIONS         "subscriptions"
#define OGS_SBI_RESOURCE_NAME_NF_STATUS_NOTIFY      "nf-status-notify"

#define OGS_SBI_RESOURCE_NAME_UE_AUTHENTICATIONS    "ue-authentications"
#define OGS_SBI_RESOURCE_NAME_5G_AKA                "5g-aka"
#define OGS_SBI_RESOURCE_NAME_5G_AKA_CONFIRMATION   "5g-aka-confirmation"
#define OGS_SBI_RESOURCE_NAME_EAP_SESSION           "eap-session"

#define OGS_SBI_RESOURCE_NAME_AM_DATA               "am-data"
#define OGS_SBI_RESOURCE_NAME_SM_DATA               "sm-data"
#define OGS_SBI_RESOURCE_NAME_SMF_SELECT_DATA       "smf-select-data"
#define OGS_SBI_RESOURCE_NAME_UE_CONTEXT_IN_SMF_DATA "ue-context-in-smf-data"
#define OGS_SBI_RESOURCE_NAME_NSSAI                 "nssai"
#define OGS_SBI_RESOURCE_NAME_SMF_SELECTION_SUBSCRIPTION_DATA \
                                            "smf-selection-subscription-data"
#define OGS_SBI_RESOURCE_NAME_SDM_SUBSCRIPTIONS     "sdm-subscriptions"

#define OGS_SBI_RESOURCE_NAME_SECURITY_INFORMATION  "security-information"
#define OGS_SBI_RESOURCE_NAME_GENERATE_AUTH_DATA    "generate-auth-data"
#define OGS_SBI_RESOURCE_NAME_AUTH_EVENTS           "auth-events"
#define OGS_SBI_RESOURCE_NAME_REGISTRATIONS         "registrations"
#define OGS_SBI_RESOURCE_NAME_AMF_3GPP_ACCESS       "amf-3gpp-access"
#define OGS_SBI_RESOURCE_NAME_SMF_REGISTRATIONS     "smf-registrations"

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

#define OGS_SBI_RESOURCE_NAME_SM_CONTEXTS           "sm-contexts"
#define OGS_SBI_RESOURCE_NAME_MODIFY                "modify"
#define OGS_SBI_RESOURCE_NAME_RELEASE               "release"

#define OGS_SBI_RESOURCE_NAME_SM_POLICY_NOTIFY      "sm-policy-notify"
#define OGS_SBI_RESOURCE_NAME_N1_N2_FAILURE_NOTIFY  "n1-n2-failure-notify"

#define OGS_SBI_RESOURCE_NAME_UE_CONTEXTS           "ue-contexts"
#define OGS_SBI_RESOURCE_NAME_N1_N2_MESSAGES        "n1-n2-messages"
#define OGS_SBI_RESOURCE_NAME_TRANSFER              "transfer"
#define OGS_SBI_RESOURCE_NAME_TRANSFER_UPDATE       "transfer-update"

#define OGS_SBI_RESOURCE_NAME_SM_CONTEXT_STATUS     "sm-context-status"
#define OGS_SBI_RESOURCE_NAME_AM_POLICY_NOTIFY      "am-policy-notify"
#define OGS_SBI_RESOURCE_NAME_DEREG_NOTIFY          "dereg-notify"
#define OGS_SBI_RESOURCE_NAME_SDMSUBSCRIPTION_NOTIFY \
                                            "sdmsubscription-notify"

#define OGS_SBI_RESOURCE_NAME_POLICIES              "policies"
#define OGS_SBI_RESOURCE_NAME_SM_POLICIES           "sm-policies"
#define OGS_SBI_RESOURCE_NAME_DELETE                "delete"
#define OGS_SBI_RESOURCE_NAME_APP_SESSIONS          "app-sessions"
#define OGS_SBI_RESOURCE_NAME_NOTIFY                "notify"
#define OGS_SBI_RESOURCE_NAME_UPDATE                "update"
#define OGS_SBI_RESOURCE_NAME_TERMINATE             "terminate"

#define OGS_SBI_RESOURCE_NAME_NETWORK_SLICE_INFORMATION \
                                                    "network-slice-information"

#define OGS_SBI_RESOURCE_NAME_PCF_BINDINGS          "pcfBindings"

#define OGS_SBI_RESOURCE_NAME_EXCHANGE_CAPABILITY   "exchange-capability"

#define OGS_SBI_PATCH_PATH_NF_STATUS                "/nfStatus"
#define OGS_SBI_PATCH_PATH_LOAD                     "/load"
#define OGS_SBI_PATCH_PATH_PLMN_LIST                "/plmnList"
#define OGS_SBI_PATCH_PATH_VALIDITY_TIME            "/validityTime"


#define OGS_SBI_FEATURES_IS_SET(__fEATURES, __n) \
    (__fEATURES & (1 << ((__n)-1)))
#define OGS_SBI_FEATURES_SET(__fEATURES, __n) \
    __fEATURES |= (1 << ((__n)-1))

#define OGS_SBI_NNRF_NFM_SERVICE_MAP 1
#define OGS_SBI_NNRF_NFM_EMPTY_OBJECTS_NRF_INFO 2

#define OGS_SBI_NNRF_DISC_SERVICE_MAP 6

#define OGS_SBI_NPCF_AM_POLICY_CONTROL_SLICE_SUPPORT 1
#define OGS_SBI_NPCF_AM_POLICY_CONTROL_PENDING_TRANSACTION 2
#define OGS_SBI_NPCF_AM_POLICY_CONTROL_UE_AMBR_AUTHORIZATION 3
#define OGS_SBI_NPCF_AM_POLICY_CONTROL_DNN_REPLACEMENT_CONTROL 4
#define OGS_SBI_NPCF_AM_POLICY_CONTROL_MULTIPLE_ACCESS_TYPES 5
#define OGS_SBI_NPCF_AM_POLICY_CONTROL_WIRELINE_WIRELESS_CONVERGE 6

#define OGS_SBI_NUDM_SDM_SHARED_DATA 1
#define OGS_SBI_NUDM_SDM_IMMEDIATE_REPORT 2
#define OGS_SBI_NUDM_SDM_PATCH_REPORT 3
#define OGS_SBI_NUDM_SDM_NSSAA 4
#define OGS_SBI_NUDM_SDM_CAG_FEATURE 5
#define OGS_SBI_NUDM_SDM_LIMITED_SUBSCRIPTIONS 13

#define OGS_SBI_NPCF_SMPOLICYCONTROL_TSC 1
#define OGS_SBI_NPCF_SMPOLICYCONTROL_RES_SHARE 2
#define OGS_SBI_NPCF_SMPOLICYCONTROL_3GPP_PS_DATA_OFF 3
#define OGS_SBI_NPCF_SMPOLICYCONTROL_ADC 4
#define OGS_SBI_NPCF_SMPOLICYCONTROL_UMC 5
#define OGS_SBI_NPCF_SMPOLICYCONTROL_NET_LOC 6
#define OGS_SBI_NPCF_SMPOLICYCONTROL_RAN_NAS_CAUSE 7
#define OGS_SBI_NPCF_SMPOLICYCONTROL_PROV_AF_SIGNAL_FLOW 8
#define OGS_SBI_NPCF_SMPOLICYCONTROL_PCSCF_RESTORATION_ENHANCEMENT 9
#define OGS_SBI_NPCF_SMPOLICYCONTROL_PRA 10
#define OGS_SBI_NPCF_SMPOLICYCONTROL_RULE_VERSIONING 11
#define OGS_SBI_NPCF_SMPOLICYCONTROL_SPONSORED_CONNECTIVITY 12
#define OGS_SBI_NPCF_SMPOLICYCONTROL_RAN_SUPPORT_INFO 13
#define OGS_SBI_NPCF_SMPOLICYCONTROL_POLICY_UPDATE_WHEN_UE_SUSPENDS 14
#define OGS_SBI_NPCF_SMPOLICYCONTROL_ACCESS_TYPE_CONDITION 15
#define OGS_SBI_NPCF_SMPOLICYCONTROL_MULTI_IPV6_ADDR_PREFIX 16
#define OGS_SBI_NPCF_SMPOLICYCONTROL_SESSION_RULE_ERROR_HANDLING 17
#define OGS_SBI_NPCF_SMPOLICYCONTROL_AF_CHARGING_IDENTIFIER 18
#define OGS_SBI_NPCF_SMPOLICYCONTROL_ATSSS 19
#define OGS_SBI_NPCF_SMPOLICYCONTROL_PENDING_TRANSACTION 20
#define OGS_SBI_NPCF_SMPOLICYCONTROL_URLLC 21
#define OGS_SBI_NPCF_SMPOLICYCONTROL_MAC_ADDRESS_RANGE 22
#define OGS_SBI_NPCF_SMPOLICYCONTROL_WWC 23
#define OGS_SBI_NPCF_SMPOLICYCONTROL_QOS_MONITORING 24
#define OGS_SBI_NPCF_SMPOLICYCONTROL_AUTHORIZATION_WITH_REQUIRED_QOS 25
#define OGS_SBI_NPCF_SMPOLICYCONTROL_ENHANCED_BACKGROUND_DATA_TRANSFER 26
#define OGS_SBI_NPCF_SMPOLICYCONTROL_DN_AUTHORIZATION 27
#define OGS_SBI_NPCF_SMPOLICYCONTROL_PDU_SESSION_REL_CAUSE 28
#define OGS_SBI_NPCF_SMPOLICYCONTROL_SAME_PCF 29
#define OGS_SBI_NPCF_SMPOLICYCONTROL_ADC_MULTI_REDIRECTION 30
#define OGS_SBI_NPCF_SMPOLICYCONTROL_RESP_BASED_SESSION_REL 31
#define OGS_SBI_NPCF_SMPOLICYCONTROL_TIME_SENSITIVE_NETWORKING 32
#define OGS_SBI_NPCF_SMPOLICYCONTROL_EMDBV 33
#define OGS_SBI_NPCF_SMPOLICYCONTROL_DNN_SELECTION_MODE 34
#define OGS_SBI_NPCF_SMPOLICYCONTROL_EPS_FALLBACK_REPORT 35
#define OGS_SBI_NPCF_SMPOLICYCONTROL_POLICY_DECISION_ERROR_HANDLING 36
#define OGS_SBI_NPCF_SMPOLICYCONTROL_DDN_EVENT_POLICY_CONTROL 37
#define OGS_SBI_NPCF_SMPOLICYCONTROL_REALLOCATION_OF_CREDIT 38

#define OGS_SBI_NPCF_POLICYAUTHORIZATION_INFLUENCE_ON_TRAFFIC_ROUTING 1
#define OGS_SBI_NPCF_POLICYAUTHORIZATION_SPONSORED_CONNECTIVITY 2
#define OGS_SBI_NPCF_POLICYAUTHORIZATION_MEDIA_COMPONENT_VERSIONING 3
#define OGS_SBI_NPCF_POLICYAUTHORIZATION_URLLC 4
#define OGS_SBI_NPCF_POLICYAUTHORIZATION_IMS_SBI 5
#define OGS_SBI_NPCF_POLICYAUTHORIZATION_NETLOC 6
#define OGS_SBI_NPCF_POLICYAUTHORIZATION_PROV_AF_SIGNAL_FLOW 7
#define OGS_SBI_NPCF_POLICYAUTHORIZATION_RESOURCE_SHARING 8
#define OGS_SBI_NPCF_POLICYAUTHORIZATION_MCPTT 9
#define OGS_SBI_NPCF_POLICYAUTHORIZATION_MCVIDEO 10
#define OGS_SBI_NPCF_POLICYAUTHORIZATION_PRIORITY_SHARING 11
#define OGS_SBI_NPCF_POLICYAUTHORIZATION_MCPTT_PREEMPTION 12
#define OGS_SBI_NPCF_POLICYAUTHORIZATION_MAC_ADDRESS_RANGE 13
#define OGS_SBI_NPCF_POLICYAUTHORIZATION_RAN_NAS_CAUSE 14
#define OGS_SBI_NPCF_POLICYAUTHORIZATION_ENHANCED_SUBSCRIPTION_TO_NOTIFICATION 15
#define OGS_SBI_NPCF_POLICYAUTHORIZATION_QOS_MONITORING 16
#define OGS_SBI_NPCF_POLICYAUTHORIZATION_AUTHORIZATION_WITH_REQUIRED_QOS 17
#define OGS_SBI_NPCF_POLICYAUTHORIZATION_TIME_SENSITIVE_NETWORKING 18
#define OGS_SBI_NPCF_POLICYAUTHORIZATION_PCSCF_RESTORATION_ENHANCEMENT 19
#define OGS_SBI_NPCF_POLICYAUTHORIZATION_CHEM 20
#define OGS_SBI_NPCF_POLICYAUTHORIZATION_FLUS 21
#define OGS_SBI_NPCF_POLICYAUTHORIZATION_EPS_FALLBACK_REPORT 22
#define OGS_SBI_NPCF_POLICYAUTHORIZATION_ATSSS 23
#define OGS_SBI_NPCF_POLICYAUTHORIZATION_QOS_HINT 24
#define OGS_SBI_NPCF_POLICYAUTHORIZATION_REALLOCATION_OF_CREDIT 25
#define OGS_SBI_NPCF_POLICYAUTHORIZATION_ES3XX 26
#define OGS_SBI_NPCF_POLICYAUTHORIZATION_DISABLE_UE_NOTIFICATION 27
#define OGS_SBI_NPCF_POLICYAUTHORIZATION_PATCH_CORRECTION 28

#define OGS_SBI_NBSF_MANAGEMENT_MULTI_UE_ADDR 1
#define OGS_SBI_NBSF_MANAGEMENT_BINDING_UPDATE 2
#define OGS_SBI_NBSF_MANAGEMENT_SAME_PCF 3
#define OGS_SBI_NBSF_MANAGEMENT_ES3XX 4
#define OGS_SBI_NBSF_MANAGEMENT_EXTENDED_SAME_PCF 5

#define OGS_SBI_N32_HANDSHAKE_NFTLST 1

#define OGS_SBI_SCHEME                              ":scheme"
#define OGS_SBI_AUTHORITY                           ":authority"
#define OGS_SBI_ACCEPT                              "Accept"
#define OGS_SBI_ACCEPT_ENCODING                     "Accept-Encoding"
#define OGS_SBI_USER_AGENT                          "User-Agent"
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

#define OGS_SBI_CUSTOM_3GPP_COMMON                  "3gpp-Sbi-"
#define OGS_SBI_CUSTOM_MESSAGE_PRIORITY  \
    OGS_SBI_CUSTOM_3GPP_COMMON "Message-Priority"
#define OGS_SBI_CUSTOM_CALLBACK          \
    OGS_SBI_CUSTOM_3GPP_COMMON "Callback"
#define OGS_SBI_CUSTOM_TARGET_APIROOT    \
    OGS_SBI_CUSTOM_3GPP_COMMON "Target-apiRoot"
#define OGS_SBI_CUSTOM_ROUTING_BINDING   \
    OGS_SBI_CUSTOM_3GPP_COMMON "Routing-Binding"
#define OGS_SBI_CUSTOM_BINDING           \
    OGS_SBI_CUSTOM_3GPP_COMMON "Binding"
#define OGS_SBI_CUSTOM_DISCOVERY_COMMON  \
    OGS_SBI_CUSTOM_3GPP_COMMON "Discovery-"
#define OGS_SBI_CUSTOM_DISCOVERY_TARGET_NF_TYPE     \
    OGS_SBI_CUSTOM_DISCOVERY_COMMON OGS_SBI_PARAM_TARGET_NF_TYPE
#define OGS_SBI_CUSTOM_DISCOVERY_REQUESTER_NF_TYPE  \
    OGS_SBI_CUSTOM_DISCOVERY_COMMON OGS_SBI_PARAM_REQUESTER_NF_TYPE
#define OGS_SBI_CUSTOM_DISCOVERY_TARGET_NF_INSTANCE_ID \
    OGS_SBI_CUSTOM_DISCOVERY_COMMON OGS_SBI_PARAM_TARGET_NF_INSTANCE_ID
#define OGS_SBI_CUSTOM_DISCOVERY_REQUESTER_NF_INSTANCE_ID \
    OGS_SBI_CUSTOM_DISCOVERY_COMMON OGS_SBI_PARAM_REQUESTER_NF_INSTANCE_ID
#define OGS_SBI_CUSTOM_DISCOVERY_SERVICE_NAMES  \
    OGS_SBI_CUSTOM_DISCOVERY_COMMON OGS_SBI_PARAM_SERVICE_NAMES
#define OGS_SBI_CUSTOM_DISCOVERY_SNSSAIS  \
    OGS_SBI_CUSTOM_DISCOVERY_COMMON OGS_SBI_PARAM_SNSSAIS
#define OGS_SBI_CUSTOM_DISCOVERY_DNN  \
    OGS_SBI_CUSTOM_DISCOVERY_COMMON OGS_SBI_PARAM_DNN
#define OGS_SBI_CUSTOM_DISCOVERY_TAI  \
    OGS_SBI_CUSTOM_DISCOVERY_COMMON OGS_SBI_PARAM_TAI
#define OGS_SBI_CUSTOM_DISCOVERY_TARGET_PLMN_LIST  \
    OGS_SBI_CUSTOM_DISCOVERY_COMMON OGS_SBI_PARAM_TARGET_PLMN_LIST
#define OGS_SBI_CUSTOM_DISCOVERY_REQUESTER_PLMN_LIST  \
    OGS_SBI_CUSTOM_DISCOVERY_COMMON OGS_SBI_PARAM_REQUESTER_PLMN_LIST
#define OGS_SBI_CUSTOM_DISCOVERY_REQUESTER_FEATURES  \
    OGS_SBI_CUSTOM_DISCOVERY_COMMON OGS_SBI_PARAM_REQUESTER_FEATURES
#define OGS_SBI_CUSTOM_DISCOVERY_GUAMI  \
    OGS_SBI_CUSTOM_DISCOVERY_COMMON OGS_SBI_PARAM_GUAMI
#define OGS_SBI_CUSTOM_PRODUCER_ID       \
    OGS_SBI_CUSTOM_3GPP_COMMON "Producer-Id"
#define OGS_SBI_CUSTOM_OCI               \
    OGS_SBI_CUSTOM_3GPP_COMMON "Oci"
#define OGS_SBI_CUSTOM_CLIENT_CREDENTIALS   \
    OGS_SBI_CUSTOM_3GPP_COMMON "Client-Credentials"
#define OGS_SBI_CUSTOM_NRF_URI           \
    OGS_SBI_CUSTOM_3GPP_COMMON "Nrf-Uri"
#define OGS_SBI_CUSTOM_TARGET_NF_ID      \
    OGS_SBI_CUSTOM_3GPP_COMMON "Target-Nf-Id"
#define OGS_SBI_CUSTOM_ACCESS_SCOPE      \
    OGS_SBI_CUSTOM_3GPP_COMMON "Access-Scope"
#define OGS_SBI_CUSTOM_ACCESS_TOKEN      \
    OGS_SBI_CUSTOM_3GPP_COMMON "Access-Token"
#define OGS_SBI_OPTIONAL_CUSTOM_SENDER_TIMESTAMP      \
    OGS_SBI_CUSTOM_3GPP_COMMON "Sender-Timestamp"
#define OGS_SBI_OPTIONAL_CUSTOM_MAX_RSP_TIME      \
    OGS_SBI_CUSTOM_3GPP_COMMON "Max-Rsp-Time"

#define OGS_SBI_PARAM_TARGET_NF_TYPE                "target-nf-type"
#define OGS_SBI_PARAM_REQUESTER_NF_TYPE             "requester-nf-type"
#define OGS_SBI_PARAM_TARGET_NF_INSTANCE_ID         "target-nf-instance-id"
#define OGS_SBI_PARAM_REQUESTER_NF_INSTANCE_ID      "requester-nf-instance-id"
#define OGS_SBI_PARAM_SERVICE_NAMES                 "service-names"
#define OGS_SBI_PARAM_TARGET_PLMN_LIST              "target-plmn-list"
#define OGS_SBI_PARAM_REQUESTER_PLMN_LIST           "requester-plmn-list"
#define OGS_SBI_PARAM_REQUESTER_FEATURES            "requester-features"

#define OGS_SBI_PARAM_NF_ID                         "nf-id"
#define OGS_SBI_PARAM_NF_TYPE                       "nf-type"
#define OGS_SBI_PARAM_LIMIT                         "limit"
#define OGS_SBI_PARAM_DNN                           "dnn"
#define OGS_SBI_PARAM_PLMN_ID                       "plmn-id"
#define OGS_SBI_PARAM_SINGLE_NSSAI                  "single-nssai"
#define OGS_SBI_PARAM_SNSSAI                        "snssai"
#define OGS_SBI_PARAM_GUAMI                         "guami"
#define OGS_SBI_PARAM_SNSSAIS                       "snssais"
#define OGS_SBI_PARAM_TAI                           "tai"
#define OGS_SBI_PARAM_SLICE_INFO_REQUEST_FOR_PDU_SESSION \
        "slice-info-request-for-pdu-session"
#define OGS_SBI_PARAM_FIELDS                        "fields"
#define OGS_SBI_PARAM_IPV4ADDR                      "ipv4Addr"
#define OGS_SBI_PARAM_IPV6PREFIX                    "ipv6Prefix"

#define OGS_SBI_PARAM_FIELDS_GPSIS                       "gpsis"
#define OGS_SBI_PARAM_FIELDS_SUBSCRIBED_UE_AMBR          "subscribedUeAmbr"
#define OGS_SBI_PARAM_FIELDS_NSSAI                       "nssai"
#define OGS_SBI_MAX_NUM_OF_FIELDS                         8

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

#define OGS_SBI_CALLBACK_NSMF_PDUSESSION_UPDATE \
    "Nsmf_PDUSession_Update"
#define OGS_SBI_CALLBACK_NSMF_PDUSESSION_STATUS_NOTIFY \
    "Nsmf_PDUSession_StatusNotify"
#define OGS_SBI_CALLBACK_NUDM_SDM_NOTIFICATION \
    "Nudm_SDM_Notification"
#define OGS_SBI_CALLBACK_NUDM_UECM_DEREGISTRATION_NOTIFICATION \
    "Nudm_UECM_DeregistrationNotification"
#define OGS_SBI_CALLBACK_NUDM_UECM_PCSCF_RESTORATION_NOTIFICATION \
    "Nudm_UECM_PCSCFRestorationNotification"
#define OGS_SBI_CALLBACK_NNRF_NFMANAGEMENT_NF_STATUS_NOTIFY \
    "Nnrf_NFManagement_NFStatusNotify"
#define OGS_SBI_CALLBACK_NAMF_EVENTEXPOSURE_NOTIFY \
    "Namf_EventExposure_Notify"
#define OGS_SBI_CALLBACK_NPCF_UEPOLICYCONTROL_UPDATE_NOTIFY \
    "Npcf_UEPolicyControl_UpdateNotify"
#define OGS_SBI_CALLBACK_NNSSF_NSSAIAVAILABILITY_NOTIFICATION \
    "Nnssf_NSSAIAvailability_Notification"
#define OGS_SBI_CALLBACK_NAMF_COMMUNICATION_AMF_STATUS_CHANGE_NOTIFY \
    "Namf_Communication_AMFStatusChangeNotify"
#define OGS_SBI_CALLBACK_NGMLC_LOCATION_EVENT_NOTIFY \
    "Ngmlc_Location_EventNotify"
#define OGS_SBI_CALLBACK_NCHF_CONVERGEDCHARGING_NOTIFY \
    "Nchf_ConvergedCharging_Notify"
#define OGS_SBI_CALLBACK_NNSSAAF_NSSAA_RE_AUTHENTICATION \
    "Nnssaaf_NSSAA_ReAuthentication"
#define OGS_SBI_CALLBACK_NNSSAAF_NSSAA_REVOCATION \
    "Nnssaaf_NSSAA_Revocation"
#define OGS_SBI_CALLBACK_N5G_DDNMF_DISCOVERY_MONITOR_UPDATE_RESULT \
    "N5g-ddnmf_Discovery_MonitorUpdateResult"
#define OGS_SBI_CALLBACK_N5G_DDNMF_DISCOVERY_MATCH_INFORMATION \
    "N5g-ddnmf_Discovery_MatchInformation"
#define OGS_SBI_CALLBACK_NAMF_COMMUNICATION_ONN1N2TRANSFERFAILURE \
    "Namf_Communication_onN1N2TransferFailure"

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

typedef struct ogs_sbi_discovery_option_s {
    char *target_nf_instance_id;
    char *requester_nf_instance_id;

    int num_of_service_names;
    char *service_names[OGS_SBI_MAX_NUM_OF_SERVICE_TYPE];

    int num_of_snssais;
    ogs_s_nssai_t snssais[OGS_MAX_NUM_OF_SLICE];
    char *dnn;
    bool tai_presence;
    ogs_5gs_tai_t tai;

    bool guami_presence;
    ogs_guami_t guami;

    int num_of_target_plmn_list;
    ogs_plmn_id_t target_plmn_list[OGS_MAX_NUM_OF_PLMN];
    int num_of_requester_plmn_list;
    ogs_plmn_id_t requester_plmn_list[OGS_MAX_NUM_OF_PLMN];

    uint64_t requester_features;
} ogs_sbi_discovery_option_t;

typedef struct ogs_sbi_message_s {
    ogs_sbi_header_t h;

    struct {
        char *accept;
        char *content_encoding;
        char *content_type;
        char *location;
        char *cache_control;

        struct {
            char *callback;
            char *nrf_uri;
        } custom;
    } http;

    struct {
        /* Discovery Parameter */
        OpenAPI_nf_type_e target_nf_type;
        OpenAPI_nf_type_e requester_nf_type;

        /* Discovery Option Parameter */
        ogs_sbi_discovery_option_t *discovery_option;

        /* URL Query Parameter */
        char *nf_id;
        OpenAPI_nf_type_e nf_type;
        int limit;
        char *dnn;
        int num_of_fields;
        char *fields[OGS_SBI_MAX_NUM_OF_FIELDS];

        /* Shared memory */
        ogs_plmn_id_t plmn_id;
        ogs_s_nssai_t s_nssai;

        bool plmn_id_presence;
        bool single_nssai_presence;
        bool snssai_presence;
        bool slice_info_request_for_pdu_session_presence;
        OpenAPI_roaming_indication_e roaming_indication;

        char *ipv4addr;
        char *ipv6prefix;
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
    OpenAPI_amf3_gpp_access_registration_modification_t
        *Amf3GppAccessRegistrationModification;
    OpenAPI_nssai_t *Nssai;
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
    OpenAPI_list_t *SessionManagementSubscriptionDataList;
    OpenAPI_n1_n2_message_transfer_req_data_t *N1N2MessageTransferReqData;
    OpenAPI_n1_n2_message_transfer_rsp_data_t *N1N2MessageTransferRspData;
    OpenAPI_n1_n2_msg_txfr_failure_notification_t
        *N1N2MsgTxfrFailureNotification;
    OpenAPI_sm_context_status_notification_t *SmContextStatusNotification;
    OpenAPI_policy_association_request_t *PolicyAssociationRequest;
    OpenAPI_policy_association_t *PolicyAssociation;
    OpenAPI_policy_update_t *PolicyUpdate;
    OpenAPI_am_policy_data_t *AmPolicyData;
    OpenAPI_sm_policy_context_data_t *SmPolicyContextData;
    OpenAPI_sm_policy_decision_t *SmPolicyDecision;
    OpenAPI_sm_policy_data_t *SmPolicyData;
    OpenAPI_sm_policy_delete_data_t *SmPolicyDeleteData;
    OpenAPI_authorized_network_slice_info_t *AuthorizedNetworkSliceInfo;
    OpenAPI_pcf_binding_t *PcfBinding;
    OpenAPI_app_session_context_t *AppSessionContext;
    OpenAPI_app_session_context_update_data_patch_t
        *AppSessionContextUpdateDataPatch;
    OpenAPI_sm_policy_notification_t *SmPolicyNotification;
    OpenAPI_termination_notification_t *TerminationNotification;
    OpenAPI_deregistration_data_t *DeregistrationData;
    OpenAPI_sdm_subscription_t *SDMSubscription;
    OpenAPI_modification_notification_t *ModificationNotification;
    OpenAPI_smf_registration_t *SmfRegistration;
    OpenAPI_sec_negotiate_req_data_t *SecNegotiateReqData;
    OpenAPI_sec_negotiate_rsp_data_t *SecNegotiateRspData;
    OpenAPI_ue_context_transfer_req_data_t *UeContextTransferReqData;
    OpenAPI_ue_context_transfer_rsp_data_t *UeContextTransferRspData;
    OpenAPI_ue_reg_status_update_req_data_t *UeRegStatusUpdateReqData;
    OpenAPI_ue_reg_status_update_rsp_data_t *UeRegStatusUpdateRspData;

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

#define ogs_sbi_header_set(ht, key, val) \
    ogs_hash_set(ht, ogs_strdup(key), strlen(key), ogs_strdup(val))
#define ogs_sbi_header_get(ht, key) \
    ogs_hash_get(ht, key, strlen(key))

ogs_pkbuf_t *ogs_sbi_find_part_by_content_id(
        ogs_sbi_message_t *message, char *content_id);

int ogs_sbi_parse_header(ogs_sbi_message_t *message, ogs_sbi_header_t *header);
void ogs_sbi_header_free(ogs_sbi_header_t *h);

void ogs_sbi_http_hash_free(ogs_hash_t *hash);

ogs_sbi_discovery_option_t *ogs_sbi_discovery_option_new(void);
void ogs_sbi_discovery_option_free(
        ogs_sbi_discovery_option_t *discovery_option);

void ogs_sbi_discovery_option_set_target_nf_instance_id(
        ogs_sbi_discovery_option_t *discovery_option,
        char *target_nf_instance_id);
void ogs_sbi_discovery_option_set_requester_nf_instance_id(
        ogs_sbi_discovery_option_t *discovery_option,
        char *requester_nf_instance_id);
void ogs_sbi_discovery_option_set_dnn(
        ogs_sbi_discovery_option_t *discovery_option, char *dnn);

void ogs_sbi_discovery_option_add_service_names(
        ogs_sbi_discovery_option_t *discovery_option,
        char *service_name);
char *ogs_sbi_discovery_option_build_service_names(
        ogs_sbi_discovery_option_t *discovery_option);
void ogs_sbi_discovery_option_parse_service_names(
        ogs_sbi_discovery_option_t *discovery_option,
        char *service_names);

void ogs_sbi_discovery_option_add_snssais(
        ogs_sbi_discovery_option_t *discovery_option, ogs_s_nssai_t *s_nssai);
char *ogs_sbi_discovery_option_build_snssais(
        ogs_sbi_discovery_option_t *discovery_option);
void ogs_sbi_discovery_option_parse_snssais(
        ogs_sbi_discovery_option_t *discovery_option, char *snssais);

void ogs_sbi_discovery_option_set_guami(
        ogs_sbi_discovery_option_t *discovery_option, ogs_guami_t *guami);
char *ogs_sbi_discovery_option_build_guami(
        ogs_sbi_discovery_option_t *discovery_option);
void ogs_sbi_discovery_option_parse_guami(
        ogs_sbi_discovery_option_t *discovery_option, char *guami);

void ogs_sbi_discovery_option_set_tai(
        ogs_sbi_discovery_option_t *discovery_option, ogs_5gs_tai_t *tai);
char *ogs_sbi_discovery_option_build_tai(
        ogs_sbi_discovery_option_t *discovery_option);
void ogs_sbi_discovery_option_parse_tai(
        ogs_sbi_discovery_option_t *discovery_option, char *tai);

void ogs_sbi_discovery_option_add_target_plmn_list(
        ogs_sbi_discovery_option_t *discovery_option,
        ogs_plmn_id_t *target_plmn_id);
void ogs_sbi_discovery_option_add_requester_plmn_list(
        ogs_sbi_discovery_option_t *discovery_option,
        ogs_plmn_id_t *requester_plmn_id);

char *ogs_sbi_discovery_option_build_plmn_list(
        ogs_plmn_id_t *plmn_list, int num_of_plmn_list);
int ogs_sbi_discovery_option_parse_plmn_list(
        ogs_plmn_id_t *plmn_list, char *v);

#ifdef __cplusplus
}
#endif

#endif /* OGS_SBI_MESSAGE_H */
