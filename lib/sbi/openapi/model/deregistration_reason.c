
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "deregistration_reason.h"

OpenAPI_deregistration_reason_t *OpenAPI_deregistration_reason_create(
    )
{
    OpenAPI_deregistration_reason_t *deregistration_reason_local_var = OpenAPI_malloc(sizeof(OpenAPI_deregistration_reason_t));
    if (!deregistration_reason_local_var) {
        return NULL;
    }

    return deregistration_reason_local_var;
}

void OpenAPI_deregistration_reason_free(OpenAPI_deregistration_reason_t *deregistration_reason)
{
    if (NULL == deregistration_reason) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(deregistration_reason);
}

cJSON *OpenAPI_deregistration_reason_convertToJSON(OpenAPI_deregistration_reason_t *deregistration_reason)
{
    cJSON *item = NULL;

    if (deregistration_reason == NULL) {
        ogs_error("OpenAPI_deregistration_reason_convertToJSON() failed [DeregistrationReason]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_deregistration_reason_t *OpenAPI_deregistration_reason_parseFromJSON(cJSON *deregistration_reasonJSON)
{
    OpenAPI_deregistration_reason_t *deregistration_reason_local_var = NULL;
    deregistration_reason_local_var = OpenAPI_deregistration_reason_create (
        );

    return deregistration_reason_local_var;
end:
    return NULL;
}

OpenAPI_deregistration_reason_t *OpenAPI_deregistration_reason_copy(OpenAPI_deregistration_reason_t *dst, OpenAPI_deregistration_reason_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_deregistration_reason_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_deregistration_reason_convertToJSON() failed");
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

    OpenAPI_deregistration_reason_free(dst);
    dst = OpenAPI_deregistration_reason_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

