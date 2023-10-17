
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "temporal_validity.h"

OpenAPI_temporal_validity_t *OpenAPI_temporal_validity_create(
    char *start_time,
    char *stop_time
)
{
    OpenAPI_temporal_validity_t *temporal_validity_local_var = ogs_malloc(sizeof(OpenAPI_temporal_validity_t));
    ogs_assert(temporal_validity_local_var);

    temporal_validity_local_var->start_time = start_time;
    temporal_validity_local_var->stop_time = stop_time;

    return temporal_validity_local_var;
}

void OpenAPI_temporal_validity_free(OpenAPI_temporal_validity_t *temporal_validity)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == temporal_validity) {
        return;
    }
    if (temporal_validity->start_time) {
        ogs_free(temporal_validity->start_time);
        temporal_validity->start_time = NULL;
    }
    if (temporal_validity->stop_time) {
        ogs_free(temporal_validity->stop_time);
        temporal_validity->stop_time = NULL;
    }
    ogs_free(temporal_validity);
}

cJSON *OpenAPI_temporal_validity_convertToJSON(OpenAPI_temporal_validity_t *temporal_validity)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (temporal_validity == NULL) {
        ogs_error("OpenAPI_temporal_validity_convertToJSON() failed [TemporalValidity]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (temporal_validity->start_time) {
    if (cJSON_AddStringToObject(item, "startTime", temporal_validity->start_time) == NULL) {
        ogs_error("OpenAPI_temporal_validity_convertToJSON() failed [start_time]");
        goto end;
    }
    }

    if (temporal_validity->stop_time) {
    if (cJSON_AddStringToObject(item, "stopTime", temporal_validity->stop_time) == NULL) {
        ogs_error("OpenAPI_temporal_validity_convertToJSON() failed [stop_time]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_temporal_validity_t *OpenAPI_temporal_validity_parseFromJSON(cJSON *temporal_validityJSON)
{
    OpenAPI_temporal_validity_t *temporal_validity_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *start_time = NULL;
    cJSON *stop_time = NULL;
    start_time = cJSON_GetObjectItemCaseSensitive(temporal_validityJSON, "startTime");
    if (start_time) {
    if (!cJSON_IsString(start_time) && !cJSON_IsNull(start_time)) {
        ogs_error("OpenAPI_temporal_validity_parseFromJSON() failed [start_time]");
        goto end;
    }
    }

    stop_time = cJSON_GetObjectItemCaseSensitive(temporal_validityJSON, "stopTime");
    if (stop_time) {
    if (!cJSON_IsString(stop_time) && !cJSON_IsNull(stop_time)) {
        ogs_error("OpenAPI_temporal_validity_parseFromJSON() failed [stop_time]");
        goto end;
    }
    }

    temporal_validity_local_var = OpenAPI_temporal_validity_create (
        start_time && !cJSON_IsNull(start_time) ? ogs_strdup(start_time->valuestring) : NULL,
        stop_time && !cJSON_IsNull(stop_time) ? ogs_strdup(stop_time->valuestring) : NULL
    );

    return temporal_validity_local_var;
end:
    return NULL;
}

OpenAPI_temporal_validity_t *OpenAPI_temporal_validity_copy(OpenAPI_temporal_validity_t *dst, OpenAPI_temporal_validity_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_temporal_validity_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_temporal_validity_convertToJSON() failed");
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

    OpenAPI_temporal_validity_free(dst);
    dst = OpenAPI_temporal_validity_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

