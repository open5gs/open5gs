
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "threshold_value.h"

OpenAPI_threshold_value_t *OpenAPI_threshold_value_create(
    bool is_rtt_thres_null,
    bool is_rtt_thres,
    int rtt_thres,
    bool is_plr_thres_null,
    bool is_plr_thres,
    int plr_thres
)
{
    OpenAPI_threshold_value_t *threshold_value_local_var = ogs_malloc(sizeof(OpenAPI_threshold_value_t));
    ogs_assert(threshold_value_local_var);

    threshold_value_local_var->is_rtt_thres_null = is_rtt_thres_null;
    threshold_value_local_var->is_rtt_thres = is_rtt_thres;
    threshold_value_local_var->rtt_thres = rtt_thres;
    threshold_value_local_var->is_plr_thres_null = is_plr_thres_null;
    threshold_value_local_var->is_plr_thres = is_plr_thres;
    threshold_value_local_var->plr_thres = plr_thres;

    return threshold_value_local_var;
}

void OpenAPI_threshold_value_free(OpenAPI_threshold_value_t *threshold_value)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == threshold_value) {
        return;
    }
    ogs_free(threshold_value);
}

cJSON *OpenAPI_threshold_value_convertToJSON(OpenAPI_threshold_value_t *threshold_value)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (threshold_value == NULL) {
        ogs_error("OpenAPI_threshold_value_convertToJSON() failed [ThresholdValue]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (threshold_value->is_rtt_thres) {
    if (cJSON_AddNumberToObject(item, "rttThres", threshold_value->rtt_thres) == NULL) {
        ogs_error("OpenAPI_threshold_value_convertToJSON() failed [rtt_thres]");
        goto end;
    }
    } else if (threshold_value->is_rtt_thres_null) {
        if (cJSON_AddNullToObject(item, "rttThres") == NULL) {
            ogs_error("OpenAPI_threshold_value_convertToJSON() failed [rtt_thres]");
            goto end;
        }
    }

    if (threshold_value->is_plr_thres) {
    if (cJSON_AddNumberToObject(item, "plrThres", threshold_value->plr_thres) == NULL) {
        ogs_error("OpenAPI_threshold_value_convertToJSON() failed [plr_thres]");
        goto end;
    }
    } else if (threshold_value->is_plr_thres_null) {
        if (cJSON_AddNullToObject(item, "plrThres") == NULL) {
            ogs_error("OpenAPI_threshold_value_convertToJSON() failed [plr_thres]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_threshold_value_t *OpenAPI_threshold_value_parseFromJSON(cJSON *threshold_valueJSON)
{
    OpenAPI_threshold_value_t *threshold_value_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *rtt_thres = NULL;
    cJSON *plr_thres = NULL;
    rtt_thres = cJSON_GetObjectItemCaseSensitive(threshold_valueJSON, "rttThres");
    if (rtt_thres) {
    if (!cJSON_IsNull(rtt_thres)) {
    if (!cJSON_IsNumber(rtt_thres)) {
        ogs_error("OpenAPI_threshold_value_parseFromJSON() failed [rtt_thres]");
        goto end;
    }
    }
    }

    plr_thres = cJSON_GetObjectItemCaseSensitive(threshold_valueJSON, "plrThres");
    if (plr_thres) {
    if (!cJSON_IsNull(plr_thres)) {
    if (!cJSON_IsNumber(plr_thres)) {
        ogs_error("OpenAPI_threshold_value_parseFromJSON() failed [plr_thres]");
        goto end;
    }
    }
    }

    threshold_value_local_var = OpenAPI_threshold_value_create (
        rtt_thres && cJSON_IsNull(rtt_thres) ? true : false,
        rtt_thres ? true : false,
        rtt_thres ? rtt_thres->valuedouble : 0,
        plr_thres && cJSON_IsNull(plr_thres) ? true : false,
        plr_thres ? true : false,
        plr_thres ? plr_thres->valuedouble : 0
    );

    return threshold_value_local_var;
end:
    return NULL;
}

OpenAPI_threshold_value_t *OpenAPI_threshold_value_copy(OpenAPI_threshold_value_t *dst, OpenAPI_threshold_value_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_threshold_value_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_threshold_value_convertToJSON() failed");
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

    OpenAPI_threshold_value_free(dst);
    dst = OpenAPI_threshold_value_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

