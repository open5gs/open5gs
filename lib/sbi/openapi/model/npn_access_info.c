
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "npn_access_info.h"

OpenAPI_npn_access_info_t *OpenAPI_npn_access_info_create(
    OpenAPI_list_t *cell_cag_info
)
{
    OpenAPI_npn_access_info_t *npn_access_info_local_var = ogs_malloc(sizeof(OpenAPI_npn_access_info_t));
    ogs_assert(npn_access_info_local_var);

    npn_access_info_local_var->cell_cag_info = cell_cag_info;

    return npn_access_info_local_var;
}

void OpenAPI_npn_access_info_free(OpenAPI_npn_access_info_t *npn_access_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == npn_access_info) {
        return;
    }
    if (npn_access_info->cell_cag_info) {
        OpenAPI_list_for_each(npn_access_info->cell_cag_info, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(npn_access_info->cell_cag_info);
        npn_access_info->cell_cag_info = NULL;
    }
    ogs_free(npn_access_info);
}

cJSON *OpenAPI_npn_access_info_convertToJSON(OpenAPI_npn_access_info_t *npn_access_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (npn_access_info == NULL) {
        ogs_error("OpenAPI_npn_access_info_convertToJSON() failed [NpnAccessInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (npn_access_info->cell_cag_info) {
    cJSON *cell_cag_infoList = cJSON_AddArrayToObject(item, "cellCagInfo");
    if (cell_cag_infoList == NULL) {
        ogs_error("OpenAPI_npn_access_info_convertToJSON() failed [cell_cag_info]");
        goto end;
    }
    OpenAPI_list_for_each(npn_access_info->cell_cag_info, node) {
        if (cJSON_AddStringToObject(cell_cag_infoList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_npn_access_info_convertToJSON() failed [cell_cag_info]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_npn_access_info_t *OpenAPI_npn_access_info_parseFromJSON(cJSON *npn_access_infoJSON)
{
    OpenAPI_npn_access_info_t *npn_access_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *cell_cag_info = NULL;
    OpenAPI_list_t *cell_cag_infoList = NULL;
    cell_cag_info = cJSON_GetObjectItemCaseSensitive(npn_access_infoJSON, "cellCagInfo");
    if (cell_cag_info) {
        cJSON *cell_cag_info_local = NULL;
        if (!cJSON_IsArray(cell_cag_info)) {
            ogs_error("OpenAPI_npn_access_info_parseFromJSON() failed [cell_cag_info]");
            goto end;
        }

        cell_cag_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(cell_cag_info_local, cell_cag_info) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(cell_cag_info_local)) {
                ogs_error("OpenAPI_npn_access_info_parseFromJSON() failed [cell_cag_info]");
                goto end;
            }
            OpenAPI_list_add(cell_cag_infoList, ogs_strdup(cell_cag_info_local->valuestring));
        }
    }

    npn_access_info_local_var = OpenAPI_npn_access_info_create (
        cell_cag_info ? cell_cag_infoList : NULL
    );

    return npn_access_info_local_var;
end:
    if (cell_cag_infoList) {
        OpenAPI_list_for_each(cell_cag_infoList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(cell_cag_infoList);
        cell_cag_infoList = NULL;
    }
    return NULL;
}

OpenAPI_npn_access_info_t *OpenAPI_npn_access_info_copy(OpenAPI_npn_access_info_t *dst, OpenAPI_npn_access_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_npn_access_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_npn_access_info_convertToJSON() failed");
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

    OpenAPI_npn_access_info_free(dst);
    dst = OpenAPI_npn_access_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

