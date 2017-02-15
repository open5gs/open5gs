#ifndef _NAS_MESSAGE_H__
#define _NAS_MESSAGE_H__

#include "core_pkbuf.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define NAS_SDU_SIZE 2048

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

#define NAS_KEY_SET_IDENTIFIER_NATIVE 0
#define NAS_KEY_SET_IDENTIFIER_MAPPED 1
#define NAS_KEY_SET_IDENTIFIER_NOT_AVAILABLE 0x111

#define NAS_ATTACH_TYPE_EPS_ATTACH 1
#define NAS_ATTACH_TYPE_COMBINED_EPS_IMSI_ATTAACH 2
#define NAS_ATTACH_TYPE_EPS_ERMERGENCY_ATTCH 3
#define NAS_ATTACH_TYPE_RESERVED 4
typedef struct _nas_attach_info_t {
ED4(c_uint8_t tsc:1;,
    c_uint8_t nas_key_set_identifier:3;,
    c_uint8_t spare:1;,
    c_uint8_t attach_type:3;)
} __attribute__ ((packed)) nas_attach_info_t;

#define NAS_EPS_MOBILE_IDENTITY_IMSI 1
#define NAS_EPS_MOBILE_IDENTITY_GUTI 6
#define NAS_EPS_MOBILE_IDENTITY_IMEI 3

#define NAS_EPS_MOBILE_IDENTITY_EVEN 0
#define NAS_EPS_MOBILE_IDENTITY_ODD 1
typedef struct _nas_eps_mobile_identity_guti_t {
ED2(c_uint8_t mcc_digit2:4;,
    c_uint8_t mcc_digit1:4;)
ED2(c_uint8_t mnc_digit3:4;,
    c_uint8_t mcc_digit3:4;)
ED2(c_uint8_t mnc_digit2:4;,
    c_uint8_t mnc_digit1:4;)
    c_uint16_t mme_group_id;
    c_uint8_t mme_code;
    c_uint32_t m_tmsi;
} __attribute__ ((packed)) nas_eps_mobile_identity_guti_t;

typedef struct _nas_eps_mobile_identity_imsi_t {
ED2(c_uint8_t digit2:4;,
    c_uint8_t digit3:4;)
ED2(c_uint8_t digit4:4;,
    c_uint8_t digit5:4;)
ED2(c_uint8_t digit6:4;,
    c_uint8_t digit7:4;)
ED2(c_uint8_t digit8:4;,
    c_uint8_t digit9:4;)
ED2(c_uint8_t digit10:4;,
    c_uint8_t digit11:4;)
ED2(c_uint8_t digit12:4;,
    c_uint8_t digit13:4;)
ED2(c_uint8_t digit14:4;,
    c_uint8_t digit15:4;)
} __attribute__ ((packed)) nas_eps_mobile_identity_imsi_t;

typedef nas_eps_mobile_identity_imsi_t nas_eps_mobile_identity_imei_t;

typedef struct _nas_eps_mobile_identity_t {
    c_uint8_t len;
ED3(c_uint8_t digit1:4;,
    c_uint8_t odd_even:1;,
    c_uint8_t type_of_identity:3;)
    union {
        nas_eps_mobile_identity_imsi_t imsi;
        nas_eps_mobile_identity_guti_t guti;
        nas_eps_mobile_identity_imei_t imei;
    } u;
} __attribute__ ((packed)) nas_eps_mobile_identity_t;

typedef struct _nas_ue_network_capability_t {
    c_uint8_t len;
ED8(c_uint8_t eea0:1;,
    c_uint8_t eea1:1;,
    c_uint8_t eea2:1;,
    c_uint8_t eea3:1;,
    c_uint8_t eea4:1;,
    c_uint8_t eea5:1;,
    c_uint8_t eea6:1;,
    c_uint8_t eea7:1;)
ED8(c_uint8_t eia0:1;,
    c_uint8_t eia1:1;,
    c_uint8_t eia2:1;,
    c_uint8_t eia3:1;,
    c_uint8_t eia4:1;,
    c_uint8_t eia5:1;,
    c_uint8_t eia6:1;,
    c_uint8_t eia7:1;)
ED8(c_uint8_t uea0:1;,
    c_uint8_t uea1:1;,
    c_uint8_t uea2:1;,
    c_uint8_t uea3:1;,
    c_uint8_t uea4:1;,
    c_uint8_t uea5:1;,
    c_uint8_t uea6:1;,
    c_uint8_t uea7:1;)
ED8(c_uint8_t ucs2:1;,
    c_uint8_t uia1:1;,
    c_uint8_t uia2:1;,
    c_uint8_t uia3:1;,
    c_uint8_t uia4:1;,
    c_uint8_t uia5:1;,
    c_uint8_t uia6:1;,
    c_uint8_t uia7:1;)
ED8(c_uint8_t prose_dd:1;,
    c_uint8_t prose:1;,
    c_uint8_t h_245_ash:1;,
    c_uint8_t acc_csfb:1;,
    c_uint8_t lpp:1;,
    c_uint8_t lcs:1;,
    c_uint8_t srvcc:1;,
    c_uint8_t nf:1;)
ED8(c_uint8_t epco:1;,
    c_uint8_t hc_cp_ciot:1;,
    c_uint8_t erw_opdn:1;,
    c_uint8_t s1u_data:1;,
    c_uint8_t up_ciot:1;,
    c_uint8_t cp_ciot:1;,
    c_uint8_t prose_relay:1;,
    c_uint8_t prose_dc:1;)
ED2(c_uint8_t spare:7;,
    c_uint8_t multiple_drb:1;)
} __attribute__ ((packed)) nas_ue_network_capability_t;

typedef struct _nas_esm_message_container_t {
    c_uint16_t len;
    c_uint8_t *esm;
} __attribute__ ((packed)) nas_esm_message_container_t;

typedef c_uint8_t nas_p_tmsi_signature_t;
typedef c_uint8_t nas_tracking_area_identity_t;
typedef c_uint8_t nas_drx_parameter_t;
typedef c_uint8_t nas_ms_network_capability_t;
typedef c_uint8_t nas_location_area_identification_t;
typedef c_uint8_t nas_tmsi_status_t;
typedef c_uint8_t nas_mobile_station_classmark_2_t;
typedef c_uint8_t nas_mobile_station_classmark_3_t;
typedef c_uint8_t nas_supported_codec_list_t;
typedef c_uint8_t nas_additional_update_type_t;
typedef c_uint8_t nas_voice_domain_preference_and_ue_usage_setting_t;
typedef c_uint8_t nas_device_properties_t;
typedef c_uint8_t nas_guti_type_t;
typedef c_uint8_t nas_ms_network_feature_support__t;
typedef c_uint8_t nas_network_resource_identifier_container_t;
typedef c_uint8_t nas_gprs_timer_2_t;
typedef c_uint8_t nas_gprs_timer_3_t;
typedef c_uint8_t nas_extended_drx_parameters_t;

#define NAS_ATTACH_REQUEST_OLD_PTMSI_SIGNATURE_PRESENT (1<<0)
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
#define NAS_ATTACH_REQUEST_OLD_GUTI_TYPE_PRESENT (1<<11)
#define NAS_ATTACH_REQUEST_VOICE_DOMAIN_PREFERENCE_AND_UE_USAGE_SETTING_PRESENT (1<<12)
#define NAS_ATTACH_REQUEST_MS_NETWORK_FEATURE_SUPPORT_PRESENT (1<<13)

#define NAS_ATTACH_REQUEST_OLD_PTMSI_SIGNATURE_IEI 0x19
#define NAS_ATTACH_REQUEST_ADDITIONAL_GUTI_IEI 0x50
#define NAS_ATTACH_REQUEST_LAST_VISITED_REGISTERED_TAI_IEI 0x52
#define NAS_ATTACH_REQUEST_DRX_PARAMETER_IEI 0x5C
#define NAS_ATTACH_REQUEST_MS_NETWORK_FEATURE_SUPPORT_IEI 0xC0
#define NAS_ATTACH_REQUEST_MS_NETWORK_CAPABILITY_IEI 0x31
#define NAS_ATTACH_REQUEST_OLD_LOCATION_AREA_IDENTIFICATION_IEI 0x13
#define NAS_ATTACH_REQUEST_TMSI_STATUS_IEI 0x90
#define NAS_ATTACH_REQUEST_MOBILE_STATION_CLASSMARK_2_IEI 0x11
#define NAS_ATTACH_REQUEST_MOBILE_STATION_CLASSMARK_3_IEI 0x20
#define NAS_ATTACH_REQUEST_SUPPORTED_CODECS_IEI 0x40
#define NAS_ATTACH_REQUEST_ADDITIONAL_UPDATE_TYPE_IEI 0xF0
#define NAS_ATTACH_REQUEST_OLD_GUTI_TYPE_IEI 0xE0
#define NAS_ATTACH_REQUEST_VOICE_DOMAIN_PREFERENCE_AND_UE_USAGE_SETTING_IEI 0x5D

typedef struct _nas_attach_request_t {
    /* Mandatory fields */
    nas_attach_info_t attach_info;
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
    nas_ms_network_feature_support__t ms_network_feature_support_;
    nas_network_resource_identifier_container_t tmsi_based_nri_container;
    nas_gprs_timer_2_t t3324_value;
    nas_gprs_timer_3_t t3412_extended_value;
    nas_extended_drx_parameters_t extended_drx_parameters;
} nas_attach_request_t;

typedef union _nas_message_t {
    union {
        nas_attach_request_t attach_request;
    } emm;
} nas_message_t;

CORE_DECLARE(int) nas_decode_pdu(nas_message_t *message, pkbuf_t *pkbuf);
CORE_DECLARE(int) nas_encode_pdu(pkbuf_t **pkbuf, nas_message_t *message);
CORE_DECLARE(int) nas_free_pdu(nas_message_t *message);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

