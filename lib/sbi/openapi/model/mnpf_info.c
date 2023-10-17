
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mnpf_info.h"

OpenAPI_mnpf_info_t *OpenAPI_mnpf_info_create(
    OpenAPI_list_t *msisdn_ranges
)
{
    OpenAPI_mnpf_info_t *mnpf_info_local_var = ogs_malloc(sizeof(OpenAPI_mnpf_info_t));
    ogs_assert(mnpf_info_local_var);

    mnpf_info_local_var->msisdn_ranges = msisdn_ranges;

    return mnpf_info_local_var;
}

void OpenAPI_mnpf_info_free(OpenAPI_mnpf_info_t *mnpf_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == mnpf_info) {
        return;
    }
    if (mnpf_info->msisdn_ranges) {
        OpenAPI_list_for_each(mnpf_info->msisdn_ranges, node) {
            OpenAPI_identity_range_free(node->data);
        }
        OpenAPI_list_free(mnpf_info->msisdn_ranges);
        mnpf_info->msisdn_ranges = NULL;
    }
    ogs_free(mnpf_info);
}

cJSON *OpenAPI_mnpf_info_convertToJSON(OpenAPI_mnpf_info_t *mnpf_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (mnpf_info == NULL) {
        ogs_error("OpenAPI_mnpf_info_convertToJSON() failed [MnpfInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!mnpf_info->msisdn_ranges) {
        ogs_error("OpenAPI_mnpf_info_convertToJSON() failed [msisdn_ranges]");
        return NULL;
    }
    cJSON *msisdn_rangesList = cJSON_AddArrayToObject(item, "msisdnRanges");
    if (msisdn_rangesList == NULL) {
        ogs_error("OpenAPI_mnpf_info_convertToJSON() failed [msisdn_ranges]");
        goto end;
    }
    OpenAPI_list_for_each(mnpf_info->msisdn_ranges, node) {
        cJSON *itemLocal = OpenAPI_identity_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_mnpf_info_convertToJSON() failed [msisdn_ranges]");
            goto end;
        }
        cJSON_AddItemToArray(msisdn_rangesList, itemLocal);
    }

end:
    return item;
}

OpenAPI_mnpf_info_t *OpenAPI_mnpf_info_parseFromJSON(cJSON *mnpf_infoJSON)
{
    OpenAPI_mnpf_info_t *mnpf_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *msisdn_ranges = NULL;
    OpenAPI_list_t *msisdn_rangesList = NULL;
    msisdn_ranges = cJSON_GetObjectItemCaseSensitive(mnpf_infoJSON, "msisdnRanges");
    if (!msisdn_ranges) {
        ogs_error("OpenAPI_mnpf_info_parseFromJSON() failed [msisdn_ranges]");
        goto end;
    }
        cJSON *msisdn_ranges_local = NULL;
        if (!cJSON_IsArray(msisdn_ranges)) {
            ogs_error("OpenAPI_mnpf_info_parseFromJSON() failed [msisdn_ranges]");
            goto end;
        }

        msisdn_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(msisdn_ranges_local, msisdn_ranges) {
            if (!cJSON_IsObject(msisdn_ranges_local)) {
                ogs_error("OpenAPI_mnpf_info_parseFromJSON() failed [msisdn_ranges]");
                goto end;
            }
            OpenAPI_identity_range_t *msisdn_rangesItem = OpenAPI_identity_range_parseFromJSON(msisdn_ranges_local);
            if (!msisdn_rangesItem) {
                ogs_error("No msisdn_rangesItem");
                goto end;
            }
            OpenAPI_list_add(msisdn_rangesList, msisdn_rangesItem);
        }

    mnpf_info_local_var = OpenAPI_mnpf_info_create (
        msisdn_rangesList
    );

    return mnpf_info_local_var;
end:
    if (msisdn_rangesList) {
        OpenAPI_list_for_each(msisdn_rangesList, node) {
            OpenAPI_identity_range_free(node->data);
        }
        OpenAPI_list_free(msisdn_rangesList);
        msisdn_rangesList = NULL;
    }
    return NULL;
}

OpenAPI_mnpf_info_t *OpenAPI_mnpf_info_copy(OpenAPI_mnpf_info_t *dst, OpenAPI_mnpf_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_mnpf_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_mnpf_info_convertToJSON() failed");
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

    OpenAPI_mnpf_info_free(dst);
    dst = OpenAPI_mnpf_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

