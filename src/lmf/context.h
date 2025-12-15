/*
 * Copyright (C) 2025 by Juraj Elias <juraj.elias@gmail.com>
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

#ifndef LMF_CONTEXT_H
#define LMF_CONTEXT_H

#include "ogs-app.h"
#include "ogs-sbi.h"

#include "lmf-sm.h"
#include "timer.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int __lmf_log_domain;

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __lmf_log_domain

typedef struct lmf_context_s {
    ogs_list_t location_request_list;  /* Active location requests */
    
    /* Cell database is managed separately in cell-database.c */
} lmf_context_t;

typedef struct lmf_location_request_s lmf_location_request_t;

typedef struct lmf_location_request_s {
    ogs_lnode_t lnode;

    ogs_sbi_object_t sbi;                /* SBI object for transactions */
    ogs_pool_id_t id;
    
    char *supi;                          /* UE SUPI */
    char *amf_id;                        /* AMF instance ID */
    
    ogs_sbi_message_t *input_message;    /* Location request input message */
    char *positioning_method;            /* ECID, OTDOA, etc. */
    
    ogs_sbi_xact_t *xact;                /* Transaction for AMF communication */
    
    /* NRPPa context */
    uint32_t measurement_id;             /* NRPPa measurement ID */
    ogs_pkbuf_t *nrppa_pdu;              /* NRPPa PDU data */
    
    /* Location info context (for CELLID positioning without NCGI) */
    OpenAPI_ncgi_t *ncgi_from_amf;       /* NCGI retrieved from AMF location-info API */
    
    /* Fallback configuration */
    bool ecid_fallback_to_cellid;        /* If true, fallback to CELLID when ECID fails */
    
    /* Location result */
    ogs_sbi_message_t *output_message;   /* Location response message */
    
    /* Callback */
    ogs_sbi_client_t *client;            /* Client for callback */
    char *callback_reference;            /* Callback URI */
    
    /* Stream reference for async response (stored as stream_id) */
    ogs_pool_id_t stream_id;             /* Stream ID for async response */
} lmf_location_request_t;

void lmf_context_init(void);
void lmf_context_final(void);
lmf_context_t *lmf_self(void);

int lmf_context_parse_config(void);

lmf_location_request_t *lmf_location_request_add(void);
void lmf_location_request_remove(lmf_location_request_t *location_request);
void lmf_location_request_remove_all(void);
lmf_location_request_t *lmf_location_request_find_by_id(ogs_pool_id_t id);
lmf_location_request_t *lmf_location_request_try_find_by_id(ogs_pool_id_t id);
lmf_location_request_t *lmf_location_request_find_by_supi(const char *supi);
lmf_location_request_t *lmf_location_request_find_by_measurement_id(uint32_t measurement_id);

#ifdef __cplusplus
}
#endif

#endif /* LMF_CONTEXT_H */

