#ifndef __S6A_MESSAGE_H__
#define __S6A_MESSAGE_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "core_sha2.h"

#include "3gpp_types.h"

typedef struct _e_utran_vector_t {
    c_uint8_t               xres[MAX_RES_LEN];
    c_uint8_t               xres_len;
    c_uint8_t               kasme[SHA256_DIGEST_SIZE];
    c_uint8_t               rand[RAND_LEN];
    c_uint8_t               autn[AUTN_LEN];
} e_utran_vector_t;

typedef struct _s6a_aia_message_t {
    e_utran_vector_t        e_utran_vector;
} s6a_aia_message_t;

typedef struct _s6a_subscription_data_t {
#define HSS_ACCESS_RESTRICTION_UTRAN_NOT_ALLOWED                    (1)
#define HSS_ACCESS_RESTRICTION_GERAN_NOT_ALLOWED                    (1<<1)
#define HSS_ACCESS_RESTRICTION_GAN_NOT_ALLOWED                      (1<<2)
#define HSS_ACCESS_RESTRICTION_I_HSPA_EVOLUTION_NOT_ALLOWED         (1<<3)
#define HSS_ACCESS_RESTRICTION_WB_E_UTRAN_NOT_ALLOWED               (1<<4)
#define HSS_ACCESS_RESTRICTION_HO_TO_NON_3GPP_ACCESS_NOT_ALLOWED    (1<<5)
#define HSS_ACCESS_RESTRICTION_NB_IOT_NOT_ALLOWED                   (1<<6)
    c_uint32_t              access_restriction_data;
#define HSS_SUBSCRIBER_STATUS_SERVICE_GRANTED               0
#define HSS_SUBSCRIBER_STATUS_OPERATOR_DETERMINED_BARRING   1 
    c_uint32_t              subscriber_status;
#define HSS_NETWORK_ACCESS_MODE_PACKET_AND_CIRCUIT          0
#define HSS_NETWORK_ACCESS_MODE_RESERVED                    1
#define HSS_NETWORK_ACCESS_MODE_ONLY_PACKET                 2
    c_uint32_t              network_access_mode;

    bitrate_t               ambr;                       /* UE-AMBR */
    c_uint32_t              subscribed_rau_tau_timer;   /* unit : minutes */

    c_uint32_t              context_identifier;         /* default APN */
    pdn_t                   pdn[MAX_NUM_OF_SESS];
    int                     num_of_pdn;
} s6a_subscription_data_t;

typedef struct _s6a_ula_message_t {
#define S6A_ULA_FLAGS_SEPARATION_INDICATION       (0)
#define S6A_ULA_FLAGS_MME_REGISTERED_FOR_SMS      (1)
    c_uint32_t              ula_flags;
    s6a_subscription_data_t subscription_data;
} s6a_ula_message_t;

typedef struct _s6a_message_t {
#define S6A_CMD_CODE_UPDATE_LOCATION                    316
#define S6A_CMD_CODE_AUTHENTICATION_INFORMATION         318
    c_uint16_t              cmd_code;

    /* Experimental Result Code */
#define S6A_DIAMETER_AUTHENTICATION_DATA_UNAVAILABLE    4181
#define S6A_DIAMETER_ERROR_USER_UNKNOWN                 5001
#define S6A_DIAMETER_ERROR_ROAMING_NOT_ALLOWED          5004
#define S6A_DIAMETER_ERROR_UNKNOWN_EPS_SUBSCRIPTION     5420
#define S6A_DIAMETER_ERROR_RAT_NOT_ALLOWED              5421
#define S6A_DIAMETER_ERROR_EQUIPMENT_UNKNOWN            5422
#define S6A_DIAMETER_ERROR_UNKOWN_SERVING_NODE          5423
    c_uint32_t              result_code;

    s6a_aia_message_t       aia_message;
    s6a_ula_message_t       ula_message;
} s6a_message_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __S6A_MESSAGE_H__ */
