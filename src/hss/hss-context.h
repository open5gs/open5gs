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

#ifndef HSS_CONTEXT_H
#define HSS_CONTEXT_H

#include "ogs-diameter-s6a.h"
#include "ogs-diameter-cx.h"
#include "ogs-dbi.h"
#include "ogs-app.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int __hss_log_domain;

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __hss_log_domain

typedef struct _hss_context_t {
    const char          *diam_conf_path;/* HSS Diameter conf path */
    ogs_diam_config_t   *diam_config;   /* HSS Diameter config */

    ogs_thread_mutex_t  db_lock;
    ogs_thread_mutex_t  cx_lock;

    ogs_list_t          impi_list;
    ogs_hash_t          *impi_hash;     /* hash table (IMPI) */
} hss_context_t;

typedef struct hss_impi_s {
    ogs_lnode_t lnode;

    char *id;

    ogs_list_t impu_list;
} hss_impi_t;

typedef struct hss_impu_s {
    ogs_lnode_t lnode;

    char *id;
    char *server_name;

    hss_impi_t *impi;
} hss_impu_t;

void hss_context_init(void);
void hss_context_final(void);
hss_context_t *hss_self(void);

int hss_context_parse_config(void);

int hss_db_auth_info(char *imsi_bcd, ogs_dbi_auth_info_t *auth_info);
int hss_db_update_sqn(char *imsi_bcd, uint8_t *rand, uint64_t sqn);
int hss_db_increment_sqn(char *imsi_bcd);

int hss_db_subscription_data(
    char *imsi_bcd, ogs_subscription_data_t *subscription_data);

void hss_cx_associate_identity(char *user_name, char *public_identity);
bool hss_cx_identity_is_associated(char *user_name, char *public_identity);

char *hss_cx_get_server_name(char *user_name, char *public_identity);
void hss_cx_set_server_name(
        char *user_name, char *public_identity,
        char *server_name, bool overwrite);

#ifdef __cplusplus
}
#endif

#endif /* HSS_CONTEXT_H */
