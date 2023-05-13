
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nsag_info.h"

OpenAPI_nsag_info_t *OpenAPI_nsag_info_create(
    OpenAPI_list_t *nsag_ids,
    OpenAPI_list_t *snssai_list,
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *tai_range_list
)
{
    OpenAPI_nsag_info_t *nsag_info_local_var = ogs_malloc(sizeof(OpenAPI_nsag_info_t));
    ogs_assert(nsag_info_local_var);

    nsag_info_local_var->nsag_ids = nsag_ids;
    nsag_info_local_var->snssai_list = snssai_list;
    nsag_info_local_var->tai_list = tai_list;
    nsag_info_local_var->tai_range_list = tai_range_list;

    return nsag_info_local_var;
}

void OpenAPI_nsag_info_free(OpenAPI_nsag_info_t *nsag_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nsag_info) {
        return;
    }
    if (nsag_info->nsag_ids) {
        OpenAPI_list_for_each(nsag_info->nsag_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(nsag_info->nsag_ids);
        nsag_info->nsag_ids = NULL;
    }
    if (nsag_info->snssai_list) {
        OpenAPI_list_for_each(nsag_info->snssai_list, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(nsag_info->snssai_list);
        nsag_info->snssai_list = NULL;
    }
    if (nsag_info->tai_list) {
        OpenAPI_list_for_each(nsag_info->tai_list, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(nsag_info->tai_list);
        nsag_info->tai_list = NULL;
    }
    if (nsag_info->tai_range_list) {
        OpenAPI_list_for_each(nsag_info->tai_range_list, node) {
            OpenAPI_tai_range_free(node->data);
        }
        OpenAPI_list_free(nsag_info->tai_range_list);
        nsag_info->tai_range_list = NULL;
    }
    ogs_free(nsag_info);
}

cJSON *OpenAPI_nsag_info_convertToJSON(OpenAPI_nsag_info_t *nsag_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nsag_info == NULL) {
        ogs_error("OpenAPI_nsag_info_convertToJSON() failed [NsagInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!nsag_info->nsag_ids) {
        ogs_error("OpenAPI_nsag_info_convertToJSON() failed [nsag_ids]");
        return NULL;
    }
    cJSON *nsag_idsList = cJSON_AddArrayToObject(item, "nsagIds");
    if (nsag_idsList == NULL) {
        ogs_error("OpenAPI_nsag_info_convertToJSON() failed [nsag_ids]");
        goto end;
    }
    OpenAPI_list_for_each(nsag_info->nsag_ids, node) {
        if (node->data == NULL) {
            ogs_error("OpenAPI_nsag_info_convertToJSON() failed [nsag_ids]");
            goto end;
        }
        if (cJSON_AddNumberToObject(nsag_idsList, "", *(double *)node->data) == NULL) {
            ogs_error("OpenAPI_nsag_info_convertToJSON() failed [nsag_ids]");
            goto end;
        }
    }

    if (!nsag_info->snssai_list) {
        ogs_error("OpenAPI_nsag_info_convertToJSON() failed [snssai_list]");
        return NULL;
    }
    cJSON *snssai_listList = cJSON_AddArrayToObject(item, "snssaiList");
    if (snssai_listList == NULL) {
        ogs_error("OpenAPI_nsag_info_convertToJSON() failed [snssai_list]");
        goto end;
    }
    OpenAPI_list_for_each(nsag_info->snssai_list, node) {
        cJSON *itemLocal = OpenAPI_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nsag_info_convertToJSON() failed [snssai_list]");
            goto end;
        }
        cJSON_AddItemToArray(snssai_listList, itemLocal);
    }

    if (nsag_info->tai_list) {
    cJSON *tai_listList = cJSON_AddArrayToObject(item, "taiList");
    if (tai_listList == NULL) {
        ogs_error("OpenAPI_nsag_info_convertToJSON() failed [tai_list]");
        goto end;
    }
    OpenAPI_list_for_each(nsag_info->tai_list, node) {
        cJSON *itemLocal = OpenAPI_tai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nsag_info_convertToJSON() failed [tai_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_listList, itemLocal);
    }
    }

    if (nsag_info->tai_range_list) {
    cJSON *tai_range_listList = cJSON_AddArrayToObject(item, "taiRangeList");
    if (tai_range_listList == NULL) {
        ogs_error("OpenAPI_nsag_info_convertToJSON() failed [tai_range_list]");
        goto end;
    }
    OpenAPI_list_for_each(nsag_info->tai_range_list, node) {
        cJSON *itemLocal = OpenAPI_tai_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nsag_info_convertToJSON() failed [tai_range_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_range_listList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_nsag_info_t *OpenAPI_nsag_info_parseFromJSON(cJSON *nsag_infoJSON)
{
    OpenAPI_nsag_info_t *nsag_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *nsag_ids = NULL;
    OpenAPI_list_t *nsag_idsList = NULL;
    cJSON *snssai_list = NULL;
    OpenAPI_list_t *snssai_listList = NULL;
    cJSON *tai_list = NULL;
    OpenAPI_list_t *tai_listList = NULL;
    cJSON *tai_range_list = NULL;
    OpenAPI_list_t *tai_range_listList = NULL;
    nsag_ids = cJSON_GetObjectItemCaseSensitive(nsag_infoJSON, "nsagIds");
    if (!nsag_ids) {
        ogs_error("OpenAPI_nsag_info_parseFromJSON() failed [nsag_ids]");
        goto end;
    }
        cJSON *nsag_ids_local = NULL;
        if (!cJSON_IsArray(nsag_ids)) {
            ogs_error("OpenAPI_nsag_info_parseFromJSON() failed [nsag_ids]");
            goto end;
        }

        nsag_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(nsag_ids_local, nsag_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsNumber(nsag_ids_local)) {
                ogs_error("OpenAPI_nsag_info_parseFromJSON() failed [nsag_ids]");
                goto end;
            }
            localDouble = (double *)ogs_calloc(1, sizeof(double));
            if (!localDouble) {
                ogs_error("OpenAPI_nsag_info_parseFromJSON() failed [nsag_ids]");
                goto end;
            }
            *localDouble = nsag_ids_local->valuedouble;
            OpenAPI_list_add(nsag_idsList, localDouble);
        }

    snssai_list = cJSON_GetObjectItemCaseSensitive(nsag_infoJSON, "snssaiList");
    if (!snssai_list) {
        ogs_error("OpenAPI_nsag_info_parseFromJSON() failed [snssai_list]");
        goto end;
    }
        cJSON *snssai_list_local = NULL;
        if (!cJSON_IsArray(snssai_list)) {
            ogs_error("OpenAPI_nsag_info_parseFromJSON() failed [snssai_list]");
            goto end;
        }

        snssai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(snssai_list_local, snssai_list) {
            if (!cJSON_IsObject(snssai_list_local)) {
                ogs_error("OpenAPI_nsag_info_parseFromJSON() failed [snssai_list]");
                goto end;
            }
            OpenAPI_snssai_t *snssai_listItem = OpenAPI_snssai_parseFromJSON(snssai_list_local);
            if (!snssai_listItem) {
                ogs_error("No snssai_listItem");
                goto end;
            }
            OpenAPI_list_add(snssai_listList, snssai_listItem);
        }

    tai_list = cJSON_GetObjectItemCaseSensitive(nsag_infoJSON, "taiList");
    if (tai_list) {
        cJSON *tai_list_local = NULL;
        if (!cJSON_IsArray(tai_list)) {
            ogs_error("OpenAPI_nsag_info_parseFromJSON() failed [tai_list]");
            goto end;
        }

        tai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_list_local, tai_list) {
            if (!cJSON_IsObject(tai_list_local)) {
                ogs_error("OpenAPI_nsag_info_parseFromJSON() failed [tai_list]");
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

    tai_range_list = cJSON_GetObjectItemCaseSensitive(nsag_infoJSON, "taiRangeList");
    if (tai_range_list) {
        cJSON *tai_range_list_local = NULL;
        if (!cJSON_IsArray(tai_range_list)) {
            ogs_error("OpenAPI_nsag_info_parseFromJSON() failed [tai_range_list]");
            goto end;
        }

        tai_range_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_range_list_local, tai_range_list) {
            if (!cJSON_IsObject(tai_range_list_local)) {
                ogs_error("OpenAPI_nsag_info_parseFromJSON() failed [tai_range_list]");
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

    nsag_info_local_var = OpenAPI_nsag_info_create (
        nsag_idsList,
        snssai_listList,
        tai_list ? tai_listList : NULL,
        tai_range_list ? tai_range_listList : NULL
    );

    return nsag_info_local_var;
end:
    if (nsag_idsList) {
        OpenAPI_list_for_each(nsag_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(nsag_idsList);
        nsag_idsList = NULL;
    }
    if (snssai_listList) {
        OpenAPI_list_for_each(snssai_listList, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(snssai_listList);
        snssai_listList = NULL;
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
    return NULL;
}

OpenAPI_nsag_info_t *OpenAPI_nsag_info_copy(OpenAPI_nsag_info_t *dst, OpenAPI_nsag_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nsag_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nsag_info_convertToJSON() failed");
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

    OpenAPI_nsag_info_free(dst);
    dst = OpenAPI_nsag_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

