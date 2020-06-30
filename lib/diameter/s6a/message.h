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

#if !defined(OGS_DIAMETER_INSIDE) && !defined(OGS_DIAMETER_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_DIAM_S6A_MESSAGE_H
#define OGS_DIAM_S6A_MESSAGE_H

#include "ogs-crypt.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OGS_DIAM_S6A_APPLICATION_ID                     16777251

#define OGS_DIAM_S6A_AVP_CODE_CONTEXT_IDENTIFIER        (1423)
#define OGS_DIAM_S6A_AVP_CODE_ALL_APN_CONFIG_INC_IND    (1428)
#define OGS_DIAM_S6A_AVP_CODE_APN_CONFIGURATION         (1430)
#define OGS_DIAM_S6A_AVP_CODE_MIP_HOME_AGENT_ADDRESS    (334)
#define OGS_DIAM_S6A_AVP_CODE_SERVED_PARTY_IP_ADDRESS   (848)

#define OGS_DIAM_S6A_RAT_TYPE_WLAN                      0
#define OGS_DIAM_S6A_RAT_TYPE_VIRTUAL                   1
#define OGS_DIAM_S6A_RAT_TYPE_UTRAN                     1000
#define OGS_DIAM_S6A_RAT_TYPE_GERAN                     1001
#define OGS_DIAM_S6A_RAT_TYPE_GAN                       1002
#define OGS_DIAM_S6A_RAT_TYPE_HSPA_EVOLUTION            1003
#define OGS_DIAM_S6A_RAT_TYPE_EUTRAN                    1004
#define OGS_DIAM_S6A_RAT_TYPE_EUTRAN_NB_IOT             1005
#define OGS_DIAM_S6A_RAT_TYPE_CDMA2000_1X               2000
#define OGS_DIAM_S6A_RAT_TYPE_HRPD                      2001
#define OGS_DIAM_S6A_RAT_TYPE_UMB                       2002
#define OGS_DIAM_S6A_RAT_TYPE_EHRPD                     2003

#define OGS_DIAM_S6A_ULR_SINGLE_REGISTRATION_IND        (1)
#define OGS_DIAM_S6A_ULR_S6A_S6D_INDICATOR              (1 << 1)
#define OGS_DIAM_S6A_ULR_SKIP_SUBSCRIBER_DATA           (1 << 2)
#define OGS_DIAM_S6A_ULR_GPRS_SUBSCRIPTION_DATA_IND     (1 << 3)
#define OGS_DIAM_S6A_ULR_NODE_TYPE_IND                  (1 << 4)
#define OGS_DIAM_S6A_ULR_INITIAL_ATTACH_IND             (1 << 5)
#define OGS_DIAM_S6A_ULR_PS_LCS_SUPPORTED_BY_UE         (1 << 6)

#define OGS_DIAM_S6A_UE_SRVCC_NOT_SUPPORTED             (0)
#define OGS_DIAM_S6A_UE_SRVCC_SUPPORTED                 (1)

extern struct dict_object *ogs_diam_s6a_application;

extern struct dict_object *ogs_diam_s6a_cmd_air;
extern struct dict_object *ogs_diam_s6a_cmd_aia;
extern struct dict_object *ogs_diam_s6a_cmd_ulr;
extern struct dict_object *ogs_diam_s6a_cmd_ula;
extern struct dict_object *ogs_diam_s6a_cmd_pur;
extern struct dict_object *ogs_diam_s6a_cmd_pua;

extern struct dict_object *ogs_diam_s6a_visited_plmn_id;
extern struct dict_object *ogs_diam_s6a_rat_type;
extern struct dict_object *ogs_diam_s6a_ulr_flags;
extern struct dict_object *ogs_diam_s6a_ula_flags;
extern struct dict_object *ogs_diam_s6a_subscription_data;
extern struct dict_object *ogs_diam_s6a_req_eutran_auth_info;
extern struct dict_object *ogs_diam_s6a_number_of_requested_vectors;
extern struct dict_object *ogs_diam_s6a_immediate_response_preferred;
extern struct dict_object *ogs_diam_s6a_authentication_info;
extern struct dict_object *ogs_diam_s6a_re_synchronization_info;
extern struct dict_object *ogs_diam_s6a_service_selection;
extern struct dict_object *ogs_diam_s6a_ue_srvcc_capability;
extern struct dict_object *ogs_diam_s6a_e_utran_vector;
extern struct dict_object *ogs_diam_s6a_rand;
extern struct dict_object *ogs_diam_s6a_xres;
extern struct dict_object *ogs_diam_s6a_autn;
extern struct dict_object *ogs_diam_s6a_kasme;
extern struct dict_object *ogs_diam_s6a_subscriber_status;
extern struct dict_object *ogs_diam_s6a_ambr;
extern struct dict_object *ogs_diam_s6a_network_access_mode;
extern struct dict_object *ogs_diam_s6a_access_restriction_data;
extern struct dict_object *ogs_diam_s6a_apn_configuration_profile;
extern struct dict_object *ogs_diam_s6a_subscribed_rau_tau_timer;
extern struct dict_object *ogs_diam_s6a_context_identifier;
extern struct dict_object *ogs_diam_s6a_all_apn_configuration_included_indicator;
extern struct dict_object *ogs_diam_s6a_apn_configuration;
extern struct dict_object *ogs_diam_s6a_max_bandwidth_ul;
extern struct dict_object *ogs_diam_s6a_max_bandwidth_dl;
extern struct dict_object *ogs_diam_s6a_pdn_type;
extern struct dict_object *ogs_diam_s6a_served_party_ip_address;
extern struct dict_object *ogs_diam_s6a_eps_subscribed_qos_profile;
extern struct dict_object *ogs_diam_s6a_qos_class_identifier;
extern struct dict_object *ogs_diam_s6a_allocation_retention_priority;
extern struct dict_object *ogs_diam_s6a_priority_level;
extern struct dict_object *ogs_diam_s6a_pre_emption_capability;
extern struct dict_object *ogs_diam_s6a_pre_emption_vulnerability;

extern struct dict_object *ogs_diam_s6a_terminal_information;
extern struct dict_object *ogs_diam_s6a_imei;
extern struct dict_object *ogs_diam_s6a_software_version;

extern struct dict_object *ogs_diam_s6a_msisdn;
extern struct dict_object *ogs_diam_s6a_a_msisdn;

typedef struct ogs_diam_e_utran_vector_s {
    uint8_t                 xres[OGS_MAX_RES_LEN];
    uint8_t                 xres_len;
    uint8_t                 kasme[OGS_SHA256_DIGEST_SIZE];
    uint8_t                 rand[OGS_RAND_LEN];
    uint8_t                 autn[OGS_AUTN_LEN];
} ogs_diam_e_utran_vector_t;

typedef struct ogs_diam_s6a_aia_message_s {
    ogs_diam_e_utran_vector_t e_utran_vector;
} ogs_diam_s6a_aia_message_t;

typedef struct ogs_diam_s6a_ula_message_s {
#define OGS_DIAM_S6A_ULA_FLAGS_SEPARATION_INDICATION        (0)
#define OGS_DIAM_S6A_ULA_FLAGS_MME_REGISTERED_FOR_SMS       (1)
    uint32_t ula_flags;
    ogs_subscription_data_t subscription_data;
} ogs_diam_s6a_ula_message_t;

typedef struct ogs_diam_s6a_message_s {
#define OGS_DIAM_S6A_CMD_CODE_UPDATE_LOCATION               316
#define OGS_DIAM_S6A_CMD_CODE_AUTHENTICATION_INFORMATION    318
    uint16_t                        cmd_code;

    /* Experimental Result Code */
#define OGS_DIAM_S6A_AUTHENTICATION_DATA_UNAVAILABLE        4181
#define OGS_DIAM_S6A_ERROR_USER_UNKNOWN                     5001
#define OGS_DIAM_S6A_ERROR_ROAMING_NOT_ALLOWED              5004
#define OGS_DIAM_S6A_ERROR_UNKNOWN_EPS_SUBSCRIPTION         5420
#define OGS_DIAM_S6A_ERROR_RAT_NOT_ALLOWED                  5421
#define OGS_DIAM_S6A_ERROR_EQUIPMENT_UNKNOWN                5422
#define OGS_DIAM_S6A_ERROR_UNKOWN_SERVING_NODE              5423
    uint32_t                        result_code;
    uint32_t                        *err;
    uint32_t                        *exp_err;

    ogs_diam_s6a_aia_message_t      aia_message;
    ogs_diam_s6a_ula_message_t      ula_message;
} ogs_diam_s6a_message_t;

int ogs_diam_s6a_init(void);

#ifdef __cplusplus
}
#endif

#endif /* OGS_DIAM_S6A_MESSAGE_H */
