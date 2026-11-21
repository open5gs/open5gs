/*
 * Copyright (C) 2019-2026 by Arthur SC Chan <arthur.chan@adalogics.com>
 *
 * This file is part of Open5GS.
 *
 * Threat model: SBI JSON bodies from a peer NF. sbi_openapi_model_fuzz calls
 * about 30 model parsers. The models below are 0-covered in the 20261114
 * report (each 400-1300 lines) because no harness calls them. Nested keys
 * are mutation-resistant, so a direct parseFromJSON dispatch is the harness
 * fix rather than a corpus for the existing target.
 */

#include <stdint.h>
#include <string.h>

#include "fuzzing.h"
#include "ogs-sbi.h"

#include "model/dnn_upf_info_item.h"
#include "model/nrf_info_served_amf_info_value.h"
#include "model/nrf_info_served_bsf_info_value.h"
#include "model/nrf_info_served_nef_info_value.h"
#include "model/nrf_info_served_nwdaf_info_value.h"
#include "model/nrf_info_served_pcf_info_value.h"
#include "model/nrf_info_served_smf_info_value.h"
#include "model/nrf_info_served_upf_info_value.h"
#include "model/nnwdaf_events_subscription.h"
#include "model/qos_monitoring_report.h"
#include "model/qos_policy_assist_set.h"
#include "model/subscription_data_sets.h"

#define kMinInputLength 2
#define kMaxInputLength 16384

#define TRY_MODEL(name)                                                     \
    do {                                                                    \
        OpenAPI_##name##_t *model =                                         \
            OpenAPI_##name##_parseFromJSON(json);                           \
        if (model)                                                          \
            OpenAPI_##name##_free(model);                                   \
    } while (0)

extern int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size)
{
    char *text;
    cJSON *json;

    if (Size < kMinInputLength || Size > kMaxInputLength)
        return 0;

    if (!initialized) {
        initialize();
        ogs_log_install_domain(&__ogs_sbi_domain, "sbi", OGS_LOG_NONE);
    }

    text = ogs_malloc(Size + 1);
    if (!text)
        return 0;
    memcpy(text, Data, Size);
    text[Size] = '\0';
    json = cJSON_Parse(text);
    ogs_free(text);
    if (!json)
        return 0;

    TRY_MODEL(subscription_data_sets);
    TRY_MODEL(nrf_info_served_upf_info_value);
    TRY_MODEL(nrf_info_served_smf_info_value);
    TRY_MODEL(nrf_info_served_amf_info_value);
    TRY_MODEL(nrf_info_served_nwdaf_info_value);
    TRY_MODEL(nrf_info_served_nef_info_value);
    TRY_MODEL(nrf_info_served_bsf_info_value);
    TRY_MODEL(nrf_info_served_pcf_info_value);
    TRY_MODEL(dnn_upf_info_item);
    TRY_MODEL(qos_policy_assist_set);
    TRY_MODEL(qos_monitoring_report);
    TRY_MODEL(nnwdaf_events_subscription);

    cJSON_Delete(json);
    return 0;
}
