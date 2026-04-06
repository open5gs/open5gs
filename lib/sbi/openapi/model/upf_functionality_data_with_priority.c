
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "upf_functionality_data_with_priority.h"

OpenAPI_upf_functionality_data_with_priority_t *OpenAPI_upf_functionality_data_with_priority_create(
    bool is_priority,
    int priority,
    OpenAPI_upf_functionality_data_t *upf_functionality
)
{
    OpenAPI_upf_functionality_data_with_priority_t *upf_functionality_data_with_priority_local_var = ogs_malloc(sizeof(OpenAPI_upf_functionality_data_with_priority_t));
    ogs_assert(upf_functionality_data_with_priority_local_var);

    upf_functionality_data_with_priority_local_var->is_priority = is_priority;
    upf_functionality_data_with_priority_local_var->priority = priority;
    upf_functionality_data_with_priority_local_var->upf_functionality = upf_functionality;

    return upf_functionality_data_with_priority_local_var;
}

void OpenAPI_upf_functionality_data_with_priority_free(OpenAPI_upf_functionality_data_with_priority_t *upf_functionality_data_with_priority)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == upf_functionality_data_with_priority) {
        return;
    }
    if (upf_functionality_data_with_priority->upf_functionality) {
        OpenAPI_upf_functionality_data_free(upf_functionality_data_with_priority->upf_functionality);
        upf_functionality_data_with_priority->upf_functionality = NULL;
    }
    ogs_free(upf_functionality_data_with_priority);
}

cJSON *OpenAPI_upf_functionality_data_with_priority_convertToJSON(OpenAPI_upf_functionality_data_with_priority_t *upf_functionality_data_with_priority)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (upf_functionality_data_with_priority == NULL) {
        ogs_error("OpenAPI_upf_functionality_data_with_priority_convertToJSON() failed [UpfFunctionalityDataWithPriority]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (upf_functionality_data_with_priority->is_priority) {
    if (cJSON_AddNumberToObject(item, "priority", upf_functionality_data_with_priority->priority) == NULL) {
        ogs_error("OpenAPI_upf_functionality_data_with_priority_convertToJSON() failed [priority]");
        goto end;
    }
    }

    if (!upf_functionality_data_with_priority->upf_functionality) {
        ogs_error("OpenAPI_upf_functionality_data_with_priority_convertToJSON() failed [upf_functionality]");
        return NULL;
    }
    cJSON *upf_functionality_local_JSON = OpenAPI_upf_functionality_data_convertToJSON(upf_functionality_data_with_priority->upf_functionality);
    if (upf_functionality_local_JSON == NULL) {
        ogs_error("OpenAPI_upf_functionality_data_with_priority_convertToJSON() failed [upf_functionality]");
        goto end;
    }
    cJSON_AddItemToObject(item, "upfFunctionality", upf_functionality_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_upf_functionality_data_with_priority_convertToJSON() failed [upf_functionality]");
        goto end;
    }

end:
    return item;
}

OpenAPI_upf_functionality_data_with_priority_t *OpenAPI_upf_functionality_data_with_priority_parseFromJSON(cJSON *upf_functionality_data_with_priorityJSON)
{
    OpenAPI_upf_functionality_data_with_priority_t *upf_functionality_data_with_priority_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *priority = NULL;
    cJSON *upf_functionality = NULL;
    OpenAPI_upf_functionality_data_t *upf_functionality_local_nonprim = NULL;
    priority = cJSON_GetObjectItemCaseSensitive(upf_functionality_data_with_priorityJSON, "priority");
    if (priority) {
    if (!cJSON_IsNumber(priority)) {
        ogs_error("OpenAPI_upf_functionality_data_with_priority_parseFromJSON() failed [priority]");
        goto end;
    }
    }

    upf_functionality = cJSON_GetObjectItemCaseSensitive(upf_functionality_data_with_priorityJSON, "upfFunctionality");
    if (!upf_functionality) {
        ogs_error("OpenAPI_upf_functionality_data_with_priority_parseFromJSON() failed [upf_functionality]");
        goto end;
    }
    upf_functionality_local_nonprim = OpenAPI_upf_functionality_data_parseFromJSON(upf_functionality);
    if (!upf_functionality_local_nonprim) {
        ogs_error("OpenAPI_upf_functionality_data_parseFromJSON failed [upf_functionality]");
        goto end;
    }

    upf_functionality_data_with_priority_local_var = OpenAPI_upf_functionality_data_with_priority_create (
        priority ? true : false,
        priority ? priority->valuedouble : 0,
        upf_functionality_local_nonprim
    );

    return upf_functionality_data_with_priority_local_var;
end:
    if (upf_functionality_local_nonprim) {
        OpenAPI_upf_functionality_data_free(upf_functionality_local_nonprim);
        upf_functionality_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_upf_functionality_data_with_priority_t *OpenAPI_upf_functionality_data_with_priority_copy(OpenAPI_upf_functionality_data_with_priority_t *dst, OpenAPI_upf_functionality_data_with_priority_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_upf_functionality_data_with_priority_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_upf_functionality_data_with_priority_convertToJSON() failed");
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

    OpenAPI_upf_functionality_data_with_priority_free(dst);
    dst = OpenAPI_upf_functionality_data_with_priority_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

