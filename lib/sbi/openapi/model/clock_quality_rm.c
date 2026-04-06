
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "clock_quality_rm.h"

OpenAPI_clock_quality_rm_t *OpenAPI_clock_quality_rm_create(
    bool is_traceability_to_gnss,
    int traceability_to_gnss,
    bool is_traceability_to_utc,
    int traceability_to_utc,
    bool is_frequency_stability_null,
    bool is_frequency_stability,
    int frequency_stability,
    bool is_clock_accuracy_index_null,
    char *clock_accuracy_index,
    bool is_clock_accuracy_value_null,
    bool is_clock_accuracy_value,
    int clock_accuracy_value
)
{
    OpenAPI_clock_quality_rm_t *clock_quality_rm_local_var = ogs_malloc(sizeof(OpenAPI_clock_quality_rm_t));
    ogs_assert(clock_quality_rm_local_var);

    clock_quality_rm_local_var->is_traceability_to_gnss = is_traceability_to_gnss;
    clock_quality_rm_local_var->traceability_to_gnss = traceability_to_gnss;
    clock_quality_rm_local_var->is_traceability_to_utc = is_traceability_to_utc;
    clock_quality_rm_local_var->traceability_to_utc = traceability_to_utc;
    clock_quality_rm_local_var->is_frequency_stability_null = is_frequency_stability_null;
    clock_quality_rm_local_var->is_frequency_stability = is_frequency_stability;
    clock_quality_rm_local_var->frequency_stability = frequency_stability;
    clock_quality_rm_local_var->is_clock_accuracy_index_null = is_clock_accuracy_index_null;
    clock_quality_rm_local_var->clock_accuracy_index = clock_accuracy_index;
    clock_quality_rm_local_var->is_clock_accuracy_value_null = is_clock_accuracy_value_null;
    clock_quality_rm_local_var->is_clock_accuracy_value = is_clock_accuracy_value;
    clock_quality_rm_local_var->clock_accuracy_value = clock_accuracy_value;

    return clock_quality_rm_local_var;
}

void OpenAPI_clock_quality_rm_free(OpenAPI_clock_quality_rm_t *clock_quality_rm)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == clock_quality_rm) {
        return;
    }
    if (clock_quality_rm->clock_accuracy_index) {
        ogs_free(clock_quality_rm->clock_accuracy_index);
        clock_quality_rm->clock_accuracy_index = NULL;
    }
    ogs_free(clock_quality_rm);
}

cJSON *OpenAPI_clock_quality_rm_convertToJSON(OpenAPI_clock_quality_rm_t *clock_quality_rm)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (clock_quality_rm == NULL) {
        ogs_error("OpenAPI_clock_quality_rm_convertToJSON() failed [ClockQualityRm]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (clock_quality_rm->is_traceability_to_gnss) {
    if (cJSON_AddBoolToObject(item, "traceabilityToGnss", clock_quality_rm->traceability_to_gnss) == NULL) {
        ogs_error("OpenAPI_clock_quality_rm_convertToJSON() failed [traceability_to_gnss]");
        goto end;
    }
    }

    if (clock_quality_rm->is_traceability_to_utc) {
    if (cJSON_AddBoolToObject(item, "traceabilityToUtc", clock_quality_rm->traceability_to_utc) == NULL) {
        ogs_error("OpenAPI_clock_quality_rm_convertToJSON() failed [traceability_to_utc]");
        goto end;
    }
    }

    if (clock_quality_rm->is_frequency_stability) {
    if (cJSON_AddNumberToObject(item, "frequencyStability", clock_quality_rm->frequency_stability) == NULL) {
        ogs_error("OpenAPI_clock_quality_rm_convertToJSON() failed [frequency_stability]");
        goto end;
    }
    } else if (clock_quality_rm->is_frequency_stability_null) {
        if (cJSON_AddNullToObject(item, "frequencyStability") == NULL) {
            ogs_error("OpenAPI_clock_quality_rm_convertToJSON() failed [frequency_stability]");
            goto end;
        }
    }

    if (clock_quality_rm->clock_accuracy_index) {
    if (cJSON_AddStringToObject(item, "clockAccuracyIndex", clock_quality_rm->clock_accuracy_index) == NULL) {
        ogs_error("OpenAPI_clock_quality_rm_convertToJSON() failed [clock_accuracy_index]");
        goto end;
    }
    } else if (clock_quality_rm->is_clock_accuracy_index_null) {
        if (cJSON_AddNullToObject(item, "clockAccuracyIndex") == NULL) {
            ogs_error("OpenAPI_clock_quality_rm_convertToJSON() failed [clock_accuracy_index]");
            goto end;
        }
    }

    if (clock_quality_rm->is_clock_accuracy_value) {
    if (cJSON_AddNumberToObject(item, "clockAccuracyValue", clock_quality_rm->clock_accuracy_value) == NULL) {
        ogs_error("OpenAPI_clock_quality_rm_convertToJSON() failed [clock_accuracy_value]");
        goto end;
    }
    } else if (clock_quality_rm->is_clock_accuracy_value_null) {
        if (cJSON_AddNullToObject(item, "clockAccuracyValue") == NULL) {
            ogs_error("OpenAPI_clock_quality_rm_convertToJSON() failed [clock_accuracy_value]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_clock_quality_rm_t *OpenAPI_clock_quality_rm_parseFromJSON(cJSON *clock_quality_rmJSON)
{
    OpenAPI_clock_quality_rm_t *clock_quality_rm_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *traceability_to_gnss = NULL;
    cJSON *traceability_to_utc = NULL;
    cJSON *frequency_stability = NULL;
    cJSON *clock_accuracy_index = NULL;
    cJSON *clock_accuracy_value = NULL;
    traceability_to_gnss = cJSON_GetObjectItemCaseSensitive(clock_quality_rmJSON, "traceabilityToGnss");
    if (traceability_to_gnss) {
    if (!cJSON_IsBool(traceability_to_gnss)) {
        ogs_error("OpenAPI_clock_quality_rm_parseFromJSON() failed [traceability_to_gnss]");
        goto end;
    }
    }

    traceability_to_utc = cJSON_GetObjectItemCaseSensitive(clock_quality_rmJSON, "traceabilityToUtc");
    if (traceability_to_utc) {
    if (!cJSON_IsBool(traceability_to_utc)) {
        ogs_error("OpenAPI_clock_quality_rm_parseFromJSON() failed [traceability_to_utc]");
        goto end;
    }
    }

    frequency_stability = cJSON_GetObjectItemCaseSensitive(clock_quality_rmJSON, "frequencyStability");
    if (frequency_stability) {
    if (!cJSON_IsNull(frequency_stability)) {
    if (!cJSON_IsNumber(frequency_stability)) {
        ogs_error("OpenAPI_clock_quality_rm_parseFromJSON() failed [frequency_stability]");
        goto end;
    }
    }
    }

    clock_accuracy_index = cJSON_GetObjectItemCaseSensitive(clock_quality_rmJSON, "clockAccuracyIndex");
    if (clock_accuracy_index) {
    if (!cJSON_IsNull(clock_accuracy_index)) {
    if (!cJSON_IsString(clock_accuracy_index) && !cJSON_IsNull(clock_accuracy_index)) {
        ogs_error("OpenAPI_clock_quality_rm_parseFromJSON() failed [clock_accuracy_index]");
        goto end;
    }
    }
    }

    clock_accuracy_value = cJSON_GetObjectItemCaseSensitive(clock_quality_rmJSON, "clockAccuracyValue");
    if (clock_accuracy_value) {
    if (!cJSON_IsNull(clock_accuracy_value)) {
    if (!cJSON_IsNumber(clock_accuracy_value)) {
        ogs_error("OpenAPI_clock_quality_rm_parseFromJSON() failed [clock_accuracy_value]");
        goto end;
    }
    }
    }

    clock_quality_rm_local_var = OpenAPI_clock_quality_rm_create (
        traceability_to_gnss ? true : false,
        traceability_to_gnss ? traceability_to_gnss->valueint : 0,
        traceability_to_utc ? true : false,
        traceability_to_utc ? traceability_to_utc->valueint : 0,
        frequency_stability && cJSON_IsNull(frequency_stability) ? true : false,
        frequency_stability ? true : false,
        frequency_stability ? frequency_stability->valuedouble : 0,
        clock_accuracy_index && cJSON_IsNull(clock_accuracy_index) ? true : false,
        clock_accuracy_index && !cJSON_IsNull(clock_accuracy_index) ? ogs_strdup(clock_accuracy_index->valuestring) : NULL,
        clock_accuracy_value && cJSON_IsNull(clock_accuracy_value) ? true : false,
        clock_accuracy_value ? true : false,
        clock_accuracy_value ? clock_accuracy_value->valuedouble : 0
    );

    return clock_quality_rm_local_var;
end:
    return NULL;
}

OpenAPI_clock_quality_rm_t *OpenAPI_clock_quality_rm_copy(OpenAPI_clock_quality_rm_t *dst, OpenAPI_clock_quality_rm_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_clock_quality_rm_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_clock_quality_rm_convertToJSON() failed");
        return NULL;
    }

    content = cJSON_Print(item);
    cJSON_Delete(item);

    if (!content) {
        ogs_error("cJSON_Print() failed");
        return NULL;
    }

    item = cJSON_Parse(content);
    ogs_free(content);
    if (!item) {
        ogs_error("cJSON_Parse() failed");
        return NULL;
    }

    OpenAPI_clock_quality_rm_free(dst);
    dst = OpenAPI_clock_quality_rm_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

