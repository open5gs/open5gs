
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "udr_info.h"

OpenAPI_udr_info_t *OpenAPI_udr_info_create(
    char *group_id,
    OpenAPI_list_t *supi_ranges,
    OpenAPI_list_t *gpsi_ranges,
    OpenAPI_list_t *external_group_identifiers_ranges,
    OpenAPI_list_t *supported_data_sets,
    OpenAPI_list_t *shared_data_id_ranges
)
{
    OpenAPI_udr_info_t *udr_info_local_var = ogs_malloc(sizeof(OpenAPI_udr_info_t));
    ogs_assert(udr_info_local_var);

    udr_info_local_var->group_id = group_id;
    udr_info_local_var->supi_ranges = supi_ranges;
    udr_info_local_var->gpsi_ranges = gpsi_ranges;
    udr_info_local_var->external_group_identifiers_ranges = external_group_identifiers_ranges;
    udr_info_local_var->supported_data_sets = supported_data_sets;
    udr_info_local_var->shared_data_id_ranges = shared_data_id_ranges;

    return udr_info_local_var;
}

void OpenAPI_udr_info_free(OpenAPI_udr_info_t *udr_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == udr_info) {
        return;
    }
    if (udr_info->group_id) {
        ogs_free(udr_info->group_id);
        udr_info->group_id = NULL;
    }
    if (udr_info->supi_ranges) {
        OpenAPI_list_for_each(udr_info->supi_ranges, node) {
            OpenAPI_supi_range_free(node->data);
        }
        OpenAPI_list_free(udr_info->supi_ranges);
        udr_info->supi_ranges = NULL;
    }
    if (udr_info->gpsi_ranges) {
        OpenAPI_list_for_each(udr_info->gpsi_ranges, node) {
            OpenAPI_identity_range_free(node->data);
        }
        OpenAPI_list_free(udr_info->gpsi_ranges);
        udr_info->gpsi_ranges = NULL;
    }
    if (udr_info->external_group_identifiers_ranges) {
        OpenAPI_list_for_each(udr_info->external_group_identifiers_ranges, node) {
            OpenAPI_identity_range_free(node->data);
        }
        OpenAPI_list_free(udr_info->external_group_identifiers_ranges);
        udr_info->external_group_identifiers_ranges = NULL;
    }
    if (udr_info->supported_data_sets) {
        OpenAPI_list_free(udr_info->supported_data_sets);
        udr_info->supported_data_sets = NULL;
    }
    if (udr_info->shared_data_id_ranges) {
        OpenAPI_list_for_each(udr_info->shared_data_id_ranges, node) {
            OpenAPI_shared_data_id_range_free(node->data);
        }
        OpenAPI_list_free(udr_info->shared_data_id_ranges);
        udr_info->shared_data_id_ranges = NULL;
    }
    ogs_free(udr_info);
}

cJSON *OpenAPI_udr_info_convertToJSON(OpenAPI_udr_info_t *udr_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

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
    OpenAPI_list_for_each(udr_info->supi_ranges, node) {
        cJSON *itemLocal = OpenAPI_supi_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_udr_info_convertToJSON() failed [supi_ranges]");
            goto end;
        }
        cJSON_AddItemToArray(supi_rangesList, itemLocal);
    }
    }

    if (udr_info->gpsi_ranges) {
    cJSON *gpsi_rangesList = cJSON_AddArrayToObject(item, "gpsiRanges");
    if (gpsi_rangesList == NULL) {
        ogs_error("OpenAPI_udr_info_convertToJSON() failed [gpsi_ranges]");
        goto end;
    }
    OpenAPI_list_for_each(udr_info->gpsi_ranges, node) {
        cJSON *itemLocal = OpenAPI_identity_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_udr_info_convertToJSON() failed [gpsi_ranges]");
            goto end;
        }
        cJSON_AddItemToArray(gpsi_rangesList, itemLocal);
    }
    }

    if (udr_info->external_group_identifiers_ranges) {
    cJSON *external_group_identifiers_rangesList = cJSON_AddArrayToObject(item, "externalGroupIdentifiersRanges");
    if (external_group_identifiers_rangesList == NULL) {
        ogs_error("OpenAPI_udr_info_convertToJSON() failed [external_group_identifiers_ranges]");
        goto end;
    }
    OpenAPI_list_for_each(udr_info->external_group_identifiers_ranges, node) {
        cJSON *itemLocal = OpenAPI_identity_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_udr_info_convertToJSON() failed [external_group_identifiers_ranges]");
            goto end;
        }
        cJSON_AddItemToArray(external_group_identifiers_rangesList, itemLocal);
    }
    }

    if (udr_info->supported_data_sets != OpenAPI_data_set_id_NULL) {
    cJSON *supported_data_setsList = cJSON_AddArrayToObject(item, "supportedDataSets");
    if (supported_data_setsList == NULL) {
        ogs_error("OpenAPI_udr_info_convertToJSON() failed [supported_data_sets]");
        goto end;
    }
    OpenAPI_list_for_each(udr_info->supported_data_sets, node) {
        if (cJSON_AddStringToObject(supported_data_setsList, "", OpenAPI_data_set_id_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_udr_info_convertToJSON() failed [supported_data_sets]");
            goto end;
        }
    }
    }

    if (udr_info->shared_data_id_ranges) {
    cJSON *shared_data_id_rangesList = cJSON_AddArrayToObject(item, "sharedDataIdRanges");
    if (shared_data_id_rangesList == NULL) {
        ogs_error("OpenAPI_udr_info_convertToJSON() failed [shared_data_id_ranges]");
        goto end;
    }
    OpenAPI_list_for_each(udr_info->shared_data_id_ranges, node) {
        cJSON *itemLocal = OpenAPI_shared_data_id_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_udr_info_convertToJSON() failed [shared_data_id_ranges]");
            goto end;
        }
        cJSON_AddItemToArray(shared_data_id_rangesList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_udr_info_t *OpenAPI_udr_info_parseFromJSON(cJSON *udr_infoJSON)
{
    OpenAPI_udr_info_t *udr_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *group_id = NULL;
    cJSON *supi_ranges = NULL;
    OpenAPI_list_t *supi_rangesList = NULL;
    cJSON *gpsi_ranges = NULL;
    OpenAPI_list_t *gpsi_rangesList = NULL;
    cJSON *external_group_identifiers_ranges = NULL;
    OpenAPI_list_t *external_group_identifiers_rangesList = NULL;
    cJSON *supported_data_sets = NULL;
    OpenAPI_list_t *supported_data_setsList = NULL;
    cJSON *shared_data_id_ranges = NULL;
    OpenAPI_list_t *shared_data_id_rangesList = NULL;
    group_id = cJSON_GetObjectItemCaseSensitive(udr_infoJSON, "groupId");
    if (group_id) {
    if (!cJSON_IsString(group_id) && !cJSON_IsNull(group_id)) {
        ogs_error("OpenAPI_udr_info_parseFromJSON() failed [group_id]");
        goto end;
    }
    }

    supi_ranges = cJSON_GetObjectItemCaseSensitive(udr_infoJSON, "supiRanges");
    if (supi_ranges) {
        cJSON *supi_ranges_local = NULL;
        if (!cJSON_IsArray(supi_ranges)) {
            ogs_error("OpenAPI_udr_info_parseFromJSON() failed [supi_ranges]");
            goto end;
        }

        supi_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(supi_ranges_local, supi_ranges) {
            if (!cJSON_IsObject(supi_ranges_local)) {
                ogs_error("OpenAPI_udr_info_parseFromJSON() failed [supi_ranges]");
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

    gpsi_ranges = cJSON_GetObjectItemCaseSensitive(udr_infoJSON, "gpsiRanges");
    if (gpsi_ranges) {
        cJSON *gpsi_ranges_local = NULL;
        if (!cJSON_IsArray(gpsi_ranges)) {
            ogs_error("OpenAPI_udr_info_parseFromJSON() failed [gpsi_ranges]");
            goto end;
        }

        gpsi_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(gpsi_ranges_local, gpsi_ranges) {
            if (!cJSON_IsObject(gpsi_ranges_local)) {
                ogs_error("OpenAPI_udr_info_parseFromJSON() failed [gpsi_ranges]");
                goto end;
            }
            OpenAPI_identity_range_t *gpsi_rangesItem = OpenAPI_identity_range_parseFromJSON(gpsi_ranges_local);
            if (!gpsi_rangesItem) {
                ogs_error("No gpsi_rangesItem");
                goto end;
            }
            OpenAPI_list_add(gpsi_rangesList, gpsi_rangesItem);
        }
    }

    external_group_identifiers_ranges = cJSON_GetObjectItemCaseSensitive(udr_infoJSON, "externalGroupIdentifiersRanges");
    if (external_group_identifiers_ranges) {
        cJSON *external_group_identifiers_ranges_local = NULL;
        if (!cJSON_IsArray(external_group_identifiers_ranges)) {
            ogs_error("OpenAPI_udr_info_parseFromJSON() failed [external_group_identifiers_ranges]");
            goto end;
        }

        external_group_identifiers_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(external_group_identifiers_ranges_local, external_group_identifiers_ranges) {
            if (!cJSON_IsObject(external_group_identifiers_ranges_local)) {
                ogs_error("OpenAPI_udr_info_parseFromJSON() failed [external_group_identifiers_ranges]");
                goto end;
            }
            OpenAPI_identity_range_t *external_group_identifiers_rangesItem = OpenAPI_identity_range_parseFromJSON(external_group_identifiers_ranges_local);
            if (!external_group_identifiers_rangesItem) {
                ogs_error("No external_group_identifiers_rangesItem");
                goto end;
            }
            OpenAPI_list_add(external_group_identifiers_rangesList, external_group_identifiers_rangesItem);
        }
    }

    supported_data_sets = cJSON_GetObjectItemCaseSensitive(udr_infoJSON, "supportedDataSets");
    if (supported_data_sets) {
        cJSON *supported_data_sets_local = NULL;
        if (!cJSON_IsArray(supported_data_sets)) {
            ogs_error("OpenAPI_udr_info_parseFromJSON() failed [supported_data_sets]");
            goto end;
        }

        supported_data_setsList = OpenAPI_list_create();

        cJSON_ArrayForEach(supported_data_sets_local, supported_data_sets) {
            OpenAPI_data_set_id_e localEnum = OpenAPI_data_set_id_NULL;
            if (!cJSON_IsString(supported_data_sets_local)) {
                ogs_error("OpenAPI_udr_info_parseFromJSON() failed [supported_data_sets]");
                goto end;
            }
            localEnum = OpenAPI_data_set_id_FromString(supported_data_sets_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"supported_data_sets\" is not supported. Ignoring it ...",
                         supported_data_sets_local->valuestring);
            } else {
                OpenAPI_list_add(supported_data_setsList, (void *)localEnum);
            }
        }
        if (supported_data_setsList->count == 0) {
            ogs_error("OpenAPI_udr_info_parseFromJSON() failed: Expected supported_data_setsList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    shared_data_id_ranges = cJSON_GetObjectItemCaseSensitive(udr_infoJSON, "sharedDataIdRanges");
    if (shared_data_id_ranges) {
        cJSON *shared_data_id_ranges_local = NULL;
        if (!cJSON_IsArray(shared_data_id_ranges)) {
            ogs_error("OpenAPI_udr_info_parseFromJSON() failed [shared_data_id_ranges]");
            goto end;
        }

        shared_data_id_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(shared_data_id_ranges_local, shared_data_id_ranges) {
            if (!cJSON_IsObject(shared_data_id_ranges_local)) {
                ogs_error("OpenAPI_udr_info_parseFromJSON() failed [shared_data_id_ranges]");
                goto end;
            }
            OpenAPI_shared_data_id_range_t *shared_data_id_rangesItem = OpenAPI_shared_data_id_range_parseFromJSON(shared_data_id_ranges_local);
            if (!shared_data_id_rangesItem) {
                ogs_error("No shared_data_id_rangesItem");
                goto end;
            }
            OpenAPI_list_add(shared_data_id_rangesList, shared_data_id_rangesItem);
        }
    }

    udr_info_local_var = OpenAPI_udr_info_create (
        group_id && !cJSON_IsNull(group_id) ? ogs_strdup(group_id->valuestring) : NULL,
        supi_ranges ? supi_rangesList : NULL,
        gpsi_ranges ? gpsi_rangesList : NULL,
        external_group_identifiers_ranges ? external_group_identifiers_rangesList : NULL,
        supported_data_sets ? supported_data_setsList : NULL,
        shared_data_id_ranges ? shared_data_id_rangesList : NULL
    );

    return udr_info_local_var;
end:
    if (supi_rangesList) {
        OpenAPI_list_for_each(supi_rangesList, node) {
            OpenAPI_supi_range_free(node->data);
        }
        OpenAPI_list_free(supi_rangesList);
        supi_rangesList = NULL;
    }
    if (gpsi_rangesList) {
        OpenAPI_list_for_each(gpsi_rangesList, node) {
            OpenAPI_identity_range_free(node->data);
        }
        OpenAPI_list_free(gpsi_rangesList);
        gpsi_rangesList = NULL;
    }
    if (external_group_identifiers_rangesList) {
        OpenAPI_list_for_each(external_group_identifiers_rangesList, node) {
            OpenAPI_identity_range_free(node->data);
        }
        OpenAPI_list_free(external_group_identifiers_rangesList);
        external_group_identifiers_rangesList = NULL;
    }
    if (supported_data_setsList) {
        OpenAPI_list_free(supported_data_setsList);
        supported_data_setsList = NULL;
    }
    if (shared_data_id_rangesList) {
        OpenAPI_list_for_each(shared_data_id_rangesList, node) {
            OpenAPI_shared_data_id_range_free(node->data);
        }
        OpenAPI_list_free(shared_data_id_rangesList);
        shared_data_id_rangesList = NULL;
    }
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

