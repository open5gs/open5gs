/*
 * Copyright (C) 2019-2026 by Sukchan Lee <acetcom@gmail.com>
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

#include "ogs-sbi.h"
#include "core/abts.h"

static ogs_sbi_subscription_data_t *subscription_add(const char *id)
{
    ogs_sbi_subscription_data_t *subscription_data = NULL;

    subscription_data = ogs_sbi_subscription_data_add();
    ogs_assert(subscription_data);
    ogs_sbi_subscription_data_set_id(subscription_data, (char *)id);

    return subscription_data;
}

static void response_handle(
        const char *method, int status, ogs_pool_id_t pool_id)
{
    ogs_sbi_message_t message;

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)method;
    message.res_status = status;

    ogs_nnrf_nfm_handle_nf_status_subscription_response(
            &message, OGS_UINT_TO_POINTER(pool_id));
}

static void removed_response_handle(
        const char *method, int status, ogs_pool_id_t pool_id)
{
    ogs_log_level_e level = ogs_log_get_domain_level(__ogs_sbi_domain);

    /* Responses for removed subscriptions are expected to be rejected. */
    if (level >= OGS_LOG_ERROR && level < OGS_LOG_DEBUG)
        ogs_log_set_domain_level(__ogs_sbi_domain, OGS_LOG_FATAL);
    response_handle(method, status, pool_id);
    ogs_log_set_domain_level(__ogs_sbi_domain, level);
}

static void nnrf_subscription_test_delete(abts_case *tc, void *data)
{
    ogs_sbi_subscription_data_t *subscription_data = NULL;
    ogs_pool_id_t pool_id;
    int count = ogs_list_count(&ogs_sbi_self()->subscription_data_list);

    subscription_data = subscription_add("sub-1");
    pool_id = subscription_data->pool_id;

    response_handle(OGS_SBI_HTTP_METHOD_DELETE,
            OGS_SBI_HTTP_STATUS_NO_CONTENT, pool_id);

    ABTS_PTR_EQUAL(tc, NULL,
            ogs_sbi_subscription_data_find_by_pool_id(pool_id));
    ABTS_INT_EQUAL(tc, count,
            ogs_list_count(&ogs_sbi_self()->subscription_data_list));
}

static void nnrf_subscription_test_delete_after_remove(
        abts_case *tc, void *data)
{
    ogs_sbi_subscription_data_t *subscription_data = NULL;
    ogs_sbi_subscription_data_t *next1 = NULL, *next2 = NULL;
    ogs_pool_id_t pool_id;
    int count = ogs_list_count(&ogs_sbi_self()->subscription_data_list);

    subscription_data = subscription_add("sub-2");
    pool_id = subscription_data->pool_id;
    ogs_sbi_subscription_data_remove(subscription_data);

    removed_response_handle(OGS_SBI_HTTP_METHOD_DELETE,
            OGS_SBI_HTTP_STATUS_NO_CONTENT, pool_id);

    ABTS_INT_EQUAL(tc, count,
            ogs_list_count(&ogs_sbi_self()->subscription_data_list));

    next1 = subscription_add("sub-3");
    next2 = subscription_add("sub-4");
    ABTS_TRUE(tc, next1 != next2);
    ABTS_INT_EQUAL(tc, count + 2,
            ogs_list_count(&ogs_sbi_self()->subscription_data_list));

    ogs_sbi_subscription_data_remove(next1);
    ogs_sbi_subscription_data_remove(next2);
}

static void nnrf_subscription_test_delete_slot_reused(
        abts_case *tc, void *data)
{
    ogs_sbi_subscription_data_t *subscription_data = NULL, *next = NULL;
    ogs_sbi_subscription_data_t *filler[8];
    ogs_pool_id_t pool_id;
    int i, num_of_filler = 0;

    subscription_data = subscription_add("sub-5");
    pool_id = subscription_data->pool_id;
    ogs_sbi_subscription_data_remove(subscription_data);

    /* Take free slots until the removed one is handed out again */
    while ((next = ogs_sbi_subscription_data_add()) != subscription_data) {
        ogs_assert(next);
        ogs_assert(num_of_filler < OGS_ARRAY_SIZE(filler));
        filler[num_of_filler++] = next;
    }
    ogs_sbi_subscription_data_set_id(next, (char *)"sub-6");
    ABTS_TRUE(tc, next->pool_id != pool_id);

    removed_response_handle(OGS_SBI_HTTP_METHOD_DELETE,
            OGS_SBI_HTTP_STATUS_NO_CONTENT, pool_id);

    ABTS_PTR_EQUAL(tc, next,
            ogs_sbi_subscription_data_find_by_pool_id(next->pool_id));
    ABTS_STR_EQUAL(tc, "sub-6", next->id);

    ogs_sbi_subscription_data_remove(next);
    for (i = 0; i < num_of_filler; i++)
        ogs_sbi_subscription_data_remove(filler[i]);
}

static void nnrf_subscription_test_update(abts_case *tc, void *data)
{
    ogs_sbi_subscription_data_t *subscription_data = NULL;

    subscription_data = subscription_add("sub-9");
    subscription_data->validity_duration = ogs_time_from_sec(60);

    response_handle(OGS_SBI_HTTP_METHOD_PATCH,
            OGS_SBI_HTTP_STATUS_NO_CONTENT, subscription_data->pool_id);

    ABTS_PTR_NOTNULL(tc, subscription_data->t_validity);
    ABTS_PTR_NOTNULL(tc, subscription_data->t_patch);

    ogs_sbi_subscription_data_remove(subscription_data);
}

static void nnrf_subscription_test_update_after_remove(
        abts_case *tc, void *data)
{
    ogs_sbi_subscription_data_t *subscription_data = NULL, *next = NULL;
    ogs_pool_id_t pool_id;
    int count = ogs_list_count(&ogs_sbi_self()->subscription_data_list);

    subscription_data = subscription_add("sub-7");
    subscription_data->validity_duration = ogs_time_from_sec(60);
    pool_id = subscription_data->pool_id;
    ogs_sbi_subscription_data_remove(subscription_data);

    removed_response_handle(OGS_SBI_HTTP_METHOD_PATCH,
            OGS_SBI_HTTP_STATUS_NO_CONTENT, pool_id);

    ABTS_INT_EQUAL(tc, count,
            ogs_list_count(&ogs_sbi_self()->subscription_data_list));

    next = subscription_add("sub-8");
    ABTS_PTR_EQUAL(tc, next, ogs_sbi_subscription_data_find((char *)"sub-8"));
    ABTS_PTR_EQUAL(tc, NULL, next->t_validity);
    ABTS_PTR_EQUAL(tc, NULL, next->t_patch);
    ogs_sbi_subscription_data_remove(next);
}

abts_suite *test_nnrf_subscription(abts_suite *suite)
{
    ogs_app_context_t saved_app = *ogs_app();
    ogs_sbi_context_t saved_sbi = *ogs_sbi_self();
    ogs_log_level_e sbi_level = ogs_log_get_domain_level(__ogs_sbi_domain);
    uint64_t max_ue = ogs_global_conf()->max.ue;

    suite = ADD_SUITE(suite)
    if (suite->tail->not_run)
        return suite;

    /* The SBI context temporarily owns the unit runner's message pools. */
    ogs_sbi_message_final();
    ogs_app_context_init();
    ogs_app()->pool.nf = 8;
    ogs_app()->pool.nf_service = 8;
    ogs_app()->pool.message = 16;
    ogs_app()->pool.event = 8;
    ogs_app()->pool.subscription = 8;
    ogs_app()->pool.xact = 8;
    ogs_app()->pool.timer = 32;
    ogs_app()->pool.stream = 4;
    ogs_global_conf()->max.ue = 8;
    ogs_app()->timer_mgr = ogs_timer_mgr_create(ogs_app()->pool.timer);
    ogs_assert(ogs_app()->timer_mgr);
    ogs_app()->queue = ogs_queue_create(ogs_app()->pool.event);
    ogs_assert(ogs_app()->queue);

    ogs_sbi_context_init(OpenAPI_nf_type_AMF);
    ogs_log_set_domain_level(__ogs_sbi_domain, sbi_level);

    abts_run_test(suite, nnrf_subscription_test_delete, NULL);
    abts_run_test(suite, nnrf_subscription_test_delete_after_remove, NULL);
    abts_run_test(suite, nnrf_subscription_test_delete_slot_reused, NULL);
    abts_run_test(suite, nnrf_subscription_test_update, NULL);
    abts_run_test(suite, nnrf_subscription_test_update_after_remove, NULL);

    ogs_sbi_context_final();
    ogs_app_context_final();

    *ogs_app() = saved_app;
    *ogs_sbi_self() = saved_sbi;
    ogs_global_conf()->max.ue = max_ue;
    ogs_sbi_message_init(32, 32);

    return suite;
}
