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

#ifndef S6A_MESSAGE_H
#define S6A_MESSAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ogs-crypt.h"

#include "base/types.h"

typedef struct _e_utran_vector_t {
    uint8_t                 xres[MAX_RES_LEN];
    uint8_t                 xres_len;
    uint8_t                 kasme[OGS_SHA256_DIGEST_SIZE];
    uint8_t                 rand[RAND_LEN];
    uint8_t                 autn[AUTN_LEN];
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
    uint32_t                access_restriction_data;
#define HSS_SUBSCRIBER_STATUS_SERVICE_GRANTED               0
#define HSS_SUBSCRIBER_STATUS_OPERATOR_DETERMINED_BARRING   1 
    uint32_t                subscriber_status;
#define HSS_NETWORK_ACCESS_MODE_PACKET_AND_CIRCUIT          0
#define HSS_NETWORK_ACCESS_MODE_RESERVED                    1
#define HSS_NETWORK_ACCESS_MODE_ONLY_PACKET                 2
    uint32_t                network_access_mode;

    bitrate_t               ambr;                       /* UE-AMBR */
    uint32_t                subscribed_rau_tau_timer;   /* unit : minutes */

    uint32_t                context_identifier;         /* default APN */
    pdn_t                   pdn[MAX_NUM_OF_SESS];
    int                     num_of_pdn;
} s6a_subscription_data_t;

typedef struct _s6a_ula_message_t {
#define S6A_ULA_FLAGS_SEPARATION_INDICATION       (0)
#define S6A_ULA_FLAGS_MME_REGISTERED_FOR_SMS      (1)
    uint32_t                ula_flags;
    s6a_subscription_data_t subscription_data;
} s6a_ula_message_t;

typedef struct _s6a_message_t {
#define S6A_CMD_CODE_UPDATE_LOCATION                    316
#define S6A_CMD_CODE_AUTHENTICATION_INFORMATION         318
    uint16_t                cmd_code;

    /* Experimental Result Code */
#define S6A_DIAMETER_AUTHENTICATION_DATA_UNAVAILABLE    4181
#define S6A_DIAMETER_ERROR_USER_UNKNOWN                 5001
#define S6A_DIAMETER_ERROR_ROAMING_NOT_ALLOWED          5004
#define S6A_DIAMETER_ERROR_UNKNOWN_EPS_SUBSCRIPTION     5420
#define S6A_DIAMETER_ERROR_RAT_NOT_ALLOWED              5421
#define S6A_DIAMETER_ERROR_EQUIPMENT_UNKNOWN            5422
#define S6A_DIAMETER_ERROR_UNKOWN_SERVING_NODE          5423
    uint32_t                result_code;

    s6a_aia_message_t       aia_message;
    s6a_ula_message_t       ula_message;
} s6a_message_t;

#ifdef __cplusplus
}
#endif

#endif /* S6A_MESSAGE_H */
