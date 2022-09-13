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

typedef struct ogs_dbi_auth_info_s {
    uint8_t       k[OGS_KEY_LEN];
    uint8_t       use_opc;
    uint8_t       opc[OGS_KEY_LEN];
    uint8_t       op[OGS_KEY_LEN];
    uint8_t       amf[OGS_AMF_LEN];
    uint8_t       rand[OGS_RAND_LEN];
    uint64_t      sqn;
} ogs_dbi_auth_info_t;

int ogs_dbi_auth_info(char *supi, ogs_dbi_auth_info_t *auth_info);
int ogs_dbi_update_sqn(char *supi, uint64_t sqn);
int ogs_dbi_increment_sqn(char *supi);
int ogs_dbi_update_imeisv(char *supi, char *imeisv);
int ogs_dbi_update_mme(char *supi, char *mme_host, char *mme_realm,
    bool purge_flag);

int ogs_dbi_subscription_data(char *supi,
        ogs_subscription_data_t *subscription_data);

#ifdef __cplusplus
}
#endif

#endif /* OGS_DBI_SUBSCRIPTION_H */
