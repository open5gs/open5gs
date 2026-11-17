
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "throughput.h"

OpenAPI_throughput_t *OpenAPI_throughput_create(
    bool is_guaranteed_thpt,
    float guaranteed_thpt,
    bool is_maximum_thpt,
    float maximum_thpt
)
{
    OpenAPI_throughput_t *throughput_local_var = ogs_malloc(sizeof(OpenAPI_throughput_t));
    ogs_assert(throughput_local_var);

    throughput_local_var->is_guaranteed_thpt = is_guaranteed_thpt;
    throughput_local_var->guaranteed_thpt = guaranteed_thpt;
    throughput_local_var->is_maximum_thpt = is_maximum_thpt;
    throughput_local_var->maximum_thpt = maximum_thpt;

    return throughput_local_var;
}

void OpenAPI_throughput_free(OpenAPI_throughput_t *throughput)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == throughput) {
        return;
    }
    ogs_free(throughput);
}

cJSON *OpenAPI_throughput_convertToJSON(OpenAPI_throughput_t *throughput)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (throughput == NULL) {
        ogs_error("OpenAPI_throughput_convertToJSON() failed [Throughput]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (throughput->is_guaranteed_thpt) {
    if (cJSON_AddNumberToObject(item, "guaranteedThpt", throughput->guaranteed_thpt) == NULL) {
        ogs_error("OpenAPI_throughput_convertToJSON() failed [guaranteed_thpt]");
        goto end;
    }
    }

    if (throughput->is_maximum_thpt) {
    if (cJSON_AddNumberToObject(item, "maximumThpt", throughput->maximum_thpt) == NULL) {
        ogs_error("OpenAPI_throughput_convertToJSON() failed [maximum_thpt]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_throughput_t *OpenAPI_throughput_parseFromJSON(cJSON *throughputJSON)
{
    OpenAPI_throughput_t *throughput_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *guaranteed_thpt = NULL;
    cJSON *maximum_thpt = NULL;
    guaranteed_thpt = cJSON_GetObjectItemCaseSensitive(throughputJSON, "guaranteedThpt");
    if (guaranteed_thpt) {
    if (!cJSON_IsNumber(guaranteed_thpt)) {
        ogs_error("OpenAPI_throughput_parseFromJSON() failed [guaranteed_thpt]");
        goto end;
    }
    }

    maximum_thpt = cJSON_GetObjectItemCaseSensitive(throughputJSON, "maximumThpt");
    if (maximum_thpt) {
    if (!cJSON_IsNumber(maximum_thpt)) {
        ogs_error("OpenAPI_throughput_parseFromJSON() failed [maximum_thpt]");
        goto end;
    }
    }

    throughput_local_var = OpenAPI_throughput_create (
        guaranteed_thpt ? true : false,
        guaranteed_thpt ? guaranteed_thpt->valuedouble : 0,
        maximum_thpt ? true : false,
        maximum_thpt ? maximum_thpt->valuedouble : 0
    );

    return throughput_local_var;
end:
    return NULL;
}

OpenAPI_throughput_t *OpenAPI_throughput_copy(OpenAPI_throughput_t *dst, OpenAPI_throughput_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_throughput_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_throughput_convertToJSON() failed");
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

    OpenAPI_throughput_free(dst);
    dst = OpenAPI_throughput_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

