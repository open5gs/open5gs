
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pp_maximum_latency.h"

OpenAPI_pp_maximum_latency_t *OpenAPI_pp_maximum_latency_create(
    int maximum_latency,
    char *af_instance_id,
    int reference_id,
    char *validity_time
    )
{
    OpenAPI_pp_maximum_latency_t *pp_maximum_latency_local_var = OpenAPI_malloc(sizeof(OpenAPI_pp_maximum_latency_t));
    if (!pp_maximum_latency_local_var) {
        return NULL;
    }
    pp_maximum_latency_local_var->maximum_latency = maximum_latency;
    pp_maximum_latency_local_var->af_instance_id = af_instance_id;
    pp_maximum_latency_local_var->reference_id = reference_id;
    pp_maximum_latency_local_var->validity_time = validity_time;

    return pp_maximum_latency_local_var;
}

void OpenAPI_pp_maximum_latency_free(OpenAPI_pp_maximum_latency_t *pp_maximum_latency)
{
    if (NULL == pp_maximum_latency) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(pp_maximum_latency->af_instance_id);
    ogs_free(pp_maximum_latency->validity_time);
    ogs_free(pp_maximum_latency);
}

cJSON *OpenAPI_pp_maximum_latency_convertToJSON(OpenAPI_pp_maximum_latency_t *pp_maximum_latency)
{
    cJSON *item = NULL;

    if (pp_maximum_latency == NULL) {
        ogs_error("OpenAPI_pp_maximum_latency_convertToJSON() failed [PpMaximumLatency]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!pp_maximum_latency->maximum_latency) {
        ogs_error("OpenAPI_pp_maximum_latency_convertToJSON() failed [maximum_latency]");
        goto end;
    }
    if (cJSON_AddNumberToObject(item, "maximumLatency", pp_maximum_latency->maximum_latency) == NULL) {
        ogs_error("OpenAPI_pp_maximum_latency_convertToJSON() failed [maximum_latency]");
        goto end;
    }

    if (!pp_maximum_latency->af_instance_id) {
        ogs_error("OpenAPI_pp_maximum_latency_convertToJSON() failed [af_instance_id]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "afInstanceId", pp_maximum_latency->af_instance_id) == NULL) {
        ogs_error("OpenAPI_pp_maximum_latency_convertToJSON() failed [af_instance_id]");
        goto end;
    }

    if (!pp_maximum_latency->reference_id) {
        ogs_error("OpenAPI_pp_maximum_latency_convertToJSON() failed [reference_id]");
        goto end;
    }
    if (cJSON_AddNumberToObject(item, "referenceId", pp_maximum_latency->reference_id) == NULL) {
        ogs_error("OpenAPI_pp_maximum_latency_convertToJSON() failed [reference_id]");
        goto end;
    }

    if (pp_maximum_latency->validity_time) {
        if (cJSON_AddStringToObject(item, "validityTime", pp_maximum_latency->validity_time) == NULL) {
            ogs_error("OpenAPI_pp_maximum_latency_convertToJSON() failed [validity_time]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_pp_maximum_latency_t *OpenAPI_pp_maximum_latency_parseFromJSON(cJSON *pp_maximum_latencyJSON)
{
    OpenAPI_pp_maximum_latency_t *pp_maximum_latency_local_var = NULL;
    cJSON *maximum_latency = cJSON_GetObjectItemCaseSensitive(pp_maximum_latencyJSON, "maximumLatency");
    if (!maximum_latency) {
        ogs_error("OpenAPI_pp_maximum_latency_parseFromJSON() failed [maximum_latency]");
        goto end;
    }


    if (!cJSON_IsNumber(maximum_latency)) {
        ogs_error("OpenAPI_pp_maximum_latency_parseFromJSON() failed [maximum_latency]");
        goto end;
    }

    cJSON *af_instance_id = cJSON_GetObjectItemCaseSensitive(pp_maximum_latencyJSON, "afInstanceId");
    if (!af_instance_id) {
        ogs_error("OpenAPI_pp_maximum_latency_parseFromJSON() failed [af_instance_id]");
        goto end;
    }


    if (!cJSON_IsString(af_instance_id)) {
        ogs_error("OpenAPI_pp_maximum_latency_parseFromJSON() failed [af_instance_id]");
        goto end;
    }

    cJSON *reference_id = cJSON_GetObjectItemCaseSensitive(pp_maximum_latencyJSON, "referenceId");
    if (!reference_id) {
        ogs_error("OpenAPI_pp_maximum_latency_parseFromJSON() failed [reference_id]");
        goto end;
    }


    if (!cJSON_IsNumber(reference_id)) {
        ogs_error("OpenAPI_pp_maximum_latency_parseFromJSON() failed [reference_id]");
        goto end;
    }

    cJSON *validity_time = cJSON_GetObjectItemCaseSensitive(pp_maximum_latencyJSON, "validityTime");

    if (validity_time) {
        if (!cJSON_IsString(validity_time)) {
            ogs_error("OpenAPI_pp_maximum_latency_parseFromJSON() failed [validity_time]");
            goto end;
        }
    }

    pp_maximum_latency_local_var = OpenAPI_pp_maximum_latency_create (
        maximum_latency->valuedouble,
        ogs_strdup(af_instance_id->valuestring),
        reference_id->valuedouble,
        validity_time ? ogs_strdup(validity_time->valuestring) : NULL
        );

    return pp_maximum_latency_local_var;
end:
    return NULL;
}

OpenAPI_pp_maximum_latency_t *OpenAPI_pp_maximum_latency_copy(OpenAPI_pp_maximum_latency_t *dst, OpenAPI_pp_maximum_latency_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pp_maximum_latency_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pp_maximum_latency_convertToJSON() failed");
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

    OpenAPI_pp_maximum_latency_free(dst);
    dst = OpenAPI_pp_maximum_latency_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

