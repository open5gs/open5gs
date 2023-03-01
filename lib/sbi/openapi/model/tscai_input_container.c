
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tscai_input_container.h"

OpenAPI_tscai_input_container_t *OpenAPI_tscai_input_container_create(
    bool is_periodicity,
    int periodicity,
    char *burst_arrival_time,
    bool is_sur_time_in_num_msg,
    int sur_time_in_num_msg,
    bool is_sur_time_in_time,
    int sur_time_in_time
)
{
    OpenAPI_tscai_input_container_t *tscai_input_container_local_var = ogs_malloc(sizeof(OpenAPI_tscai_input_container_t));
    ogs_assert(tscai_input_container_local_var);

    tscai_input_container_local_var->is_periodicity = is_periodicity;
    tscai_input_container_local_var->periodicity = periodicity;
    tscai_input_container_local_var->burst_arrival_time = burst_arrival_time;
    tscai_input_container_local_var->is_sur_time_in_num_msg = is_sur_time_in_num_msg;
    tscai_input_container_local_var->sur_time_in_num_msg = sur_time_in_num_msg;
    tscai_input_container_local_var->is_sur_time_in_time = is_sur_time_in_time;
    tscai_input_container_local_var->sur_time_in_time = sur_time_in_time;

    return tscai_input_container_local_var;
}

void OpenAPI_tscai_input_container_free(OpenAPI_tscai_input_container_t *tscai_input_container)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == tscai_input_container) {
        return;
    }
    if (tscai_input_container->burst_arrival_time) {
        ogs_free(tscai_input_container->burst_arrival_time);
        tscai_input_container->burst_arrival_time = NULL;
    }
    ogs_free(tscai_input_container);
}

cJSON *OpenAPI_tscai_input_container_convertToJSON(OpenAPI_tscai_input_container_t *tscai_input_container)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

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

    if (tscai_input_container->is_sur_time_in_num_msg) {
    if (cJSON_AddNumberToObject(item, "surTimeInNumMsg", tscai_input_container->sur_time_in_num_msg) == NULL) {
        ogs_error("OpenAPI_tscai_input_container_convertToJSON() failed [sur_time_in_num_msg]");
        goto end;
    }
    }

    if (tscai_input_container->is_sur_time_in_time) {
    if (cJSON_AddNumberToObject(item, "surTimeInTime", tscai_input_container->sur_time_in_time) == NULL) {
        ogs_error("OpenAPI_tscai_input_container_convertToJSON() failed [sur_time_in_time]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_tscai_input_container_t *OpenAPI_tscai_input_container_parseFromJSON(cJSON *tscai_input_containerJSON)
{
    OpenAPI_tscai_input_container_t *tscai_input_container_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *periodicity = NULL;
    cJSON *burst_arrival_time = NULL;
    cJSON *sur_time_in_num_msg = NULL;
    cJSON *sur_time_in_time = NULL;
    periodicity = cJSON_GetObjectItemCaseSensitive(tscai_input_containerJSON, "periodicity");
    if (periodicity) {
    if (!cJSON_IsNumber(periodicity)) {
        ogs_error("OpenAPI_tscai_input_container_parseFromJSON() failed [periodicity]");
        goto end;
    }
    }

    burst_arrival_time = cJSON_GetObjectItemCaseSensitive(tscai_input_containerJSON, "burstArrivalTime");
    if (burst_arrival_time) {
    if (!cJSON_IsString(burst_arrival_time) && !cJSON_IsNull(burst_arrival_time)) {
        ogs_error("OpenAPI_tscai_input_container_parseFromJSON() failed [burst_arrival_time]");
        goto end;
    }
    }

    sur_time_in_num_msg = cJSON_GetObjectItemCaseSensitive(tscai_input_containerJSON, "surTimeInNumMsg");
    if (sur_time_in_num_msg) {
    if (!cJSON_IsNumber(sur_time_in_num_msg)) {
        ogs_error("OpenAPI_tscai_input_container_parseFromJSON() failed [sur_time_in_num_msg]");
        goto end;
    }
    }

    sur_time_in_time = cJSON_GetObjectItemCaseSensitive(tscai_input_containerJSON, "surTimeInTime");
    if (sur_time_in_time) {
    if (!cJSON_IsNumber(sur_time_in_time)) {
        ogs_error("OpenAPI_tscai_input_container_parseFromJSON() failed [sur_time_in_time]");
        goto end;
    }
    }

    tscai_input_container_local_var = OpenAPI_tscai_input_container_create (
        periodicity ? true : false,
        periodicity ? periodicity->valuedouble : 0,
        burst_arrival_time && !cJSON_IsNull(burst_arrival_time) ? ogs_strdup(burst_arrival_time->valuestring) : NULL,
        sur_time_in_num_msg ? true : false,
        sur_time_in_num_msg ? sur_time_in_num_msg->valuedouble : 0,
        sur_time_in_time ? true : false,
        sur_time_in_time ? sur_time_in_time->valuedouble : 0
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

