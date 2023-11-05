/*
 * Copyright (C) 2019-2023 by Sukchan Lee <acetcom@gmail.com>
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

#ifndef NRF_CONTEXT_H
#define NRF_CONTEXT_H

#include "ogs-app.h"
#include "ogs-sbi.h"

#include "nrf-sm.h"
#include "timer.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int __nrf_log_domain;

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __nrf_log_domain

typedef struct nrf_context_s {
    ogs_list_t assoc_list;
} nrf_context_t;

typedef struct nrf_assoc_s nrf_assoc_t;

typedef struct nrf_assoc_s {
    ogs_lnode_t lnode;

    ogs_sbi_stream_t *stream;
} nrf_assoc_t;

void nrf_context_init(void);
void nrf_context_final(void);
nrf_context_t *nrf_self(void);

int nrf_context_parse_config(void);

nrf_assoc_t *nrf_assoc_add(ogs_sbi_stream_t *stream);
void nrf_assoc_remove(nrf_assoc_t *assoc);
void nrf_assoc_remove_all(void);

#ifdef __cplusplus
}
#endif

#endif /* NRF_CONTEXT_H */
