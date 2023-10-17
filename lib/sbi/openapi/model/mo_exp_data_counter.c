
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mo_exp_data_counter.h"

OpenAPI_mo_exp_data_counter_t *OpenAPI_mo_exp_data_counter_create(
    int counter,
    char *time_stamp
)
{
    OpenAPI_mo_exp_data_counter_t *mo_exp_data_counter_local_var = ogs_malloc(sizeof(OpenAPI_mo_exp_data_counter_t));
    ogs_assert(mo_exp_data_counter_local_var);

    mo_exp_data_counter_local_var->counter = counter;
    mo_exp_data_counter_local_var->time_stamp = time_stamp;

    return mo_exp_data_counter_local_var;
}

void OpenAPI_mo_exp_data_counter_free(OpenAPI_mo_exp_data_counter_t *mo_exp_data_counter)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == mo_exp_data_counter) {
        return;
    }
    if (mo_exp_data_counter->time_stamp) {
        ogs_free(mo_exp_data_counter->time_stamp);
        mo_exp_data_counter->time_stamp = NULL;
    }
    ogs_free(mo_exp_data_counter);
}

cJSON *OpenAPI_mo_exp_data_counter_convertToJSON(OpenAPI_mo_exp_data_counter_t *mo_exp_data_counter)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (mo_exp_data_counter == NULL) {
        ogs_error("OpenAPI_mo_exp_data_counter_convertToJSON() failed [MoExpDataCounter]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "counter", mo_exp_data_counter->counter) == NULL) {
        ogs_error("OpenAPI_mo_exp_data_counter_convertToJSON() failed [counter]");
        goto end;
    }

    if (mo_exp_data_counter->time_stamp) {
    if (cJSON_AddStringToObject(item, "timeStamp", mo_exp_data_counter->time_stamp) == NULL) {
        ogs_error("OpenAPI_mo_exp_data_counter_convertToJSON() failed [time_stamp]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_mo_exp_data_counter_t *OpenAPI_mo_exp_data_counter_parseFromJSON(cJSON *mo_exp_data_counterJSON)
{
    OpenAPI_mo_exp_data_counter_t *mo_exp_data_counter_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *counter = NULL;
    cJSON *time_stamp = NULL;
    counter = cJSON_GetObjectItemCaseSensitive(mo_exp_data_counterJSON, "counter");
    if (!counter) {
        ogs_error("OpenAPI_mo_exp_data_counter_parseFromJSON() failed [counter]");
        goto end;
    }
    if (!cJSON_IsNumber(counter)) {
        ogs_error("OpenAPI_mo_exp_data_counter_parseFromJSON() failed [counter]");
        goto end;
    }

    time_stamp = cJSON_GetObjectItemCaseSensitive(mo_exp_data_counterJSON, "timeStamp");
    if (time_stamp) {
    if (!cJSON_IsString(time_stamp) && !cJSON_IsNull(time_stamp)) {
        ogs_error("OpenAPI_mo_exp_data_counter_parseFromJSON() failed [time_stamp]");
        goto end;
    }
    }

    mo_exp_data_counter_local_var = OpenAPI_mo_exp_data_counter_create (
        
        counter->valuedouble,
        time_stamp && !cJSON_IsNull(time_stamp) ? ogs_strdup(time_stamp->valuestring) : NULL
    );

    return mo_exp_data_counter_local_var;
end:
    return NULL;
}

OpenAPI_mo_exp_data_counter_t *OpenAPI_mo_exp_data_counter_copy(OpenAPI_mo_exp_data_counter_t *dst, OpenAPI_mo_exp_data_counter_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_mo_exp_data_counter_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_mo_exp_data_counter_convertToJSON() failed");
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

    OpenAPI_mo_exp_data_counter_free(dst);
    dst = OpenAPI_mo_exp_data_counter_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

