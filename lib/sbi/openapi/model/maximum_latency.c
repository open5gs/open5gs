
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "maximum_latency.h"

OpenAPI_maximum_latency_t *OpenAPI_maximum_latency_create(
    int maximum_latency,
    OpenAPI_snssai_t *snssai,
    char *dnn,
    char *validity_time
    )
{
    OpenAPI_maximum_latency_t *maximum_latency_local_var = OpenAPI_malloc(sizeof(OpenAPI_maximum_latency_t));
    if (!maximum_latency_local_var) {
        return NULL;
    }
    maximum_latency_local_var->maximum_latency = maximum_latency;
    maximum_latency_local_var->snssai = snssai;
    maximum_latency_local_var->dnn = dnn;
    maximum_latency_local_var->validity_time = validity_time;

    return maximum_latency_local_var;
}

void OpenAPI_maximum_latency_free(OpenAPI_maximum_latency_t *maximum_latency)
{
    if (NULL == maximum_latency) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_snssai_free(maximum_latency->snssai);
    ogs_free(maximum_latency->dnn);
    ogs_free(maximum_latency->validity_time);
    ogs_free(maximum_latency);
}

cJSON *OpenAPI_maximum_latency_convertToJSON(OpenAPI_maximum_latency_t *maximum_latency)
{
    cJSON *item = NULL;

    if (maximum_latency == NULL) {
        ogs_error("OpenAPI_maximum_latency_convertToJSON() failed [MaximumLatency]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!maximum_latency->maximum_latency) {
        ogs_error("OpenAPI_maximum_latency_convertToJSON() failed [maximum_latency]");
        goto end;
    }
    if (cJSON_AddNumberToObject(item, "maximumLatency", maximum_latency->maximum_latency) == NULL) {
        ogs_error("OpenAPI_maximum_latency_convertToJSON() failed [maximum_latency]");
        goto end;
    }

    if (maximum_latency->snssai) {
        cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(maximum_latency->snssai);
        if (snssai_local_JSON == NULL) {
            ogs_error("OpenAPI_maximum_latency_convertToJSON() failed [snssai]");
            goto end;
        }
        cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_maximum_latency_convertToJSON() failed [snssai]");
            goto end;
        }
    }

    if (maximum_latency->dnn) {
        if (cJSON_AddStringToObject(item, "dnn", maximum_latency->dnn) == NULL) {
            ogs_error("OpenAPI_maximum_latency_convertToJSON() failed [dnn]");
            goto end;
        }
    }

    if (maximum_latency->validity_time) {
        if (cJSON_AddStringToObject(item, "validityTime", maximum_latency->validity_time) == NULL) {
            ogs_error("OpenAPI_maximum_latency_convertToJSON() failed [validity_time]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_maximum_latency_t *OpenAPI_maximum_latency_parseFromJSON(cJSON *maximum_latencyJSON)
{
    OpenAPI_maximum_latency_t *maximum_latency_local_var = NULL;
    cJSON *maximum_latency = cJSON_GetObjectItemCaseSensitive(maximum_latencyJSON, "maximumLatency");
    if (!maximum_latency) {
        ogs_error("OpenAPI_maximum_latency_parseFromJSON() failed [maximum_latency]");
        goto end;
    }


    if (!cJSON_IsNumber(maximum_latency)) {
        ogs_error("OpenAPI_maximum_latency_parseFromJSON() failed [maximum_latency]");
        goto end;
    }

    cJSON *snssai = cJSON_GetObjectItemCaseSensitive(maximum_latencyJSON, "snssai");

    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    if (snssai) {
        snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    }

    cJSON *dnn = cJSON_GetObjectItemCaseSensitive(maximum_latencyJSON, "dnn");

    if (dnn) {
        if (!cJSON_IsString(dnn)) {
            ogs_error("OpenAPI_maximum_latency_parseFromJSON() failed [dnn]");
            goto end;
        }
    }

    cJSON *validity_time = cJSON_GetObjectItemCaseSensitive(maximum_latencyJSON, "validityTime");

    if (validity_time) {
        if (!cJSON_IsString(validity_time)) {
            ogs_error("OpenAPI_maximum_latency_parseFromJSON() failed [validity_time]");
            goto end;
        }
    }

    maximum_latency_local_var = OpenAPI_maximum_latency_create (
        maximum_latency->valuedouble,
        snssai ? snssai_local_nonprim : NULL,
        dnn ? ogs_strdup(dnn->valuestring) : NULL,
        validity_time ? ogs_strdup(validity_time->valuestring) : NULL
        );

    return maximum_latency_local_var;
end:
    return NULL;
}

OpenAPI_maximum_latency_t *OpenAPI_maximum_latency_copy(OpenAPI_maximum_latency_t *dst, OpenAPI_maximum_latency_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_maximum_latency_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_maximum_latency_convertToJSON() failed");
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

    OpenAPI_maximum_latency_free(dst);
    dst = OpenAPI_maximum_latency_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

