/*
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
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
#include "timer.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_NUM_OF_SERVED_GUAMI     8

extern int __nssf_log_domain;

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __nssf_log_domain

typedef struct nssf_context_s {
    ogs_list_t      nsi_list; /* NSI List */
} nssf_context_t;

#define NSSF_NF_INSTANCE_CLEAR(_cAUSE, _nFInstance) \
    do { \
        ogs_assert(_nFInstance); \
        if ((_nFInstance)->reference_count == 1) { \
            ogs_info("[%s] (%s) NF removed", (_nFInstance)->id, (_cAUSE)); \
            nssf_nf_fsm_fini((_nFInstance)); \
        } else { \
            /* There is an assocation with other context */ \
            ogs_info("[%s:%d] (%s) NF suspended", \
                    _nFInstance->id, _nFInstance->reference_count, (_cAUSE)); \
            OGS_FSM_TRAN(&_nFInstance->sm, nssf_nf_state_de_registered); \
            ogs_fsm_dispatch(&_nFInstance->sm, NULL); \
        } \
        ogs_sbi_nf_instance_remove(_nFInstance); \
    } while(0)

void nssf_context_init(void);
void nssf_context_final(void);
nssf_context_t *nssf_self(void);

int nssf_context_parse_config(void);

typedef struct nssf_nsi_s {
    ogs_lnode_t     lnode;

    char *nsi_id;

    ogs_sockaddr_t *addr;
    struct {
        const char  *key;
        const char  *pem;
    } tls;

    ogs_s_nssai_t s_nssai;
} nssf_nsi_t;

nssf_nsi_t *nssf_nsi_add(ogs_sockaddr_t *addr, uint8_t sst, ogs_uint24_t sd);
void nssf_nsi_remove(nssf_nsi_t *nsi);
void nssf_nsi_remove_all(void);
nssf_nsi_t *nssf_nsi_find_by_s_nssai(ogs_s_nssai_t *s_nssai);

char *nssf_nsi_nrf_uri(nssf_nsi_t *nsi);

#ifdef __cplusplus
}
#endif

#endif /* NSSF_CONTEXT_H */
