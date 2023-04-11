
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "iwmsc_info.h"

OpenAPI_iwmsc_info_t *OpenAPI_iwmsc_info_create(
    OpenAPI_list_t *msisdn_ranges,
    OpenAPI_list_t *supi_ranges,
    OpenAPI_list_t *tai_range_list,
    char *sc_number
)
{
    OpenAPI_iwmsc_info_t *iwmsc_info_local_var = ogs_malloc(sizeof(OpenAPI_iwmsc_info_t));
    ogs_assert(iwmsc_info_local_var);

    iwmsc_info_local_var->msisdn_ranges = msisdn_ranges;
    iwmsc_info_local_var->supi_ranges = supi_ranges;
    iwmsc_info_local_var->tai_range_list = tai_range_list;
    iwmsc_info_local_var->sc_number = sc_number;

    return iwmsc_info_local_var;
}

void OpenAPI_iwmsc_info_free(OpenAPI_iwmsc_info_t *iwmsc_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == iwmsc_info) {
        return;
    }
    if (iwmsc_info->msisdn_ranges) {
        OpenAPI_list_for_each(iwmsc_info->msisdn_ranges, node) {
            OpenAPI_identity_range_free(node->data);
        }
        OpenAPI_list_free(iwmsc_info->msisdn_ranges);
        iwmsc_info->msisdn_ranges = NULL;
    }
    if (iwmsc_info->supi_ranges) {
        OpenAPI_list_for_each(iwmsc_info->supi_ranges, node) {
            OpenAPI_supi_range_free(node->data);
        }
        OpenAPI_list_free(iwmsc_info->supi_ranges);
        iwmsc_info->supi_ranges = NULL;
    }
    if (iwmsc_info->tai_range_list) {
        OpenAPI_list_for_each(iwmsc_info->tai_range_list, node) {
            OpenAPI_tai_range_free(node->data);
        }
        OpenAPI_list_free(iwmsc_info->tai_range_list);
        iwmsc_info->tai_range_list = NULL;
    }
    if (iwmsc_info->sc_number) {
        ogs_free(iwmsc_info->sc_number);
        iwmsc_info->sc_number = NULL;
    }
    ogs_free(iwmsc_info);
}

cJSON *OpenAPI_iwmsc_info_convertToJSON(OpenAPI_iwmsc_info_t *iwmsc_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (iwmsc_info == NULL) {
        ogs_error("OpenAPI_iwmsc_info_convertToJSON() failed [IwmscInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (iwmsc_info->msisdn_ranges) {
    cJSON *msisdn_rangesList = cJSON_AddArrayToObject(item, "msisdnRanges");
    if (msisdn_rangesList == NULL) {
        ogs_error("OpenAPI_iwmsc_info_convertToJSON() failed [msisdn_ranges]");
        goto end;
    }
    OpenAPI_list_for_each(iwmsc_info->msisdn_ranges, node) {
        cJSON *itemLocal = OpenAPI_identity_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_iwmsc_info_convertToJSON() failed [msisdn_ranges]");
            goto end;
        }
        cJSON_AddItemToArray(msisdn_rangesList, itemLocal);
    }
    }

    if (iwmsc_info->supi_ranges) {
    cJSON *supi_rangesList = cJSON_AddArrayToObject(item, "supiRanges");
    if (supi_rangesList == NULL) {
        ogs_error("OpenAPI_iwmsc_info_convertToJSON() failed [supi_ranges]");
        goto end;
    }
    OpenAPI_list_for_each(iwmsc_info->supi_ranges, node) {
        cJSON *itemLocal = OpenAPI_supi_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_iwmsc_info_convertToJSON() failed [supi_ranges]");
            goto end;
        }
        cJSON_AddItemToArray(supi_rangesList, itemLocal);
    }
    }

    if (iwmsc_info->tai_range_list) {
    cJSON *tai_range_listList = cJSON_AddArrayToObject(item, "taiRangeList");
    if (tai_range_listList == NULL) {
        ogs_error("OpenAPI_iwmsc_info_convertToJSON() failed [tai_range_list]");
        goto end;
    }
    OpenAPI_list_for_each(iwmsc_info->tai_range_list, node) {
        cJSON *itemLocal = OpenAPI_tai_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_iwmsc_info_convertToJSON() failed [tai_range_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_range_listList, itemLocal);
    }
    }

    if (iwmsc_info->sc_number) {
    if (cJSON_AddStringToObject(item, "scNumber", iwmsc_info->sc_number) == NULL) {
        ogs_error("OpenAPI_iwmsc_info_convertToJSON() failed [sc_number]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_iwmsc_info_t *OpenAPI_iwmsc_info_parseFromJSON(cJSON *iwmsc_infoJSON)
{
    OpenAPI_iwmsc_info_t *iwmsc_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *msisdn_ranges = NULL;
    OpenAPI_list_t *msisdn_rangesList = NULL;
    cJSON *supi_ranges = NULL;
    OpenAPI_list_t *supi_rangesList = NULL;
    cJSON *tai_range_list = NULL;
    OpenAPI_list_t *tai_range_listList = NULL;
    cJSON *sc_number = NULL;
    msisdn_ranges = cJSON_GetObjectItemCaseSensitive(iwmsc_infoJSON, "msisdnRanges");
    if (msisdn_ranges) {
        cJSON *msisdn_ranges_local = NULL;
        if (!cJSON_IsArray(msisdn_ranges)) {
            ogs_error("OpenAPI_iwmsc_info_parseFromJSON() failed [msisdn_ranges]");
            goto end;
        }

        msisdn_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(msisdn_ranges_local, msisdn_ranges) {
            if (!cJSON_IsObject(msisdn_ranges_local)) {
                ogs_error("OpenAPI_iwmsc_info_parseFromJSON() failed [msisdn_ranges]");
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

    supi_ranges = cJSON_GetObjectItemCaseSensitive(iwmsc_infoJSON, "supiRanges");
    if (supi_ranges) {
        cJSON *supi_ranges_local = NULL;
        if (!cJSON_IsArray(supi_ranges)) {
            ogs_error("OpenAPI_iwmsc_info_parseFromJSON() failed [supi_ranges]");
            goto end;
        }

        supi_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(supi_ranges_local, supi_ranges) {
            if (!cJSON_IsObject(supi_ranges_local)) {
                ogs_error("OpenAPI_iwmsc_info_parseFromJSON() failed [supi_ranges]");
                goto end;
            }
            OpenAPI_supi_range_t *supi_rangesItem = OpenAPI_supi_range_parseFromJSON(supi_ranges_local);
            if (!supi_rangesItem) {
                ogs_error("No supi_rangesItem");
                goto end;
            }
            OpenAPI_list_add(supi_rangesList, supi_rangesItem);
        }
    }

    tai_range_list = cJSON_GetObjectItemCaseSensitive(iwmsc_infoJSON, "taiRangeList");
    if (tai_range_list) {
        cJSON *tai_range_list_local = NULL;
        if (!cJSON_IsArray(tai_range_list)) {
            ogs_error("OpenAPI_iwmsc_info_parseFromJSON() failed [tai_range_list]");
            goto end;
        }

        tai_range_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_range_list_local, tai_range_list) {
            if (!cJSON_IsObject(tai_range_list_local)) {
                ogs_error("OpenAPI_iwmsc_info_parseFromJSON() failed [tai_range_list]");
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

    sc_number = cJSON_GetObjectItemCaseSensitive(iwmsc_infoJSON, "scNumber");
    if (sc_number) {
    if (!cJSON_IsString(sc_number) && !cJSON_IsNull(sc_number)) {
        ogs_error("OpenAPI_iwmsc_info_parseFromJSON() failed [sc_number]");
        goto end;
    }
    }

    iwmsc_info_local_var = OpenAPI_iwmsc_info_create (
        msisdn_ranges ? msisdn_rangesList : NULL,
        supi_ranges ? supi_rangesList : NULL,
        tai_range_list ? tai_range_listList : NULL,
        sc_number && !cJSON_IsNull(sc_number) ? ogs_strdup(sc_number->valuestring) : NULL
    );

    return iwmsc_info_local_var;
end:
    if (msisdn_rangesList) {
        OpenAPI_list_for_each(msisdn_rangesList, node) {
            OpenAPI_identity_range_free(node->data);
        }
        OpenAPI_list_free(msisdn_rangesList);
        msisdn_rangesList = NULL;
    }
    if (supi_rangesList) {
        OpenAPI_list_for_each(supi_rangesList, node) {
            OpenAPI_supi_range_free(node->data);
        }
        OpenAPI_list_free(supi_rangesList);
        supi_rangesList = NULL;
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

OpenAPI_iwmsc_info_t *OpenAPI_iwmsc_info_copy(OpenAPI_iwmsc_info_t *dst, OpenAPI_iwmsc_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_iwmsc_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_iwmsc_info_convertToJSON() failed");
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

    OpenAPI_iwmsc_info_free(dst);
    dst = OpenAPI_iwmsc_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

