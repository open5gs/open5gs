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

#include "context.h"

static nrf_context_t self;

int __nrf_log_domain;

static OGS_POOL(nrf_assoc_pool, nrf_assoc_t);

static int context_initialized = 0;

static int max_num_of_nrf_assoc = 0;

void nrf_context_init(void)
{
    ogs_assert(context_initialized == 0);

    /* Initialize NRF context */
    memset(&self, 0, sizeof(nrf_context_t));

    ogs_log_install_domain(&__nrf_log_domain, "nrf", ogs_core()->log.level);

#define MAX_NUM_OF_NRF_ASSOC 8
    max_num_of_nrf_assoc = ogs_global_conf()->max.ue * MAX_NUM_OF_NRF_ASSOC;
    ogs_pool_init(&nrf_assoc_pool, max_num_of_nrf_assoc);

    context_initialized = 1;
}

void nrf_context_final(void)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL, *next_nf_instance = NULL;

    ogs_assert(context_initialized == 1);

    ogs_list_for_each_safe(
            &ogs_sbi_self()->nf_instance_list, next_nf_instance, nf_instance) {
        if (NF_INSTANCE_TYPE_IS_NRF(nf_instance))
            continue;
        if (OGS_FSM_STATE(&nf_instance->sm))
            nrf_nf_fsm_fini(nf_instance);
    }

    nrf_assoc_remove_all();

    ogs_pool_final(&nrf_assoc_pool);

    context_initialized = 0;
}

nrf_context_t *nrf_self(void)
{
    return &self;
}

static int nrf_context_prepare(void)
{
    /* NF Instance Heartbeat
     * Default value is 10 seconds if it is not configured in nrf.yaml */
    if (!ogs_local_conf()->time.nf_instance.heartbeat_interval)
        ogs_local_conf()->time.nf_instance.heartbeat_interval = 10;

    return OGS_OK;
}

static int nrf_context_validation(void)
{
    return OGS_OK;
}

int nrf_context_parse_config(void)
{
    int rv;
    yaml_document_t *document = NULL;
    ogs_yaml_iter_t root_iter;

    document = ogs_app()->document;
    ogs_assert(document);

    rv = nrf_context_prepare();
    if (rv != OGS_OK) return rv;

    ogs_yaml_iter_init(&root_iter, document);
    while (ogs_yaml_iter_next(&root_iter)) {
        const char *root_key = ogs_yaml_iter_key(&root_iter);
        ogs_assert(root_key);
        if (!strcmp(root_key, "time")) {
            ogs_yaml_iter_t time_iter;
            ogs_yaml_iter_recurse(&root_iter, &time_iter);
            while (ogs_yaml_iter_next(&time_iter)) {
                const char *time_key = ogs_yaml_iter_key(&time_iter);
                ogs_assert(time_key);
                if (!strcmp(time_key, "nf_instance")) {
                    ogs_yaml_iter_t sbi_iter;
                    ogs_yaml_iter_recurse(&time_iter, &sbi_iter);

                    while (ogs_yaml_iter_next(&sbi_iter)) {
                        const char *sbi_key =
                            ogs_yaml_iter_key(&sbi_iter);
                        ogs_assert(sbi_key);

                        if (!strcmp(sbi_key, "heartbeat")) {
                            const char *v = ogs_yaml_iter_value(&sbi_iter);
                            if (v) ogs_local_conf()->time.nf_instance.
                                    heartbeat_interval = atoi(v);
                        }
                    }
                }
            }
        }
    }

    rv = nrf_context_validation();
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}

nrf_assoc_t *nrf_assoc_add(ogs_sbi_stream_t *stream)
{
    nrf_assoc_t *assoc = NULL;

    ogs_assert(stream);

    ogs_pool_alloc(&nrf_assoc_pool, &assoc);
    if (!assoc) {
        ogs_error("Maximum number of association[%d] reached",
                    max_num_of_nrf_assoc);
        return NULL;
    }
    memset(assoc, 0, sizeof *assoc);

    assoc->stream = stream;

    ogs_list_add(&self.assoc_list, assoc);

    return assoc;
}

void nrf_assoc_remove(nrf_assoc_t *assoc)
{
    ogs_assert(assoc);

    ogs_list_remove(&self.assoc_list, assoc);

    ogs_pool_free(&nrf_assoc_pool, assoc);
}

void nrf_assoc_remove_all(void)
{
    nrf_assoc_t *assoc = NULL, *next_assoc = NULL;

    ogs_list_for_each_safe(&self.assoc_list, next_assoc, assoc)
        nrf_assoc_remove(assoc);
}
