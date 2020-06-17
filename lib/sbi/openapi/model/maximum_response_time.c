
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "maximum_response_time.h"

OpenAPI_maximum_response_time_t *OpenAPI_maximum_response_time_create(
    int maximum_response_time,
    OpenAPI_snssai_t *snssai,
    char *dnn,
    char *validity_time
    )
{
    OpenAPI_maximum_response_time_t *maximum_response_time_local_var = OpenAPI_malloc(sizeof(OpenAPI_maximum_response_time_t));
    if (!maximum_response_time_local_var) {
        return NULL;
    }
    maximum_response_time_local_var->maximum_response_time = maximum_response_time;
    maximum_response_time_local_var->snssai = snssai;
    maximum_response_time_local_var->dnn = dnn;
    maximum_response_time_local_var->validity_time = validity_time;

    return maximum_response_time_local_var;
}

void OpenAPI_maximum_response_time_free(OpenAPI_maximum_response_time_t *maximum_response_time)
{
    if (NULL == maximum_response_time) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_snssai_free(maximum_response_time->snssai);
    ogs_free(maximum_response_time->dnn);
    ogs_free(maximum_response_time->validity_time);
    ogs_free(maximum_response_time);
}

cJSON *OpenAPI_maximum_response_time_convertToJSON(OpenAPI_maximum_response_time_t *maximum_response_time)
{
    cJSON *item = NULL;

    if (maximum_response_time == NULL) {
        ogs_error("OpenAPI_maximum_response_time_convertToJSON() failed [MaximumResponseTime]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!maximum_response_time->maximum_response_time) {
        ogs_error("OpenAPI_maximum_response_time_convertToJSON() failed [maximum_response_time]");
        goto end;
    }
    if (cJSON_AddNumberToObject(item, "maximumResponseTime", maximum_response_time->maximum_response_time) == NULL) {
        ogs_error("OpenAPI_maximum_response_time_convertToJSON() failed [maximum_response_time]");
        goto end;
    }

    if (maximum_response_time->snssai) {
        cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(maximum_response_time->snssai);
        if (snssai_local_JSON == NULL) {
            ogs_error("OpenAPI_maximum_response_time_convertToJSON() failed [snssai]");
            goto end;
        }
        cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_maximum_response_time_convertToJSON() failed [snssai]");
            goto end;
        }
    }

    if (maximum_response_time->dnn) {
        if (cJSON_AddStringToObject(item, "dnn", maximum_response_time->dnn) == NULL) {
            ogs_error("OpenAPI_maximum_response_time_convertToJSON() failed [dnn]");
            goto end;
        }
    }

    if (maximum_response_time->validity_time) {
        if (cJSON_AddStringToObject(item, "validityTime", maximum_response_time->validity_time) == NULL) {
            ogs_error("OpenAPI_maximum_response_time_convertToJSON() failed [validity_time]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_maximum_response_time_t *OpenAPI_maximum_response_time_parseFromJSON(cJSON *maximum_response_timeJSON)
{
    OpenAPI_maximum_response_time_t *maximum_response_time_local_var = NULL;
    cJSON *maximum_response_time = cJSON_GetObjectItemCaseSensitive(maximum_response_timeJSON, "maximumResponseTime");
    if (!maximum_response_time) {
        ogs_error("OpenAPI_maximum_response_time_parseFromJSON() failed [maximum_response_time]");
        goto end;
    }


    if (!cJSON_IsNumber(maximum_response_time)) {
        ogs_error("OpenAPI_maximum_response_time_parseFromJSON() failed [maximum_response_time]");
        goto end;
    }

    cJSON *snssai = cJSON_GetObjectItemCaseSensitive(maximum_response_timeJSON, "snssai");

    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    if (snssai) {
        snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    }

    cJSON *dnn = cJSON_GetObjectItemCaseSensitive(maximum_response_timeJSON, "dnn");

    if (dnn) {
        if (!cJSON_IsString(dnn)) {
            ogs_error("OpenAPI_maximum_response_time_parseFromJSON() failed [dnn]");
            goto end;
        }
    }

    cJSON *validity_time = cJSON_GetObjectItemCaseSensitive(maximum_response_timeJSON, "validityTime");

    if (validity_time) {
        if (!cJSON_IsString(validity_time)) {
            ogs_error("OpenAPI_maximum_response_time_parseFromJSON() failed [validity_time]");
            goto end;
        }
    }

    maximum_response_time_local_var = OpenAPI_maximum_response_time_create (
        maximum_response_time->valuedouble,
        snssai ? snssai_local_nonprim : NULL,
        dnn ? ogs_strdup(dnn->valuestring) : NULL,
        validity_time ? ogs_strdup(validity_time->valuestring) : NULL
        );

    return maximum_response_time_local_var;
end:
    return NULL;
}

OpenAPI_maximum_response_time_t *OpenAPI_maximum_response_time_copy(OpenAPI_maximum_response_time_t *dst, OpenAPI_maximum_response_time_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_maximum_response_time_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_maximum_response_time_convertToJSON() failed");
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

    OpenAPI_maximum_response_time_free(dst);
    dst = OpenAPI_maximum_response_time_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

