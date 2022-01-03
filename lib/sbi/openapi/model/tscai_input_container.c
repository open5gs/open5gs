
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tscai_input_container.h"

OpenAPI_tscai_input_container_t *OpenAPI_tscai_input_container_create(
    bool is_periodicity,
    int periodicity,
    char *burst_arrival_time
)
{
    OpenAPI_tscai_input_container_t *tscai_input_container_local_var = ogs_malloc(sizeof(OpenAPI_tscai_input_container_t));
    ogs_assert(tscai_input_container_local_var);

    tscai_input_container_local_var->is_periodicity = is_periodicity;
    tscai_input_container_local_var->periodicity = periodicity;
    tscai_input_container_local_var->burst_arrival_time = burst_arrival_time;

    return tscai_input_container_local_var;
}

void OpenAPI_tscai_input_container_free(OpenAPI_tscai_input_container_t *tscai_input_container)
{
    if (NULL == tscai_input_container) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(tscai_input_container->burst_arrival_time);
    ogs_free(tscai_input_container);
}

cJSON *OpenAPI_tscai_input_container_convertToJSON(OpenAPI_tscai_input_container_t *tscai_input_container)
{
    cJSON *item = NULL;

    if (tscai_input_container == NULL) {
        ogs_error("OpenAPI_tscai_input_container_convertToJSON() failed [TscaiInputContainer]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (tscai_input_container->is_periodicity) {
    if (cJSON_AddNumberToObject(item, "periodicity", tscai_input_container->periodicity) == NULL) {
        ogs_error("OpenAPI_tscai_input_container_convertToJSON() failed [periodicity]");
        goto end;
    }
    }

    if (tscai_input_container->burst_arrival_time) {
    if (cJSON_AddStringToObject(item, "burstArrivalTime", tscai_input_container->burst_arrival_time) == NULL) {
        ogs_error("OpenAPI_tscai_input_container_convertToJSON() failed [burst_arrival_time]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_tscai_input_container_t *OpenAPI_tscai_input_container_parseFromJSON(cJSON *tscai_input_containerJSON)
{
    OpenAPI_tscai_input_container_t *tscai_input_container_local_var = NULL;
    cJSON *periodicity = cJSON_GetObjectItemCaseSensitive(tscai_input_containerJSON, "periodicity");

    if (periodicity) {
    if (!cJSON_IsNumber(periodicity)) {
        ogs_error("OpenAPI_tscai_input_container_parseFromJSON() failed [periodicity]");
        goto end;
    }
    }

    cJSON *burst_arrival_time = cJSON_GetObjectItemCaseSensitive(tscai_input_containerJSON, "burstArrivalTime");

    if (burst_arrival_time) {
    if (!cJSON_IsString(burst_arrival_time)) {
        ogs_error("OpenAPI_tscai_input_container_parseFromJSON() failed [burst_arrival_time]");
        goto end;
    }
    }

    tscai_input_container_local_var = OpenAPI_tscai_input_container_create (
        periodicity ? true : false,
        periodicity ? periodicity->valuedouble : 0,
        burst_arrival_time ? ogs_strdup(burst_arrival_time->valuestring) : NULL
    );

    return tscai_input_container_local_var;
end:
    return NULL;
}

OpenAPI_tscai_input_container_t *OpenAPI_tscai_input_container_copy(OpenAPI_tscai_input_container_t *dst, OpenAPI_tscai_input_container_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_tscai_input_container_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_tscai_input_container_convertToJSON() failed");
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

    OpenAPI_tscai_input_container_free(dst);
    dst = OpenAPI_tscai_input_container_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

