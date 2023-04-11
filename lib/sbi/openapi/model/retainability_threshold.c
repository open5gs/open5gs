
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "retainability_threshold.h"

OpenAPI_retainability_threshold_t *OpenAPI_retainability_threshold_create(
    bool is_rel_flow_num,
    int rel_flow_num,
    OpenAPI_time_unit_t *rel_time_unit,
    bool is_rel_flow_ratio,
    int rel_flow_ratio
)
{
    OpenAPI_retainability_threshold_t *retainability_threshold_local_var = ogs_malloc(sizeof(OpenAPI_retainability_threshold_t));
    ogs_assert(retainability_threshold_local_var);

    retainability_threshold_local_var->is_rel_flow_num = is_rel_flow_num;
    retainability_threshold_local_var->rel_flow_num = rel_flow_num;
    retainability_threshold_local_var->rel_time_unit = rel_time_unit;
    retainability_threshold_local_var->is_rel_flow_ratio = is_rel_flow_ratio;
    retainability_threshold_local_var->rel_flow_ratio = rel_flow_ratio;

    return retainability_threshold_local_var;
}

void OpenAPI_retainability_threshold_free(OpenAPI_retainability_threshold_t *retainability_threshold)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == retainability_threshold) {
        return;
    }
    if (retainability_threshold->rel_time_unit) {
        OpenAPI_time_unit_free(retainability_threshold->rel_time_unit);
        retainability_threshold->rel_time_unit = NULL;
    }
    ogs_free(retainability_threshold);
}

cJSON *OpenAPI_retainability_threshold_convertToJSON(OpenAPI_retainability_threshold_t *retainability_threshold)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (retainability_threshold == NULL) {
        ogs_error("OpenAPI_retainability_threshold_convertToJSON() failed [RetainabilityThreshold]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (retainability_threshold->is_rel_flow_num) {
    if (cJSON_AddNumberToObject(item, "relFlowNum", retainability_threshold->rel_flow_num) == NULL) {
        ogs_error("OpenAPI_retainability_threshold_convertToJSON() failed [rel_flow_num]");
        goto end;
    }
    }

    if (retainability_threshold->rel_time_unit) {
    cJSON *rel_time_unit_local_JSON = OpenAPI_time_unit_convertToJSON(retainability_threshold->rel_time_unit);
    if (rel_time_unit_local_JSON == NULL) {
        ogs_error("OpenAPI_retainability_threshold_convertToJSON() failed [rel_time_unit]");
        goto end;
    }
    cJSON_AddItemToObject(item, "relTimeUnit", rel_time_unit_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_retainability_threshold_convertToJSON() failed [rel_time_unit]");
        goto end;
    }
    }

    if (retainability_threshold->is_rel_flow_ratio) {
    if (cJSON_AddNumberToObject(item, "relFlowRatio", retainability_threshold->rel_flow_ratio) == NULL) {
        ogs_error("OpenAPI_retainability_threshold_convertToJSON() failed [rel_flow_ratio]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_retainability_threshold_t *OpenAPI_retainability_threshold_parseFromJSON(cJSON *retainability_thresholdJSON)
{
    OpenAPI_retainability_threshold_t *retainability_threshold_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *rel_flow_num = NULL;
    cJSON *rel_time_unit = NULL;
    OpenAPI_time_unit_t *rel_time_unit_local_nonprim = NULL;
    cJSON *rel_flow_ratio = NULL;
    rel_flow_num = cJSON_GetObjectItemCaseSensitive(retainability_thresholdJSON, "relFlowNum");
    if (rel_flow_num) {
    if (!cJSON_IsNumber(rel_flow_num)) {
        ogs_error("OpenAPI_retainability_threshold_parseFromJSON() failed [rel_flow_num]");
        goto end;
    }
    }

    rel_time_unit = cJSON_GetObjectItemCaseSensitive(retainability_thresholdJSON, "relTimeUnit");
    if (rel_time_unit) {
    rel_time_unit_local_nonprim = OpenAPI_time_unit_parseFromJSON(rel_time_unit);
    if (!rel_time_unit_local_nonprim) {
        ogs_error("OpenAPI_time_unit_parseFromJSON failed [rel_time_unit]");
        goto end;
    }
    }

    rel_flow_ratio = cJSON_GetObjectItemCaseSensitive(retainability_thresholdJSON, "relFlowRatio");
    if (rel_flow_ratio) {
    if (!cJSON_IsNumber(rel_flow_ratio)) {
        ogs_error("OpenAPI_retainability_threshold_parseFromJSON() failed [rel_flow_ratio]");
        goto end;
    }
    }

    retainability_threshold_local_var = OpenAPI_retainability_threshold_create (
        rel_flow_num ? true : false,
        rel_flow_num ? rel_flow_num->valuedouble : 0,
        rel_time_unit ? rel_time_unit_local_nonprim : NULL,
        rel_flow_ratio ? true : false,
        rel_flow_ratio ? rel_flow_ratio->valuedouble : 0
    );

    return retainability_threshold_local_var;
end:
    if (rel_time_unit_local_nonprim) {
        OpenAPI_time_unit_free(rel_time_unit_local_nonprim);
        rel_time_unit_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_retainability_threshold_t *OpenAPI_retainability_threshold_copy(OpenAPI_retainability_threshold_t *dst, OpenAPI_retainability_threshold_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_retainability_threshold_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_retainability_threshold_convertToJSON() failed");
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

    OpenAPI_retainability_threshold_free(dst);
    dst = OpenAPI_retainability_threshold_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

