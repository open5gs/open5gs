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

#include "sbi-path.h"

static udm_context_t self;

int __udm_log_domain;

static OGS_POOL(udm_ue_pool, udm_ue_t);
static OGS_POOL(udm_sess_pool, udm_sess_t);
static OGS_POOL(udm_sdm_subscription_pool, udm_sdm_subscription_t);

static int context_initialized = 0;

static int max_num_of_udm_sdm_subscriptions = 0;

void udm_context_init(void)
{
    ogs_assert(context_initialized == 0);

    /* Initialize UDM context */
    memset(&self, 0, sizeof(udm_context_t));

    ogs_log_install_domain(&__udm_log_domain, "udm", ogs_core()->log.level);

    ogs_pool_init(&udm_ue_pool, ogs_global_conf()->max.ue);
    ogs_pool_init(&udm_sess_pool, ogs_app()->pool.sess);
#define MAX_NUM_OF_UDM_SDM_SUBSCRIPTIONS_PER_UE 4
    max_num_of_udm_sdm_subscriptions = ogs_global_conf()->max.ue *
            MAX_NUM_OF_UDM_SDM_SUBSCRIPTIONS_PER_UE;
    ogs_pool_init(&udm_sdm_subscription_pool, max_num_of_udm_sdm_subscriptions);

    ogs_list_init(&self.udm_ue_list);
    self.suci_hash = ogs_hash_make();
    ogs_assert(self.suci_hash);
    self.supi_hash = ogs_hash_make();
    ogs_assert(self.supi_hash);

    ogs_list_init(&self.sdm_subscription_list);
    self.sdm_subscription_id_hash = ogs_hash_make();
    ogs_assert(self.sdm_subscription_id_hash);

    context_initialized = 1;
}

void udm_context_final(void)
{
    ogs_assert(context_initialized == 1);

    udm_ue_remove_all();

    ogs_assert(self.suci_hash);
    ogs_hash_destroy(self.suci_hash);
    ogs_assert(self.supi_hash);
    ogs_hash_destroy(self.supi_hash);
    ogs_assert(self.sdm_subscription_id_hash);
    ogs_hash_destroy(self.sdm_subscription_id_hash);

    ogs_pool_final(&udm_ue_pool);
    ogs_pool_final(&udm_sess_pool);
    ogs_pool_final(&udm_sdm_subscription_pool);

    context_initialized = 0;
}

udm_context_t *udm_self(void)
{
    return &self;
}

static int udm_context_prepare(void)
{
    return OGS_OK;
}

static int udm_context_validation(void)
{
    return OGS_OK;
}

int udm_context_parse_config(void)
{
    int rv;
    yaml_document_t *document = NULL;
    ogs_yaml_iter_t root_iter;
    int idx = 0;

    document = ogs_app()->document;
    ogs_assert(document);

    rv = udm_context_prepare();
    if (rv != OGS_OK) return rv;

    ogs_yaml_iter_init(&root_iter, document);
    while (ogs_yaml_iter_next(&root_iter)) {
        const char *root_key = ogs_yaml_iter_key(&root_iter);
        ogs_assert(root_key);
        if ((!strcmp(root_key, "udm")) &&
            (idx++ == ogs_app()->config_section_id)) {
            ogs_yaml_iter_t udm_iter;
            ogs_yaml_iter_recurse(&root_iter, &udm_iter);
            while (ogs_yaml_iter_next(&udm_iter)) {
                const char *udm_key = ogs_yaml_iter_key(&udm_iter);
                ogs_assert(udm_key);
                if (!strcmp(udm_key, "default")) {
                    /* handle config in sbi library */
                } else if (!strcmp(udm_key, "sbi")) {
                    /* handle config in sbi library */
                } else if (!strcmp(udm_key, "nrf")) {
                    /* handle config in sbi library */
                } else if (!strcmp(udm_key, "scp")) {
                    /* handle config in sbi library */
                } else if (!strcmp(udm_key, "service_name")) {
                    /* handle config in sbi library */
                } else if (!strcmp(udm_key, "discovery")) {
                    /* handle config in sbi library */
                } else if (!strcmp(udm_key, "hnet")) {
                    rv = ogs_sbi_context_parse_hnet_config(&udm_iter);
                    if (rv != OGS_OK) return rv;
                } else
                    ogs_warn("unknown key `%s`", udm_key);
            }
        }
    }

    rv = udm_context_validation();
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}

udm_ue_t *udm_ue_add(char *suci)
{
    udm_event_t e;
    udm_ue_t *udm_ue = NULL;

    ogs_assert(suci);

    ogs_pool_id_calloc(&udm_ue_pool, &udm_ue);
    if (!udm_ue) {
        ogs_error("No memory pool [%s]", suci);
        return NULL;
    }

    /* SBI Type */
    udm_ue->sbi.type = OGS_SBI_OBJ_UE_TYPE;

    udm_ue->ctx_id = ogs_msprintf("%d",
            (int)ogs_pool_index(&udm_ue_pool, udm_ue));
    if (!udm_ue->ctx_id) {
        ogs_error("No memory for udm_ue->ctx_id [%s]", suci);
        ogs_pool_id_free(&udm_ue_pool, udm_ue);
        return NULL;
    }

    udm_ue->suci = ogs_strdup(suci);
    if (!udm_ue->suci) {
        ogs_error("No memory for udm_ue->suci [%s]", suci);
        ogs_free(udm_ue->ctx_id);
        ogs_pool_id_free(&udm_ue_pool, udm_ue);
        return NULL;
    }

    udm_ue->supi = ogs_supi_from_supi_or_suci(udm_ue->suci);
    if (!udm_ue->supi) {
        ogs_error("No memory for udm_ue->supi [%s]", suci);
        ogs_free(udm_ue->suci);
        ogs_free(udm_ue->ctx_id);
        ogs_pool_id_free(&udm_ue_pool, udm_ue);
        return NULL;
    }

    ogs_hash_set(self.suci_hash, udm_ue->suci, strlen(udm_ue->suci), udm_ue);
    ogs_hash_set(self.supi_hash, udm_ue->supi, strlen(udm_ue->supi), udm_ue);

    memset(&e, 0, sizeof(e));
    e.udm_ue_id = udm_ue->id;
    ogs_fsm_init(&udm_ue->sm, udm_ue_state_initial, udm_ue_state_final, &e);

    ogs_list_add(&self.udm_ue_list, udm_ue);

    return udm_ue;
}

void udm_ue_remove(udm_ue_t *udm_ue)
{
    udm_event_t e;

    ogs_assert(udm_ue);

    ogs_list_remove(&self.udm_ue_list, udm_ue);

    memset(&e, 0, sizeof(e));
    e.udm_ue_id = udm_ue->id;
    ogs_fsm_fini(&udm_ue->sm, &e);

    /* Free SBI object memory */
    ogs_sbi_object_free(&udm_ue->sbi);

    udm_sess_remove_all(udm_ue);
    udm_sdm_subscription_remove_all(udm_ue);

    OpenAPI_auth_event_free(udm_ue->auth_event);
    OpenAPI_amf3_gpp_access_registration_free(
            udm_ue->amf_3gpp_access_registration);

    ogs_assert(udm_ue->ctx_id);
    ogs_free(udm_ue->ctx_id);

    ogs_assert(udm_ue->suci);
    ogs_hash_set(self.suci_hash, udm_ue->suci, strlen(udm_ue->suci), NULL);
    ogs_free(udm_ue->suci);

    ogs_assert(udm_ue->supi);
    ogs_hash_set(self.supi_hash, udm_ue->supi, strlen(udm_ue->supi), NULL);
    ogs_free(udm_ue->supi);

    if (udm_ue->serving_network_name)
        ogs_free(udm_ue->serving_network_name);
    if (udm_ue->ausf_instance_id)
        ogs_free(udm_ue->ausf_instance_id);
    if (udm_ue->amf_instance_id)
        ogs_free(udm_ue->amf_instance_id);
    if (udm_ue->dereg_callback_uri)
        ogs_free(udm_ue->dereg_callback_uri);

    ogs_pool_id_free(&udm_ue_pool, udm_ue);
}

void udm_ue_remove_all(void)
{
    udm_ue_t *udm_ue = NULL, *next = NULL;;

    ogs_list_for_each_safe(&self.udm_ue_list, next, udm_ue)
        udm_ue_remove(udm_ue);
}

udm_ue_t *udm_ue_find_by_suci(char *suci)
{
    ogs_assert(suci);
    return (udm_ue_t *)ogs_hash_get(self.suci_hash, suci, strlen(suci));
}

udm_ue_t *udm_ue_find_by_supi(char *supi)
{
    ogs_assert(supi);
    return (udm_ue_t *)ogs_hash_get(self.supi_hash, supi, strlen(supi));
}

udm_ue_t *udm_ue_find_by_suci_or_supi(char *suci_or_supi)
{
    ogs_assert(suci_or_supi);
    if (strncmp(suci_or_supi, "suci-", strlen("suci-")) == 0)
        return udm_ue_find_by_suci(suci_or_supi);
    else
        return udm_ue_find_by_supi(suci_or_supi);
}

udm_ue_t *udm_ue_find_by_ctx_id(char *ctx_id)
{
    ogs_assert(ctx_id);
    return ogs_pool_find(&udm_ue_pool, atoll(ctx_id));
}

udm_sess_t *udm_sess_add(udm_ue_t *udm_ue, uint8_t psi)
{
    udm_event_t e;
    udm_sess_t *sess = NULL;

    ogs_assert(udm_ue);
    ogs_assert(psi != OGS_NAS_PDU_SESSION_IDENTITY_UNASSIGNED);

    ogs_pool_id_calloc(&udm_sess_pool, &sess);
    ogs_assert(sess);

    /* SBI Type */
    sess->sbi.type = OGS_SBI_OBJ_SESS_TYPE;

    sess->udm_ue_id = udm_ue->id;
    sess->psi = psi;

    memset(&e, 0, sizeof(e));
    e.sess_id = sess->id;
    ogs_fsm_init(&sess->sm, udm_sess_state_initial, udm_sess_state_final, &e);

    ogs_list_add(&udm_ue->sess_list, sess);

    return sess;
}

void udm_sess_remove(udm_sess_t *sess)
{
    udm_event_t e;
    udm_ue_t *udm_ue = NULL;

    ogs_assert(sess);
    udm_ue = udm_ue_find_by_id(sess->udm_ue_id);
    ogs_assert(udm_ue);

    ogs_list_remove(&udm_ue->sess_list, sess);

    memset(&e, 0, sizeof(e));
    e.sess_id = sess->id;
    ogs_fsm_fini(&sess->sm, &e);

    /* Free SBI object memory */
    if (ogs_list_count(&sess->sbi.xact_list))
        ogs_error("Session transaction [%d]",
                ogs_list_count(&sess->sbi.xact_list));
    ogs_sbi_object_free(&sess->sbi);

    OpenAPI_smf_registration_free(sess->smf_registration);

    if (sess->smf_instance_id)
        ogs_free(sess->smf_instance_id);

    ogs_pool_id_free(&udm_sess_pool, sess);
}

void udm_sess_remove_all(udm_ue_t *udm_ue)
{
    udm_sess_t *sess = NULL, *next_sess = NULL;

    ogs_assert(udm_ue);

    ogs_list_for_each_safe(&udm_ue->sess_list, next_sess, sess)
        udm_sess_remove(sess);
}

udm_sess_t *udm_sess_find_by_psi(udm_ue_t *udm_ue, uint8_t psi)
{
    udm_sess_t *sess = NULL;

    ogs_list_for_each(&udm_ue->sess_list, sess)
        if (psi == sess->psi) return sess;

    return NULL;
}

udm_ue_t *udm_ue_find_by_id(ogs_pool_id_t id)
{
    return ogs_pool_find_by_id(&udm_ue_pool, id);
}

udm_sess_t *udm_sess_find_by_id(ogs_pool_id_t id)
{
    return ogs_pool_find_by_id(&udm_sess_pool, id);
}

udm_sdm_subscription_t *udm_sdm_subscription_add(udm_ue_t *udm_ue)
{
    udm_sdm_subscription_t *sdm_subscription = NULL;

    char id[OGS_UUID_FORMATTED_LENGTH + 1];
    ogs_uuid_t uuid;

    ogs_assert(udm_ue);

    ogs_uuid_get(&uuid);
    ogs_uuid_format(id, &uuid);

    ogs_pool_alloc(&udm_sdm_subscription_pool, &sdm_subscription);
    if (!sdm_subscription) {
        ogs_error("Maximum number of SDM Subscriptions [%d] reached",
                    max_num_of_udm_sdm_subscriptions);
        return NULL;
    }
    memset(sdm_subscription, 0, sizeof *sdm_subscription);

    sdm_subscription->id = ogs_strdup(id);
    if (!sdm_subscription->id) {
        ogs_error("No memory for sdm_subscription->id [%s]", udm_ue->suci);
        ogs_pool_free(&udm_sdm_subscription_pool, sdm_subscription);
        return NULL;
    }

    sdm_subscription->udm_ue = udm_ue;

    ogs_hash_set(self.sdm_subscription_id_hash, sdm_subscription->id,
            strlen(sdm_subscription->id), sdm_subscription);

    ogs_list_add(&udm_ue->sdm_subscription_list, sdm_subscription);

    return sdm_subscription;
}

void udm_sdm_subscription_remove(udm_sdm_subscription_t *sdm_subscription)
{
    ogs_assert(sdm_subscription);
    ogs_assert(sdm_subscription->udm_ue);

    ogs_list_remove(&sdm_subscription->udm_ue->sdm_subscription_list,
            sdm_subscription);

    ogs_assert(sdm_subscription->id);
    ogs_hash_set(self.sdm_subscription_id_hash, sdm_subscription->id, 
            strlen(sdm_subscription->id), NULL);
    ogs_free(sdm_subscription->id);

    if (sdm_subscription->data_change_callback_uri)
        ogs_free(sdm_subscription->data_change_callback_uri);

    ogs_pool_free(&udm_sdm_subscription_pool, sdm_subscription);
}

void udm_sdm_subscription_remove_all(udm_ue_t *udm_ue)
{
    udm_sdm_subscription_t *sdm_subscription = NULL,
            *next_sdm_subscription = NULL;

    ogs_assert(udm_ue);

    ogs_list_for_each_safe(&udm_ue->sdm_subscription_list,
            next_sdm_subscription, sdm_subscription)
        udm_sdm_subscription_remove(sdm_subscription);
}

udm_sdm_subscription_t *udm_sdm_subscription_find_by_id(char *id)
{
    ogs_assert(id);
    return (udm_sdm_subscription_t *)ogs_hash_get(self.sdm_subscription_id_hash,
            id, strlen(id));
}

int get_ue_load(void)
{
    return (((ogs_pool_size(&udm_ue_pool) -
            ogs_pool_avail(&udm_ue_pool)) * 100) /
            ogs_pool_size(&udm_ue_pool));
}
