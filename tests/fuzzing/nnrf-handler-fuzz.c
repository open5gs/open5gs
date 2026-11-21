/*
 * Copyright (C) 2019-2026 by Arthur SC Chan <arthur.chan@adalogics.com>
 *
 * This file is part of Open5GS.
 *
 * Threat model: NRF NF-profile and NF-discovery JSON from a peer NF (or a
 * compromised NRF). sbi_nf_profile_fuzz parses the model but never applies
 * it. nnrf-handler.c (1316), nnrf-build.c (1482) and nf-sm.c (480) are
 * 0-covered in the 20261114 report. Required JSON keys (validityPeriod,
 * nfInstances, nfInstanceId, nfType, nfStatus) are checked before the
 * handlers' ogs_assert() preconditions. SUPI-shaped ids are attacker data.
 */

#include <stdint.h>
#include <string.h>

#include "fuzzing.h"
#include "ogs-app.h"
#include "ogs-sbi.h"

#define kMinInputLength 2
#define kMaxInputLength 16384

static void init_once(void)
{
    if (initialized)
        return;
    initialize();
    ogs_log_install_domain(&__ogs_sbi_domain, "sbi", OGS_LOG_NONE);
    ogs_app_context_init();
    ogs_app()->timer_mgr = ogs_timer_mgr_create(256);
    ogs_app()->pool.nf = 24;
    ogs_app()->pool.nf_service = 48;
    ogs_app()->pool.message = 16;
    ogs_app()->pool.event = 16;
    ogs_app()->pool.subscription = 16;
    ogs_app()->pool.xact = 16;
    ogs_local_conf()->time.message.duration = ogs_time_from_sec(4);
    ogs_local_conf()->time.message.sbi.reconnect_interval = ogs_time_from_sec(3);
    ogs_local_conf()->time.message.sbi.client_wait_duration = ogs_time_from_sec(2);
    ogs_local_conf()->time.nf_instance.heartbeat_interval = 10;
    ogs_local_conf()->time.nf_instance.validity_duration = 60;
    ogs_sbi_context_init(OpenAPI_nf_type_SMF);
    /* Discovery associates a client and asserts a non-NULL scheme. */
    ogs_sbi_self()->tls.client.scheme = OpenAPI_uri_scheme_http;
}

static void drop_discovered(void)
{
    ogs_sbi_nf_instance_t *nf = NULL, *next = NULL;

    ogs_list_for_each_safe(&ogs_sbi_self()->nf_instance_list, next, nf) {
        if (nf == ogs_sbi_self()->nf_instance ||
                nf == ogs_sbi_self()->nrf_instance ||
                nf == ogs_sbi_self()->scp_instance)
            continue;
        if (nf->t_registration_interval)
            ogs_sbi_nf_fsm_fini(nf);
        ogs_sbi_nf_instance_remove(nf);
    }
}

static void apply_profile(OpenAPI_nf_profile_t *profile)
{
    ogs_sbi_nf_instance_t *nf;
    OpenAPI_nf_profile_t *built;

    if (!profile || !profile->nf_instance_id || !profile->nf_type ||
            !profile->nf_status)
        return;

    nf = ogs_sbi_nf_instance_add();
    if (!nf)
        return;
    ogs_sbi_nf_instance_set_id(nf, profile->nf_instance_id);
    ogs_sbi_nf_instance_set_type(nf, profile->nf_type);
    ogs_sbi_nf_fsm_init(nf);
    if (ogs_nnrf_nfm_handle_nf_profile(nf, profile)) {
        built = ogs_nnrf_nfm_build_nf_profile(
                nf, OpenAPI_service_name_NULL, NULL, false);
        if (built)
            ogs_nnrf_nfm_free_nf_profile(built);
    }
}

extern int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size)
{
    char *text;
    cJSON *json;
    OpenAPI_search_result_t *search = NULL;
    OpenAPI_nf_profile_t *profile = NULL;

    if (Size < kMinInputLength || Size > kMaxInputLength)
        return 0;

    init_once();

    text = ogs_malloc(Size + 1);
    if (!text)
        return 0;
    memcpy(text, Data, Size);
    text[Size] = '\0';
    json = cJSON_Parse(text);
    ogs_free(text);
    if (!json)
        return 0;

    search = OpenAPI_search_result_parseFromJSON(json);
    if (search) {
        if (search->validity_period && search->nf_instances)
            ogs_nnrf_disc_handle_nf_discover_search_result(search);
        OpenAPI_search_result_free(search);
    }

    profile = OpenAPI_nf_profile_parseFromJSON(json);
    if (profile) {
        apply_profile(profile);
        OpenAPI_nf_profile_free(profile);
    }

    drop_discovered();
    cJSON_Delete(json);
    return 0;
}
