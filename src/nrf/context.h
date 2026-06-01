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

    /*
     * Pool-ID of the inbound HTTP/2 stream that triggered an
     * inter-PLMN NF discovery forwarding request to the Home NRF.
     *
     * The raw ogs_sbi_stream_t pointer must NOT be cached here.
     * The original client may RST_STREAM or otherwise disconnect
     * before the (possibly delayed) Home-NRF response arrives,
     * in which case nghttp2 frees the stream and any cached
     * pointer becomes dangling. Storing the ID and resolving it
     * through ogs_sbi_stream_find_by_id() on the response path
     * lets us drop the response cleanly when the originating
     * stream is already gone, instead of asserting on a freed
     * socket fd inside the SBI server.
     */
    ogs_pool_id_t stream_id;
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
