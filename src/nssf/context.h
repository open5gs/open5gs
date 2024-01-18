/*
 * Copyright (C) 2019-2022 by Sukchan Lee <acetcom@gmail.com>
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

#ifndef NSSF_CONTEXT_H
#define NSSF_CONTEXT_H

#include "ogs-app.h"
#include "ogs-sbi.h"

#include "nssf-sm.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int __nssf_log_domain;

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __nssf_log_domain

typedef struct nssf_context_s {
    ogs_list_t      nsi_list; /* NSI List */
} nssf_context_t;

void nssf_context_init(void);
void nssf_context_final(void);
nssf_context_t *nssf_self(void);

int nssf_context_parse_config(void);

typedef struct nssf_nsi_s {
    ogs_lnode_t     lnode;

    char *nrf_id;
    char *nsi_id;

    ogs_s_nssai_t s_nssai;
} nssf_nsi_t;

nssf_nsi_t *nssf_nsi_add(char *nrf_id, uint8_t sst, ogs_uint24_t sd);
void nssf_nsi_remove(nssf_nsi_t *nsi);
void nssf_nsi_remove_all(void);
nssf_nsi_t *nssf_nsi_find_by_s_nssai(ogs_s_nssai_t *s_nssai);

int get_nsi_load(void);

#ifdef __cplusplus
}
#endif

#endif /* NSSF_CONTEXT_H */
