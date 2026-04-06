
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "clock_quality_1.h"

OpenAPI_clock_quality_1_t *OpenAPI_clock_quality_1_create(
    bool is_traceability_to_gnss,
    int traceability_to_gnss,
    bool is_traceability_to_utc,
    int traceability_to_utc,
    bool is_frequency_stability,
    int frequency_stability,
    char *clock_accuracy_index,
    bool is_clock_accuracy_value,
    int clock_accuracy_value
)
{
    OpenAPI_clock_quality_1_t *clock_quality_1_local_var = ogs_malloc(sizeof(OpenAPI_clock_quality_1_t));
    ogs_assert(clock_quality_1_local_var);

    clock_quality_1_local_var->is_traceability_to_gnss = is_traceability_to_gnss;
    clock_quality_1_local_var->traceability_to_gnss = traceability_to_gnss;
    clock_quality_1_local_var->is_traceability_to_utc = is_traceability_to_utc;
    clock_quality_1_local_var->traceability_to_utc = traceability_to_utc;
    clock_quality_1_local_var->is_frequency_stability = is_frequency_stability;
    clock_quality_1_local_var->frequency_stability = frequency_stability;
    clock_quality_1_local_var->clock_accuracy_index = clock_accuracy_index;
    clock_quality_1_local_var->is_clock_accuracy_value = is_clock_accuracy_value;
    clock_quality_1_local_var->clock_accuracy_value = clock_accuracy_value;

    return clock_quality_1_local_var;
}

void OpenAPI_clock_quality_1_free(OpenAPI_clock_quality_1_t *clock_quality_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == clock_quality_1) {
        return;
    }
    if (clock_quality_1->clock_accuracy_index) {
        ogs_free(clock_quality_1->clock_accuracy_index);
        clock_quality_1->clock_accuracy_index = NULL;
    }
    ogs_free(clock_quality_1);
}

cJSON *OpenAPI_clock_quality_1_convertToJSON(OpenAPI_clock_quality_1_t *clock_quality_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (clock_quality_1 == NULL) {
        ogs_error("OpenAPI_clock_quality_1_convertToJSON() failed [ClockQuality_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (clock_quality_1->is_traceability_to_gnss) {
    if (cJSON_AddBoolToObject(item, "traceabilityToGnss", clock_quality_1->traceability_to_gnss) == NULL) {
        ogs_error("OpenAPI_clock_quality_1_convertToJSON() failed [traceability_to_gnss]");
        goto end;
    }
    }

    if (clock_quality_1->is_traceability_to_utc) {
    if (cJSON_AddBoolToObject(item, "traceabilityToUtc", clock_quality_1->traceability_to_utc) == NULL) {
        ogs_error("OpenAPI_clock_quality_1_convertToJSON() failed [traceability_to_utc]");
        goto end;
    }
    }

    if (clock_quality_1->is_frequency_stability) {
    if (cJSON_AddNumberToObject(item, "frequencyStability", clock_quality_1->frequency_stability) == NULL) {
        ogs_error("OpenAPI_clock_quality_1_convertToJSON() failed [frequency_stability]");
        goto end;
    }
    }

    if (clock_quality_1->clock_accuracy_index) {
    if (cJSON_AddStringToObject(item, "clockAccuracyIndex", clock_quality_1->clock_accuracy_index) == NULL) {
        ogs_error("OpenAPI_clock_quality_1_convertToJSON() failed [clock_accuracy_index]");
        goto end;
    }
    }

    if (clock_quality_1->is_clock_accuracy_value) {
    if (cJSON_AddNumberToObject(item, "clockAccuracyValue", clock_quality_1->clock_accuracy_value) == NULL) {
        ogs_error("OpenAPI_clock_quality_1_convertToJSON() failed [clock_accuracy_value]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_clock_quality_1_t *OpenAPI_clock_quality_1_parseFromJSON(cJSON *clock_quality_1JSON)
{
    OpenAPI_clock_quality_1_t *clock_quality_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *traceability_to_gnss = NULL;
    cJSON *traceability_to_utc = NULL;
    cJSON *frequency_stability = NULL;
    cJSON *clock_accuracy_index = NULL;
    cJSON *clock_accuracy_value = NULL;
    traceability_to_gnss = cJSON_GetObjectItemCaseSensitive(clock_quality_1JSON, "traceabilityToGnss");
    if (traceability_to_gnss) {
    if (!cJSON_IsBool(traceability_to_gnss)) {
        ogs_error("OpenAPI_clock_quality_1_parseFromJSON() failed [traceability_to_gnss]");
        goto end;
    }
    }

    traceability_to_utc = cJSON_GetObjectItemCaseSensitive(clock_quality_1JSON, "traceabilityToUtc");
    if (traceability_to_utc) {
    if (!cJSON_IsBool(traceability_to_utc)) {
        ogs_error("OpenAPI_clock_quality_1_parseFromJSON() failed [traceability_to_utc]");
        goto end;
    }
    }

    frequency_stability = cJSON_GetObjectItemCaseSensitive(clock_quality_1JSON, "frequencyStability");
    if (frequency_stability) {
    if (!cJSON_IsNumber(frequency_stability)) {
        ogs_error("OpenAPI_clock_quality_1_parseFromJSON() failed [frequency_stability]");
        goto end;
    }
    }

    clock_accuracy_index = cJSON_GetObjectItemCaseSensitive(clock_quality_1JSON, "clockAccuracyIndex");
    if (clock_accuracy_index) {
    if (!cJSON_IsString(clock_accuracy_index) && !cJSON_IsNull(clock_accuracy_index)) {
        ogs_error("OpenAPI_clock_quality_1_parseFromJSON() failed [clock_accuracy_index]");
        goto end;
    }
    }

    clock_accuracy_value = cJSON_GetObjectItemCaseSensitive(clock_quality_1JSON, "clockAccuracyValue");
    if (clock_accuracy_value) {
    if (!cJSON_IsNumber(clock_accuracy_value)) {
        ogs_error("OpenAPI_clock_quality_1_parseFromJSON() failed [clock_accuracy_value]");
        goto end;
    }
    }

    clock_quality_1_local_var = OpenAPI_clock_quality_1_create (
        traceability_to_gnss ? true : false,
        traceability_to_gnss ? traceability_to_gnss->valueint : 0,
        traceability_to_utc ? true : false,
        traceability_to_utc ? traceability_to_utc->valueint : 0,
        frequency_stability ? true : false,
        frequency_stability ? frequency_stability->valuedouble : 0,
        clock_accuracy_index && !cJSON_IsNull(clock_accuracy_index) ? ogs_strdup(clock_accuracy_index->valuestring) : NULL,
        clock_accuracy_value ? true : false,
        clock_accuracy_value ? clock_accuracy_value->valuedouble : 0
    );

    return clock_quality_1_local_var;
end:
    return NULL;
}

OpenAPI_clock_quality_1_t *OpenAPI_clock_quality_1_copy(OpenAPI_clock_quality_1_t *dst, OpenAPI_clock_quality_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_clock_quality_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_clock_quality_1_convertToJSON() failed");
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

    OpenAPI_clock_quality_1_free(dst);
    dst = OpenAPI_clock_quality_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

