/*
 * Copyright (C) 2026 by Daniel Brune <daniel.brune.so@outlook.com>
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

/*
 * Timer values received from the NRF (or the SCP) are passed to
 * ogs_timer_start(): a non-positive validityPeriod or heartBeatTimer is
 * not used.
 */

#include "ogs-sbi.h"
#include "core/abts.h"

#define TEST_NF_INSTANCE_ID "c5f7e1a6-3b2d-41f0-9c8e-7a6b5d4c3b2a"

static OpenAPI_search_result_t *search_result_new(int validity_period)
{
    OpenAPI_search_result_t *SearchResult = NULL;
    OpenAPI_nf_profile_t *NFProfile = NULL;

    NFProfile = ogs_calloc(1, sizeof(*NFProfile));
    ogs_assert(NFProfile);
    NFProfile->nf_instance_id = ogs_strdup(TEST_NF_INSTANCE_ID);
    NFProfile->nf_type = OpenAPI_nf_type_UDM;
    NFProfile->nf_status = OpenAPI_nf_status_REGISTERED;

    SearchResult = ogs_calloc(1, sizeof(*SearchResult));
    ogs_assert(SearchResult);
    SearchResult->validity_period = validity_period;
    SearchResult->nf_instances = OpenAPI_list_create();
    ogs_assert(SearchResult->nf_instances);
    OpenAPI_list_add(SearchResult->nf_instances, NFProfile);

    return SearchResult;
}

static void discovered_remove(void)
{
    ogs_sbi_nf_instance_t *nf_instance =
        ogs_sbi_nf_instance_find((char *)TEST_NF_INSTANCE_ID);

    if (nf_instance) {
        ogs_sbi_nf_fsm_fini(nf_instance);
        ogs_sbi_nf_instance_remove(nf_instance);
    }
}

/* A positive validityPeriod caches the discovered NF instance */
static void test_validity_period(abts_case *tc, void *data)
{
    OpenAPI_search_result_t *SearchResult = search_result_new(10);
    ogs_sbi_nf_instance_t *nf_instance = NULL;

    ogs_nnrf_disc_handle_nf_discover_search_result(SearchResult);
    OpenAPI_search_result_free(SearchResult);

    nf_instance = ogs_sbi_nf_instance_find((char *)TEST_NF_INSTANCE_ID);
    ABTS_PTR_NOTNULL(tc, nf_instance);
    if (nf_instance) {
        ABTS_INT_EQUAL(tc, 10, nf_instance->time.validity_duration);
        ABTS_TRUE(tc, nf_instance->t_validity->running == true);
    }

    discovered_remove();
}

/* A non-positive validityPeriod is not used */
static void test_validity_period_not_positive(abts_case *tc, void *data)
{
    int validity_period[] = { -1, 0 };
    int i;

    for (i = 0; i < OGS_ARRAY_SIZE(validity_period); i++) {
        OpenAPI_search_result_t *SearchResult =
            search_result_new(validity_period[i]);

        ogs_nnrf_disc_handle_nf_discover_search_result(SearchResult);
        OpenAPI_search_result_free(SearchResult);

        ABTS_PTR_EQUAL(tc, NULL,
                ogs_sbi_nf_instance_find((char *)TEST_NF_INSTANCE_ID));
        discovered_remove();
    }
}

/*
 * heartBeatTimer in the NRF response to the registration: a non-positive
 * value is not used, the configured heartbeat interval stays
 */
static void test_heart_beat_timer(abts_case *tc, void *data)
{
    int heart_beat_timer[] = { -5, 0, 20 };
    int expected[] = { 10, 10, 20 };
    ogs_sbi_nf_instance_t *nf_instance = NULL;
    int i;

    nf_instance = ogs_sbi_nf_instance_add();
    ogs_assert(nf_instance);
    nf_instance->time.heartbeat_interval = 10;

    for (i = 0; i < OGS_ARRAY_SIZE(heart_beat_timer); i++) {
        ogs_sbi_message_t message;
        OpenAPI_nf_profile_t NFProfile;

        memset(&message, 0, sizeof(message));
        memset(&NFProfile, 0, sizeof(NFProfile));
        NFProfile.is_heart_beat_timer = true;
        NFProfile.heart_beat_timer = heart_beat_timer[i];
        message.NFProfile = &NFProfile;

        ogs_nnrf_nfm_handle_nf_register(nf_instance, &message);
        ABTS_INT_EQUAL(tc, expected[i], nf_instance->time.heartbeat_interval);
    }

    ogs_sbi_nf_instance_remove(nf_instance);
}

abts_suite *test_nnrf_timer_value(abts_suite *suite)
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
    ogs_sbi_self()->tls.client.scheme = OpenAPI_uri_scheme_http;
    /* The invalid values are logged as errors */
    ogs_log_set_domain_level(__ogs_sbi_domain, OGS_LOG_FATAL);

    abts_run_test(suite, test_validity_period, NULL);
    abts_run_test(suite, test_validity_period_not_positive, NULL);
    abts_run_test(suite, test_heart_beat_timer, NULL);

    ogs_sbi_context_final();
    ogs_app_context_final();

    *ogs_app() = saved_app;
    *ogs_sbi_self() = saved_sbi;
    ogs_global_conf()->max.ue = max_ue;
    ogs_sbi_message_init(32, 32);
    ogs_log_set_domain_level(__ogs_sbi_domain, sbi_level);

    return suite;
}
