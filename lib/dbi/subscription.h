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

#if !defined(OGS_DBI_INSIDE) && !defined(OGS_DBI_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_DBI_SUBSCRIPTION_H
#define OGS_DBI_SUBSCRIPTION_H

#ifdef __cplusplus
extern "C" {
#endif

#define OGS_DBI_UE_ID_TYPE_IMSI         "imsi"

typedef struct ogs_dbi_subscription_data_s {
#define OGS_DBI_ACCESS_RESTRICTION_UTRAN_NOT_ALLOWED                (1)
#define OGS_DBI_ACCESS_RESTRICTION_GERAN_NOT_ALLOWED                (1<<1)
#define OGS_DBI_ACCESS_RESTRICTION_GAN_NOT_ALLOWED                  (1<<2)
#define OGS_DBI_ACCESS_RESTRICTION_I_HSPA_EVOLUTION_NOT_ALLOWED     (1<<3)
#define OGS_DBI_ACCESS_RESTRICTION_WB_E_UTRAN_NOT_ALLOWED           (1<<4)
#define OGS_DBI_ACCESS_RESTRICTION_HO_TO_NON_3GPP_ACCESS_NOT_ALLOWED (1<<5)
#define OGS_DBI_ACCESS_RESTRICTION_NB_IOT_NOT_ALLOWED               (1<<6)
    uint32_t                access_restriction_data;
#define OGS_DBI_SUBSCRIBER_STATUS_SERVICE_GRANTED                   0
#define OGS_DBI_SUBSCRIBER_STATUS_OPERATOR_DETERMINED_BARRING       1 
    uint32_t                subscriber_status;
#define OGS_DBI_NETWORK_ACCESS_MODE_PACKET_AND_CIRCUIT              0
#define OGS_DBI_NETWORK_ACCESS_MODE_RESERVED                        1
#define OGS_DBI_NETWORK_ACCESS_MODE_ONLY_PACKET                     2
    uint32_t                network_access_mode;

    ogs_bitrate_t           ambr;                           /* UE-AMBR */

#define OGS_DBI_RAU_TAU_DEFAULT_TIME            (12*60)     /* 12 min */
    uint32_t                subscribed_rau_tau_timer;       /* unit : seconds */

    ogs_pdn_t               pdn[OGS_MAX_NUM_OF_SESS];
    int                     num_of_pdn;
} ogs_dbi_subscription_data_t;

typedef struct ogs_dbi_auth_info_s {
    uint8_t       k[OGS_KEY_LEN];
    uint8_t       use_opc;
    uint8_t       opc[OGS_KEY_LEN];
    uint8_t       op[OGS_KEY_LEN];
    uint8_t       amf[OGS_AMF_LEN];
    uint8_t       rand[OGS_RAND_LEN];
    uint64_t      sqn;
} ogs_dbi_auth_info_t;

int ogs_dbi_auth_info(const char *id_type, const char *ue_id,
        ogs_dbi_auth_info_t *auth_info);
int ogs_dbi_update_sqn(const char *id_type, const char *ue_id, uint64_t sqn);
int ogs_dbi_increment_sqn(const char *id_type, const char *ue_id);

int ogs_dbi_subscription_data(const char *id_type, const char *ue_id,
        ogs_dbi_subscription_data_t *subscription_data);

#ifdef __cplusplus
}
#endif

#endif /* OGS_DBI_SUBSCRIPTION_H */
