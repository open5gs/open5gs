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
#include "event.h"
#include "nudr-handler.h"

static udr_context_t self;

int __udr_log_domain;

static int context_initialized = 0;

#define UDR_MAX_NUM_OF_SUBSCRIPTION_PER_UE 4
static int max_num_of_subscription = 0;
static OGS_POOL(subscription_pool, udr_subscription_t);

void udr_context_init(void)
{
    ogs_assert(context_initialized == 0);

    /* Initialize UDR context */
    memset(&self, 0, sizeof(udr_context_t));

    ogs_log_install_domain(&__ogs_dbi_domain, "dbi", ogs_core()->log.level);
    ogs_log_install_domain(&__udr_log_domain, "udr", ogs_core()->log.level);

    ogs_thread_mutex_init(&self.db_lock);

    ogs_list_init(&self.subscription_list);
    max_num_of_subscription =
        ogs_global_conf()->max.ue * UDR_MAX_NUM_OF_SUBSCRIPTION_PER_UE;
    ogs_pool_init(&subscription_pool, max_num_of_subscription);

    context_initialized = 1;
}

void udr_context_final(void)
{
    ogs_assert(context_initialized == 1);

    udr_subscription_remove_all();
    ogs_pool_final(&subscription_pool);

    ogs_thread_mutex_destroy(&self.db_lock);

    context_initialized = 0;
}

udr_context_t *udr_self(void)
{
    return &self;
}

static int udr_context_prepare(void)
{
    return OGS_OK;
}

static int udr_context_validation(void)
{
    return OGS_OK;
}

int udr_context_parse_config(void)
{
    int rv;
    yaml_document_t *document = NULL;
    ogs_yaml_iter_t root_iter;
    int idx = 0;

    document = ogs_app()->document;
    ogs_assert(document);

    rv = udr_context_prepare();
    if (rv != OGS_OK) return rv;

    ogs_yaml_iter_init(&root_iter, document);
    while (ogs_yaml_iter_next(&root_iter)) {
        const char *root_key = ogs_yaml_iter_key(&root_iter);
        ogs_assert(root_key);
        if ((!strcmp(root_key, "udr")) &&
            (idx++ == ogs_app()->config_section_id)) {
            ogs_yaml_iter_t udr_iter;
            ogs_yaml_iter_recurse(&root_iter, &udr_iter);
            while (ogs_yaml_iter_next(&udr_iter)) {
                const char *udr_key = ogs_yaml_iter_key(&udr_iter);
                ogs_assert(udr_key);
                if (!strcmp(udr_key, "default")) {
                    /* handle config in sbi library */
                } else if (!strcmp(udr_key, "sbi")) {
                    /* handle config in sbi library */
                } else if (!strcmp(udr_key, "nrf")) {
                    /* handle config in sbi library */
                } else if (!strcmp(udr_key, "scp")) {
                    /* handle config in sbi library */
                } else if (!strcmp(udr_key, "service_name")) {
                    /* handle config in sbi library */
                } else if (!strcmp(udr_key, "discovery")) {
                    /* handle config in sbi library */
                } else if (!strcmp(udr_key, "use_mongodb_change_stream")) {
#if MONGOC_CHECK_VERSION(1, 9, 0)
                    self.use_mongodb_change_stream =
                        ogs_yaml_iter_bool(&udr_iter);
#else
                    self.use_mongodb_change_stream = false;
#endif
                } else
                    ogs_warn("unknown key `%s`", udr_key);
            }
        }
    }

    rv = udr_context_validation();
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}

static bool key_touches_slice(const char *key)
{
    return !strcmp(key, "slice") || !strncmp(key, "slice.", strlen("slice."));
}

/* sm-data is derived from the subscriber's `slice` array, so only
 * updates that touch `slice` invalidate a subscriber's sm-data resource. */
static bool update_description_touches_slice(bson_iter_t *update_desc_iter)
{
    bson_iter_t area_iter, field_iter;

    bson_iter_recurse(update_desc_iter, &area_iter);
    while (bson_iter_next(&area_iter)) {
        const char *area_key = bson_iter_key(&area_iter);

        if (!strcmp(area_key, "updatedFields") &&
                BSON_ITER_HOLDS_DOCUMENT(&area_iter)) {
            bson_iter_recurse(&area_iter, &field_iter);
            while (bson_iter_next(&field_iter))
                if (key_touches_slice(bson_iter_key(&field_iter)))
                    return true;

        } else if (!strcmp(area_key, "removedFields") &&
                BSON_ITER_HOLDS_ARRAY(&area_iter)) {
            bson_iter_recurse(&area_iter, &field_iter);
            while (bson_iter_next(&field_iter))
                if (BSON_ITER_HOLDS_UTF8(&field_iter) &&
                        key_touches_slice(bson_iter_utf8(&field_iter, NULL)))
                    return true;

        } else if (!strcmp(area_key, "truncatedArrays") &&
                BSON_ITER_HOLDS_ARRAY(&area_iter)) {
            bson_iter_t item_iter;
            bson_iter_recurse(&area_iter, &field_iter);
            while (bson_iter_next(&field_iter)) {
                if (BSON_ITER_HOLDS_DOCUMENT(&field_iter) &&
                        bson_iter_recurse(&field_iter, &item_iter) &&
                        bson_iter_find(&item_iter, "field") &&
                        BSON_ITER_HOLDS_UTF8(&item_iter) &&
                        key_touches_slice(bson_iter_utf8(&item_iter, NULL)))
                    return true;
            }
        }
    }

    return false;
}

int udr_handle_change_event(const bson_t *document)
{
    bson_iter_t iter, full_doc_iter;
    const char *operation_type = NULL;
    char *imsi_bcd = NULL;
    char *supi = NULL;
    bool sm_data_changed = false;

    ogs_assert(document);

    if (!bson_iter_init_find(&iter, document, "operationType") ||
            !BSON_ITER_HOLDS_UTF8(&iter)) {
        ogs_error("No 'operationType' field in this document.");
        return OGS_ERROR;
    }
    operation_type = bson_iter_utf8(&iter, NULL);

    if (strcmp(operation_type, "update") &&
            strcmp(operation_type, "replace")) {
        /* Inserts/deletes are not handled in this first iteration. */
        return OGS_OK;
    }

    if (!bson_iter_init_find(&iter, document, "fullDocument") ||
            !BSON_ITER_HOLDS_DOCUMENT(&iter)) {
        ogs_error("No 'fullDocument' field in this document.");
        return OGS_ERROR;
    }
    bson_iter_recurse(&iter, &full_doc_iter);
    while (bson_iter_next(&full_doc_iter)) {
        if (!strcmp(bson_iter_key(&full_doc_iter), "imsi") &&
                BSON_ITER_HOLDS_UTF8(&full_doc_iter)) {
            uint32_t length = 0;
            const char *utf8 = bson_iter_utf8(&full_doc_iter, &length);
            imsi_bcd = ogs_strndup(utf8,
                    ogs_min(length, OGS_MAX_IMSI_BCD_LEN) + 1);
            ogs_assert(imsi_bcd);
        }
    }

    if (!imsi_bcd) {
        ogs_error("No 'imsi' field in this document.");
        return OGS_ERROR;
    }

    if (!strcmp(operation_type, "replace")) {
        /* No pre-image is available, so a whole-document replacement is
         * conservatively treated as an sm-data change. */
        sm_data_changed = true;
    } else if (bson_iter_init_find(&iter, document, "updateDescription") &&
            BSON_ITER_HOLDS_DOCUMENT(&iter)) {
        sm_data_changed = update_description_touches_slice(&iter);
    }

    if (sm_data_changed) {
        supi = ogs_msprintf("%s-%s", OGS_ID_SUPI_TYPE_IMSI, imsi_bcd);
        ogs_assert(supi);

        ogs_info("[%s] sm-data changed", supi);

        udr_nudr_dr_notify_sm_data_change(supi);

        ogs_free(supi);
    }

    ogs_free(imsi_bcd);

    return OGS_OK;
}

static int poll_change_stream(void);

int udr_db_poll_change_stream(void)
{
    int rv;

    ogs_thread_mutex_lock(&self.db_lock);

    rv = poll_change_stream();

    ogs_thread_mutex_unlock(&self.db_lock);

    return rv;
}

static int process_change_stream(const bson_t *document)
{
    int rv;

    udr_event_t *e = NULL;

    e = udr_event_new(UDR_EVENT_DBI_MESSAGE);
    ogs_assert(e);
    e->dbi.document = bson_copy(document);
    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_error("ogs_queue_push() failed:%d", (int)rv);
        bson_destroy(e->dbi.document);
        ogs_event_free(e);
    } else {
        ogs_pollset_notify(ogs_app()->pollset);
    }

    return OGS_OK;
}

static int poll_change_stream(void)
{
#if MONGOC_CHECK_VERSION(1, 9, 0)
    int rv;

    const bson_t *document;
    const bson_t *err_document;
    bson_error_t error;

    while (mongoc_change_stream_next(ogs_mongoc()->stream, &document)) {
        rv = process_change_stream(document);
        if (rv != OGS_OK) return rv;
    }

    if (mongoc_change_stream_error_document(ogs_mongoc()->stream, &error,
            &err_document)) {
        if (!bson_empty(err_document)) {
            ogs_debug("Server Error: %s\n",
                    bson_as_relaxed_extended_json(err_document, NULL));
        } else {
            ogs_debug("Client Error: %s\n", error.message);
        }
        return OGS_ERROR;
    }

    return OGS_OK;
#else
    return OGS_ERROR;
#endif
}

udr_subscription_t *udr_subscription_add(void)
{
    udr_subscription_t *subscription = NULL;

    ogs_pool_alloc(&subscription_pool, &subscription);
    if (!subscription) {
        ogs_error("OVERFLOW subscription_pool [pool:%d]",
                max_num_of_subscription);
        return NULL;
    }
    memset(subscription, 0, sizeof(udr_subscription_t));

    ogs_list_add(&self.subscription_list, subscription);

    return subscription;
}

udr_subscription_t *udr_subscription_find_by_id(const char *id)
{
    udr_subscription_t *subscription = NULL;

    ogs_assert(id);

    ogs_list_for_each(&self.subscription_list, subscription) {
        ogs_assert(subscription->id);
        if (strcmp(subscription->id, id) == 0)
            break;
    }

    return subscription;
}

void udr_subscription_remove(udr_subscription_t *subscription)
{
    int i;

    ogs_assert(subscription);

    ogs_list_remove(&self.subscription_list, subscription);

    if (subscription->id)
        ogs_free(subscription->id);
    if (subscription->callback_reference)
        ogs_free(subscription->callback_reference);
    if (subscription->original_callback_reference)
        ogs_free(subscription->original_callback_reference);
    if (subscription->ue_id)
        ogs_free(subscription->ue_id);

    for (i = 0; i < subscription->num_of_monitored_resource_uri; i++)
        ogs_free(subscription->monitored_resource_uri[i]);

    if (subscription->client)
        ogs_sbi_client_remove(subscription->client);

    ogs_pool_free(&subscription_pool, subscription);
}

void udr_subscription_remove_all(void)
{
    udr_subscription_t *subscription = NULL, *next_subscription = NULL;

    ogs_list_for_each_safe(&self.subscription_list,
            next_subscription, subscription) {
        udr_subscription_remove(subscription);
    }
}
