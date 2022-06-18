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

#include "sbi-path.h"

static ausf_context_t self;

int __ausf_log_domain;

static OGS_POOL(ausf_ue_pool, ausf_ue_t);

static int context_initialized = 0;

void ausf_context_init(void)
{
    ogs_assert(context_initialized == 0);

    /* Initialize AUSF context */
    memset(&self, 0, sizeof(ausf_context_t));

    ogs_log_install_domain(&__ausf_log_domain, "ausf", ogs_core()->log.level);

    ogs_pool_init(&ausf_ue_pool, ogs_app()->max.ue);

    ogs_list_init(&self.ausf_ue_list);
    self.suci_hash = ogs_hash_make();
    ogs_assert(self.suci_hash);
    self.supi_hash = ogs_hash_make();
    ogs_assert(self.supi_hash);

    context_initialized = 1;
}

void ausf_context_final(void)
{
    ogs_assert(context_initialized == 1);

    ausf_ue_remove_all();

    ogs_assert(self.suci_hash);
    ogs_hash_destroy(self.suci_hash);
    ogs_assert(self.supi_hash);
    ogs_hash_destroy(self.supi_hash);

    ogs_pool_final(&ausf_ue_pool);

    context_initialized = 0;
}

ausf_context_t *ausf_self(void)
{
    return &self;
}

static int ausf_context_prepare(void)
{
    return OGS_OK;
}

static int ausf_context_validation(void)
{
    return OGS_OK;
}

int ausf_context_parse_config(void)
{
    int rv;
    yaml_document_t *document = NULL;
    ogs_yaml_iter_t root_iter;

    document = ogs_app()->document;
    ogs_assert(document);

    rv = ausf_context_prepare();
    if (rv != OGS_OK) return rv;

    ogs_yaml_iter_init(&root_iter, document);
    while (ogs_yaml_iter_next(&root_iter)) {
        const char *root_key = ogs_yaml_iter_key(&root_iter);
        ogs_assert(root_key);
        if (!strcmp(root_key, "ausf")) {
            ogs_yaml_iter_t ausf_iter;
            ogs_yaml_iter_recurse(&root_iter, &ausf_iter);
            while (ogs_yaml_iter_next(&ausf_iter)) {
                const char *ausf_key = ogs_yaml_iter_key(&ausf_iter);
                ogs_assert(ausf_key);
                if (!strcmp(ausf_key, "sbi")) {
                    /* handle config in sbi library */
                } else
                    ogs_warn("unknown key `%s`", ausf_key);
            }
        }
    }

    rv = ausf_context_validation();
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}

ausf_ue_t *ausf_ue_add(char *suci)
{
    ausf_event_t e;
    ausf_ue_t *ausf_ue = NULL;

    ogs_assert(suci);

    ogs_pool_alloc(&ausf_ue_pool, &ausf_ue);
    ogs_assert(ausf_ue);
    memset(ausf_ue, 0, sizeof *ausf_ue);

    ausf_ue->ctx_id =
        ogs_msprintf("%d", (int)ogs_pool_index(&ausf_ue_pool, ausf_ue));
    ogs_assert(ausf_ue->ctx_id);

    ausf_ue->suci = ogs_strdup(suci);
    ogs_assert(ausf_ue->suci);
    ogs_hash_set(self.suci_hash, ausf_ue->suci, strlen(ausf_ue->suci), ausf_ue);

    ausf_ue->supi = ogs_supi_from_suci(ausf_ue->suci);
    ogs_assert(ausf_ue->supi);
    ogs_hash_set(self.supi_hash, ausf_ue->supi, strlen(ausf_ue->supi), ausf_ue);

    memset(&e, 0, sizeof(e));
    e.ausf_ue = ausf_ue;
    ogs_fsm_create(&ausf_ue->sm, ausf_ue_state_initial, ausf_ue_state_final);
    ogs_fsm_init(&ausf_ue->sm, &e);

    ogs_list_add(&self.ausf_ue_list, ausf_ue);

    return ausf_ue;
}

void ausf_ue_remove(ausf_ue_t *ausf_ue)
{
    ausf_event_t e;

    ogs_assert(ausf_ue);

    ogs_list_remove(&self.ausf_ue_list, ausf_ue);

    memset(&e, 0, sizeof(e));
    e.ausf_ue = ausf_ue;
    ogs_fsm_fini(&ausf_ue->sm, &e);
    ogs_fsm_delete(&ausf_ue->sm);

    /* Free SBI object memory */
    ogs_sbi_object_free(&ausf_ue->sbi);

    ogs_assert(ausf_ue->ctx_id);
    ogs_free(ausf_ue->ctx_id);

    ogs_assert(ausf_ue->suci);
    ogs_hash_set(self.suci_hash, ausf_ue->suci, strlen(ausf_ue->suci), NULL);
    ogs_free(ausf_ue->suci);

    ogs_assert(ausf_ue->supi);
    ogs_hash_set(self.supi_hash, ausf_ue->supi, strlen(ausf_ue->supi), NULL);
    ogs_free(ausf_ue->supi);

    if (ausf_ue->auth_events_url)
        ogs_free(ausf_ue->auth_events_url);

    if (ausf_ue->serving_network_name)
        ogs_free(ausf_ue->serving_network_name);
    
    ogs_pool_free(&ausf_ue_pool, ausf_ue);
}

void ausf_ue_remove_all()
{
    ausf_ue_t *ausf_ue = NULL, *next = NULL;;

    ogs_list_for_each_safe(&self.ausf_ue_list, next, ausf_ue)
        ausf_ue_remove(ausf_ue);
}

ausf_ue_t *ausf_ue_find_by_suci(char *suci)
{
    ogs_assert(suci);
    return (ausf_ue_t *)ogs_hash_get(self.suci_hash, suci, strlen(suci));
}

ausf_ue_t *ausf_ue_find_by_supi(char *supi)
{
    ogs_assert(supi);
    return (ausf_ue_t *)ogs_hash_get(self.supi_hash, supi, strlen(supi));
}

ausf_ue_t *ausf_ue_find_by_suci_or_supi(char *suci_or_supi)
{
    ogs_assert(suci_or_supi);
    if (strncmp(suci_or_supi, "suci-", strlen("suci-")) == 0)
        return ausf_ue_find_by_suci(suci_or_supi);
    else
        return ausf_ue_find_by_supi(suci_or_supi);
}

ausf_ue_t *ausf_ue_find_by_ctx_id(char *ctx_id)
{
    ogs_assert(ctx_id);
    return ogs_pool_find(&ausf_ue_pool, atoll(ctx_id));
}

ausf_ue_t *ausf_ue_cycle(ausf_ue_t *ausf_ue)
{
    return ogs_pool_cycle(&ausf_ue_pool, ausf_ue);
}

void ausf_ue_select_nf(ausf_ue_t *ausf_ue, OpenAPI_nf_type_e nf_type)
{
    ogs_assert(ausf_ue);
    ogs_assert(nf_type);

    ogs_sbi_select_nf(&ausf_ue->sbi, nf_type, ausf_nf_state_registered);
}
