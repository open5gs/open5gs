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

#ifndef PCRF_CONTEXT_H
#define PCRF_CONTEXT_H

#include "ogs-diameter-gx.h"
#include "ogs-diameter-rx.h"
#include "ogs-dbi.h"
#include "ogs-app.h"
#include "metrics.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int __pcrf_log_domain;

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __pcrf_log_domain

typedef struct fd_config_s fd_config_t;
struct session;

typedef struct pcrf_context_s {
    const char          *diam_conf_path;  /* PCRF Diameter conf path */
    ogs_diam_config_t   *diam_config;     /* PCRF Diameter config */

    ogs_thread_mutex_t  db_lock;

    ogs_hash_t          *ip_hash; /* hash table for Gx Frame IPv4/IPv6 */
    ogs_thread_mutex_t  hash_lock;
} pcrf_context_t;

void pcrf_context_init(void);
void pcrf_context_final(void);
pcrf_context_t *pcrf_self(void);

int pcrf_context_parse_config(void);

int pcrf_db_qos_data(char *imsi_bcd, char *apn,
        ogs_session_data_t *session_data);

void pcrf_sess_set_ipv4(const void *key, uint8_t *sid);
void pcrf_sess_set_ipv6(const void *key, uint8_t *sid);
uint8_t *pcrf_sess_find_by_ipv4(const void *key);
uint8_t *pcrf_sess_find_by_ipv6(const void *key);

#ifdef __cplusplus
}
#endif

#endif /* PCRF_CONTEXT_H */
