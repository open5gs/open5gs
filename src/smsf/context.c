/*
 * Copyright (C) 2024 by Sukchan Lee <acetcom@gmail.com>
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

static smsf_context_t self;

int __smsf_log_domain;

static OGS_POOL(smsf_ue_pool, smsf_ue_t);

static int context_initialized = 0;

void smsf_context_init(void)
{
    ogs_assert(context_initialized == 0);

    /* Initialize smsf context */
    memset(&self, 0, sizeof(smsf_context_t));

    ogs_log_install_domain(&__smsf_log_domain, "smsf", ogs_core()->log.level);

    ogs_pool_init(&smsf_ue_pool, ogs_global_conf()->max.ue);

    ogs_list_init(&self.smsf_ue_list);
    self.supi_hash = ogs_hash_make();
    ogs_assert(self.supi_hash);

    context_initialized = 1;
}

void smsf_context_final(void)
{
    ogs_assert(context_initialized == 1);

    smsf_ue_remove_all();

    ogs_assert(self.supi_hash);
    ogs_hash_destroy(self.supi_hash);

    ogs_pool_final(&smsf_ue_pool);

    context_initialized = 0;
}

smsf_context_t *smsf_self(void)
{
    return &self;
}

static int smsf_context_prepare(void)
{
    return OGS_OK;
}

static int smsf_context_validation(void)
{
    return OGS_OK;
}

int smsf_context_parse_config(void)
{
    int rv;
    yaml_document_t *document = NULL;
    ogs_yaml_iter_t root_iter;

    document = ogs_app()->document;
    ogs_assert(document);

    rv = smsf_context_prepare();
    if (rv != OGS_OK) return rv;

    ogs_yaml_iter_init(&root_iter, document);
    while (ogs_yaml_iter_next(&root_iter)) {
        const char *root_key = ogs_yaml_iter_key(&root_iter);
        ogs_assert(root_key);
        if (!strcmp(root_key, "smsf")) {
            ogs_yaml_iter_t smsf_iter;
            ogs_yaml_iter_recurse(&root_iter, &smsf_iter);
            while (ogs_yaml_iter_next(&smsf_iter)) {
                const char *smsf_key = ogs_yaml_iter_key(&smsf_iter);
                ogs_assert(smsf_key);
                if (!strcmp(smsf_key, "default")) {
                    /* handle config in sbi library */
                } else if (!strcmp(smsf_key, "serving")) {
                    /* handle config in sbi library */
                } else if (!strcmp(smsf_key, "sbi")) {
                    /* handle config in sbi library */
                    ogs_yaml_iter_t sbi_iter;
                    ogs_yaml_iter_recurse(&smsf_iter, &sbi_iter);
                    while (ogs_yaml_iter_next(&sbi_iter)) {
                        const char *sbi_key = ogs_yaml_iter_key(&sbi_iter);
                        ogs_assert(sbi_key);
                        if (!strcmp(sbi_key, "server")) {
                        } else if (!strcmp(sbi_key, "client")) {
                            ogs_yaml_iter_t client_iter;
                            ogs_yaml_iter_recurse(&sbi_iter, &client_iter);
                            while (ogs_yaml_iter_next(&client_iter)) {
                                const char *client_key =
                                    ogs_yaml_iter_key(&client_iter);
                                ogs_assert(client_key);
                            }
                        } else
                            ogs_warn("unknown key `%s`", sbi_key);
                    }
                } else if (!strcmp(smsf_key, "nrf")) {
                    /* handle config in sbi library */
                } else if (!strcmp(smsf_key, "scp")) {
                    /* handle config in sbi library */
                } else if (!strcmp(smsf_key, "service_name")) {
                    /* handle config in sbi library */
                } else if (!strcmp(smsf_key, "discovery")) {
                    /* handle config in sbi library */
                } else
                    ogs_warn("unknown key `%s`", smsf_key);
            }
        }
    }

    rv = smsf_context_validation();
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}

smsf_ue_t *smsf_ue_add(char *supi)
{
    smsf_event_t e;
    smsf_ue_t *smsf_ue = NULL;

    ogs_assert(supi);

    ogs_pool_alloc(&smsf_ue_pool, &smsf_ue);
    if (!smsf_ue) {
        ogs_error("No memory pool [%s]", supi);
        return NULL;
    }
    memset(smsf_ue, 0, sizeof *smsf_ue);

    /* SBI Type */
    smsf_ue->sbi.type = OGS_SBI_OBJ_UE_TYPE;

    smsf_ue->ctx_id = ogs_msprintf("%d",
            (int)ogs_pool_index(&smsf_ue_pool, smsf_ue));
    if (!smsf_ue->ctx_id) {
        ogs_error("No memory for smsf_ue->ctx_id [%s]", supi);
        ogs_pool_free(&smsf_ue_pool, smsf_ue);
        return NULL;
    }

    smsf_ue->supi = ogs_strdup(supi);
    if (!smsf_ue->supi) {
        ogs_error("No memory for smsf_ue->supi [%s]", supi);
        ogs_free(smsf_ue->ctx_id);
        ogs_pool_free(&smsf_ue_pool, smsf_ue);
        return NULL;
    }

    ogs_hash_set(self.supi_hash, smsf_ue->supi, strlen(smsf_ue->supi), smsf_ue);

    memset(&e, 0, sizeof(e));
    e.smsf_ue = smsf_ue;
    ogs_fsm_init(&smsf_ue->sm, smsf_ue_state_initial, smsf_ue_state_final, &e);

    ogs_list_add(&self.smsf_ue_list, smsf_ue);

    return smsf_ue;
}

void smsf_ue_remove(smsf_ue_t *smsf_ue)
{
    smsf_event_t e;

    ogs_assert(smsf_ue);

    ogs_list_remove(&self.smsf_ue_list, smsf_ue);

    memset(&e, 0, sizeof(e));
    e.smsf_ue = smsf_ue;
    ogs_fsm_fini(&smsf_ue->sm, &e);

    /* Free SBI object memory */
    ogs_sbi_object_free(&smsf_ue->sbi);

    ogs_assert(smsf_ue->ctx_id);
    ogs_free(smsf_ue->ctx_id);

    ogs_assert(smsf_ue->supi);
    ogs_hash_set(self.supi_hash, smsf_ue->supi, strlen(smsf_ue->supi), NULL);
    ogs_free(smsf_ue->supi);

    if (smsf_ue->amf_instance_id)
        ogs_free(smsf_ue->amf_instance_id);
    if (smsf_ue->gpsi)
        ogs_free(smsf_ue->gpsi);
    if (smsf_ue->data_change_subscription_id)
        ogs_free(smsf_ue->data_change_subscription_id);

    ogs_pool_free(&smsf_ue_pool, smsf_ue);
}

void smsf_ue_remove_all(void)
{
    smsf_ue_t *smsf_ue = NULL, *next = NULL;;

    ogs_list_for_each_safe(&self.smsf_ue_list, next, smsf_ue)
        smsf_ue_remove(smsf_ue);
}

smsf_ue_t *smsf_ue_find_by_supi(char *supi)
{
    ogs_assert(supi);
    return (smsf_ue_t *)ogs_hash_get(self.supi_hash, supi, strlen(supi));
}

smsf_ue_t *smsf_ue_find_by_gpsi(char *gpsi)
{
    ogs_assert(gpsi);

    smsf_ue_t *smsf_ue = NULL, *next = NULL;

    ogs_list_for_each_safe(&self.smsf_ue_list, next, smsf_ue) {
        if (smsf_ue->gpsi) {
            if (!strcmp(smsf_ue->gpsi, gpsi)) {
                return smsf_ue;
            }
        }
    }
    return NULL;
}

smsf_ue_t *smsf_ue_cycle(smsf_ue_t *smsf_ue)
{
    return ogs_pool_cycle(&smsf_ue_pool, smsf_ue);
}

void smsf_sms_increment_tio(smsf_ue_t *smsf_ue) {
    ogs_assert(smsf_ue);

    smsf_ue->mt_tio += 1;
    if (smsf_ue->mt_tio > 7)
        smsf_ue->mt_tio = 0;
}

void smsf_sms_increment_message_reference(smsf_ue_t *smsf_ue) {
    ogs_assert(smsf_ue);

    smsf_ue->mt_message_reference += 1;
    if (smsf_ue->mt_message_reference == 0)
        smsf_ue->mt_message_reference = 1;
}

int get_ue_load(void)
{
    return (((ogs_pool_size(&smsf_ue_pool) -
            ogs_pool_avail(&smsf_ue_pool)) * 100) /
            ogs_pool_size(&smsf_ue_pool));
}
