
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nsacf_info.h"

OpenAPI_nsacf_info_t *OpenAPI_nsacf_info_create(
    OpenAPI_nsacf_capability_t *nsacf_capability,
    OpenAPI_list_t *snssai_list_for_entire_plmn,
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *tai_range_list,
    OpenAPI_list_t *nsac_sai_list
)
{
    OpenAPI_nsacf_info_t *nsacf_info_local_var = ogs_malloc(sizeof(OpenAPI_nsacf_info_t));
    ogs_assert(nsacf_info_local_var);

    nsacf_info_local_var->nsacf_capability = nsacf_capability;
    nsacf_info_local_var->snssai_list_for_entire_plmn = snssai_list_for_entire_plmn;
    nsacf_info_local_var->tai_list = tai_list;
    nsacf_info_local_var->tai_range_list = tai_range_list;
    nsacf_info_local_var->nsac_sai_list = nsac_sai_list;

    return nsacf_info_local_var;
}

void OpenAPI_nsacf_info_free(OpenAPI_nsacf_info_t *nsacf_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nsacf_info) {
        return;
    }
    if (nsacf_info->nsacf_capability) {
        OpenAPI_nsacf_capability_free(nsacf_info->nsacf_capability);
        nsacf_info->nsacf_capability = NULL;
    }
    if (nsacf_info->snssai_list_for_entire_plmn) {
        OpenAPI_list_for_each(nsacf_info->snssai_list_for_entire_plmn, node) {
            OpenAPI_ext_snssai_free(node->data);
        }
        OpenAPI_list_free(nsacf_info->snssai_list_for_entire_plmn);
        nsacf_info->snssai_list_for_entire_plmn = NULL;
    }
    if (nsacf_info->tai_list) {
        OpenAPI_list_for_each(nsacf_info->tai_list, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(nsacf_info->tai_list);
        nsacf_info->tai_list = NULL;
    }
    if (nsacf_info->tai_range_list) {
        OpenAPI_list_for_each(nsacf_info->tai_range_list, node) {
            OpenAPI_tai_range_free(node->data);
        }
        OpenAPI_list_free(nsacf_info->tai_range_list);
        nsacf_info->tai_range_list = NULL;
    }
    if (nsacf_info->nsac_sai_list) {
        OpenAPI_list_for_each(nsacf_info->nsac_sai_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(nsacf_info->nsac_sai_list);
        nsacf_info->nsac_sai_list = NULL;
    }
    ogs_free(nsacf_info);
}

cJSON *OpenAPI_nsacf_info_convertToJSON(OpenAPI_nsacf_info_t *nsacf_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nsacf_info == NULL) {
        ogs_error("OpenAPI_nsacf_info_convertToJSON() failed [NsacfInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!nsacf_info->nsacf_capability) {
        ogs_error("OpenAPI_nsacf_info_convertToJSON() failed [nsacf_capability]");
        return NULL;
    }
    cJSON *nsacf_capability_local_JSON = OpenAPI_nsacf_capability_convertToJSON(nsacf_info->nsacf_capability);
    if (nsacf_capability_local_JSON == NULL) {
        ogs_error("OpenAPI_nsacf_info_convertToJSON() failed [nsacf_capability]");
        goto end;
    }
    cJSON_AddItemToObject(item, "nsacfCapability", nsacf_capability_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_nsacf_info_convertToJSON() failed [nsacf_capability]");
        goto end;
    }

    if (nsacf_info->snssai_list_for_entire_plmn) {
    cJSON *snssai_list_for_entire_plmnList = cJSON_AddArrayToObject(item, "snssaiListForEntirePlmn");
    if (snssai_list_for_entire_plmnList == NULL) {
        ogs_error("OpenAPI_nsacf_info_convertToJSON() failed [snssai_list_for_entire_plmn]");
        goto end;
    }
    OpenAPI_list_for_each(nsacf_info->snssai_list_for_entire_plmn, node) {
        cJSON *itemLocal = OpenAPI_ext_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nsacf_info_convertToJSON() failed [snssai_list_for_entire_plmn]");
            goto end;
        }
        cJSON_AddItemToArray(snssai_list_for_entire_plmnList, itemLocal);
    }
    }

    if (nsacf_info->tai_list) {
    cJSON *tai_listList = cJSON_AddArrayToObject(item, "taiList");
    if (tai_listList == NULL) {
        ogs_error("OpenAPI_nsacf_info_convertToJSON() failed [tai_list]");
        goto end;
    }
    OpenAPI_list_for_each(nsacf_info->tai_list, node) {
        cJSON *itemLocal = OpenAPI_tai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nsacf_info_convertToJSON() failed [tai_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_listList, itemLocal);
    }
    }

    if (nsacf_info->tai_range_list) {
    cJSON *tai_range_listList = cJSON_AddArrayToObject(item, "taiRangeList");
    if (tai_range_listList == NULL) {
        ogs_error("OpenAPI_nsacf_info_convertToJSON() failed [tai_range_list]");
        goto end;
    }
    OpenAPI_list_for_each(nsacf_info->tai_range_list, node) {
        cJSON *itemLocal = OpenAPI_tai_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nsacf_info_convertToJSON() failed [tai_range_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_range_listList, itemLocal);
    }
    }

    if (nsacf_info->nsac_sai_list) {
    cJSON *nsac_sai_listList = cJSON_AddArrayToObject(item, "nsacSaiList");
    if (nsac_sai_listList == NULL) {
        ogs_error("OpenAPI_nsacf_info_convertToJSON() failed [nsac_sai_list]");
        goto end;
    }
    OpenAPI_list_for_each(nsacf_info->nsac_sai_list, node) {
        if (cJSON_AddStringToObject(nsac_sai_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_nsacf_info_convertToJSON() failed [nsac_sai_list]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_nsacf_info_t *OpenAPI_nsacf_info_parseFromJSON(cJSON *nsacf_infoJSON)
{
    OpenAPI_nsacf_info_t *nsacf_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *nsacf_capability = NULL;
    OpenAPI_nsacf_capability_t *nsacf_capability_local_nonprim = NULL;
    cJSON *snssai_list_for_entire_plmn = NULL;
    OpenAPI_list_t *snssai_list_for_entire_plmnList = NULL;
    cJSON *tai_list = NULL;
    OpenAPI_list_t *tai_listList = NULL;
    cJSON *tai_range_list = NULL;
    OpenAPI_list_t *tai_range_listList = NULL;
    cJSON *nsac_sai_list = NULL;
    OpenAPI_list_t *nsac_sai_listList = NULL;
    nsacf_capability = cJSON_GetObjectItemCaseSensitive(nsacf_infoJSON, "nsacfCapability");
    if (!nsacf_capability) {
        ogs_error("OpenAPI_nsacf_info_parseFromJSON() failed [nsacf_capability]");
        goto end;
    }
    nsacf_capability_local_nonprim = OpenAPI_nsacf_capability_parseFromJSON(nsacf_capability);
    if (!nsacf_capability_local_nonprim) {
        ogs_error("OpenAPI_nsacf_capability_parseFromJSON failed [nsacf_capability]");
        goto end;
    }

    snssai_list_for_entire_plmn = cJSON_GetObjectItemCaseSensitive(nsacf_infoJSON, "snssaiListForEntirePlmn");
    if (snssai_list_for_entire_plmn) {
        cJSON *snssai_list_for_entire_plmn_local = NULL;
        if (!cJSON_IsArray(snssai_list_for_entire_plmn)) {
            ogs_error("OpenAPI_nsacf_info_parseFromJSON() failed [snssai_list_for_entire_plmn]");
            goto end;
        }

        snssai_list_for_entire_plmnList = OpenAPI_list_create();

        cJSON_ArrayForEach(snssai_list_for_entire_plmn_local, snssai_list_for_entire_plmn) {
            if (!cJSON_IsObject(snssai_list_for_entire_plmn_local)) {
                ogs_error("OpenAPI_nsacf_info_parseFromJSON() failed [snssai_list_for_entire_plmn]");
                goto end;
            }
            OpenAPI_ext_snssai_t *snssai_list_for_entire_plmnItem = OpenAPI_ext_snssai_parseFromJSON(snssai_list_for_entire_plmn_local);
            if (!snssai_list_for_entire_plmnItem) {
                ogs_error("No snssai_list_for_entire_plmnItem");
                goto end;
            }
            OpenAPI_list_add(snssai_list_for_entire_plmnList, snssai_list_for_entire_plmnItem);
        }
    }

    tai_list = cJSON_GetObjectItemCaseSensitive(nsacf_infoJSON, "taiList");
    if (tai_list) {
        cJSON *tai_list_local = NULL;
        if (!cJSON_IsArray(tai_list)) {
            ogs_error("OpenAPI_nsacf_info_parseFromJSON() failed [tai_list]");
            goto end;
        }

        tai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_list_local, tai_list) {
            if (!cJSON_IsObject(tai_list_local)) {
                ogs_error("OpenAPI_nsacf_info_parseFromJSON() failed [tai_list]");
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

    tai_range_list = cJSON_GetObjectItemCaseSensitive(nsacf_infoJSON, "taiRangeList");
    if (tai_range_list) {
        cJSON *tai_range_list_local = NULL;
        if (!cJSON_IsArray(tai_range_list)) {
            ogs_error("OpenAPI_nsacf_info_parseFromJSON() failed [tai_range_list]");
            goto end;
        }

        tai_range_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_range_list_local, tai_range_list) {
            if (!cJSON_IsObject(tai_range_list_local)) {
                ogs_error("OpenAPI_nsacf_info_parseFromJSON() failed [tai_range_list]");
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

    nsac_sai_list = cJSON_GetObjectItemCaseSensitive(nsacf_infoJSON, "nsacSaiList");
    if (nsac_sai_list) {
        cJSON *nsac_sai_list_local = NULL;
        if (!cJSON_IsArray(nsac_sai_list)) {
            ogs_error("OpenAPI_nsacf_info_parseFromJSON() failed [nsac_sai_list]");
            goto end;
        }

        nsac_sai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(nsac_sai_list_local, nsac_sai_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(nsac_sai_list_local)) {
                ogs_error("OpenAPI_nsacf_info_parseFromJSON() failed [nsac_sai_list]");
                goto end;
            }
            OpenAPI_list_add(nsac_sai_listList, ogs_strdup(nsac_sai_list_local->valuestring));
        }
    }

    nsacf_info_local_var = OpenAPI_nsacf_info_create (
        nsacf_capability_local_nonprim,
        snssai_list_for_entire_plmn ? snssai_list_for_entire_plmnList : NULL,
        tai_list ? tai_listList : NULL,
        tai_range_list ? tai_range_listList : NULL,
        nsac_sai_list ? nsac_sai_listList : NULL
    );

    return nsacf_info_local_var;
end:
    if (nsacf_capability_local_nonprim) {
        OpenAPI_nsacf_capability_free(nsacf_capability_local_nonprim);
        nsacf_capability_local_nonprim = NULL;
    }
    if (snssai_list_for_entire_plmnList) {
        OpenAPI_list_for_each(snssai_list_for_entire_plmnList, node) {
            OpenAPI_ext_snssai_free(node->data);
        }
        OpenAPI_list_free(snssai_list_for_entire_plmnList);
        snssai_list_for_entire_plmnList = NULL;
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
    if (nsac_sai_listList) {
        OpenAPI_list_for_each(nsac_sai_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(nsac_sai_listList);
        nsac_sai_listList = NULL;
    }
    return NULL;
}

OpenAPI_nsacf_info_t *OpenAPI_nsacf_info_copy(OpenAPI_nsacf_info_t *dst, OpenAPI_nsacf_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nsacf_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nsacf_info_convertToJSON() failed");
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

    OpenAPI_nsacf_info_free(dst);
    dst = OpenAPI_nsacf_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

