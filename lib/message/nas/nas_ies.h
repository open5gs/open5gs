#ifndef _NAS_IES_H__
#define _NAS_IES_H__

#include "core_pkbuf.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

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

CORE_DECLARE(status_t) nas_decode_attach_info(
    nas_attach_info_t *attach_info, pkbuf_t *pkbuf);

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

CORE_DECLARE(status_t) nas_decode_eps_mobile_identity(
    nas_eps_mobile_identity_t *eps_mobile_identity, pkbuf_t *pkbuf);

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

CORE_DECLARE(status_t) nas_decode_ue_network_capability(
    nas_ue_network_capability_t *ue_network_capability, pkbuf_t *pkbuf);

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

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

