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

#include "context.h"

static scp_context_t self;

int __scp_log_domain;

static OGS_POOL(scp_assoc_pool, scp_assoc_t);

static int context_initialized = 0;

static int max_num_of_scp_assoc = 0;

void scp_context_init(void)
{
    ogs_assert(context_initialized == 0);

    /* Initialize SCP context */
    memset(&self, 0, sizeof(scp_context_t));

    ogs_log_install_domain(&__scp_log_domain, "scp", ogs_core()->log.level);

#define MAX_NUM_OF_SCP_ASSOC 8
    max_num_of_scp_assoc = ogs_app()->max.ue * MAX_NUM_OF_SCP_ASSOC;

    ogs_pool_init(&scp_assoc_pool, max_num_of_scp_assoc);

    context_initialized = 1;
}

void scp_context_final(void)
{
    ogs_assert(context_initialized == 1);

    scp_assoc_remove_all();

    ogs_pool_final(&scp_assoc_pool);

    context_initialized = 0;
}

scp_context_t *scp_self(void)
{
    return &self;
}

static int scp_context_prepare(void)
{
    return OGS_OK;
}

static int scp_context_validation(void)
{
    return OGS_OK;
}

int scp_context_parse_config(void)
{
    int rv;
    yaml_document_t *document = NULL;
    ogs_yaml_iter_t root_iter;

    document = ogs_app()->document;
    ogs_assert(document);

    rv = scp_context_prepare();
    if (rv != OGS_OK) return rv;

    ogs_yaml_iter_init(&root_iter, document);
    while (ogs_yaml_iter_next(&root_iter)) {
        const char *root_key = ogs_yaml_iter_key(&root_iter);
        ogs_assert(root_key);
        if (!strcmp(root_key, "scp")) {
            ogs_yaml_iter_t scp_iter;
            ogs_yaml_iter_recurse(&root_iter, &scp_iter);
            while (ogs_yaml_iter_next(&scp_iter)) {
                const char *scp_key = ogs_yaml_iter_key(&scp_iter);
                ogs_assert(scp_key);
                if (!strcmp(scp_key, "sbi")) {
                    /* handle config in sbi library */
                } else if (!strcmp(scp_key, "service_name")) {
                    /* handle config in sbi library */
                } else if (!strcmp(scp_key, "discovery")) {
                    /* handle config in sbi library */
                } else
                    ogs_warn("unknown key `%s`", scp_key);
            }
        }
    }

    rv = scp_context_validation();
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}

scp_assoc_t *scp_assoc_add(ogs_sbi_stream_t *stream)
{
    scp_assoc_t *assoc = NULL;

    ogs_assert(stream);

    ogs_pool_alloc(&scp_assoc_pool, &assoc);
    if (!assoc) {
        ogs_error("Maximum number of association[%d] reached",
                    max_num_of_scp_assoc);
        return NULL;
    }
    memset(assoc, 0, sizeof *assoc);

    assoc->stream = stream;

    ogs_list_add(&self.assoc_list, assoc);

    return assoc;
}

void scp_assoc_remove(scp_assoc_t *assoc)
{
    ogs_assert(assoc);

    ogs_list_remove(&self.assoc_list, assoc);

    if (assoc->client)
        ogs_sbi_client_remove(assoc->client);
    if (assoc->nrf_client)
        ogs_sbi_client_remove(assoc->nrf_client);

    ogs_pool_free(&scp_assoc_pool, assoc);
}

void scp_assoc_remove_all(void)
{
    scp_assoc_t *assoc = NULL, *next_assoc = NULL;

    ogs_list_for_each_safe(&self.assoc_list, next_assoc, assoc)
        scp_assoc_remove(assoc);
}

scp_assoc_t *scp_assoc_find(uint32_t index)
{
    return ogs_pool_find(&scp_assoc_pool, index);
}
