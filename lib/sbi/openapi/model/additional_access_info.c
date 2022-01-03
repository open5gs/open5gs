
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "additional_access_info.h"

OpenAPI_additional_access_info_t *OpenAPI_additional_access_info_create(
    OpenAPI_access_type_e access_type,
    OpenAPI_rat_type_e rat_type
)
{
    OpenAPI_additional_access_info_t *additional_access_info_local_var = ogs_malloc(sizeof(OpenAPI_additional_access_info_t));
    ogs_assert(additional_access_info_local_var);

    additional_access_info_local_var->access_type = access_type;
    additional_access_info_local_var->rat_type = rat_type;

    return additional_access_info_local_var;
}

void OpenAPI_additional_access_info_free(OpenAPI_additional_access_info_t *additional_access_info)
{
    if (NULL == additional_access_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(additional_access_info);
}

cJSON *OpenAPI_additional_access_info_convertToJSON(OpenAPI_additional_access_info_t *additional_access_info)
{
    cJSON *item = NULL;

    if (additional_access_info == NULL) {
        ogs_error("OpenAPI_additional_access_info_convertToJSON() failed [AdditionalAccessInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddStringToObject(item, "accessType", OpenAPI_access_type_ToString(additional_access_info->access_type)) == NULL) {
        ogs_error("OpenAPI_additional_access_info_convertToJSON() failed [access_type]");
        goto end;
    }

    if (additional_access_info->rat_type) {
    if (cJSON_AddStringToObject(item, "ratType", OpenAPI_rat_type_ToString(additional_access_info->rat_type)) == NULL) {
        ogs_error("OpenAPI_additional_access_info_convertToJSON() failed [rat_type]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_additional_access_info_t *OpenAPI_additional_access_info_parseFromJSON(cJSON *additional_access_infoJSON)
{
    OpenAPI_additional_access_info_t *additional_access_info_local_var = NULL;
    cJSON *access_type = cJSON_GetObjectItemCaseSensitive(additional_access_infoJSON, "accessType");
    if (!access_type) {
        ogs_error("OpenAPI_additional_access_info_parseFromJSON() failed [access_type]");
        goto end;
    }

    OpenAPI_access_type_e access_typeVariable;
    if (!cJSON_IsString(access_type)) {
        ogs_error("OpenAPI_additional_access_info_parseFromJSON() failed [access_type]");
        goto end;
    }
    access_typeVariable = OpenAPI_access_type_FromString(access_type->valuestring);

    cJSON *rat_type = cJSON_GetObjectItemCaseSensitive(additional_access_infoJSON, "ratType");

    OpenAPI_rat_type_e rat_typeVariable;
    if (rat_type) {
    if (!cJSON_IsString(rat_type)) {
        ogs_error("OpenAPI_additional_access_info_parseFromJSON() failed [rat_type]");
        goto end;
    }
    rat_typeVariable = OpenAPI_rat_type_FromString(rat_type->valuestring);
    }

    additional_access_info_local_var = OpenAPI_additional_access_info_create (
        access_typeVariable,
        rat_type ? rat_typeVariable : 0
    );

    return additional_access_info_local_var;
end:
    return NULL;
}

OpenAPI_additional_access_info_t *OpenAPI_additional_access_info_copy(OpenAPI_additional_access_info_t *dst, OpenAPI_additional_access_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_additional_access_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_additional_access_info_convertToJSON() failed");
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

    OpenAPI_additional_access_info_free(dst);
    dst = OpenAPI_additional_access_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

