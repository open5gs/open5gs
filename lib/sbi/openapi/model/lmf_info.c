
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lmf_info.h"

OpenAPI_lmf_info_t *OpenAPI_lmf_info_create(
    OpenAPI_list_t *serving_client_types,
    char *lmf_id,
    OpenAPI_list_t *serving_access_types,
    OpenAPI_list_t *serving_an_node_types,
    OpenAPI_list_t *serving_rat_types,
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *tai_range_list,
    OpenAPI_list_t *supported_gad_shapes
)
{
    OpenAPI_lmf_info_t *lmf_info_local_var = ogs_malloc(sizeof(OpenAPI_lmf_info_t));
    ogs_assert(lmf_info_local_var);

    lmf_info_local_var->serving_client_types = serving_client_types;
    lmf_info_local_var->lmf_id = lmf_id;
    lmf_info_local_var->serving_access_types = serving_access_types;
    lmf_info_local_var->serving_an_node_types = serving_an_node_types;
    lmf_info_local_var->serving_rat_types = serving_rat_types;
    lmf_info_local_var->tai_list = tai_list;
    lmf_info_local_var->tai_range_list = tai_range_list;
    lmf_info_local_var->supported_gad_shapes = supported_gad_shapes;

    return lmf_info_local_var;
}

void OpenAPI_lmf_info_free(OpenAPI_lmf_info_t *lmf_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == lmf_info) {
        return;
    }
    if (lmf_info->serving_client_types) {
        OpenAPI_list_for_each(lmf_info->serving_client_types, node) {
            OpenAPI_external_client_type_free(node->data);
        }
        OpenAPI_list_free(lmf_info->serving_client_types);
        lmf_info->serving_client_types = NULL;
    }
    if (lmf_info->lmf_id) {
        ogs_free(lmf_info->lmf_id);
        lmf_info->lmf_id = NULL;
    }
    if (lmf_info->serving_access_types) {
        OpenAPI_list_free(lmf_info->serving_access_types);
        lmf_info->serving_access_types = NULL;
    }
    if (lmf_info->serving_an_node_types) {
        OpenAPI_list_free(lmf_info->serving_an_node_types);
        lmf_info->serving_an_node_types = NULL;
    }
    if (lmf_info->serving_rat_types) {
        OpenAPI_list_free(lmf_info->serving_rat_types);
        lmf_info->serving_rat_types = NULL;
    }
    if (lmf_info->tai_list) {
        OpenAPI_list_for_each(lmf_info->tai_list, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(lmf_info->tai_list);
        lmf_info->tai_list = NULL;
    }
    if (lmf_info->tai_range_list) {
        OpenAPI_list_for_each(lmf_info->tai_range_list, node) {
            OpenAPI_tai_range_free(node->data);
        }
        OpenAPI_list_free(lmf_info->tai_range_list);
        lmf_info->tai_range_list = NULL;
    }
    if (lmf_info->supported_gad_shapes) {
        OpenAPI_list_for_each(lmf_info->supported_gad_shapes, node) {
            OpenAPI_supported_gad_shapes_free(node->data);
        }
        OpenAPI_list_free(lmf_info->supported_gad_shapes);
        lmf_info->supported_gad_shapes = NULL;
    }
    ogs_free(lmf_info);
}

cJSON *OpenAPI_lmf_info_convertToJSON(OpenAPI_lmf_info_t *lmf_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (lmf_info == NULL) {
        ogs_error("OpenAPI_lmf_info_convertToJSON() failed [LmfInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (lmf_info->serving_client_types) {
    cJSON *serving_client_typesList = cJSON_AddArrayToObject(item, "servingClientTypes");
    if (serving_client_typesList == NULL) {
        ogs_error("OpenAPI_lmf_info_convertToJSON() failed [serving_client_types]");
        goto end;
    }
    OpenAPI_list_for_each(lmf_info->serving_client_types, node) {
        cJSON *itemLocal = OpenAPI_external_client_type_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_lmf_info_convertToJSON() failed [serving_client_types]");
            goto end;
        }
        cJSON_AddItemToArray(serving_client_typesList, itemLocal);
    }
    }

    if (lmf_info->lmf_id) {
    if (cJSON_AddStringToObject(item, "lmfId", lmf_info->lmf_id) == NULL) {
        ogs_error("OpenAPI_lmf_info_convertToJSON() failed [lmf_id]");
        goto end;
    }
    }

    if (lmf_info->serving_access_types != OpenAPI_access_type_NULL) {
    cJSON *serving_access_typesList = cJSON_AddArrayToObject(item, "servingAccessTypes");
    if (serving_access_typesList == NULL) {
        ogs_error("OpenAPI_lmf_info_convertToJSON() failed [serving_access_types]");
        goto end;
    }
    OpenAPI_list_for_each(lmf_info->serving_access_types, node) {
        if (cJSON_AddStringToObject(serving_access_typesList, "", OpenAPI_access_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_lmf_info_convertToJSON() failed [serving_access_types]");
            goto end;
        }
    }
    }

    if (lmf_info->serving_an_node_types != OpenAPI_an_node_type_NULL) {
    cJSON *serving_an_node_typesList = cJSON_AddArrayToObject(item, "servingAnNodeTypes");
    if (serving_an_node_typesList == NULL) {
        ogs_error("OpenAPI_lmf_info_convertToJSON() failed [serving_an_node_types]");
        goto end;
    }
    OpenAPI_list_for_each(lmf_info->serving_an_node_types, node) {
        if (cJSON_AddStringToObject(serving_an_node_typesList, "", OpenAPI_an_node_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_lmf_info_convertToJSON() failed [serving_an_node_types]");
            goto end;
        }
    }
    }

    if (lmf_info->serving_rat_types != OpenAPI_rat_type_NULL) {
    cJSON *serving_rat_typesList = cJSON_AddArrayToObject(item, "servingRatTypes");
    if (serving_rat_typesList == NULL) {
        ogs_error("OpenAPI_lmf_info_convertToJSON() failed [serving_rat_types]");
        goto end;
    }
    OpenAPI_list_for_each(lmf_info->serving_rat_types, node) {
        if (cJSON_AddStringToObject(serving_rat_typesList, "", OpenAPI_rat_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_lmf_info_convertToJSON() failed [serving_rat_types]");
            goto end;
        }
    }
    }

    if (lmf_info->tai_list) {
    cJSON *tai_listList = cJSON_AddArrayToObject(item, "taiList");
    if (tai_listList == NULL) {
        ogs_error("OpenAPI_lmf_info_convertToJSON() failed [tai_list]");
        goto end;
    }
    OpenAPI_list_for_each(lmf_info->tai_list, node) {
        cJSON *itemLocal = OpenAPI_tai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_lmf_info_convertToJSON() failed [tai_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_listList, itemLocal);
    }
    }

    if (lmf_info->tai_range_list) {
    cJSON *tai_range_listList = cJSON_AddArrayToObject(item, "taiRangeList");
    if (tai_range_listList == NULL) {
        ogs_error("OpenAPI_lmf_info_convertToJSON() failed [tai_range_list]");
        goto end;
    }
    OpenAPI_list_for_each(lmf_info->tai_range_list, node) {
        cJSON *itemLocal = OpenAPI_tai_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_lmf_info_convertToJSON() failed [tai_range_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_range_listList, itemLocal);
    }
    }

    if (lmf_info->supported_gad_shapes) {
    cJSON *supported_gad_shapesList = cJSON_AddArrayToObject(item, "supportedGADShapes");
    if (supported_gad_shapesList == NULL) {
        ogs_error("OpenAPI_lmf_info_convertToJSON() failed [supported_gad_shapes]");
        goto end;
    }
    OpenAPI_list_for_each(lmf_info->supported_gad_shapes, node) {
        cJSON *itemLocal = OpenAPI_supported_gad_shapes_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_lmf_info_convertToJSON() failed [supported_gad_shapes]");
            goto end;
        }
        cJSON_AddItemToArray(supported_gad_shapesList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_lmf_info_t *OpenAPI_lmf_info_parseFromJSON(cJSON *lmf_infoJSON)
{
    OpenAPI_lmf_info_t *lmf_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *serving_client_types = NULL;
    OpenAPI_list_t *serving_client_typesList = NULL;
    cJSON *lmf_id = NULL;
    cJSON *serving_access_types = NULL;
    OpenAPI_list_t *serving_access_typesList = NULL;
    cJSON *serving_an_node_types = NULL;
    OpenAPI_list_t *serving_an_node_typesList = NULL;
    cJSON *serving_rat_types = NULL;
    OpenAPI_list_t *serving_rat_typesList = NULL;
    cJSON *tai_list = NULL;
    OpenAPI_list_t *tai_listList = NULL;
    cJSON *tai_range_list = NULL;
    OpenAPI_list_t *tai_range_listList = NULL;
    cJSON *supported_gad_shapes = NULL;
    OpenAPI_list_t *supported_gad_shapesList = NULL;
    serving_client_types = cJSON_GetObjectItemCaseSensitive(lmf_infoJSON, "servingClientTypes");
    if (serving_client_types) {
        cJSON *serving_client_types_local = NULL;
        if (!cJSON_IsArray(serving_client_types)) {
            ogs_error("OpenAPI_lmf_info_parseFromJSON() failed [serving_client_types]");
            goto end;
        }

        serving_client_typesList = OpenAPI_list_create();

        cJSON_ArrayForEach(serving_client_types_local, serving_client_types) {
            if (!cJSON_IsObject(serving_client_types_local)) {
                ogs_error("OpenAPI_lmf_info_parseFromJSON() failed [serving_client_types]");
                goto end;
            }
            OpenAPI_external_client_type_t *serving_client_typesItem = OpenAPI_external_client_type_parseFromJSON(serving_client_types_local);
            if (!serving_client_typesItem) {
                ogs_error("No serving_client_typesItem");
                goto end;
            }
            OpenAPI_list_add(serving_client_typesList, serving_client_typesItem);
        }
    }

    lmf_id = cJSON_GetObjectItemCaseSensitive(lmf_infoJSON, "lmfId");
    if (lmf_id) {
    if (!cJSON_IsString(lmf_id) && !cJSON_IsNull(lmf_id)) {
        ogs_error("OpenAPI_lmf_info_parseFromJSON() failed [lmf_id]");
        goto end;
    }
    }

    serving_access_types = cJSON_GetObjectItemCaseSensitive(lmf_infoJSON, "servingAccessTypes");
    if (serving_access_types) {
        cJSON *serving_access_types_local = NULL;
        if (!cJSON_IsArray(serving_access_types)) {
            ogs_error("OpenAPI_lmf_info_parseFromJSON() failed [serving_access_types]");
            goto end;
        }

        serving_access_typesList = OpenAPI_list_create();

        cJSON_ArrayForEach(serving_access_types_local, serving_access_types) {
            OpenAPI_access_type_e localEnum = OpenAPI_access_type_NULL;
            if (!cJSON_IsString(serving_access_types_local)) {
                ogs_error("OpenAPI_lmf_info_parseFromJSON() failed [serving_access_types]");
                goto end;
            }
            localEnum = OpenAPI_access_type_FromString(serving_access_types_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"serving_access_types\" is not supported. Ignoring it ...",
                         serving_access_types_local->valuestring);
            } else {
                OpenAPI_list_add(serving_access_typesList, (void *)localEnum);
            }
        }
        if (serving_access_typesList->count == 0) {
            ogs_error("OpenAPI_lmf_info_parseFromJSON() failed: Expected serving_access_typesList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    serving_an_node_types = cJSON_GetObjectItemCaseSensitive(lmf_infoJSON, "servingAnNodeTypes");
    if (serving_an_node_types) {
        cJSON *serving_an_node_types_local = NULL;
        if (!cJSON_IsArray(serving_an_node_types)) {
            ogs_error("OpenAPI_lmf_info_parseFromJSON() failed [serving_an_node_types]");
            goto end;
        }

        serving_an_node_typesList = OpenAPI_list_create();

        cJSON_ArrayForEach(serving_an_node_types_local, serving_an_node_types) {
            OpenAPI_an_node_type_e localEnum = OpenAPI_an_node_type_NULL;
            if (!cJSON_IsString(serving_an_node_types_local)) {
                ogs_error("OpenAPI_lmf_info_parseFromJSON() failed [serving_an_node_types]");
                goto end;
            }
            localEnum = OpenAPI_an_node_type_FromString(serving_an_node_types_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"serving_an_node_types\" is not supported. Ignoring it ...",
                         serving_an_node_types_local->valuestring);
            } else {
                OpenAPI_list_add(serving_an_node_typesList, (void *)localEnum);
            }
        }
        if (serving_an_node_typesList->count == 0) {
            ogs_error("OpenAPI_lmf_info_parseFromJSON() failed: Expected serving_an_node_typesList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    serving_rat_types = cJSON_GetObjectItemCaseSensitive(lmf_infoJSON, "servingRatTypes");
    if (serving_rat_types) {
        cJSON *serving_rat_types_local = NULL;
        if (!cJSON_IsArray(serving_rat_types)) {
            ogs_error("OpenAPI_lmf_info_parseFromJSON() failed [serving_rat_types]");
            goto end;
        }

        serving_rat_typesList = OpenAPI_list_create();

        cJSON_ArrayForEach(serving_rat_types_local, serving_rat_types) {
            OpenAPI_rat_type_e localEnum = OpenAPI_rat_type_NULL;
            if (!cJSON_IsString(serving_rat_types_local)) {
                ogs_error("OpenAPI_lmf_info_parseFromJSON() failed [serving_rat_types]");
                goto end;
            }
            localEnum = OpenAPI_rat_type_FromString(serving_rat_types_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"serving_rat_types\" is not supported. Ignoring it ...",
                         serving_rat_types_local->valuestring);
            } else {
                OpenAPI_list_add(serving_rat_typesList, (void *)localEnum);
            }
        }
        if (serving_rat_typesList->count == 0) {
            ogs_error("OpenAPI_lmf_info_parseFromJSON() failed: Expected serving_rat_typesList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    tai_list = cJSON_GetObjectItemCaseSensitive(lmf_infoJSON, "taiList");
    if (tai_list) {
        cJSON *tai_list_local = NULL;
        if (!cJSON_IsArray(tai_list)) {
            ogs_error("OpenAPI_lmf_info_parseFromJSON() failed [tai_list]");
            goto end;
        }

        tai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_list_local, tai_list) {
            if (!cJSON_IsObject(tai_list_local)) {
                ogs_error("OpenAPI_lmf_info_parseFromJSON() failed [tai_list]");
                goto end;
            }
            OpenAPI_tai_t *tai_listItem = OpenAPI_tai_parseFromJSON(tai_list_local);
            if (!tai_listItem) {
                ogs_error("No tai_listItem");
                goto end;
            }
            OpenAPI_list_add(tai_listList, tai_listItem);
        }
    }

    tai_range_list = cJSON_GetObjectItemCaseSensitive(lmf_infoJSON, "taiRangeList");
    if (tai_range_list) {
        cJSON *tai_range_list_local = NULL;
        if (!cJSON_IsArray(tai_range_list)) {
            ogs_error("OpenAPI_lmf_info_parseFromJSON() failed [tai_range_list]");
            goto end;
        }

        tai_range_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_range_list_local, tai_range_list) {
            if (!cJSON_IsObject(tai_range_list_local)) {
                ogs_error("OpenAPI_lmf_info_parseFromJSON() failed [tai_range_list]");
                goto end;
            }
            OpenAPI_tai_range_t *tai_range_listItem = OpenAPI_tai_range_parseFromJSON(tai_range_list_local);
            if (!tai_range_listItem) {
                ogs_error("No tai_range_listItem");
                goto end;
            }
            OpenAPI_list_add(tai_range_listList, tai_range_listItem);
        }
    }

    supported_gad_shapes = cJSON_GetObjectItemCaseSensitive(lmf_infoJSON, "supportedGADShapes");
    if (supported_gad_shapes) {
        cJSON *supported_gad_shapes_local = NULL;
        if (!cJSON_IsArray(supported_gad_shapes)) {
            ogs_error("OpenAPI_lmf_info_parseFromJSON() failed [supported_gad_shapes]");
            goto end;
        }

        supported_gad_shapesList = OpenAPI_list_create();

        cJSON_ArrayForEach(supported_gad_shapes_local, supported_gad_shapes) {
            if (!cJSON_IsObject(supported_gad_shapes_local)) {
                ogs_error("OpenAPI_lmf_info_parseFromJSON() failed [supported_gad_shapes]");
                goto end;
            }
            OpenAPI_supported_gad_shapes_t *supported_gad_shapesItem = OpenAPI_supported_gad_shapes_parseFromJSON(supported_gad_shapes_local);
            if (!supported_gad_shapesItem) {
                ogs_error("No supported_gad_shapesItem");
                goto end;
            }
            OpenAPI_list_add(supported_gad_shapesList, supported_gad_shapesItem);
        }
    }

    lmf_info_local_var = OpenAPI_lmf_info_create (
        serving_client_types ? serving_client_typesList : NULL,
        lmf_id && !cJSON_IsNull(lmf_id) ? ogs_strdup(lmf_id->valuestring) : NULL,
        serving_access_types ? serving_access_typesList : NULL,
        serving_an_node_types ? serving_an_node_typesList : NULL,
        serving_rat_types ? serving_rat_typesList : NULL,
        tai_list ? tai_listList : NULL,
        tai_range_list ? tai_range_listList : NULL,
        supported_gad_shapes ? supported_gad_shapesList : NULL
    );

    return lmf_info_local_var;
end:
    if (serving_client_typesList) {
        OpenAPI_list_for_each(serving_client_typesList, node) {
            OpenAPI_external_client_type_free(node->data);
        }
        OpenAPI_list_free(serving_client_typesList);
        serving_client_typesList = NULL;
    }
    if (serving_access_typesList) {
        OpenAPI_list_free(serving_access_typesList);
        serving_access_typesList = NULL;
    }
    if (serving_an_node_typesList) {
        OpenAPI_list_free(serving_an_node_typesList);
        serving_an_node_typesList = NULL;
    }
    if (serving_rat_typesList) {
        OpenAPI_list_free(serving_rat_typesList);
        serving_rat_typesList = NULL;
    }
    if (tai_listList) {
        OpenAPI_list_for_each(tai_listList, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(tai_listList);
        tai_listList = NULL;
    }
    if (tai_range_listList) {
        OpenAPI_list_for_each(tai_range_listList, node) {
            OpenAPI_tai_range_free(node->data);
        }
        OpenAPI_list_free(tai_range_listList);
        tai_range_listList = NULL;
    }
    if (supported_gad_shapesList) {
        OpenAPI_list_for_each(supported_gad_shapesList, node) {
            OpenAPI_supported_gad_shapes_free(node->data);
        }
        OpenAPI_list_free(supported_gad_shapesList);
        supported_gad_shapesList = NULL;
    }
    return NULL;
}

OpenAPI_lmf_info_t *OpenAPI_lmf_info_copy(OpenAPI_lmf_info_t *dst, OpenAPI_lmf_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_lmf_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_lmf_info_convertToJSON() failed");
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

    OpenAPI_lmf_info_free(dst);
    dst = OpenAPI_lmf_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

