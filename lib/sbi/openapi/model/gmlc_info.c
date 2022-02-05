
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "gmlc_info.h"

OpenAPI_gmlc_info_t *OpenAPI_gmlc_info_create(
    OpenAPI_list_t *serving_client_types
)
{
    OpenAPI_gmlc_info_t *gmlc_info_local_var = ogs_malloc(sizeof(OpenAPI_gmlc_info_t));
    ogs_assert(gmlc_info_local_var);

    gmlc_info_local_var->serving_client_types = serving_client_types;

    return gmlc_info_local_var;
}

void OpenAPI_gmlc_info_free(OpenAPI_gmlc_info_t *gmlc_info)
{
    if (NULL == gmlc_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(gmlc_info->serving_client_types, node) {
        OpenAPI_external_client_type_free(node->data);
    }
    OpenAPI_list_free(gmlc_info->serving_client_types);
    ogs_free(gmlc_info);
}

cJSON *OpenAPI_gmlc_info_convertToJSON(OpenAPI_gmlc_info_t *gmlc_info)
{
    cJSON *item = NULL;

    if (gmlc_info == NULL) {
        ogs_error("OpenAPI_gmlc_info_convertToJSON() failed [GmlcInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (gmlc_info->serving_client_types) {
    cJSON *serving_client_typesList = cJSON_AddArrayToObject(item, "servingClientTypes");
    if (serving_client_typesList == NULL) {
        ogs_error("OpenAPI_gmlc_info_convertToJSON() failed [serving_client_types]");
        goto end;
    }

    OpenAPI_lnode_t *serving_client_types_node;
    if (gmlc_info->serving_client_types) {
        OpenAPI_list_for_each(gmlc_info->serving_client_types, serving_client_types_node) {
            cJSON *itemLocal = OpenAPI_external_client_type_convertToJSON(serving_client_types_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_gmlc_info_convertToJSON() failed [serving_client_types]");
                goto end;
            }
            cJSON_AddItemToArray(serving_client_typesList, itemLocal);
        }
    }
    }

end:
    return item;
}

OpenAPI_gmlc_info_t *OpenAPI_gmlc_info_parseFromJSON(cJSON *gmlc_infoJSON)
{
    OpenAPI_gmlc_info_t *gmlc_info_local_var = NULL;
    cJSON *serving_client_types = cJSON_GetObjectItemCaseSensitive(gmlc_infoJSON, "servingClientTypes");

    OpenAPI_list_t *serving_client_typesList;
    if (serving_client_types) {
    cJSON *serving_client_types_local_nonprimitive;
    if (!cJSON_IsArray(serving_client_types)){
        ogs_error("OpenAPI_gmlc_info_parseFromJSON() failed [serving_client_types]");
        goto end;
    }

    serving_client_typesList = OpenAPI_list_create();

    cJSON_ArrayForEach(serving_client_types_local_nonprimitive, serving_client_types ) {
        if (!cJSON_IsObject(serving_client_types_local_nonprimitive)) {
            ogs_error("OpenAPI_gmlc_info_parseFromJSON() failed [serving_client_types]");
            goto end;
        }
        OpenAPI_external_client_type_t *serving_client_typesItem = OpenAPI_external_client_type_parseFromJSON(serving_client_types_local_nonprimitive);

        if (!serving_client_typesItem) {
            ogs_error("No serving_client_typesItem");
            OpenAPI_list_free(serving_client_typesList);
            goto end;
        }

        OpenAPI_list_add(serving_client_typesList, serving_client_typesItem);
    }
    }

    gmlc_info_local_var = OpenAPI_gmlc_info_create (
        serving_client_types ? serving_client_typesList : NULL
    );

    return gmlc_info_local_var;
end:
    return NULL;
}

OpenAPI_gmlc_info_t *OpenAPI_gmlc_info_copy(OpenAPI_gmlc_info_t *dst, OpenAPI_gmlc_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_gmlc_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_gmlc_info_convertToJSON() failed");
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

    OpenAPI_gmlc_info_free(dst);
    dst = OpenAPI_gmlc_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

