
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dcsf_info.h"

OpenAPI_dcsf_info_t *OpenAPI_dcsf_info_create(
    OpenAPI_list_t *ims_domain_name_list,
    OpenAPI_list_t *imsi_ranges,
    OpenAPI_list_t *ims_private_identity_ranges,
    OpenAPI_list_t *ims_public_identity_ranges,
    OpenAPI_list_t *msisdn_ranges
)
{
    OpenAPI_dcsf_info_t *dcsf_info_local_var = ogs_malloc(sizeof(OpenAPI_dcsf_info_t));
    ogs_assert(dcsf_info_local_var);

    dcsf_info_local_var->ims_domain_name_list = ims_domain_name_list;
    dcsf_info_local_var->imsi_ranges = imsi_ranges;
    dcsf_info_local_var->ims_private_identity_ranges = ims_private_identity_ranges;
    dcsf_info_local_var->ims_public_identity_ranges = ims_public_identity_ranges;
    dcsf_info_local_var->msisdn_ranges = msisdn_ranges;

    return dcsf_info_local_var;
}

void OpenAPI_dcsf_info_free(OpenAPI_dcsf_info_t *dcsf_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == dcsf_info) {
        return;
    }
    if (dcsf_info->ims_domain_name_list) {
        OpenAPI_list_for_each(dcsf_info->ims_domain_name_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(dcsf_info->ims_domain_name_list);
        dcsf_info->ims_domain_name_list = NULL;
    }
    if (dcsf_info->imsi_ranges) {
        OpenAPI_list_for_each(dcsf_info->imsi_ranges, node) {
            OpenAPI_imsi_range_free(node->data);
        }
        OpenAPI_list_free(dcsf_info->imsi_ranges);
        dcsf_info->imsi_ranges = NULL;
    }
    if (dcsf_info->ims_private_identity_ranges) {
        OpenAPI_list_for_each(dcsf_info->ims_private_identity_ranges, node) {
            OpenAPI_identity_range_free(node->data);
        }
        OpenAPI_list_free(dcsf_info->ims_private_identity_ranges);
        dcsf_info->ims_private_identity_ranges = NULL;
    }
    if (dcsf_info->ims_public_identity_ranges) {
        OpenAPI_list_for_each(dcsf_info->ims_public_identity_ranges, node) {
            OpenAPI_identity_range_free(node->data);
        }
        OpenAPI_list_free(dcsf_info->ims_public_identity_ranges);
        dcsf_info->ims_public_identity_ranges = NULL;
    }
    if (dcsf_info->msisdn_ranges) {
        OpenAPI_list_for_each(dcsf_info->msisdn_ranges, node) {
            OpenAPI_identity_range_free(node->data);
        }
        OpenAPI_list_free(dcsf_info->msisdn_ranges);
        dcsf_info->msisdn_ranges = NULL;
    }
    ogs_free(dcsf_info);
}

cJSON *OpenAPI_dcsf_info_convertToJSON(OpenAPI_dcsf_info_t *dcsf_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (dcsf_info == NULL) {
        ogs_error("OpenAPI_dcsf_info_convertToJSON() failed [DcsfInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (dcsf_info->ims_domain_name_list) {
    cJSON *ims_domain_name_listList = cJSON_AddArrayToObject(item, "imsDomainNameList");
    if (ims_domain_name_listList == NULL) {
        ogs_error("OpenAPI_dcsf_info_convertToJSON() failed [ims_domain_name_list]");
        goto end;
    }
    OpenAPI_list_for_each(dcsf_info->ims_domain_name_list, node) {
        if (cJSON_AddStringToObject(ims_domain_name_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_dcsf_info_convertToJSON() failed [ims_domain_name_list]");
            goto end;
        }
    }
    }

    if (dcsf_info->imsi_ranges) {
    cJSON *imsi_rangesList = cJSON_AddArrayToObject(item, "imsiRanges");
    if (imsi_rangesList == NULL) {
        ogs_error("OpenAPI_dcsf_info_convertToJSON() failed [imsi_ranges]");
        goto end;
    }
    OpenAPI_list_for_each(dcsf_info->imsi_ranges, node) {
        cJSON *itemLocal = OpenAPI_imsi_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_dcsf_info_convertToJSON() failed [imsi_ranges]");
            goto end;
        }
        cJSON_AddItemToArray(imsi_rangesList, itemLocal);
    }
    }

    if (dcsf_info->ims_private_identity_ranges) {
    cJSON *ims_private_identity_rangesList = cJSON_AddArrayToObject(item, "imsPrivateIdentityRanges");
    if (ims_private_identity_rangesList == NULL) {
        ogs_error("OpenAPI_dcsf_info_convertToJSON() failed [ims_private_identity_ranges]");
        goto end;
    }
    OpenAPI_list_for_each(dcsf_info->ims_private_identity_ranges, node) {
        cJSON *itemLocal = OpenAPI_identity_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_dcsf_info_convertToJSON() failed [ims_private_identity_ranges]");
            goto end;
        }
        cJSON_AddItemToArray(ims_private_identity_rangesList, itemLocal);
    }
    }

    if (dcsf_info->ims_public_identity_ranges) {
    cJSON *ims_public_identity_rangesList = cJSON_AddArrayToObject(item, "imsPublicIdentityRanges");
    if (ims_public_identity_rangesList == NULL) {
        ogs_error("OpenAPI_dcsf_info_convertToJSON() failed [ims_public_identity_ranges]");
        goto end;
    }
    OpenAPI_list_for_each(dcsf_info->ims_public_identity_ranges, node) {
        cJSON *itemLocal = OpenAPI_identity_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_dcsf_info_convertToJSON() failed [ims_public_identity_ranges]");
            goto end;
        }
        cJSON_AddItemToArray(ims_public_identity_rangesList, itemLocal);
    }
    }

    if (dcsf_info->msisdn_ranges) {
    cJSON *msisdn_rangesList = cJSON_AddArrayToObject(item, "msisdnRanges");
    if (msisdn_rangesList == NULL) {
        ogs_error("OpenAPI_dcsf_info_convertToJSON() failed [msisdn_ranges]");
        goto end;
    }
    OpenAPI_list_for_each(dcsf_info->msisdn_ranges, node) {
        cJSON *itemLocal = OpenAPI_identity_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_dcsf_info_convertToJSON() failed [msisdn_ranges]");
            goto end;
        }
        cJSON_AddItemToArray(msisdn_rangesList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_dcsf_info_t *OpenAPI_dcsf_info_parseFromJSON(cJSON *dcsf_infoJSON)
{
    OpenAPI_dcsf_info_t *dcsf_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ims_domain_name_list = NULL;
    OpenAPI_list_t *ims_domain_name_listList = NULL;
    cJSON *imsi_ranges = NULL;
    OpenAPI_list_t *imsi_rangesList = NULL;
    cJSON *ims_private_identity_ranges = NULL;
    OpenAPI_list_t *ims_private_identity_rangesList = NULL;
    cJSON *ims_public_identity_ranges = NULL;
    OpenAPI_list_t *ims_public_identity_rangesList = NULL;
    cJSON *msisdn_ranges = NULL;
    OpenAPI_list_t *msisdn_rangesList = NULL;
    ims_domain_name_list = cJSON_GetObjectItemCaseSensitive(dcsf_infoJSON, "imsDomainNameList");
    if (ims_domain_name_list) {
        cJSON *ims_domain_name_list_local = NULL;
        if (!cJSON_IsArray(ims_domain_name_list)) {
            ogs_error("OpenAPI_dcsf_info_parseFromJSON() failed [ims_domain_name_list]");
            goto end;
        }

        ims_domain_name_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(ims_domain_name_list_local, ims_domain_name_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ims_domain_name_list_local)) {
                ogs_error("OpenAPI_dcsf_info_parseFromJSON() failed [ims_domain_name_list]");
                goto end;
            }
            OpenAPI_list_add(ims_domain_name_listList, ogs_strdup(ims_domain_name_list_local->valuestring));
        }
    }

    imsi_ranges = cJSON_GetObjectItemCaseSensitive(dcsf_infoJSON, "imsiRanges");
    if (imsi_ranges) {
        cJSON *imsi_ranges_local = NULL;
        if (!cJSON_IsArray(imsi_ranges)) {
            ogs_error("OpenAPI_dcsf_info_parseFromJSON() failed [imsi_ranges]");
            goto end;
        }

        imsi_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(imsi_ranges_local, imsi_ranges) {
            if (!cJSON_IsObject(imsi_ranges_local)) {
                ogs_error("OpenAPI_dcsf_info_parseFromJSON() failed [imsi_ranges]");
                goto end;
            }
            OpenAPI_imsi_range_t *imsi_rangesItem = OpenAPI_imsi_range_parseFromJSON(imsi_ranges_local);
            if (!imsi_rangesItem) {
                ogs_error("No imsi_rangesItem");
                goto end;
            }
            OpenAPI_list_add(imsi_rangesList, imsi_rangesItem);
        }
    }

    ims_private_identity_ranges = cJSON_GetObjectItemCaseSensitive(dcsf_infoJSON, "imsPrivateIdentityRanges");
    if (ims_private_identity_ranges) {
        cJSON *ims_private_identity_ranges_local = NULL;
        if (!cJSON_IsArray(ims_private_identity_ranges)) {
            ogs_error("OpenAPI_dcsf_info_parseFromJSON() failed [ims_private_identity_ranges]");
            goto end;
        }

        ims_private_identity_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(ims_private_identity_ranges_local, ims_private_identity_ranges) {
            if (!cJSON_IsObject(ims_private_identity_ranges_local)) {
                ogs_error("OpenAPI_dcsf_info_parseFromJSON() failed [ims_private_identity_ranges]");
                goto end;
            }
            OpenAPI_identity_range_t *ims_private_identity_rangesItem = OpenAPI_identity_range_parseFromJSON(ims_private_identity_ranges_local);
            if (!ims_private_identity_rangesItem) {
                ogs_error("No ims_private_identity_rangesItem");
                goto end;
            }
            OpenAPI_list_add(ims_private_identity_rangesList, ims_private_identity_rangesItem);
        }
    }

    ims_public_identity_ranges = cJSON_GetObjectItemCaseSensitive(dcsf_infoJSON, "imsPublicIdentityRanges");
    if (ims_public_identity_ranges) {
        cJSON *ims_public_identity_ranges_local = NULL;
        if (!cJSON_IsArray(ims_public_identity_ranges)) {
            ogs_error("OpenAPI_dcsf_info_parseFromJSON() failed [ims_public_identity_ranges]");
            goto end;
        }

        ims_public_identity_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(ims_public_identity_ranges_local, ims_public_identity_ranges) {
            if (!cJSON_IsObject(ims_public_identity_ranges_local)) {
                ogs_error("OpenAPI_dcsf_info_parseFromJSON() failed [ims_public_identity_ranges]");
                goto end;
            }
            OpenAPI_identity_range_t *ims_public_identity_rangesItem = OpenAPI_identity_range_parseFromJSON(ims_public_identity_ranges_local);
            if (!ims_public_identity_rangesItem) {
                ogs_error("No ims_public_identity_rangesItem");
                goto end;
            }
            OpenAPI_list_add(ims_public_identity_rangesList, ims_public_identity_rangesItem);
        }
    }

    msisdn_ranges = cJSON_GetObjectItemCaseSensitive(dcsf_infoJSON, "msisdnRanges");
    if (msisdn_ranges) {
        cJSON *msisdn_ranges_local = NULL;
        if (!cJSON_IsArray(msisdn_ranges)) {
            ogs_error("OpenAPI_dcsf_info_parseFromJSON() failed [msisdn_ranges]");
            goto end;
        }

        msisdn_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(msisdn_ranges_local, msisdn_ranges) {
            if (!cJSON_IsObject(msisdn_ranges_local)) {
                ogs_error("OpenAPI_dcsf_info_parseFromJSON() failed [msisdn_ranges]");
                goto end;
            }
            OpenAPI_identity_range_t *msisdn_rangesItem = OpenAPI_identity_range_parseFromJSON(msisdn_ranges_local);
            if (!msisdn_rangesItem) {
                ogs_error("No msisdn_rangesItem");
                goto end;
            }
            OpenAPI_list_add(msisdn_rangesList, msisdn_rangesItem);
        }
    }

    dcsf_info_local_var = OpenAPI_dcsf_info_create (
        ims_domain_name_list ? ims_domain_name_listList : NULL,
        imsi_ranges ? imsi_rangesList : NULL,
        ims_private_identity_ranges ? ims_private_identity_rangesList : NULL,
        ims_public_identity_ranges ? ims_public_identity_rangesList : NULL,
        msisdn_ranges ? msisdn_rangesList : NULL
    );

    return dcsf_info_local_var;
end:
    if (ims_domain_name_listList) {
        OpenAPI_list_for_each(ims_domain_name_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ims_domain_name_listList);
        ims_domain_name_listList = NULL;
    }
    if (imsi_rangesList) {
        OpenAPI_list_for_each(imsi_rangesList, node) {
            OpenAPI_imsi_range_free(node->data);
        }
        OpenAPI_list_free(imsi_rangesList);
        imsi_rangesList = NULL;
    }
    if (ims_private_identity_rangesList) {
        OpenAPI_list_for_each(ims_private_identity_rangesList, node) {
            OpenAPI_identity_range_free(node->data);
        }
        OpenAPI_list_free(ims_private_identity_rangesList);
        ims_private_identity_rangesList = NULL;
    }
    if (ims_public_identity_rangesList) {
        OpenAPI_list_for_each(ims_public_identity_rangesList, node) {
            OpenAPI_identity_range_free(node->data);
        }
        OpenAPI_list_free(ims_public_identity_rangesList);
        ims_public_identity_rangesList = NULL;
    }
    if (msisdn_rangesList) {
        OpenAPI_list_for_each(msisdn_rangesList, node) {
            OpenAPI_identity_range_free(node->data);
        }
        OpenAPI_list_free(msisdn_rangesList);
        msisdn_rangesList = NULL;
    }
    return NULL;
}

OpenAPI_dcsf_info_t *OpenAPI_dcsf_info_copy(OpenAPI_dcsf_info_t *dst, OpenAPI_dcsf_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_dcsf_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_dcsf_info_convertToJSON() failed");
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

    OpenAPI_dcsf_info_free(dst);
    dst = OpenAPI_dcsf_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

