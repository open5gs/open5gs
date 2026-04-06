
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cag_info_2.h"

OpenAPI_cag_info_2_t *OpenAPI_cag_info_2_create(
    OpenAPI_list_t *cag_list
)
{
    OpenAPI_cag_info_2_t *cag_info_2_local_var = ogs_malloc(sizeof(OpenAPI_cag_info_2_t));
    ogs_assert(cag_info_2_local_var);

    cag_info_2_local_var->cag_list = cag_list;

    return cag_info_2_local_var;
}

void OpenAPI_cag_info_2_free(OpenAPI_cag_info_2_t *cag_info_2)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == cag_info_2) {
        return;
    }
    if (cag_info_2->cag_list) {
        OpenAPI_list_for_each(cag_info_2->cag_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(cag_info_2->cag_list);
        cag_info_2->cag_list = NULL;
    }
    ogs_free(cag_info_2);
}

cJSON *OpenAPI_cag_info_2_convertToJSON(OpenAPI_cag_info_2_t *cag_info_2)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (cag_info_2 == NULL) {
        ogs_error("OpenAPI_cag_info_2_convertToJSON() failed [CagInfo_2]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!cag_info_2->cag_list) {
        ogs_error("OpenAPI_cag_info_2_convertToJSON() failed [cag_list]");
        return NULL;
    }
    cJSON *cag_listList = cJSON_AddArrayToObject(item, "cagList");
    if (cag_listList == NULL) {
        ogs_error("OpenAPI_cag_info_2_convertToJSON() failed [cag_list]");
        goto end;
    }
    OpenAPI_list_for_each(cag_info_2->cag_list, node) {
        if (cJSON_AddStringToObject(cag_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_cag_info_2_convertToJSON() failed [cag_list]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_cag_info_2_t *OpenAPI_cag_info_2_parseFromJSON(cJSON *cag_info_2JSON)
{
    OpenAPI_cag_info_2_t *cag_info_2_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *cag_list = NULL;
    OpenAPI_list_t *cag_listList = NULL;
    cag_list = cJSON_GetObjectItemCaseSensitive(cag_info_2JSON, "cagList");
    if (!cag_list) {
        ogs_error("OpenAPI_cag_info_2_parseFromJSON() failed [cag_list]");
        goto end;
    }
        cJSON *cag_list_local = NULL;
        if (!cJSON_IsArray(cag_list)) {
            ogs_error("OpenAPI_cag_info_2_parseFromJSON() failed [cag_list]");
            goto end;
        }

        cag_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(cag_list_local, cag_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(cag_list_local)) {
                ogs_error("OpenAPI_cag_info_2_parseFromJSON() failed [cag_list]");
                goto end;
            }
            OpenAPI_list_add(cag_listList, ogs_strdup(cag_list_local->valuestring));
        }

    cag_info_2_local_var = OpenAPI_cag_info_2_create (
        cag_listList
    );

    return cag_info_2_local_var;
end:
    if (cag_listList) {
        OpenAPI_list_for_each(cag_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(cag_listList);
        cag_listList = NULL;
    }
    return NULL;
}

OpenAPI_cag_info_2_t *OpenAPI_cag_info_2_copy(OpenAPI_cag_info_2_t *dst, OpenAPI_cag_info_2_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_cag_info_2_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_cag_info_2_convertToJSON() failed");
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

    OpenAPI_cag_info_2_free(dst);
    dst = OpenAPI_cag_info_2_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

