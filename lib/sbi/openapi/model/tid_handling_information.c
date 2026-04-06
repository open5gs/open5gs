
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tid_handling_information.h"

OpenAPI_tid_handling_information_t *OpenAPI_tid_handling_information_create(
    OpenAPI_tid_type_e tid_type,
    bool is_tid_handling_information,
    int tid_handling_information
)
{
    OpenAPI_tid_handling_information_t *tid_handling_information_local_var = ogs_malloc(sizeof(OpenAPI_tid_handling_information_t));
    ogs_assert(tid_handling_information_local_var);

    tid_handling_information_local_var->tid_type = tid_type;
    tid_handling_information_local_var->is_tid_handling_information = is_tid_handling_information;
    tid_handling_information_local_var->tid_handling_information = tid_handling_information;

    return tid_handling_information_local_var;
}

void OpenAPI_tid_handling_information_free(OpenAPI_tid_handling_information_t *tid_handling_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == tid_handling_information) {
        return;
    }
    ogs_free(tid_handling_information);
}

cJSON *OpenAPI_tid_handling_information_convertToJSON(OpenAPI_tid_handling_information_t *tid_handling_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (tid_handling_information == NULL) {
        ogs_error("OpenAPI_tid_handling_information_convertToJSON() failed [TidHandlingInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (tid_handling_information->tid_type == OpenAPI_tid_type_NULL) {
        ogs_error("OpenAPI_tid_handling_information_convertToJSON() failed [tid_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "tidType", OpenAPI_tid_type_ToString(tid_handling_information->tid_type)) == NULL) {
        ogs_error("OpenAPI_tid_handling_information_convertToJSON() failed [tid_type]");
        goto end;
    }

    if (tid_handling_information->is_tid_handling_information) {
    if (cJSON_AddBoolToObject(item, "tidHandlingInformation", tid_handling_information->tid_handling_information) == NULL) {
        ogs_error("OpenAPI_tid_handling_information_convertToJSON() failed [tid_handling_information]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_tid_handling_information_t *OpenAPI_tid_handling_information_parseFromJSON(cJSON *tid_handling_informationJSON)
{
    OpenAPI_tid_handling_information_t *tid_handling_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *tid_type = NULL;
    OpenAPI_tid_type_e tid_typeVariable = 0;
    cJSON *tid_handling_information = NULL;
    tid_type = cJSON_GetObjectItemCaseSensitive(tid_handling_informationJSON, "tidType");
    if (!tid_type) {
        ogs_error("OpenAPI_tid_handling_information_parseFromJSON() failed [tid_type]");
        goto end;
    }
    if (!cJSON_IsString(tid_type)) {
        ogs_error("OpenAPI_tid_handling_information_parseFromJSON() failed [tid_type]");
        goto end;
    }
    tid_typeVariable = OpenAPI_tid_type_FromString(tid_type->valuestring);

    tid_handling_information = cJSON_GetObjectItemCaseSensitive(tid_handling_informationJSON, "tidHandlingInformation");
    if (tid_handling_information) {
    if (!cJSON_IsBool(tid_handling_information)) {
        ogs_error("OpenAPI_tid_handling_information_parseFromJSON() failed [tid_handling_information]");
        goto end;
    }
    }

    tid_handling_information_local_var = OpenAPI_tid_handling_information_create (
        tid_typeVariable,
        tid_handling_information ? true : false,
        tid_handling_information ? tid_handling_information->valueint : 0
    );

    return tid_handling_information_local_var;
end:
    return NULL;
}

OpenAPI_tid_handling_information_t *OpenAPI_tid_handling_information_copy(OpenAPI_tid_handling_information_t *dst, OpenAPI_tid_handling_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_tid_handling_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_tid_handling_information_convertToJSON() failed");
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

    OpenAPI_tid_handling_information_free(dst);
    dst = OpenAPI_tid_handling_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

