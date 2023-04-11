
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nssaaf_info.h"

OpenAPI_nssaaf_info_t *OpenAPI_nssaaf_info_create(
    OpenAPI_list_t *supi_ranges,
    OpenAPI_list_t *internal_group_identifiers_ranges
)
{
    OpenAPI_nssaaf_info_t *nssaaf_info_local_var = ogs_malloc(sizeof(OpenAPI_nssaaf_info_t));
    ogs_assert(nssaaf_info_local_var);

    nssaaf_info_local_var->supi_ranges = supi_ranges;
    nssaaf_info_local_var->internal_group_identifiers_ranges = internal_group_identifiers_ranges;

    return nssaaf_info_local_var;
}

void OpenAPI_nssaaf_info_free(OpenAPI_nssaaf_info_t *nssaaf_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nssaaf_info) {
        return;
    }
    if (nssaaf_info->supi_ranges) {
        OpenAPI_list_for_each(nssaaf_info->supi_ranges, node) {
            OpenAPI_supi_range_free(node->data);
        }
        OpenAPI_list_free(nssaaf_info->supi_ranges);
        nssaaf_info->supi_ranges = NULL;
    }
    if (nssaaf_info->internal_group_identifiers_ranges) {
        OpenAPI_list_for_each(nssaaf_info->internal_group_identifiers_ranges, node) {
            OpenAPI_internal_group_id_range_free(node->data);
        }
        OpenAPI_list_free(nssaaf_info->internal_group_identifiers_ranges);
        nssaaf_info->internal_group_identifiers_ranges = NULL;
    }
    ogs_free(nssaaf_info);
}

cJSON *OpenAPI_nssaaf_info_convertToJSON(OpenAPI_nssaaf_info_t *nssaaf_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nssaaf_info == NULL) {
        ogs_error("OpenAPI_nssaaf_info_convertToJSON() failed [NssaafInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (nssaaf_info->supi_ranges) {
    cJSON *supi_rangesList = cJSON_AddArrayToObject(item, "supiRanges");
    if (supi_rangesList == NULL) {
        ogs_error("OpenAPI_nssaaf_info_convertToJSON() failed [supi_ranges]");
        goto end;
    }
    OpenAPI_list_for_each(nssaaf_info->supi_ranges, node) {
        cJSON *itemLocal = OpenAPI_supi_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nssaaf_info_convertToJSON() failed [supi_ranges]");
            goto end;
        }
        cJSON_AddItemToArray(supi_rangesList, itemLocal);
    }
    }

    if (nssaaf_info->internal_group_identifiers_ranges) {
    cJSON *internal_group_identifiers_rangesList = cJSON_AddArrayToObject(item, "internalGroupIdentifiersRanges");
    if (internal_group_identifiers_rangesList == NULL) {
        ogs_error("OpenAPI_nssaaf_info_convertToJSON() failed [internal_group_identifiers_ranges]");
        goto end;
    }
    OpenAPI_list_for_each(nssaaf_info->internal_group_identifiers_ranges, node) {
        cJSON *itemLocal = OpenAPI_internal_group_id_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nssaaf_info_convertToJSON() failed [internal_group_identifiers_ranges]");
            goto end;
        }
        cJSON_AddItemToArray(internal_group_identifiers_rangesList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_nssaaf_info_t *OpenAPI_nssaaf_info_parseFromJSON(cJSON *nssaaf_infoJSON)
{
    OpenAPI_nssaaf_info_t *nssaaf_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *supi_ranges = NULL;
    OpenAPI_list_t *supi_rangesList = NULL;
    cJSON *internal_group_identifiers_ranges = NULL;
    OpenAPI_list_t *internal_group_identifiers_rangesList = NULL;
    supi_ranges = cJSON_GetObjectItemCaseSensitive(nssaaf_infoJSON, "supiRanges");
    if (supi_ranges) {
        cJSON *supi_ranges_local = NULL;
        if (!cJSON_IsArray(supi_ranges)) {
            ogs_error("OpenAPI_nssaaf_info_parseFromJSON() failed [supi_ranges]");
            goto end;
        }

        supi_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(supi_ranges_local, supi_ranges) {
            if (!cJSON_IsObject(supi_ranges_local)) {
                ogs_error("OpenAPI_nssaaf_info_parseFromJSON() failed [supi_ranges]");
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

    internal_group_identifiers_ranges = cJSON_GetObjectItemCaseSensitive(nssaaf_infoJSON, "internalGroupIdentifiersRanges");
    if (internal_group_identifiers_ranges) {
        cJSON *internal_group_identifiers_ranges_local = NULL;
        if (!cJSON_IsArray(internal_group_identifiers_ranges)) {
            ogs_error("OpenAPI_nssaaf_info_parseFromJSON() failed [internal_group_identifiers_ranges]");
            goto end;
        }

        internal_group_identifiers_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(internal_group_identifiers_ranges_local, internal_group_identifiers_ranges) {
            if (!cJSON_IsObject(internal_group_identifiers_ranges_local)) {
                ogs_error("OpenAPI_nssaaf_info_parseFromJSON() failed [internal_group_identifiers_ranges]");
                goto end;
            }
            OpenAPI_internal_group_id_range_t *internal_group_identifiers_rangesItem = OpenAPI_internal_group_id_range_parseFromJSON(internal_group_identifiers_ranges_local);
            if (!internal_group_identifiers_rangesItem) {
                ogs_error("No internal_group_identifiers_rangesItem");
                goto end;
            }
            OpenAPI_list_add(internal_group_identifiers_rangesList, internal_group_identifiers_rangesItem);
        }
    }

    nssaaf_info_local_var = OpenAPI_nssaaf_info_create (
        supi_ranges ? supi_rangesList : NULL,
        internal_group_identifiers_ranges ? internal_group_identifiers_rangesList : NULL
    );

    return nssaaf_info_local_var;
end:
    if (supi_rangesList) {
        OpenAPI_list_for_each(supi_rangesList, node) {
            OpenAPI_supi_range_free(node->data);
        }
        OpenAPI_list_free(supi_rangesList);
        supi_rangesList = NULL;
    }
    if (internal_group_identifiers_rangesList) {
        OpenAPI_list_for_each(internal_group_identifiers_rangesList, node) {
            OpenAPI_internal_group_id_range_free(node->data);
        }
        OpenAPI_list_free(internal_group_identifiers_rangesList);
        internal_group_identifiers_rangesList = NULL;
    }
    return NULL;
}

OpenAPI_nssaaf_info_t *OpenAPI_nssaaf_info_copy(OpenAPI_nssaaf_info_t *dst, OpenAPI_nssaaf_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nssaaf_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nssaaf_info_convertToJSON() failed");
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

    OpenAPI_nssaaf_info_free(dst);
    dst = OpenAPI_nssaaf_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

