
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "hss_info.h"

OpenAPI_hss_info_t *OpenAPI_hss_info_create(
    char *group_id,
    OpenAPI_list_t *ims_ranges
    )
{
    OpenAPI_hss_info_t *hss_info_local_var = OpenAPI_malloc(sizeof(OpenAPI_hss_info_t));
    if (!hss_info_local_var) {
        return NULL;
    }
    hss_info_local_var->group_id = group_id;
    hss_info_local_var->ims_ranges = ims_ranges;

    return hss_info_local_var;
}

void OpenAPI_hss_info_free(OpenAPI_hss_info_t *hss_info)
{
    if (NULL == hss_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(hss_info->group_id);
    OpenAPI_list_for_each(hss_info->ims_ranges, node) {
        OpenAPI_imsi_range_free(node->data);
    }
    OpenAPI_list_free(hss_info->ims_ranges);
    ogs_free(hss_info);
}

cJSON *OpenAPI_hss_info_convertToJSON(OpenAPI_hss_info_t *hss_info)
{
    cJSON *item = NULL;

    if (hss_info == NULL) {
        ogs_error("OpenAPI_hss_info_convertToJSON() failed [HssInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (hss_info->group_id) {
        if (cJSON_AddStringToObject(item, "groupId", hss_info->group_id) == NULL) {
            ogs_error("OpenAPI_hss_info_convertToJSON() failed [group_id]");
            goto end;
        }
    }

    if (hss_info->ims_ranges) {
        cJSON *ims_rangesList = cJSON_AddArrayToObject(item, "imsRanges");
        if (ims_rangesList == NULL) {
            ogs_error("OpenAPI_hss_info_convertToJSON() failed [ims_ranges]");
            goto end;
        }

        OpenAPI_lnode_t *ims_ranges_node;
        if (hss_info->ims_ranges) {
            OpenAPI_list_for_each(hss_info->ims_ranges, ims_ranges_node) {
                cJSON *itemLocal = OpenAPI_imsi_range_convertToJSON(ims_ranges_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_hss_info_convertToJSON() failed [ims_ranges]");
                    goto end;
                }
                cJSON_AddItemToArray(ims_rangesList, itemLocal);
            }
        }
    }

end:
    return item;
}

OpenAPI_hss_info_t *OpenAPI_hss_info_parseFromJSON(cJSON *hss_infoJSON)
{
    OpenAPI_hss_info_t *hss_info_local_var = NULL;
    cJSON *group_id = cJSON_GetObjectItemCaseSensitive(hss_infoJSON, "groupId");

    if (group_id) {
        if (!cJSON_IsString(group_id)) {
            ogs_error("OpenAPI_hss_info_parseFromJSON() failed [group_id]");
            goto end;
        }
    }

    cJSON *ims_ranges = cJSON_GetObjectItemCaseSensitive(hss_infoJSON, "imsRanges");

    OpenAPI_list_t *ims_rangesList;
    if (ims_ranges) {
        cJSON *ims_ranges_local_nonprimitive;
        if (!cJSON_IsArray(ims_ranges)) {
            ogs_error("OpenAPI_hss_info_parseFromJSON() failed [ims_ranges]");
            goto end;
        }

        ims_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(ims_ranges_local_nonprimitive, ims_ranges ) {
            if (!cJSON_IsObject(ims_ranges_local_nonprimitive)) {
                ogs_error("OpenAPI_hss_info_parseFromJSON() failed [ims_ranges]");
                goto end;
            }
            OpenAPI_imsi_range_t *ims_rangesItem = OpenAPI_imsi_range_parseFromJSON(ims_ranges_local_nonprimitive);

            OpenAPI_list_add(ims_rangesList, ims_rangesItem);
        }
    }

    hss_info_local_var = OpenAPI_hss_info_create (
        group_id ? ogs_strdup(group_id->valuestring) : NULL,
        ims_ranges ? ims_rangesList : NULL
        );

    return hss_info_local_var;
end:
    return NULL;
}

OpenAPI_hss_info_t *OpenAPI_hss_info_copy(OpenAPI_hss_info_t *dst, OpenAPI_hss_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_hss_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_hss_info_convertToJSON() failed");
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

    OpenAPI_hss_info_free(dst);
    dst = OpenAPI_hss_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

