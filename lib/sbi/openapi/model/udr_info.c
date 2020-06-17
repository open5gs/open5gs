
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "udr_info.h"

OpenAPI_udr_info_t *OpenAPI_udr_info_create(
    char *group_id,
    OpenAPI_list_t *supi_ranges,
    OpenAPI_list_t *gpsi_ranges,
    OpenAPI_list_t *external_group_identifiers_ranges,
    OpenAPI_list_t *supported_data_sets
    )
{
    OpenAPI_udr_info_t *udr_info_local_var = OpenAPI_malloc(sizeof(OpenAPI_udr_info_t));
    if (!udr_info_local_var) {
        return NULL;
    }
    udr_info_local_var->group_id = group_id;
    udr_info_local_var->supi_ranges = supi_ranges;
    udr_info_local_var->gpsi_ranges = gpsi_ranges;
    udr_info_local_var->external_group_identifiers_ranges = external_group_identifiers_ranges;
    udr_info_local_var->supported_data_sets = supported_data_sets;

    return udr_info_local_var;
}

void OpenAPI_udr_info_free(OpenAPI_udr_info_t *udr_info)
{
    if (NULL == udr_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(udr_info->group_id);
    OpenAPI_list_for_each(udr_info->supi_ranges, node) {
        OpenAPI_supi_range_free(node->data);
    }
    OpenAPI_list_free(udr_info->supi_ranges);
    OpenAPI_list_for_each(udr_info->gpsi_ranges, node) {
        OpenAPI_identity_range_free(node->data);
    }
    OpenAPI_list_free(udr_info->gpsi_ranges);
    OpenAPI_list_for_each(udr_info->external_group_identifiers_ranges, node) {
        OpenAPI_identity_range_free(node->data);
    }
    OpenAPI_list_free(udr_info->external_group_identifiers_ranges);
    OpenAPI_list_for_each(udr_info->supported_data_sets, node) {
        OpenAPI_data_set_id_free(node->data);
    }
    OpenAPI_list_free(udr_info->supported_data_sets);
    ogs_free(udr_info);
}

cJSON *OpenAPI_udr_info_convertToJSON(OpenAPI_udr_info_t *udr_info)
{
    cJSON *item = NULL;

    if (udr_info == NULL) {
        ogs_error("OpenAPI_udr_info_convertToJSON() failed [UdrInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (udr_info->group_id) {
        if (cJSON_AddStringToObject(item, "groupId", udr_info->group_id) == NULL) {
            ogs_error("OpenAPI_udr_info_convertToJSON() failed [group_id]");
            goto end;
        }
    }

    if (udr_info->supi_ranges) {
        cJSON *supi_rangesList = cJSON_AddArrayToObject(item, "supiRanges");
        if (supi_rangesList == NULL) {
            ogs_error("OpenAPI_udr_info_convertToJSON() failed [supi_ranges]");
            goto end;
        }

        OpenAPI_lnode_t *supi_ranges_node;
        if (udr_info->supi_ranges) {
            OpenAPI_list_for_each(udr_info->supi_ranges, supi_ranges_node) {
                cJSON *itemLocal = OpenAPI_supi_range_convertToJSON(supi_ranges_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_udr_info_convertToJSON() failed [supi_ranges]");
                    goto end;
                }
                cJSON_AddItemToArray(supi_rangesList, itemLocal);
            }
        }
    }

    if (udr_info->gpsi_ranges) {
        cJSON *gpsi_rangesList = cJSON_AddArrayToObject(item, "gpsiRanges");
        if (gpsi_rangesList == NULL) {
            ogs_error("OpenAPI_udr_info_convertToJSON() failed [gpsi_ranges]");
            goto end;
        }

        OpenAPI_lnode_t *gpsi_ranges_node;
        if (udr_info->gpsi_ranges) {
            OpenAPI_list_for_each(udr_info->gpsi_ranges, gpsi_ranges_node) {
                cJSON *itemLocal = OpenAPI_identity_range_convertToJSON(gpsi_ranges_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_udr_info_convertToJSON() failed [gpsi_ranges]");
                    goto end;
                }
                cJSON_AddItemToArray(gpsi_rangesList, itemLocal);
            }
        }
    }

    if (udr_info->external_group_identifiers_ranges) {
        cJSON *external_group_identifiers_rangesList = cJSON_AddArrayToObject(item, "externalGroupIdentifiersRanges");
        if (external_group_identifiers_rangesList == NULL) {
            ogs_error("OpenAPI_udr_info_convertToJSON() failed [external_group_identifiers_ranges]");
            goto end;
        }

        OpenAPI_lnode_t *external_group_identifiers_ranges_node;
        if (udr_info->external_group_identifiers_ranges) {
            OpenAPI_list_for_each(udr_info->external_group_identifiers_ranges, external_group_identifiers_ranges_node) {
                cJSON *itemLocal = OpenAPI_identity_range_convertToJSON(external_group_identifiers_ranges_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_udr_info_convertToJSON() failed [external_group_identifiers_ranges]");
                    goto end;
                }
                cJSON_AddItemToArray(external_group_identifiers_rangesList, itemLocal);
            }
        }
    }

    if (udr_info->supported_data_sets) {
        cJSON *supported_data_setsList = cJSON_AddArrayToObject(item, "supportedDataSets");
        if (supported_data_setsList == NULL) {
            ogs_error("OpenAPI_udr_info_convertToJSON() failed [supported_data_sets]");
            goto end;
        }

        OpenAPI_lnode_t *supported_data_sets_node;
        if (udr_info->supported_data_sets) {
            OpenAPI_list_for_each(udr_info->supported_data_sets, supported_data_sets_node) {
                cJSON *itemLocal = OpenAPI_data_set_id_convertToJSON(supported_data_sets_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_udr_info_convertToJSON() failed [supported_data_sets]");
                    goto end;
                }
                cJSON_AddItemToArray(supported_data_setsList, itemLocal);
            }
        }
    }

end:
    return item;
}

OpenAPI_udr_info_t *OpenAPI_udr_info_parseFromJSON(cJSON *udr_infoJSON)
{
    OpenAPI_udr_info_t *udr_info_local_var = NULL;
    cJSON *group_id = cJSON_GetObjectItemCaseSensitive(udr_infoJSON, "groupId");

    if (group_id) {
        if (!cJSON_IsString(group_id)) {
            ogs_error("OpenAPI_udr_info_parseFromJSON() failed [group_id]");
            goto end;
        }
    }

    cJSON *supi_ranges = cJSON_GetObjectItemCaseSensitive(udr_infoJSON, "supiRanges");

    OpenAPI_list_t *supi_rangesList;
    if (supi_ranges) {
        cJSON *supi_ranges_local_nonprimitive;
        if (!cJSON_IsArray(supi_ranges)) {
            ogs_error("OpenAPI_udr_info_parseFromJSON() failed [supi_ranges]");
            goto end;
        }

        supi_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(supi_ranges_local_nonprimitive, supi_ranges ) {
            if (!cJSON_IsObject(supi_ranges_local_nonprimitive)) {
                ogs_error("OpenAPI_udr_info_parseFromJSON() failed [supi_ranges]");
                goto end;
            }
            OpenAPI_supi_range_t *supi_rangesItem = OpenAPI_supi_range_parseFromJSON(supi_ranges_local_nonprimitive);

            OpenAPI_list_add(supi_rangesList, supi_rangesItem);
        }
    }

    cJSON *gpsi_ranges = cJSON_GetObjectItemCaseSensitive(udr_infoJSON, "gpsiRanges");

    OpenAPI_list_t *gpsi_rangesList;
    if (gpsi_ranges) {
        cJSON *gpsi_ranges_local_nonprimitive;
        if (!cJSON_IsArray(gpsi_ranges)) {
            ogs_error("OpenAPI_udr_info_parseFromJSON() failed [gpsi_ranges]");
            goto end;
        }

        gpsi_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(gpsi_ranges_local_nonprimitive, gpsi_ranges ) {
            if (!cJSON_IsObject(gpsi_ranges_local_nonprimitive)) {
                ogs_error("OpenAPI_udr_info_parseFromJSON() failed [gpsi_ranges]");
                goto end;
            }
            OpenAPI_identity_range_t *gpsi_rangesItem = OpenAPI_identity_range_parseFromJSON(gpsi_ranges_local_nonprimitive);

            OpenAPI_list_add(gpsi_rangesList, gpsi_rangesItem);
        }
    }

    cJSON *external_group_identifiers_ranges = cJSON_GetObjectItemCaseSensitive(udr_infoJSON, "externalGroupIdentifiersRanges");

    OpenAPI_list_t *external_group_identifiers_rangesList;
    if (external_group_identifiers_ranges) {
        cJSON *external_group_identifiers_ranges_local_nonprimitive;
        if (!cJSON_IsArray(external_group_identifiers_ranges)) {
            ogs_error("OpenAPI_udr_info_parseFromJSON() failed [external_group_identifiers_ranges]");
            goto end;
        }

        external_group_identifiers_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(external_group_identifiers_ranges_local_nonprimitive, external_group_identifiers_ranges ) {
            if (!cJSON_IsObject(external_group_identifiers_ranges_local_nonprimitive)) {
                ogs_error("OpenAPI_udr_info_parseFromJSON() failed [external_group_identifiers_ranges]");
                goto end;
            }
            OpenAPI_identity_range_t *external_group_identifiers_rangesItem = OpenAPI_identity_range_parseFromJSON(external_group_identifiers_ranges_local_nonprimitive);

            OpenAPI_list_add(external_group_identifiers_rangesList, external_group_identifiers_rangesItem);
        }
    }

    cJSON *supported_data_sets = cJSON_GetObjectItemCaseSensitive(udr_infoJSON, "supportedDataSets");

    OpenAPI_list_t *supported_data_setsList;
    if (supported_data_sets) {
        cJSON *supported_data_sets_local_nonprimitive;
        if (!cJSON_IsArray(supported_data_sets)) {
            ogs_error("OpenAPI_udr_info_parseFromJSON() failed [supported_data_sets]");
            goto end;
        }

        supported_data_setsList = OpenAPI_list_create();

        cJSON_ArrayForEach(supported_data_sets_local_nonprimitive, supported_data_sets ) {
            if (!cJSON_IsObject(supported_data_sets_local_nonprimitive)) {
                ogs_error("OpenAPI_udr_info_parseFromJSON() failed [supported_data_sets]");
                goto end;
            }
            OpenAPI_data_set_id_t *supported_data_setsItem = OpenAPI_data_set_id_parseFromJSON(supported_data_sets_local_nonprimitive);

            OpenAPI_list_add(supported_data_setsList, supported_data_setsItem);
        }
    }

    udr_info_local_var = OpenAPI_udr_info_create (
        group_id ? ogs_strdup(group_id->valuestring) : NULL,
        supi_ranges ? supi_rangesList : NULL,
        gpsi_ranges ? gpsi_rangesList : NULL,
        external_group_identifiers_ranges ? external_group_identifiers_rangesList : NULL,
        supported_data_sets ? supported_data_setsList : NULL
        );

    return udr_info_local_var;
end:
    return NULL;
}

OpenAPI_udr_info_t *OpenAPI_udr_info_copy(OpenAPI_udr_info_t *dst, OpenAPI_udr_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_udr_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_udr_info_convertToJSON() failed");
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

    OpenAPI_udr_info_free(dst);
    dst = OpenAPI_udr_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

