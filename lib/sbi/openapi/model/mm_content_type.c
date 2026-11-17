
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mm_content_type.h"

OpenAPI_mm_content_type_t *OpenAPI_mm_content_type_create(
    char *type_number,
    char *addtype_info,
    bool is_content_size,
    int content_size,
    OpenAPI_list_t *mm_add_content_info
)
{
    OpenAPI_mm_content_type_t *mm_content_type_local_var = ogs_malloc(sizeof(OpenAPI_mm_content_type_t));
    ogs_assert(mm_content_type_local_var);

    mm_content_type_local_var->type_number = type_number;
    mm_content_type_local_var->addtype_info = addtype_info;
    mm_content_type_local_var->is_content_size = is_content_size;
    mm_content_type_local_var->content_size = content_size;
    mm_content_type_local_var->mm_add_content_info = mm_add_content_info;

    return mm_content_type_local_var;
}

void OpenAPI_mm_content_type_free(OpenAPI_mm_content_type_t *mm_content_type)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == mm_content_type) {
        return;
    }
    if (mm_content_type->type_number) {
        ogs_free(mm_content_type->type_number);
        mm_content_type->type_number = NULL;
    }
    if (mm_content_type->addtype_info) {
        ogs_free(mm_content_type->addtype_info);
        mm_content_type->addtype_info = NULL;
    }
    if (mm_content_type->mm_add_content_info) {
        OpenAPI_list_for_each(mm_content_type->mm_add_content_info, node) {
            OpenAPI_mm_add_content_info_free(node->data);
        }
        OpenAPI_list_free(mm_content_type->mm_add_content_info);
        mm_content_type->mm_add_content_info = NULL;
    }
    ogs_free(mm_content_type);
}

cJSON *OpenAPI_mm_content_type_convertToJSON(OpenAPI_mm_content_type_t *mm_content_type)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (mm_content_type == NULL) {
        ogs_error("OpenAPI_mm_content_type_convertToJSON() failed [MMContentType]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (mm_content_type->type_number) {
    if (cJSON_AddStringToObject(item, "typeNumber", mm_content_type->type_number) == NULL) {
        ogs_error("OpenAPI_mm_content_type_convertToJSON() failed [type_number]");
        goto end;
    }
    }

    if (mm_content_type->addtype_info) {
    if (cJSON_AddStringToObject(item, "addtypeInfo", mm_content_type->addtype_info) == NULL) {
        ogs_error("OpenAPI_mm_content_type_convertToJSON() failed [addtype_info]");
        goto end;
    }
    }

    if (mm_content_type->is_content_size) {
    if (cJSON_AddNumberToObject(item, "contentSize", mm_content_type->content_size) == NULL) {
        ogs_error("OpenAPI_mm_content_type_convertToJSON() failed [content_size]");
        goto end;
    }
    }

    if (mm_content_type->mm_add_content_info) {
    cJSON *mm_add_content_infoList = cJSON_AddArrayToObject(item, "mmAddContentInfo");
    if (mm_add_content_infoList == NULL) {
        ogs_error("OpenAPI_mm_content_type_convertToJSON() failed [mm_add_content_info]");
        goto end;
    }
    OpenAPI_list_for_each(mm_content_type->mm_add_content_info, node) {
        cJSON *itemLocal = OpenAPI_mm_add_content_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_mm_content_type_convertToJSON() failed [mm_add_content_info]");
            goto end;
        }
        cJSON_AddItemToArray(mm_add_content_infoList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_mm_content_type_t *OpenAPI_mm_content_type_parseFromJSON(cJSON *mm_content_typeJSON)
{
    OpenAPI_mm_content_type_t *mm_content_type_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *type_number = NULL;
    cJSON *addtype_info = NULL;
    cJSON *content_size = NULL;
    cJSON *mm_add_content_info = NULL;
    OpenAPI_list_t *mm_add_content_infoList = NULL;
    type_number = cJSON_GetObjectItemCaseSensitive(mm_content_typeJSON, "typeNumber");
    if (type_number) {
    if (!cJSON_IsString(type_number) && !cJSON_IsNull(type_number)) {
        ogs_error("OpenAPI_mm_content_type_parseFromJSON() failed [type_number]");
        goto end;
    }
    }

    addtype_info = cJSON_GetObjectItemCaseSensitive(mm_content_typeJSON, "addtypeInfo");
    if (addtype_info) {
    if (!cJSON_IsString(addtype_info) && !cJSON_IsNull(addtype_info)) {
        ogs_error("OpenAPI_mm_content_type_parseFromJSON() failed [addtype_info]");
        goto end;
    }
    }

    content_size = cJSON_GetObjectItemCaseSensitive(mm_content_typeJSON, "contentSize");
    if (content_size) {
    if (!cJSON_IsNumber(content_size)) {
        ogs_error("OpenAPI_mm_content_type_parseFromJSON() failed [content_size]");
        goto end;
    }
    }

    mm_add_content_info = cJSON_GetObjectItemCaseSensitive(mm_content_typeJSON, "mmAddContentInfo");
    if (mm_add_content_info) {
        cJSON *mm_add_content_info_local = NULL;
        if (!cJSON_IsArray(mm_add_content_info)) {
            ogs_error("OpenAPI_mm_content_type_parseFromJSON() failed [mm_add_content_info]");
            goto end;
        }

        mm_add_content_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(mm_add_content_info_local, mm_add_content_info) {
            if (!cJSON_IsObject(mm_add_content_info_local)) {
                ogs_error("OpenAPI_mm_content_type_parseFromJSON() failed [mm_add_content_info]");
                goto end;
            }
            OpenAPI_mm_add_content_info_t *mm_add_content_infoItem = OpenAPI_mm_add_content_info_parseFromJSON(mm_add_content_info_local);
            if (!mm_add_content_infoItem) {
                ogs_error("No mm_add_content_infoItem");
                goto end;
            }
            OpenAPI_list_add(mm_add_content_infoList, mm_add_content_infoItem);
        }
    }

    mm_content_type_local_var = OpenAPI_mm_content_type_create (
        type_number && !cJSON_IsNull(type_number) ? ogs_strdup(type_number->valuestring) : NULL,
        addtype_info && !cJSON_IsNull(addtype_info) ? ogs_strdup(addtype_info->valuestring) : NULL,
        content_size ? true : false,
        content_size ? content_size->valuedouble : 0,
        mm_add_content_info ? mm_add_content_infoList : NULL
    );

    return mm_content_type_local_var;
end:
    if (mm_add_content_infoList) {
        OpenAPI_list_for_each(mm_add_content_infoList, node) {
            OpenAPI_mm_add_content_info_free(node->data);
        }
        OpenAPI_list_free(mm_add_content_infoList);
        mm_add_content_infoList = NULL;
    }
    return NULL;
}

OpenAPI_mm_content_type_t *OpenAPI_mm_content_type_copy(OpenAPI_mm_content_type_t *dst, OpenAPI_mm_content_type_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_mm_content_type_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_mm_content_type_convertToJSON() failed");
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

    OpenAPI_mm_content_type_free(dst);
    dst = OpenAPI_mm_content_type_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

