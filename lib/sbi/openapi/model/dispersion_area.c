
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dispersion_area.h"

OpenAPI_dispersion_area_t *OpenAPI_dispersion_area_create(
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *ncgi_list,
    OpenAPI_list_t *ecgi_list,
    bool is_n3ga_ind,
    int n3ga_ind
)
{
    OpenAPI_dispersion_area_t *dispersion_area_local_var = ogs_malloc(sizeof(OpenAPI_dispersion_area_t));
    ogs_assert(dispersion_area_local_var);

    dispersion_area_local_var->tai_list = tai_list;
    dispersion_area_local_var->ncgi_list = ncgi_list;
    dispersion_area_local_var->ecgi_list = ecgi_list;
    dispersion_area_local_var->is_n3ga_ind = is_n3ga_ind;
    dispersion_area_local_var->n3ga_ind = n3ga_ind;

    return dispersion_area_local_var;
}

void OpenAPI_dispersion_area_free(OpenAPI_dispersion_area_t *dispersion_area)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == dispersion_area) {
        return;
    }
    if (dispersion_area->tai_list) {
        OpenAPI_list_for_each(dispersion_area->tai_list, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(dispersion_area->tai_list);
        dispersion_area->tai_list = NULL;
    }
    if (dispersion_area->ncgi_list) {
        OpenAPI_list_for_each(dispersion_area->ncgi_list, node) {
            OpenAPI_ncgi_free(node->data);
        }
        OpenAPI_list_free(dispersion_area->ncgi_list);
        dispersion_area->ncgi_list = NULL;
    }
    if (dispersion_area->ecgi_list) {
        OpenAPI_list_for_each(dispersion_area->ecgi_list, node) {
            OpenAPI_ecgi_free(node->data);
        }
        OpenAPI_list_free(dispersion_area->ecgi_list);
        dispersion_area->ecgi_list = NULL;
    }
    ogs_free(dispersion_area);
}

cJSON *OpenAPI_dispersion_area_convertToJSON(OpenAPI_dispersion_area_t *dispersion_area)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (dispersion_area == NULL) {
        ogs_error("OpenAPI_dispersion_area_convertToJSON() failed [DispersionArea]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (dispersion_area->tai_list) {
    cJSON *tai_listList = cJSON_AddArrayToObject(item, "taiList");
    if (tai_listList == NULL) {
        ogs_error("OpenAPI_dispersion_area_convertToJSON() failed [tai_list]");
        goto end;
    }
    OpenAPI_list_for_each(dispersion_area->tai_list, node) {
        cJSON *itemLocal = OpenAPI_tai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_dispersion_area_convertToJSON() failed [tai_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_listList, itemLocal);
    }
    }

    if (dispersion_area->ncgi_list) {
    cJSON *ncgi_listList = cJSON_AddArrayToObject(item, "ncgiList");
    if (ncgi_listList == NULL) {
        ogs_error("OpenAPI_dispersion_area_convertToJSON() failed [ncgi_list]");
        goto end;
    }
    OpenAPI_list_for_each(dispersion_area->ncgi_list, node) {
        cJSON *itemLocal = OpenAPI_ncgi_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_dispersion_area_convertToJSON() failed [ncgi_list]");
            goto end;
        }
        cJSON_AddItemToArray(ncgi_listList, itemLocal);
    }
    }

    if (dispersion_area->ecgi_list) {
    cJSON *ecgi_listList = cJSON_AddArrayToObject(item, "ecgiList");
    if (ecgi_listList == NULL) {
        ogs_error("OpenAPI_dispersion_area_convertToJSON() failed [ecgi_list]");
        goto end;
    }
    OpenAPI_list_for_each(dispersion_area->ecgi_list, node) {
        cJSON *itemLocal = OpenAPI_ecgi_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_dispersion_area_convertToJSON() failed [ecgi_list]");
            goto end;
        }
        cJSON_AddItemToArray(ecgi_listList, itemLocal);
    }
    }

    if (dispersion_area->is_n3ga_ind) {
    if (cJSON_AddBoolToObject(item, "n3gaInd", dispersion_area->n3ga_ind) == NULL) {
        ogs_error("OpenAPI_dispersion_area_convertToJSON() failed [n3ga_ind]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_dispersion_area_t *OpenAPI_dispersion_area_parseFromJSON(cJSON *dispersion_areaJSON)
{
    OpenAPI_dispersion_area_t *dispersion_area_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *tai_list = NULL;
    OpenAPI_list_t *tai_listList = NULL;
    cJSON *ncgi_list = NULL;
    OpenAPI_list_t *ncgi_listList = NULL;
    cJSON *ecgi_list = NULL;
    OpenAPI_list_t *ecgi_listList = NULL;
    cJSON *n3ga_ind = NULL;
    tai_list = cJSON_GetObjectItemCaseSensitive(dispersion_areaJSON, "taiList");
    if (tai_list) {
        cJSON *tai_list_local = NULL;
        if (!cJSON_IsArray(tai_list)) {
            ogs_error("OpenAPI_dispersion_area_parseFromJSON() failed [tai_list]");
            goto end;
        }

        tai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_list_local, tai_list) {
            if (!cJSON_IsObject(tai_list_local)) {
                ogs_error("OpenAPI_dispersion_area_parseFromJSON() failed [tai_list]");
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

    ncgi_list = cJSON_GetObjectItemCaseSensitive(dispersion_areaJSON, "ncgiList");
    if (ncgi_list) {
        cJSON *ncgi_list_local = NULL;
        if (!cJSON_IsArray(ncgi_list)) {
            ogs_error("OpenAPI_dispersion_area_parseFromJSON() failed [ncgi_list]");
            goto end;
        }

        ncgi_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(ncgi_list_local, ncgi_list) {
            if (!cJSON_IsObject(ncgi_list_local)) {
                ogs_error("OpenAPI_dispersion_area_parseFromJSON() failed [ncgi_list]");
                goto end;
            }
            OpenAPI_ncgi_t *ncgi_listItem = OpenAPI_ncgi_parseFromJSON(ncgi_list_local);
            if (!ncgi_listItem) {
                ogs_error("No ncgi_listItem");
                goto end;
            }
            OpenAPI_list_add(ncgi_listList, ncgi_listItem);
        }
    }

    ecgi_list = cJSON_GetObjectItemCaseSensitive(dispersion_areaJSON, "ecgiList");
    if (ecgi_list) {
        cJSON *ecgi_list_local = NULL;
        if (!cJSON_IsArray(ecgi_list)) {
            ogs_error("OpenAPI_dispersion_area_parseFromJSON() failed [ecgi_list]");
            goto end;
        }

        ecgi_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(ecgi_list_local, ecgi_list) {
            if (!cJSON_IsObject(ecgi_list_local)) {
                ogs_error("OpenAPI_dispersion_area_parseFromJSON() failed [ecgi_list]");
                goto end;
            }
            OpenAPI_ecgi_t *ecgi_listItem = OpenAPI_ecgi_parseFromJSON(ecgi_list_local);
            if (!ecgi_listItem) {
                ogs_error("No ecgi_listItem");
                goto end;
            }
            OpenAPI_list_add(ecgi_listList, ecgi_listItem);
        }
    }

    n3ga_ind = cJSON_GetObjectItemCaseSensitive(dispersion_areaJSON, "n3gaInd");
    if (n3ga_ind) {
    if (!cJSON_IsBool(n3ga_ind)) {
        ogs_error("OpenAPI_dispersion_area_parseFromJSON() failed [n3ga_ind]");
        goto end;
    }
    }

    dispersion_area_local_var = OpenAPI_dispersion_area_create (
        tai_list ? tai_listList : NULL,
        ncgi_list ? ncgi_listList : NULL,
        ecgi_list ? ecgi_listList : NULL,
        n3ga_ind ? true : false,
        n3ga_ind ? n3ga_ind->valueint : 0
    );

    return dispersion_area_local_var;
end:
    if (tai_listList) {
        OpenAPI_list_for_each(tai_listList, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(tai_listList);
        tai_listList = NULL;
    }
    if (ncgi_listList) {
        OpenAPI_list_for_each(ncgi_listList, node) {
            OpenAPI_ncgi_free(node->data);
        }
        OpenAPI_list_free(ncgi_listList);
        ncgi_listList = NULL;
    }
    if (ecgi_listList) {
        OpenAPI_list_for_each(ecgi_listList, node) {
            OpenAPI_ecgi_free(node->data);
        }
        OpenAPI_list_free(ecgi_listList);
        ecgi_listList = NULL;
    }
    return NULL;
}

OpenAPI_dispersion_area_t *OpenAPI_dispersion_area_copy(OpenAPI_dispersion_area_t *dst, OpenAPI_dispersion_area_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_dispersion_area_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_dispersion_area_convertToJSON() failed");
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

    OpenAPI_dispersion_area_free(dst);
    dst = OpenAPI_dispersion_area_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

