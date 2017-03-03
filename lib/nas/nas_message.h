#ifndef _NAS_MESSAGE_H__
#define _NAS_MESSAGE_H__

#include "3gpp_message.h"
#include "nas_ies.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define NAS_SECURITY_HEADER_PLAIN_NAS_MESSAGE 0
#define NAS_SECURITY_HEADER_INTEGRITY_PROTECTED 1
#define NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED 2
#define NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_NEW_SECURITY_CONTEXT 3
#define NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHTERD_WITH_NEW_INTEGRITY_CONTEXT 4
#define NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_PARTICALLY_CIPHTERD 5
#define NAS_SECURITY_HEADER_FOR_SERVICE_REQUEST_MESSAGE 6

#define NAS_PROTOCOL_DISCRIMINATOR_ESM 0x2
#define NAS_PROTOCOL_DISCRIMINATOR_EMM 0x7

typedef struct _nas_header_t {
ED2(c_uint8_t security_header_type:4;,
    c_uint8_t protocol_discriminator:4;)
    c_uint8_t message_type;
} __attribute__ ((packed)) nas_header_t;

typedef struct _nas_security_header_t {
ED2(c_uint8_t security_header_type:4;,
    c_uint8_t protocol_discriminator:4;)
    c_uint32_t message_authentication_code;
    c_uint8_t sequence_number;
} __attribute__ ((packed)) nas_security_header_t;

#define NAS_ATTACH_REQUEST                                       65
#define NAS_ATTACH_ACCEPT                                        66
#define NAS_ATTACH_COMPLETE                                      67
#define NAS_ATTACH_REJECT                                        68
#define NAS_DETACH_REQUEST                                       69
#define NAS_DETACH_ACCEPT                                        70
#define NAS_TRACKING_AREA_UPDATE_REQUEST                         72
#define NAS_TRACKING_AREA_UPDATE_ACCEPT                          73
#define NAS_TRACKING_AREA_UPDATE_COMPLETE                        74
#define NAS_TRACKING_AREA_UPDATE_REJECT                          75
#define NAS_EXTENDED_SERVICE_REQUEST                             76
#define NAS_SERVICE_REJECT                                       78
#define NAS_GUTI_REALLOCATION_COMMAND                            80
#define NAS_GUTI_REALLOCATION_COMPLETE                           81
#define NAS_AUTHENTICATION_REQUEST                               82
#define NAS_AUTHENTICATION_RESPONSE                              83
#define NAS_AUTHENTICATION_REJECT                                84
#define NAS_AUTHENTICATION_FAILURE                               92
#define NAS_IDENTITY_REQUEST                                     85
#define NAS_IDENTITY_RESPONSE                                    86
#define NAS_SECURITY_MODE_COMMAND                                93
#define NAS_SECURITY_MODE_COMPLETE                               94
#define NAS_SECURITY_MODE_REJECT                                 95
#define NAS_EMM_STATUS                                           96
#define NAS_EMM_INFORMATION                                      97
#define NAS_DOWNLINK_NAS_TRANSPORT                               98
#define NAS_UPLINK_NAS_TRANSPORT                                 99
#define NAS_CS_SERVICE_NOTIFICATION                              100
#define NAS_DOWNLINK_GENERIC_NAS_TRANSPORT                       104
#define NAS_UPLINK_GENERIC_NAS_TRANSPORT                         101

#define NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST          193
#define NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_ACCEPT           194
#define NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REJECT           195
#define NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST        197
#define NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT         198
#define NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT         199
#define NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST                    201
#define NAS_MODIFY_EPS_BEARER_CONTEXT_ACCEPT                     202
#define NAS_MODIFY_EPS_BEARER_CONTEXT_REJECT                     203
#define NAS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST                205
#define NAS_DEACTIVATE_EPS_BEARER_CONTEXT_ACCEPT                 206
#define NAS_PDN_CONNECTIVITY_REQUEST                             208
#define NAS_PDN_CONNECTIVITY_REJECT                              209
#define NAS_PDN_DISCONNECT_REQUEST                               210
#define NAS_PDN_DISCONNECT_REJECT                                211
#define NAS_BEARER_RESOURCE_ALLOCATION_REQUEST                   212
#define NAS_BEARER_RESOURCE_ALLOCATION_REJECT                    213
#define NAS_BEARER_RESOURCE_MODIFICATION_REQUEST                 214
#define NAS_BEARER_RESOURCE_MODIFICATION_REJECT                  215
#define NAS_ESM_INFORMATION_REQUEST                              217
#define NAS_ESM_INFORMATION_RESPONSE                             218
#define NAS_ESM_STATUS                                           232

/************************
 * 8.2.1 Attach accept
 ************************/
#define NAS_ATTACH_ACCEPT_GUTI_PRESENT (1<<0)
#define NAS_ATTACH_ACCEPT_LOCATION_AREA_IDENTIFICATION_PRESENT (1<<1)
#define NAS_ATTACH_ACCEPT_MS_IDENTITY_PRESENT (1<<2)
#define NAS_ATTACH_ACCEPT_EMM_CAUSE_PRESENT (1<<3)
#define NAS_ATTACH_ACCEPT_T3402_VALUE_PRESENT (1<<4)
#define NAS_ATTACH_ACCEPT_T3423_VALUE_PRESENT (1<<5)
#define NAS_ATTACH_ACCEPT_EQUIVALENT_PLMNS_PRESENT (1<<6)
#define NAS_ATTACH_ACCEPT_EMERGENCY_NUMBER_LIST_PRESENT (1<<7)
#define NAS_ATTACH_ACCEPT_EPS_NETWORK_FEATURE_SUPPORT_PRESENT (1<<8)
#define NAS_ATTACH_ACCEPT_ADDITIONAL_UPDATE_RESULT_PRESENT (1<<9)
#define NAS_ATTACH_ACCEPT_T3412_EXTENDED_VALUE_PRESENT (1<<10)
#define NAS_ATTACH_ACCEPT_T3324_VALUE_PRESENT (1<<11)
#define NAS_ATTACH_ACCEPT_EXTENDED_DRX_PARAMETERS_PRESENT (1<<12)

#define NAS_ATTACH_ACCEPT_GUTI_TYPE 0x50
#define NAS_ATTACH_ACCEPT_LOCATION_AREA_IDENTIFICATION_TYPE 0x13
#define NAS_ATTACH_ACCEPT_MS_IDENTITY_TYPE 0x23
#define NAS_ATTACH_ACCEPT_EMM_CAUSE_TYPE 0x53
#define NAS_ATTACH_ACCEPT_T3402_VALUE_TYPE 0x17
#define NAS_ATTACH_ACCEPT_T3423_VALUE_TYPE 0x59
#define NAS_ATTACH_ACCEPT_EQUIVALENT_PLMNS_TYPE 0x4A
#define NAS_ATTACH_ACCEPT_EMERGENCY_NUMBER_LIST_TYPE 0x34
#define NAS_ATTACH_ACCEPT_EPS_NETWORK_FEATURE_SUPPORT_TYPE 0x64
#define NAS_ATTACH_ACCEPT_ADDITIONAL_UPDATE_RESULT_TYPE 0xF0
#define NAS_ATTACH_ACCEPT_T3412_EXTENDED_VALUE_TYPE 0x5E
#define NAS_ATTACH_ACCEPT_T3324_VALUE_TYPE 0x6A
#define NAS_ATTACH_ACCEPT_EXTENDED_DRX_PARAMETERS_TYPE 0x6E

typedef struct _nas_attach_accept_t {
    /* Mandatory fields */
    nas_attach_result_t attach_result;
    nas_gprs_timer_t t3412_value;
    nas_tracking_area_identity_list_t tai_list;
    nas_esm_message_container_t esm_message_container;

    /* Optional fields */
    c_uint32_t presencemask;
    nas_eps_mobile_identity_t guti;
    nas_location_area_identification_t location_area_identification;
    nas_mobile_identity_t ms_identity;
    nas_emm_cause_t emm_cause;
    nas_gprs_timer_t t3402_value;
    nas_gprs_timer_t t3423_value;
    nas_plmn_list_t equivalent_plmns;

    /* 9.9.3.37 Emergency number list
     * See subclause 10.5.3.13 in 3GPP TS 24.008 [13].
     * O TLV 5-50 
       nas_emergency_number_list_t emergency_number_list; */
    nas_eps_network_feature_support_t eps_network_feature_support;
    nas_additional_update_result_t additional_update_result;
    nas_gprs_timer_3_t t3412_extended_value;
    nas_gprs_timer_2_t t3324_value;
    nas_extended_drx_parameters_t extended_drx_parameters;
} nas_attach_accept_t;

/************************
 * 8.2.2 Attach complete
 ************************/
typedef struct _nas_attach_complete {
    /* Mandatory fields */
    nas_esm_message_container_t esm_message_container;

} nas_attach_complete_t;

/************************
 * 8.2.3 Attach rejct
 ************************/
#define NAS_ATTACH_REJECT_ESM_MESSAGE_CONTAINER_PRESENT (1 << 0)
#define NAS_ATTACH_REJECT_T3346_VALUE_PRESENT (1 << 1)
#define NAS_ATTACH_REJECT_T3402_VALUE_PRESENT (1 << 2)
#define NAS_ATTACH_REJECT_EXTENDED_EMM_CAUSE_PRESENT (1 << 3)

#define NAS_ATTACH_REJECT_ESM_MESSAGE_CONTAINER_TYPE 0x78
#define NAS_ATTACH_REJECT_T3346_VALUE_TYPE 0x5F
#define NAS_ATTACH_REJECT_T3402_VALUE_TYPE 0x16
#define NAS_ATTACH_REJECT_EXTENDED_EMM_CAUSE_TYPE 0xA0
typedef struct _nas_attach_reject {
    /* Mandatory fields */
    nas_emm_cause_t emm_cause;
    
    /* Optional fields */
    c_uint32_t presencemask;
    nas_esm_message_container_t esm_message_container;
    nas_gprs_timer_2_t t3346_value;
    nas_gprs_timer_2_t t3402_value;
    nas_extended_emm_cause_t extended_emm_cause;

} nas_attach_reject_t;

/************************
 * 8.2.4 Attach request
 ************************/
#define NAS_ATTACH_REQUEST_OLD_P_TMSI_SIGNATURE_PRESENT (1<<0)
#define NAS_ATTACH_REQUEST_ADDITIONAL_GUTI_PRESENT (1<<1)
#define NAS_ATTACH_REQUEST_LAST_VISITED_REGISTERED_TAI_PRESENT (1<<2)
#define NAS_ATTACH_REQUEST_DRX_PARAMETER_PRESENT (1<<3)
#define NAS_ATTACH_REQUEST_MS_NETWORK_CAPABILITY_PRESENT (1<<4)
#define NAS_ATTACH_REQUEST_OLD_LOCATION_AREA_IDENTIFICATION_PRESENT (1<<5)
#define NAS_ATTACH_REQUEST_TMSI_STATUS_PRESENT (1<<6)
#define NAS_ATTACH_REQUEST_MOBILE_STATION_CLASSMARK_2_PRESENT (1<<7)
#define NAS_ATTACH_REQUEST_MOBILE_STATION_CLASSMARK_3_PRESENT (1<<8)
#define NAS_ATTACH_REQUEST_SUPPORTED_CODECS_PRESENT (1<<9)
#define NAS_ATTACH_REQUEST_ADDITIONAL_UPDATE_TYPE_PRESENT (1<<10)
#define NAS_ATTACH_REQUEST_VOICE_DOMAIN_PREFERENCE_AND_UE_USAGE_SETTING_PRESENT (1<<11)
#define NAS_ATTACH_REQUEST_DEVICE_PROPERTIES_PRESENT (1<<12)
#define NAS_ATTACH_REQUEST_OLD_GUTI_TYPE_PRESENT (1<<13)
#define NAS_ATTACH_REQUEST_MS_NETWORK_FEATURE_SUPPORT_PRESENT (1<<14)
#define NAS_ATTACH_REQUEST_TMSI_BASED_NRI_CONTAINER_PRESENT (1<<15)
#define NAS_ATTACH_REQUEST_T3324_VALUE_PRESENT (1<<16)
#define NAS_ATTACH_REQUEST_T3412_EXTENDED_VALUE_PRESENT (1<<17)
#define NAS_ATTACH_REQUEST_EXTENDED_DRX_PARAMETERS_PRESENT (1<<18)

#define NAS_ATTACH_REQUEST_OLD_P_TMSI_SIGNATURE_TYPE 0x19
#define NAS_ATTACH_REQUEST_ADDITIONAL_GUTI_TYPE 0x50
#define NAS_ATTACH_REQUEST_LAST_VISITED_REGISTERED_TAI_TYPE 0x52
#define NAS_ATTACH_REQUEST_DRX_PARAMETER_TYPE 0x5C
#define NAS_ATTACH_REQUEST_MS_NETWORK_CAPABILITY_TYPE 0x31
#define NAS_ATTACH_REQUEST_OLD_LOCATION_AREA_IDENTIFICATION_TYPE 0x13
#define NAS_ATTACH_REQUEST_TMSI_STATUS_TYPE 0x90
#define NAS_ATTACH_REQUEST_MOBILE_STATION_CLASSMARK_2_TYPE 0x11
#define NAS_ATTACH_REQUEST_MOBILE_STATION_CLASSMARK_3_TYPE 0x20
#define NAS_ATTACH_REQUEST_SUPPORTED_CODECS_TYPE 0x40
#define NAS_ATTACH_REQUEST_ADDITIONAL_UPDATE_TYPE_TYPE 0xF0
#define NAS_ATTACH_REQUEST_VOICE_DOMAIN_PREFERENCE_AND_UE_USAGE_SETTING_TYPE 0x5D
#define NAS_ATTACH_REQUEST_DEVICE_PROPERTIES_TYPE 0xD0
#define NAS_ATTACH_REQUEST_OLD_GUTI_TYPE_TYPE 0xE0
#define NAS_ATTACH_REQUEST_MS_NETWORK_FEATURE_SUPPORT_TYPE 0xC0
#define NAS_ATTACH_REQUEST_TMSI_BASED_NRI_CONTAINER_TYPE 0x10
#define NAS_ATTACH_REQUEST_T3324_VALUE_TYPE 0x6A
#define NAS_ATTACH_REQUEST_T3412_EXTENDED_VALUE_TYPE 0x5E
#define NAS_ATTACH_REQUEST_EXTENDED_DRX_PARAMETERS_TYPE 0x6E

typedef struct _nas_attach_request_t {
    /* Mandatory fields */
    nas_attach_type_t attach_type;
    nas_eps_mobile_identity_t eps_mobile_identity;
    nas_ue_network_capability_t ue_network_capability;
    nas_esm_message_container_t esm_message_container;

    /* Optional fields */
    c_uint32_t presencemask;
    nas_p_tmsi_signature_t old_p_tmsi_signature;
    nas_eps_mobile_identity_t additional_guti;
    nas_tracking_area_identity_t last_visited_registered_tai;
    nas_drx_parameter_t drx_parameter;
    nas_ms_network_capability_t ms_network_capability;
    nas_location_area_identification_t old_location_area_identification;
    nas_tmsi_status_t tmsi_status;
    nas_mobile_station_classmark_2_t mobile_station_classmark_2;
    nas_mobile_station_classmark_3_t mobile_station_classmark_3;
    nas_supported_codec_list_t supported_codecs;
    nas_additional_update_type_t additional_update_type;
    nas_voice_domain_preference_and_ue_usage_setting_t 
        voice_domain_preference_and_ue_usage_setting;
    nas_device_properties_t device_properties;
    nas_guti_type_t old_guti_type;
    nas_ms_network_feature_support_t ms_network_feature_support;
    nas_network_resource_identifier_container_t tmsi_based_nri_container;
    nas_gprs_timer_2_t t3324_value;
    nas_gprs_timer_3_t t3412_extended_value;
    nas_extended_drx_parameters_t extended_drx_parameters;
} nas_attach_request_t;

typedef struct _nas_message_t {
    nas_header_t h;
    union {
        nas_attach_accept_t attach_accept;
        nas_attach_complete_t attach_complete;
        nas_attach_reject_t attach_reject;
        nas_attach_request_t attach_request;
    } emm;
} nas_message_t;

CORE_DECLARE(int) nas_decode_pdu(nas_message_t *message, pkbuf_t *pkbuf);
CORE_DECLARE(int) nas_encode_pdu(pkbuf_t **pkbuf, nas_message_t *message);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

