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

#ifndef OGS_DBI_IMS_H
#define OGS_DBI_IMS_H

#ifdef __cplusplus
extern "C" {
#endif


/* The ogs_msisdn_data_s structure is always used with the db library. */
typedef struct ogs_msisdn_data_s {
    struct {
        uint8_t buf[OGS_MAX_IMSI_LEN];
        int len;
        char bcd[OGS_MAX_IMSI_BCD_LEN+1];
    } imsi;

    int num_of_msisdn;
    struct {
        uint8_t buf[OGS_MAX_MSISDN_LEN];
        int len;
        char bcd[OGS_MAX_MSISDN_BCD_LEN+1];
    } msisdn[OGS_MAX_NUM_OF_MSISDN];
} ogs_msisdn_data_t;

int ogs_dbi_msisdn_data(
        char *imsi_or_msisdn_bcd, ogs_msisdn_data_t *msisdn_data);

int ogs_dbi_ims_data(char *supi, ogs_ims_data_t *ims_data);

#ifdef __cplusplus
}
#endif

#endif /* OGS_DBI_IMS_H */
