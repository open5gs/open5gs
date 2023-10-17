
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mbs_service_area.h"

OpenAPI_mbs_service_area_t *OpenAPI_mbs_service_area_create(
    OpenAPI_list_t *ncgi_list,
    OpenAPI_list_t *tai_list
)
{
    OpenAPI_mbs_service_area_t *mbs_service_area_local_var = ogs_malloc(sizeof(OpenAPI_mbs_service_area_t));
    ogs_assert(mbs_service_area_local_var);

    mbs_service_area_local_var->ncgi_list = ncgi_list;
    mbs_service_area_local_var->tai_list = tai_list;

    return mbs_service_area_local_var;
}

void OpenAPI_mbs_service_area_free(OpenAPI_mbs_service_area_t *mbs_service_area)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == mbs_service_area) {
        return;
    }
    if (mbs_service_area->ncgi_list) {
        OpenAPI_list_for_each(mbs_service_area->ncgi_list, node) {
            OpenAPI_ncgi_tai_free(node->data);
        }
        OpenAPI_list_free(mbs_service_area->ncgi_list);
        mbs_service_area->ncgi_list = NULL;
    }
    if (mbs_service_area->tai_list) {
        OpenAPI_list_for_each(mbs_service_area->tai_list, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(mbs_service_area->tai_list);
        mbs_service_area->tai_list = NULL;
    }
    ogs_free(mbs_service_area);
}

cJSON *OpenAPI_mbs_service_area_convertToJSON(OpenAPI_mbs_service_area_t *mbs_service_area)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (mbs_service_area == NULL) {
        ogs_error("OpenAPI_mbs_service_area_convertToJSON() failed [MbsServiceArea]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (mbs_service_area->ncgi_list) {
    cJSON *ncgi_listList = cJSON_AddArrayToObject(item, "ncgiList");
    if (ncgi_listList == NULL) {
        ogs_error("OpenAPI_mbs_service_area_convertToJSON() failed [ncgi_list]");
        goto end;
    }
    OpenAPI_list_for_each(mbs_service_area->ncgi_list, node) {
        cJSON *itemLocal = OpenAPI_ncgi_tai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_mbs_service_area_convertToJSON() failed [ncgi_list]");
            goto end;
        }
        cJSON_AddItemToArray(ncgi_listList, itemLocal);
    }
    }

    if (mbs_service_area->tai_list) {
    cJSON *tai_listList = cJSON_AddArrayToObject(item, "taiList");
    if (tai_listList == NULL) {
        ogs_error("OpenAPI_mbs_service_area_convertToJSON() failed [tai_list]");
        goto end;
    }
    OpenAPI_list_for_each(mbs_service_area->tai_list, node) {
        cJSON *itemLocal = OpenAPI_tai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_mbs_service_area_convertToJSON() failed [tai_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_listList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_mbs_service_area_t *OpenAPI_mbs_service_area_parseFromJSON(cJSON *mbs_service_areaJSON)
{
    OpenAPI_mbs_service_area_t *mbs_service_area_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ncgi_list = NULL;
    OpenAPI_list_t *ncgi_listList = NULL;
    cJSON *tai_list = NULL;
    OpenAPI_list_t *tai_listList = NULL;
    ncgi_list = cJSON_GetObjectItemCaseSensitive(mbs_service_areaJSON, "ncgiList");
    if (ncgi_list) {
        cJSON *ncgi_list_local = NULL;
        if (!cJSON_IsArray(ncgi_list)) {
            ogs_error("OpenAPI_mbs_service_area_parseFromJSON() failed [ncgi_list]");
            goto end;
        }

        ncgi_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(ncgi_list_local, ncgi_list) {
            if (!cJSON_IsObject(ncgi_list_local)) {
                ogs_error("OpenAPI_mbs_service_area_parseFromJSON() failed [ncgi_list]");
                goto end;
            }
            OpenAPI_ncgi_tai_t *ncgi_listItem = OpenAPI_ncgi_tai_parseFromJSON(ncgi_list_local);
            if (!ncgi_listItem) {
                ogs_error("No ncgi_listItem");
                goto end;
            }
            OpenAPI_list_add(ncgi_listList, ncgi_listItem);
        }
    }

    tai_list = cJSON_GetObjectItemCaseSensitive(mbs_service_areaJSON, "taiList");
    if (tai_list) {
        cJSON *tai_list_local = NULL;
        if (!cJSON_IsArray(tai_list)) {
            ogs_error("OpenAPI_mbs_service_area_parseFromJSON() failed [tai_list]");
            goto end;
        }

        tai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_list_local, tai_list) {
            if (!cJSON_IsObject(tai_list_local)) {
                ogs_error("OpenAPI_mbs_service_area_parseFromJSON() failed [tai_list]");
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

    mbs_service_area_local_var = OpenAPI_mbs_service_area_create (
        ncgi_list ? ncgi_listList : NULL,
        tai_list ? tai_listList : NULL
    );

    return mbs_service_area_local_var;
end:
    if (ncgi_listList) {
        OpenAPI_list_for_each(ncgi_listList, node) {
            OpenAPI_ncgi_tai_free(node->data);
        }
        OpenAPI_list_free(ncgi_listList);
        ncgi_listList = NULL;
    }
    if (tai_listList) {
        OpenAPI_list_for_each(tai_listList, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(tai_listList);
        tai_listList = NULL;
    }
    return NULL;
}

OpenAPI_mbs_service_area_t *OpenAPI_mbs_service_area_copy(OpenAPI_mbs_service_area_t *dst, OpenAPI_mbs_service_area_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_mbs_service_area_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_mbs_service_area_convertToJSON() failed");
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

    OpenAPI_mbs_service_area_free(dst);
    dst = OpenAPI_mbs_service_area_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

