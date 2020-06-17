
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "rm_info.h"

OpenAPI_rm_info_t *OpenAPI_rm_info_create(
    OpenAPI_rm_state_t *rm_state,
    OpenAPI_access_type_e access_type
    )
{
    OpenAPI_rm_info_t *rm_info_local_var = OpenAPI_malloc(sizeof(OpenAPI_rm_info_t));
    if (!rm_info_local_var) {
        return NULL;
    }
    rm_info_local_var->rm_state = rm_state;
    rm_info_local_var->access_type = access_type;

    return rm_info_local_var;
}

void OpenAPI_rm_info_free(OpenAPI_rm_info_t *rm_info)
{
    if (NULL == rm_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_rm_state_free(rm_info->rm_state);
    ogs_free(rm_info);
}

cJSON *OpenAPI_rm_info_convertToJSON(OpenAPI_rm_info_t *rm_info)
{
    cJSON *item = NULL;

    if (rm_info == NULL) {
        ogs_error("OpenAPI_rm_info_convertToJSON() failed [RmInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!rm_info->rm_state) {
        ogs_error("OpenAPI_rm_info_convertToJSON() failed [rm_state]");
        goto end;
    }
    cJSON *rm_state_local_JSON = OpenAPI_rm_state_convertToJSON(rm_info->rm_state);
    if (rm_state_local_JSON == NULL) {
        ogs_error("OpenAPI_rm_info_convertToJSON() failed [rm_state]");
        goto end;
    }
    cJSON_AddItemToObject(item, "rmState", rm_state_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_rm_info_convertToJSON() failed [rm_state]");
        goto end;
    }

    if (!rm_info->access_type) {
        ogs_error("OpenAPI_rm_info_convertToJSON() failed [access_type]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "accessType", OpenAPI_access_type_ToString(rm_info->access_type)) == NULL) {
        ogs_error("OpenAPI_rm_info_convertToJSON() failed [access_type]");
        goto end;
    }

end:
    return item;
}

OpenAPI_rm_info_t *OpenAPI_rm_info_parseFromJSON(cJSON *rm_infoJSON)
{
    OpenAPI_rm_info_t *rm_info_local_var = NULL;
    cJSON *rm_state = cJSON_GetObjectItemCaseSensitive(rm_infoJSON, "rmState");
    if (!rm_state) {
        ogs_error("OpenAPI_rm_info_parseFromJSON() failed [rm_state]");
        goto end;
    }

    OpenAPI_rm_state_t *rm_state_local_nonprim = NULL;

    rm_state_local_nonprim = OpenAPI_rm_state_parseFromJSON(rm_state);

    cJSON *access_type = cJSON_GetObjectItemCaseSensitive(rm_infoJSON, "accessType");
    if (!access_type) {
        ogs_error("OpenAPI_rm_info_parseFromJSON() failed [access_type]");
        goto end;
    }

    OpenAPI_access_type_e access_typeVariable;

    if (!cJSON_IsString(access_type)) {
        ogs_error("OpenAPI_rm_info_parseFromJSON() failed [access_type]");
        goto end;
    }
    access_typeVariable = OpenAPI_access_type_FromString(access_type->valuestring);

    rm_info_local_var = OpenAPI_rm_info_create (
        rm_state_local_nonprim,
        access_typeVariable
        );

    return rm_info_local_var;
end:
    return NULL;
}

OpenAPI_rm_info_t *OpenAPI_rm_info_copy(OpenAPI_rm_info_t *dst, OpenAPI_rm_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_rm_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_rm_info_convertToJSON() failed");
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

    OpenAPI_rm_info_free(dst);
    dst = OpenAPI_rm_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

