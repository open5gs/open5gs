
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "upf_functionality_data_with_priority_1.h"

OpenAPI_upf_functionality_data_with_priority_1_t *OpenAPI_upf_functionality_data_with_priority_1_create(
    bool is_priority,
    int priority,
    OpenAPI_upf_functionality_data_1_t *upf_functionality
)
{
    OpenAPI_upf_functionality_data_with_priority_1_t *upf_functionality_data_with_priority_1_local_var = ogs_malloc(sizeof(OpenAPI_upf_functionality_data_with_priority_1_t));
    ogs_assert(upf_functionality_data_with_priority_1_local_var);

    upf_functionality_data_with_priority_1_local_var->is_priority = is_priority;
    upf_functionality_data_with_priority_1_local_var->priority = priority;
    upf_functionality_data_with_priority_1_local_var->upf_functionality = upf_functionality;

    return upf_functionality_data_with_priority_1_local_var;
}

void OpenAPI_upf_functionality_data_with_priority_1_free(OpenAPI_upf_functionality_data_with_priority_1_t *upf_functionality_data_with_priority_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == upf_functionality_data_with_priority_1) {
        return;
    }
    if (upf_functionality_data_with_priority_1->upf_functionality) {
        OpenAPI_upf_functionality_data_1_free(upf_functionality_data_with_priority_1->upf_functionality);
        upf_functionality_data_with_priority_1->upf_functionality = NULL;
    }
    ogs_free(upf_functionality_data_with_priority_1);
}

cJSON *OpenAPI_upf_functionality_data_with_priority_1_convertToJSON(OpenAPI_upf_functionality_data_with_priority_1_t *upf_functionality_data_with_priority_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (upf_functionality_data_with_priority_1 == NULL) {
        ogs_error("OpenAPI_upf_functionality_data_with_priority_1_convertToJSON() failed [UpfFunctionalityDataWithPriority_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (upf_functionality_data_with_priority_1->is_priority) {
    if (cJSON_AddNumberToObject(item, "priority", upf_functionality_data_with_priority_1->priority) == NULL) {
        ogs_error("OpenAPI_upf_functionality_data_with_priority_1_convertToJSON() failed [priority]");
        goto end;
    }
    }

    if (!upf_functionality_data_with_priority_1->upf_functionality) {
        ogs_error("OpenAPI_upf_functionality_data_with_priority_1_convertToJSON() failed [upf_functionality]");
        return NULL;
    }
    cJSON *upf_functionality_local_JSON = OpenAPI_upf_functionality_data_1_convertToJSON(upf_functionality_data_with_priority_1->upf_functionality);
    if (upf_functionality_local_JSON == NULL) {
        ogs_error("OpenAPI_upf_functionality_data_with_priority_1_convertToJSON() failed [upf_functionality]");
        goto end;
    }
    cJSON_AddItemToObject(item, "upfFunctionality", upf_functionality_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_upf_functionality_data_with_priority_1_convertToJSON() failed [upf_functionality]");
        goto end;
    }

end:
    return item;
}

OpenAPI_upf_functionality_data_with_priority_1_t *OpenAPI_upf_functionality_data_with_priority_1_parseFromJSON(cJSON *upf_functionality_data_with_priority_1JSON)
{
    OpenAPI_upf_functionality_data_with_priority_1_t *upf_functionality_data_with_priority_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *priority = NULL;
    cJSON *upf_functionality = NULL;
    OpenAPI_upf_functionality_data_1_t *upf_functionality_local_nonprim = NULL;
    priority = cJSON_GetObjectItemCaseSensitive(upf_functionality_data_with_priority_1JSON, "priority");
    if (priority) {
    if (!cJSON_IsNumber(priority)) {
        ogs_error("OpenAPI_upf_functionality_data_with_priority_1_parseFromJSON() failed [priority]");
        goto end;
    }
    }

    upf_functionality = cJSON_GetObjectItemCaseSensitive(upf_functionality_data_with_priority_1JSON, "upfFunctionality");
    if (!upf_functionality) {
        ogs_error("OpenAPI_upf_functionality_data_with_priority_1_parseFromJSON() failed [upf_functionality]");
        goto end;
    }
    upf_functionality_local_nonprim = OpenAPI_upf_functionality_data_1_parseFromJSON(upf_functionality);
    if (!upf_functionality_local_nonprim) {
        ogs_error("OpenAPI_upf_functionality_data_1_parseFromJSON failed [upf_functionality]");
        goto end;
    }

    upf_functionality_data_with_priority_1_local_var = OpenAPI_upf_functionality_data_with_priority_1_create (
        priority ? true : false,
        priority ? priority->valuedouble : 0,
        upf_functionality_local_nonprim
    );

    return upf_functionality_data_with_priority_1_local_var;
end:
    if (upf_functionality_local_nonprim) {
        OpenAPI_upf_functionality_data_1_free(upf_functionality_local_nonprim);
        upf_functionality_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_upf_functionality_data_with_priority_1_t *OpenAPI_upf_functionality_data_with_priority_1_copy(OpenAPI_upf_functionality_data_with_priority_1_t *dst, OpenAPI_upf_functionality_data_with_priority_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_upf_functionality_data_with_priority_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_upf_functionality_data_with_priority_1_convertToJSON() failed");
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

    OpenAPI_upf_functionality_data_with_priority_1_free(dst);
    dst = OpenAPI_upf_functionality_data_with_priority_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

