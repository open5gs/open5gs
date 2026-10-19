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

#include "nrf/context.h"
#include "core/abts.h"

/* Queue the event the timer would queue on expiry */
static void timer_fire(ogs_timer_t *timer)
{
    ogs_assert(timer);
    timer->cb(timer->data);
}

static void dispatch_all(void)
{
    ogs_fsm_t sm;
    nrf_event_t *e = NULL;
    ogs_log_level_e level = ogs_log_get_domain_level(__nrf_log_domain);

    /* Stale timer events are expected to be rejected. */
    if (level >= OGS_LOG_ERROR && level < OGS_LOG_DEBUG)
        ogs_log_set_domain_level(__nrf_log_domain, OGS_LOG_FATAL);
    memset(&sm, 0, sizeof(sm));
    while (ogs_queue_trypop(ogs_app()->queue, (void **)&e) == OGS_OK) {
        nrf_state_operational(&sm, e);
        ogs_event_free(e);
    }
    ogs_log_set_domain_level(__nrf_log_domain, level);
}

static ogs_sbi_nf_instance_t *nf_instance_add(const char *id)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL;

    nf_instance = ogs_sbi_nf_instance_add();
    ogs_assert(nf_instance);
    ogs_sbi_nf_instance_set_id(nf_instance, (char *)id);
    ogs_sbi_nf_instance_set_type(nf_instance, OpenAPI_nf_type_AMF);
    nrf_nf_fsm_init(nf_instance);

    return nf_instance;
}

static void nf_instance_remove(ogs_sbi_nf_instance_t *nf_instance)
{
    nrf_nf_fsm_fini(nf_instance);
    ogs_sbi_nf_instance_remove(nf_instance);
}

static void nrf_timer_test_no_heartbeat_after_remove(
        abts_case *tc, void *data)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL, *next1 = NULL, *next2 = NULL;
    ogs_pool_id_t pool_id;
    int count = ogs_list_count(&ogs_sbi_self()->nf_instance_list);

    nf_instance = nf_instance_add("0e1b3a0c-0000-4000-8000-000000000001");
    pool_id = nf_instance->pool_id;
    ABTS_PTR_EQUAL(tc, nf_instance,
            ogs_sbi_nf_instance_find_by_pool_id(pool_id));

    timer_fire(nf_instance->t_no_heartbeat);
    nf_instance_remove(nf_instance);
    ABTS_PTR_EQUAL(tc, NULL, ogs_sbi_nf_instance_find_by_pool_id(pool_id));

    dispatch_all();

    ABTS_INT_EQUAL(tc, count,
            ogs_list_count(&ogs_sbi_self()->nf_instance_list));

    next1 = nf_instance_add("0e1b3a0c-0000-4000-8000-000000000002");
    next2 = nf_instance_add("0e1b3a0c-0000-4000-8000-000000000003");
    ABTS_TRUE(tc, next1 != next2);
    ABTS_INT_EQUAL(tc, count + 2,
            ogs_list_count(&ogs_sbi_self()->nf_instance_list));

    nf_instance_remove(next1);
    nf_instance_remove(next2);
}

static void nrf_timer_test_no_heartbeat_slot_reused(
        abts_case *tc, void *data)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL, *next = NULL;
    ogs_sbi_nf_instance_t *filler[8];
    ogs_pool_id_t pool_id;
    int i, num_of_filler = 0;

    nf_instance = nf_instance_add("0e1b3a0c-0000-4000-8000-000000000004");
    pool_id = nf_instance->pool_id;

    timer_fire(nf_instance->t_no_heartbeat);
    nf_instance_remove(nf_instance);

    /* Take free slots until the removed one is handed out again */
    while ((next = ogs_sbi_nf_instance_add()) != nf_instance) {
        ogs_assert(next);
        ogs_assert(num_of_filler < OGS_ARRAY_SIZE(filler));
        filler[num_of_filler++] = next;
    }
    ogs_sbi_nf_instance_set_id(next,
            (char *)"0e1b3a0c-0000-4000-8000-000000000005");
    ogs_sbi_nf_instance_set_type(next, OpenAPI_nf_type_AMF);
    nrf_nf_fsm_init(next);
    ABTS_TRUE(tc, next->pool_id != pool_id);

    dispatch_all();

    ABTS_PTR_EQUAL(tc, next, ogs_sbi_nf_instance_find(
                (char *)"0e1b3a0c-0000-4000-8000-000000000005"));

    nf_instance_remove(next);
    for (i = 0; i < num_of_filler; i++)
        ogs_sbi_nf_instance_remove(filler[i]);
}

static void nrf_timer_test_subscription_validity_after_remove(
        abts_case *tc, void *data)
{
    ogs_sbi_subscription_data_t *subscription_data = NULL;
    ogs_sbi_subscription_data_t *next1 = NULL, *next2 = NULL;
    ogs_pool_id_t pool_id;
    int count = ogs_list_count(&ogs_sbi_self()->subscription_data_list);

    subscription_data = ogs_sbi_subscription_data_add();
    ogs_assert(subscription_data);
    ogs_sbi_subscription_data_set_id(subscription_data, (char *)"sub-1");
    subscription_data->t_validity = ogs_timer_add(ogs_app()->timer_mgr,
            nrf_timer_subscription_validity,
            OGS_UINT_TO_POINTER(subscription_data->pool_id));
    ogs_assert(subscription_data->t_validity);
    pool_id = subscription_data->pool_id;

    timer_fire(subscription_data->t_validity);
    ogs_sbi_subscription_data_remove(subscription_data);
    ABTS_PTR_EQUAL(tc, NULL,
            ogs_sbi_subscription_data_find_by_pool_id(pool_id));

    dispatch_all();

    ABTS_INT_EQUAL(tc, count,
            ogs_list_count(&ogs_sbi_self()->subscription_data_list));

    next1 = ogs_sbi_subscription_data_add();
    next2 = ogs_sbi_subscription_data_add();
    ogs_assert(next1 && next2);
    ABTS_TRUE(tc, next1 != next2);

    ogs_sbi_subscription_data_remove(next1);
    ogs_sbi_subscription_data_remove(next2);
}

abts_suite *test_nrf_timer(abts_suite *suite)
{
    ogs_app_context_t saved_app = *ogs_app();
    ogs_sbi_context_t saved_sbi = *ogs_sbi_self();
    ogs_log_level_e sbi_level = ogs_log_get_domain_level(__ogs_sbi_domain);
    ogs_log_level_e nrf_level = ogs_log_get_domain_level(__nrf_log_domain);
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

    ogs_sbi_context_init(OpenAPI_nf_type_NRF);
    nrf_context_init();
    ogs_log_set_domain_level(__ogs_sbi_domain, sbi_level);
    ogs_log_set_domain_level(__nrf_log_domain, nrf_level);

    abts_run_test(suite, nrf_timer_test_no_heartbeat_after_remove, NULL);
    abts_run_test(suite, nrf_timer_test_no_heartbeat_slot_reused, NULL);
    abts_run_test(suite,
            nrf_timer_test_subscription_validity_after_remove, NULL);

    nrf_context_final();
    ogs_sbi_context_final();
    ogs_app_context_final();

    *ogs_app() = saved_app;
    *ogs_sbi_self() = saved_sbi;
    ogs_global_conf()->max.ue = max_ue;
    ogs_sbi_message_init(32, 32);

    return suite;
}
