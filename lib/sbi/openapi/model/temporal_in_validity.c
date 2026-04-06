
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "temporal_in_validity.h"

OpenAPI_temporal_in_validity_t *OpenAPI_temporal_in_validity_create(
    char *start_time,
    char *stop_time
)
{
    OpenAPI_temporal_in_validity_t *temporal_in_validity_local_var = ogs_malloc(sizeof(OpenAPI_temporal_in_validity_t));
    ogs_assert(temporal_in_validity_local_var);

    temporal_in_validity_local_var->start_time = start_time;
    temporal_in_validity_local_var->stop_time = stop_time;

    return temporal_in_validity_local_var;
}

void OpenAPI_temporal_in_validity_free(OpenAPI_temporal_in_validity_t *temporal_in_validity)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == temporal_in_validity) {
        return;
    }
    if (temporal_in_validity->start_time) {
        ogs_free(temporal_in_validity->start_time);
        temporal_in_validity->start_time = NULL;
    }
    if (temporal_in_validity->stop_time) {
        ogs_free(temporal_in_validity->stop_time);
        temporal_in_validity->stop_time = NULL;
    }
    ogs_free(temporal_in_validity);
}

cJSON *OpenAPI_temporal_in_validity_convertToJSON(OpenAPI_temporal_in_validity_t *temporal_in_validity)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (temporal_in_validity == NULL) {
        ogs_error("OpenAPI_temporal_in_validity_convertToJSON() failed [TemporalInValidity]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!temporal_in_validity->start_time) {
        ogs_error("OpenAPI_temporal_in_validity_convertToJSON() failed [start_time]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "startTime", temporal_in_validity->start_time) == NULL) {
        ogs_error("OpenAPI_temporal_in_validity_convertToJSON() failed [start_time]");
        goto end;
    }

    if (!temporal_in_validity->stop_time) {
        ogs_error("OpenAPI_temporal_in_validity_convertToJSON() failed [stop_time]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "stopTime", temporal_in_validity->stop_time) == NULL) {
        ogs_error("OpenAPI_temporal_in_validity_convertToJSON() failed [stop_time]");
        goto end;
    }

end:
    return item;
}

OpenAPI_temporal_in_validity_t *OpenAPI_temporal_in_validity_parseFromJSON(cJSON *temporal_in_validityJSON)
{
    OpenAPI_temporal_in_validity_t *temporal_in_validity_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *start_time = NULL;
    cJSON *stop_time = NULL;
    start_time = cJSON_GetObjectItemCaseSensitive(temporal_in_validityJSON, "startTime");
    if (!start_time) {
        ogs_error("OpenAPI_temporal_in_validity_parseFromJSON() failed [start_time]");
        goto end;
    }
    if (!cJSON_IsString(start_time) && !cJSON_IsNull(start_time)) {
        ogs_error("OpenAPI_temporal_in_validity_parseFromJSON() failed [start_time]");
        goto end;
    }

    stop_time = cJSON_GetObjectItemCaseSensitive(temporal_in_validityJSON, "stopTime");
    if (!stop_time) {
        ogs_error("OpenAPI_temporal_in_validity_parseFromJSON() failed [stop_time]");
        goto end;
    }
    if (!cJSON_IsString(stop_time) && !cJSON_IsNull(stop_time)) {
        ogs_error("OpenAPI_temporal_in_validity_parseFromJSON() failed [stop_time]");
        goto end;
    }

    temporal_in_validity_local_var = OpenAPI_temporal_in_validity_create (
        ogs_strdup(start_time->valuestring),
        ogs_strdup(stop_time->valuestring)
    );

    return temporal_in_validity_local_var;
end:
    return NULL;
}

OpenAPI_temporal_in_validity_t *OpenAPI_temporal_in_validity_copy(OpenAPI_temporal_in_validity_t *dst, OpenAPI_temporal_in_validity_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_temporal_in_validity_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_temporal_in_validity_convertToJSON() failed");
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

    OpenAPI_temporal_in_validity_free(dst);
    dst = OpenAPI_temporal_in_validity_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

