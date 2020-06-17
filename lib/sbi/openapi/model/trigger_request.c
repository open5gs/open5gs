
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "trigger_request.h"

OpenAPI_trigger_request_t *OpenAPI_trigger_request_create(
    char *supi
    )
{
    OpenAPI_trigger_request_t *trigger_request_local_var = OpenAPI_malloc(sizeof(OpenAPI_trigger_request_t));
    if (!trigger_request_local_var) {
        return NULL;
    }
    trigger_request_local_var->supi = supi;

    return trigger_request_local_var;
}

void OpenAPI_trigger_request_free(OpenAPI_trigger_request_t *trigger_request)
{
    if (NULL == trigger_request) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(trigger_request->supi);
    ogs_free(trigger_request);
}

cJSON *OpenAPI_trigger_request_convertToJSON(OpenAPI_trigger_request_t *trigger_request)
{
    cJSON *item = NULL;

    if (trigger_request == NULL) {
        ogs_error("OpenAPI_trigger_request_convertToJSON() failed [TriggerRequest]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!trigger_request->supi) {
        ogs_error("OpenAPI_trigger_request_convertToJSON() failed [supi]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "supi", trigger_request->supi) == NULL) {
        ogs_error("OpenAPI_trigger_request_convertToJSON() failed [supi]");
        goto end;
    }

end:
    return item;
}

OpenAPI_trigger_request_t *OpenAPI_trigger_request_parseFromJSON(cJSON *trigger_requestJSON)
{
    OpenAPI_trigger_request_t *trigger_request_local_var = NULL;
    cJSON *supi = cJSON_GetObjectItemCaseSensitive(trigger_requestJSON, "supi");
    if (!supi) {
        ogs_error("OpenAPI_trigger_request_parseFromJSON() failed [supi]");
        goto end;
    }


    if (!cJSON_IsString(supi)) {
        ogs_error("OpenAPI_trigger_request_parseFromJSON() failed [supi]");
        goto end;
    }

    trigger_request_local_var = OpenAPI_trigger_request_create (
        ogs_strdup(supi->valuestring)
        );

    return trigger_request_local_var;
end:
    return NULL;
}

OpenAPI_trigger_request_t *OpenAPI_trigger_request_copy(OpenAPI_trigger_request_t *dst, OpenAPI_trigger_request_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_trigger_request_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_trigger_request_convertToJSON() failed");
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

    OpenAPI_trigger_request_free(dst);
    dst = OpenAPI_trigger_request_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

