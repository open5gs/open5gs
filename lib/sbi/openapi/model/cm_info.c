
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cm_info.h"

OpenAPI_cm_info_t *OpenAPI_cm_info_create(
    OpenAPI_cm_state_t *cm_state,
    OpenAPI_access_type_e access_type
)
{
    OpenAPI_cm_info_t *cm_info_local_var = ogs_malloc(sizeof(OpenAPI_cm_info_t));
    ogs_assert(cm_info_local_var);

    cm_info_local_var->cm_state = cm_state;
    cm_info_local_var->access_type = access_type;

    return cm_info_local_var;
}

void OpenAPI_cm_info_free(OpenAPI_cm_info_t *cm_info)
{
    if (NULL == cm_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_cm_state_free(cm_info->cm_state);
    ogs_free(cm_info);
}

cJSON *OpenAPI_cm_info_convertToJSON(OpenAPI_cm_info_t *cm_info)
{
    cJSON *item = NULL;

    if (cm_info == NULL) {
        ogs_error("OpenAPI_cm_info_convertToJSON() failed [CmInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    cJSON *cm_state_local_JSON = OpenAPI_cm_state_convertToJSON(cm_info->cm_state);
    if (cm_state_local_JSON == NULL) {
        ogs_error("OpenAPI_cm_info_convertToJSON() failed [cm_state]");
        goto end;
    }
    cJSON_AddItemToObject(item, "cmState", cm_state_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_cm_info_convertToJSON() failed [cm_state]");
        goto end;
    }

    if (cJSON_AddStringToObject(item, "accessType", OpenAPI_access_type_ToString(cm_info->access_type)) == NULL) {
        ogs_error("OpenAPI_cm_info_convertToJSON() failed [access_type]");
        goto end;
    }

end:
    return item;
}

OpenAPI_cm_info_t *OpenAPI_cm_info_parseFromJSON(cJSON *cm_infoJSON)
{
    OpenAPI_cm_info_t *cm_info_local_var = NULL;
    cJSON *cm_state = cJSON_GetObjectItemCaseSensitive(cm_infoJSON, "cmState");
    if (!cm_state) {
        ogs_error("OpenAPI_cm_info_parseFromJSON() failed [cm_state]");
        goto end;
    }

    OpenAPI_cm_state_t *cm_state_local_nonprim = NULL;
    cm_state_local_nonprim = OpenAPI_cm_state_parseFromJSON(cm_state);

    cJSON *access_type = cJSON_GetObjectItemCaseSensitive(cm_infoJSON, "accessType");
    if (!access_type) {
        ogs_error("OpenAPI_cm_info_parseFromJSON() failed [access_type]");
        goto end;
    }

    OpenAPI_access_type_e access_typeVariable;
    if (!cJSON_IsString(access_type)) {
        ogs_error("OpenAPI_cm_info_parseFromJSON() failed [access_type]");
        goto end;
    }
    access_typeVariable = OpenAPI_access_type_FromString(access_type->valuestring);

    cm_info_local_var = OpenAPI_cm_info_create (
        cm_state_local_nonprim,
        access_typeVariable
    );

    return cm_info_local_var;
end:
    return NULL;
}

OpenAPI_cm_info_t *OpenAPI_cm_info_copy(OpenAPI_cm_info_t *dst, OpenAPI_cm_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_cm_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_cm_info_convertToJSON() failed");
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

    OpenAPI_cm_info_free(dst);
    dst = OpenAPI_cm_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

