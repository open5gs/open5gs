
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "hss_info.h"

OpenAPI_hss_info_t *OpenAPI_hss_info_create(
    char *group_id,
    OpenAPI_list_t *imsi_ranges,
    OpenAPI_list_t *ims_private_identity_ranges,
    OpenAPI_list_t *ims_public_identity_ranges,
    OpenAPI_list_t *msisdn_ranges
)
{
    OpenAPI_hss_info_t *hss_info_local_var = ogs_malloc(sizeof(OpenAPI_hss_info_t));
    ogs_assert(hss_info_local_var);

    hss_info_local_var->group_id = group_id;
    hss_info_local_var->imsi_ranges = imsi_ranges;
    hss_info_local_var->ims_private_identity_ranges = ims_private_identity_ranges;
    hss_info_local_var->ims_public_identity_ranges = ims_public_identity_ranges;
    hss_info_local_var->msisdn_ranges = msisdn_ranges;

    return hss_info_local_var;
}

void OpenAPI_hss_info_free(OpenAPI_hss_info_t *hss_info)
{
    if (NULL == hss_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(hss_info->group_id);
    OpenAPI_list_for_each(hss_info->imsi_ranges, node) {
        OpenAPI_imsi_range_free(node->data);
    }
    OpenAPI_list_free(hss_info->imsi_ranges);
    OpenAPI_list_for_each(hss_info->ims_private_identity_ranges, node) {
        OpenAPI_identity_range_free(node->data);
    }
    OpenAPI_list_free(hss_info->ims_private_identity_ranges);
    OpenAPI_list_for_each(hss_info->ims_public_identity_ranges, node) {
        OpenAPI_identity_range_free(node->data);
    }
    OpenAPI_list_free(hss_info->ims_public_identity_ranges);
    OpenAPI_list_for_each(hss_info->msisdn_ranges, node) {
        OpenAPI_identity_range_free(node->data);
    }
    OpenAPI_list_free(hss_info->msisdn_ranges);
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

    if (hss_info->imsi_ranges) {
    cJSON *imsi_rangesList = cJSON_AddArrayToObject(item, "imsiRanges");
    if (imsi_rangesList == NULL) {
        ogs_error("OpenAPI_hss_info_convertToJSON() failed [imsi_ranges]");
        goto end;
    }

    OpenAPI_lnode_t *imsi_ranges_node;
    if (hss_info->imsi_ranges) {
        OpenAPI_list_for_each(hss_info->imsi_ranges, imsi_ranges_node) {
            cJSON *itemLocal = OpenAPI_imsi_range_convertToJSON(imsi_ranges_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_hss_info_convertToJSON() failed [imsi_ranges]");
                goto end;
            }
            cJSON_AddItemToArray(imsi_rangesList, itemLocal);
        }
    }
    }

    if (hss_info->ims_private_identity_ranges) {
    cJSON *ims_private_identity_rangesList = cJSON_AddArrayToObject(item, "imsPrivateIdentityRanges");
    if (ims_private_identity_rangesList == NULL) {
        ogs_error("OpenAPI_hss_info_convertToJSON() failed [ims_private_identity_ranges]");
        goto end;
    }

    OpenAPI_lnode_t *ims_private_identity_ranges_node;
    if (hss_info->ims_private_identity_ranges) {
        OpenAPI_list_for_each(hss_info->ims_private_identity_ranges, ims_private_identity_ranges_node) {
            cJSON *itemLocal = OpenAPI_identity_range_convertToJSON(ims_private_identity_ranges_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_hss_info_convertToJSON() failed [ims_private_identity_ranges]");
                goto end;
            }
            cJSON_AddItemToArray(ims_private_identity_rangesList, itemLocal);
        }
    }
    }

    if (hss_info->ims_public_identity_ranges) {
    cJSON *ims_public_identity_rangesList = cJSON_AddArrayToObject(item, "imsPublicIdentityRanges");
    if (ims_public_identity_rangesList == NULL) {
        ogs_error("OpenAPI_hss_info_convertToJSON() failed [ims_public_identity_ranges]");
        goto end;
    }

    OpenAPI_lnode_t *ims_public_identity_ranges_node;
    if (hss_info->ims_public_identity_ranges) {
        OpenAPI_list_for_each(hss_info->ims_public_identity_ranges, ims_public_identity_ranges_node) {
            cJSON *itemLocal = OpenAPI_identity_range_convertToJSON(ims_public_identity_ranges_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_hss_info_convertToJSON() failed [ims_public_identity_ranges]");
                goto end;
            }
            cJSON_AddItemToArray(ims_public_identity_rangesList, itemLocal);
        }
    }
    }

    if (hss_info->msisdn_ranges) {
    cJSON *msisdn_rangesList = cJSON_AddArrayToObject(item, "msisdnRanges");
    if (msisdn_rangesList == NULL) {
        ogs_error("OpenAPI_hss_info_convertToJSON() failed [msisdn_ranges]");
        goto end;
    }

    OpenAPI_lnode_t *msisdn_ranges_node;
    if (hss_info->msisdn_ranges) {
        OpenAPI_list_for_each(hss_info->msisdn_ranges, msisdn_ranges_node) {
            cJSON *itemLocal = OpenAPI_identity_range_convertToJSON(msisdn_ranges_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_hss_info_convertToJSON() failed [msisdn_ranges]");
                goto end;
            }
            cJSON_AddItemToArray(msisdn_rangesList, itemLocal);
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

    cJSON *imsi_ranges = cJSON_GetObjectItemCaseSensitive(hss_infoJSON, "imsiRanges");

    OpenAPI_list_t *imsi_rangesList;
    if (imsi_ranges) {
    cJSON *imsi_ranges_local_nonprimitive;
    if (!cJSON_IsArray(imsi_ranges)){
        ogs_error("OpenAPI_hss_info_parseFromJSON() failed [imsi_ranges]");
        goto end;
    }

    imsi_rangesList = OpenAPI_list_create();

    cJSON_ArrayForEach(imsi_ranges_local_nonprimitive, imsi_ranges ) {
        if (!cJSON_IsObject(imsi_ranges_local_nonprimitive)) {
            ogs_error("OpenAPI_hss_info_parseFromJSON() failed [imsi_ranges]");
            goto end;
        }
        OpenAPI_imsi_range_t *imsi_rangesItem = OpenAPI_imsi_range_parseFromJSON(imsi_ranges_local_nonprimitive);

        if (!imsi_rangesItem) {
            ogs_error("No imsi_rangesItem");
            OpenAPI_list_free(imsi_rangesList);
            goto end;
        }

        OpenAPI_list_add(imsi_rangesList, imsi_rangesItem);
    }
    }

    cJSON *ims_private_identity_ranges = cJSON_GetObjectItemCaseSensitive(hss_infoJSON, "imsPrivateIdentityRanges");

    OpenAPI_list_t *ims_private_identity_rangesList;
    if (ims_private_identity_ranges) {
    cJSON *ims_private_identity_ranges_local_nonprimitive;
    if (!cJSON_IsArray(ims_private_identity_ranges)){
        ogs_error("OpenAPI_hss_info_parseFromJSON() failed [ims_private_identity_ranges]");
        goto end;
    }

    ims_private_identity_rangesList = OpenAPI_list_create();

    cJSON_ArrayForEach(ims_private_identity_ranges_local_nonprimitive, ims_private_identity_ranges ) {
        if (!cJSON_IsObject(ims_private_identity_ranges_local_nonprimitive)) {
            ogs_error("OpenAPI_hss_info_parseFromJSON() failed [ims_private_identity_ranges]");
            goto end;
        }
        OpenAPI_identity_range_t *ims_private_identity_rangesItem = OpenAPI_identity_range_parseFromJSON(ims_private_identity_ranges_local_nonprimitive);

        if (!ims_private_identity_rangesItem) {
            ogs_error("No ims_private_identity_rangesItem");
            OpenAPI_list_free(ims_private_identity_rangesList);
            goto end;
        }

        OpenAPI_list_add(ims_private_identity_rangesList, ims_private_identity_rangesItem);
    }
    }

    cJSON *ims_public_identity_ranges = cJSON_GetObjectItemCaseSensitive(hss_infoJSON, "imsPublicIdentityRanges");

    OpenAPI_list_t *ims_public_identity_rangesList;
    if (ims_public_identity_ranges) {
    cJSON *ims_public_identity_ranges_local_nonprimitive;
    if (!cJSON_IsArray(ims_public_identity_ranges)){
        ogs_error("OpenAPI_hss_info_parseFromJSON() failed [ims_public_identity_ranges]");
        goto end;
    }

    ims_public_identity_rangesList = OpenAPI_list_create();

    cJSON_ArrayForEach(ims_public_identity_ranges_local_nonprimitive, ims_public_identity_ranges ) {
        if (!cJSON_IsObject(ims_public_identity_ranges_local_nonprimitive)) {
            ogs_error("OpenAPI_hss_info_parseFromJSON() failed [ims_public_identity_ranges]");
            goto end;
        }
        OpenAPI_identity_range_t *ims_public_identity_rangesItem = OpenAPI_identity_range_parseFromJSON(ims_public_identity_ranges_local_nonprimitive);

        if (!ims_public_identity_rangesItem) {
            ogs_error("No ims_public_identity_rangesItem");
            OpenAPI_list_free(ims_public_identity_rangesList);
            goto end;
        }

        OpenAPI_list_add(ims_public_identity_rangesList, ims_public_identity_rangesItem);
    }
    }

    cJSON *msisdn_ranges = cJSON_GetObjectItemCaseSensitive(hss_infoJSON, "msisdnRanges");

    OpenAPI_list_t *msisdn_rangesList;
    if (msisdn_ranges) {
    cJSON *msisdn_ranges_local_nonprimitive;
    if (!cJSON_IsArray(msisdn_ranges)){
        ogs_error("OpenAPI_hss_info_parseFromJSON() failed [msisdn_ranges]");
        goto end;
    }

    msisdn_rangesList = OpenAPI_list_create();

    cJSON_ArrayForEach(msisdn_ranges_local_nonprimitive, msisdn_ranges ) {
        if (!cJSON_IsObject(msisdn_ranges_local_nonprimitive)) {
            ogs_error("OpenAPI_hss_info_parseFromJSON() failed [msisdn_ranges]");
            goto end;
        }
        OpenAPI_identity_range_t *msisdn_rangesItem = OpenAPI_identity_range_parseFromJSON(msisdn_ranges_local_nonprimitive);

        if (!msisdn_rangesItem) {
            ogs_error("No msisdn_rangesItem");
            OpenAPI_list_free(msisdn_rangesList);
            goto end;
        }

        OpenAPI_list_add(msisdn_rangesList, msisdn_rangesItem);
    }
    }

    hss_info_local_var = OpenAPI_hss_info_create (
        group_id ? ogs_strdup(group_id->valuestring) : NULL,
        imsi_ranges ? imsi_rangesList : NULL,
        ims_private_identity_ranges ? ims_private_identity_rangesList : NULL,
        ims_public_identity_ranges ? ims_public_identity_rangesList : NULL,
        msisdn_ranges ? msisdn_rangesList : NULL
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

