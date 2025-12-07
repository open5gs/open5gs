/*
 * Copyright (C) 2019-2025 by Sukchan Lee <acetcom@gmail.com>
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

#include "context.h"
#include "cell-database.h"

static lmf_context_t self;

int __lmf_log_domain;

static OGS_POOL(lmf_location_request_pool, lmf_location_request_t);

static int context_initialized = 0;

static int max_num_of_location_request = 0;

void lmf_context_init(void)
{
    ogs_assert(context_initialized == 0);

    /* Initialize LMF context */
    memset(&self, 0, sizeof(lmf_context_t));

    ogs_log_install_domain(&__lmf_log_domain, "lmf", ogs_core()->log.level);

#define MAX_NUM_OF_LOCATION_REQUEST 32
    max_num_of_location_request = ogs_global_conf()->max.ue * MAX_NUM_OF_LOCATION_REQUEST;
    ogs_pool_init(&lmf_location_request_pool, max_num_of_location_request);

    ogs_list_init(&self.location_request_list);

    /* Initialize cell database */
    lmf_cell_database_init();

    context_initialized = 1;
}

void lmf_context_final(void)
{
    ogs_assert(context_initialized == 1);

    lmf_location_request_remove_all();

    /* Finalize cell database */
    lmf_cell_database_final();

    ogs_pool_final(&lmf_location_request_pool);

    context_initialized = 0;
}

lmf_context_t *lmf_self(void)
{
    return &self;
}

static int lmf_context_prepare(void)
{
    return OGS_OK;
}

static int lmf_context_validation(void)
{
    return OGS_OK;
}

int lmf_context_parse_config(void)
{
    int rv;
    yaml_document_t *document = NULL;
    ogs_yaml_iter_t root_iter;

    document = ogs_app()->document;
    ogs_assert(document);

    rv = lmf_context_prepare();
    if (rv != OGS_OK) return rv;

    ogs_yaml_iter_init(&root_iter, document);
    while (ogs_yaml_iter_next(&root_iter)) {
        const char *root_key = ogs_yaml_iter_key(&root_iter);
        ogs_assert(root_key);
        /* Cell database parsing is handled separately */
    }

    /* Parse cell database configuration */
    rv = lmf_cell_database_parse_config();
    if (rv != OGS_OK) {
        ogs_error("lmf_cell_database_parse_config() failed");
        return rv;
    }

    rv = lmf_context_validation();
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}

lmf_location_request_t *lmf_location_request_add(void)
{
    lmf_location_request_t *location_request = NULL;

    ogs_pool_alloc(&lmf_location_request_pool, &location_request);
    ogs_assert(location_request);
    memset(location_request, 0, sizeof *location_request);

    location_request->id = ogs_pool_index(&lmf_location_request_pool, location_request);
    ogs_assert(location_request->id > 0 && location_request->id <= max_num_of_location_request);

    /* Initialize SBI object */
    ogs_list_init(&location_request->sbi.xact_list);

    /* Initialize stream_id to invalid */
    location_request->stream_id = OGS_INVALID_POOL_ID;

    ogs_list_add(&self.location_request_list, location_request);

    return location_request;
}

void lmf_location_request_remove(lmf_location_request_t *location_request)
{
    ogs_assert(location_request);

    ogs_list_remove(&self.location_request_list, location_request);

    /* Clear xact pointer first to avoid double-free when ogs_sbi_xact_remove_all is called */
    location_request->xact = NULL;
    
    if (location_request->supi)
        ogs_free(location_request->supi);
    if (location_request->amf_id)
        ogs_free(location_request->amf_id);
    if (location_request->positioning_method)
        ogs_free(location_request->positioning_method);
    if (location_request->callback_reference)
        ogs_free(location_request->callback_reference);
    
    if (location_request->input_message) {
        ogs_sbi_message_free(location_request->input_message);
        ogs_free(location_request->input_message);
    }
    if (location_request->output_message) {
        ogs_sbi_message_free(location_request->output_message);
        ogs_free(location_request->output_message);
    }
    if (location_request->nrppa_pdu)
        ogs_pkbuf_free(location_request->nrppa_pdu);

    /* ogs_sbi_xact_remove_all will remove and free all xacts (including the one we stored in xact) */
    ogs_sbi_xact_remove_all(&location_request->sbi);
    ogs_sbi_object_free(&location_request->sbi);

    ogs_pool_free(&lmf_location_request_pool, location_request);
}

void lmf_location_request_remove_all(void)
{
    lmf_location_request_t *location_request = NULL, *next_location_request = NULL;

    ogs_list_for_each_safe(&self.location_request_list, next_location_request, location_request)
        lmf_location_request_remove(location_request);
}

static lmf_location_request_t *lmf_location_request_lookup(ogs_pool_id_t id)
{
    lmf_location_request_t *location_request = NULL;

    if (id <= 0 || id > max_num_of_location_request)
        return NULL;

    location_request = ogs_pool_find(&lmf_location_request_pool, id);
    if (!location_request)
        return NULL;

    return location_request;
}

lmf_location_request_t *lmf_location_request_find_by_id(ogs_pool_id_t id)
{
    lmf_location_request_t *location_request = NULL;

    location_request = lmf_location_request_lookup(id);
    ogs_assert(location_request);

    return location_request;
}

lmf_location_request_t *lmf_location_request_try_find_by_id(ogs_pool_id_t id)
{
    return lmf_location_request_lookup(id);
}

lmf_location_request_t *lmf_location_request_find_by_supi(const char *supi)
{
    lmf_location_request_t *location_request = NULL;

    ogs_assert(supi);
    
    ogs_list_for_each(&self.location_request_list, location_request) {
        if (location_request->supi && 
            strcmp(location_request->supi, supi) == 0)
            return location_request;
    }

    return NULL;
}

