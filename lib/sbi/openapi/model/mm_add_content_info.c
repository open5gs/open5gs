
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mm_add_content_info.h"

OpenAPI_mm_add_content_info_t *OpenAPI_mm_add_content_info_create(
    char *type_number,
    char *addtype_info,
    bool is_content_size,
    int content_size
)
{
    OpenAPI_mm_add_content_info_t *mm_add_content_info_local_var = ogs_malloc(sizeof(OpenAPI_mm_add_content_info_t));
    ogs_assert(mm_add_content_info_local_var);

    mm_add_content_info_local_var->type_number = type_number;
    mm_add_content_info_local_var->addtype_info = addtype_info;
    mm_add_content_info_local_var->is_content_size = is_content_size;
    mm_add_content_info_local_var->content_size = content_size;

    return mm_add_content_info_local_var;
}

void OpenAPI_mm_add_content_info_free(OpenAPI_mm_add_content_info_t *mm_add_content_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == mm_add_content_info) {
        return;
    }
    if (mm_add_content_info->type_number) {
        ogs_free(mm_add_content_info->type_number);
        mm_add_content_info->type_number = NULL;
    }
    if (mm_add_content_info->addtype_info) {
        ogs_free(mm_add_content_info->addtype_info);
        mm_add_content_info->addtype_info = NULL;
    }
    ogs_free(mm_add_content_info);
}

cJSON *OpenAPI_mm_add_content_info_convertToJSON(OpenAPI_mm_add_content_info_t *mm_add_content_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (mm_add_content_info == NULL) {
        ogs_error("OpenAPI_mm_add_content_info_convertToJSON() failed [MMAddContentInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (mm_add_content_info->type_number) {
    if (cJSON_AddStringToObject(item, "typeNumber", mm_add_content_info->type_number) == NULL) {
        ogs_error("OpenAPI_mm_add_content_info_convertToJSON() failed [type_number]");
        goto end;
    }
    }

    if (mm_add_content_info->addtype_info) {
    if (cJSON_AddStringToObject(item, "addtypeInfo", mm_add_content_info->addtype_info) == NULL) {
        ogs_error("OpenAPI_mm_add_content_info_convertToJSON() failed [addtype_info]");
        goto end;
    }
    }

    if (mm_add_content_info->is_content_size) {
    if (cJSON_AddNumberToObject(item, "contentSize", mm_add_content_info->content_size) == NULL) {
        ogs_error("OpenAPI_mm_add_content_info_convertToJSON() failed [content_size]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_mm_add_content_info_t *OpenAPI_mm_add_content_info_parseFromJSON(cJSON *mm_add_content_infoJSON)
{
    OpenAPI_mm_add_content_info_t *mm_add_content_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *type_number = NULL;
    cJSON *addtype_info = NULL;
    cJSON *content_size = NULL;
    type_number = cJSON_GetObjectItemCaseSensitive(mm_add_content_infoJSON, "typeNumber");
    if (type_number) {
    if (!cJSON_IsString(type_number) && !cJSON_IsNull(type_number)) {
        ogs_error("OpenAPI_mm_add_content_info_parseFromJSON() failed [type_number]");
        goto end;
    }
    }

    addtype_info = cJSON_GetObjectItemCaseSensitive(mm_add_content_infoJSON, "addtypeInfo");
    if (addtype_info) {
    if (!cJSON_IsString(addtype_info) && !cJSON_IsNull(addtype_info)) {
        ogs_error("OpenAPI_mm_add_content_info_parseFromJSON() failed [addtype_info]");
        goto end;
    }
    }

    content_size = cJSON_GetObjectItemCaseSensitive(mm_add_content_infoJSON, "contentSize");
    if (content_size) {
    if (!cJSON_IsNumber(content_size)) {
        ogs_error("OpenAPI_mm_add_content_info_parseFromJSON() failed [content_size]");
        goto end;
    }
    }

    mm_add_content_info_local_var = OpenAPI_mm_add_content_info_create (
        type_number && !cJSON_IsNull(type_number) ? ogs_strdup(type_number->valuestring) : NULL,
        addtype_info && !cJSON_IsNull(addtype_info) ? ogs_strdup(addtype_info->valuestring) : NULL,
        content_size ? true : false,
        content_size ? content_size->valuedouble : 0
    );

    return mm_add_content_info_local_var;
end:
    return NULL;
}

OpenAPI_mm_add_content_info_t *OpenAPI_mm_add_content_info_copy(OpenAPI_mm_add_content_info_t *dst, OpenAPI_mm_add_content_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_mm_add_content_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_mm_add_content_info_convertToJSON() failed");
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

    OpenAPI_mm_add_content_info_free(dst);
    dst = OpenAPI_mm_add_content_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

