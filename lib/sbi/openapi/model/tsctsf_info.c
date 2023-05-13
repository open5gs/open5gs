
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tsctsf_info.h"

OpenAPI_tsctsf_info_t *OpenAPI_tsctsf_info_create(
    OpenAPI_list_t* s_nssai_info_list,
    OpenAPI_list_t *external_group_identifiers_ranges,
    OpenAPI_list_t *supi_ranges,
    OpenAPI_list_t *gpsi_ranges,
    OpenAPI_list_t *internal_group_identifiers_ranges
)
{
    OpenAPI_tsctsf_info_t *tsctsf_info_local_var = ogs_malloc(sizeof(OpenAPI_tsctsf_info_t));
    ogs_assert(tsctsf_info_local_var);

    tsctsf_info_local_var->s_nssai_info_list = s_nssai_info_list;
    tsctsf_info_local_var->external_group_identifiers_ranges = external_group_identifiers_ranges;
    tsctsf_info_local_var->supi_ranges = supi_ranges;
    tsctsf_info_local_var->gpsi_ranges = gpsi_ranges;
    tsctsf_info_local_var->internal_group_identifiers_ranges = internal_group_identifiers_ranges;

    return tsctsf_info_local_var;
}

void OpenAPI_tsctsf_info_free(OpenAPI_tsctsf_info_t *tsctsf_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == tsctsf_info) {
        return;
    }
    if (tsctsf_info->s_nssai_info_list) {
        OpenAPI_list_for_each(tsctsf_info->s_nssai_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_snssai_tsctsf_info_item_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(tsctsf_info->s_nssai_info_list);
        tsctsf_info->s_nssai_info_list = NULL;
    }
    if (tsctsf_info->external_group_identifiers_ranges) {
        OpenAPI_list_for_each(tsctsf_info->external_group_identifiers_ranges, node) {
            OpenAPI_identity_range_free(node->data);
        }
        OpenAPI_list_free(tsctsf_info->external_group_identifiers_ranges);
        tsctsf_info->external_group_identifiers_ranges = NULL;
    }
    if (tsctsf_info->supi_ranges) {
        OpenAPI_list_for_each(tsctsf_info->supi_ranges, node) {
            OpenAPI_supi_range_free(node->data);
        }
        OpenAPI_list_free(tsctsf_info->supi_ranges);
        tsctsf_info->supi_ranges = NULL;
    }
    if (tsctsf_info->gpsi_ranges) {
        OpenAPI_list_for_each(tsctsf_info->gpsi_ranges, node) {
            OpenAPI_identity_range_free(node->data);
        }
        OpenAPI_list_free(tsctsf_info->gpsi_ranges);
        tsctsf_info->gpsi_ranges = NULL;
    }
    if (tsctsf_info->internal_group_identifiers_ranges) {
        OpenAPI_list_for_each(tsctsf_info->internal_group_identifiers_ranges, node) {
            OpenAPI_internal_group_id_range_free(node->data);
        }
        OpenAPI_list_free(tsctsf_info->internal_group_identifiers_ranges);
        tsctsf_info->internal_group_identifiers_ranges = NULL;
    }
    ogs_free(tsctsf_info);
}

cJSON *OpenAPI_tsctsf_info_convertToJSON(OpenAPI_tsctsf_info_t *tsctsf_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (tsctsf_info == NULL) {
        ogs_error("OpenAPI_tsctsf_info_convertToJSON() failed [TsctsfInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (tsctsf_info->s_nssai_info_list) {
    cJSON *s_nssai_info_list = cJSON_AddObjectToObject(item, "sNssaiInfoList");
    if (s_nssai_info_list == NULL) {
        ogs_error("OpenAPI_tsctsf_info_convertToJSON() failed [s_nssai_info_list]");
        goto end;
    }
    cJSON *localMapObject = s_nssai_info_list;
    if (tsctsf_info->s_nssai_info_list) {
        OpenAPI_list_for_each(tsctsf_info->s_nssai_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_tsctsf_info_convertToJSON() failed [s_nssai_info_list]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_tsctsf_info_convertToJSON() failed [s_nssai_info_list]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_snssai_tsctsf_info_item_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_tsctsf_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (tsctsf_info->external_group_identifiers_ranges) {
    cJSON *external_group_identifiers_rangesList = cJSON_AddArrayToObject(item, "externalGroupIdentifiersRanges");
    if (external_group_identifiers_rangesList == NULL) {
        ogs_error("OpenAPI_tsctsf_info_convertToJSON() failed [external_group_identifiers_ranges]");
        goto end;
    }
    OpenAPI_list_for_each(tsctsf_info->external_group_identifiers_ranges, node) {
        cJSON *itemLocal = OpenAPI_identity_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_tsctsf_info_convertToJSON() failed [external_group_identifiers_ranges]");
            goto end;
        }
        cJSON_AddItemToArray(external_group_identifiers_rangesList, itemLocal);
    }
    }

    if (tsctsf_info->supi_ranges) {
    cJSON *supi_rangesList = cJSON_AddArrayToObject(item, "supiRanges");
    if (supi_rangesList == NULL) {
        ogs_error("OpenAPI_tsctsf_info_convertToJSON() failed [supi_ranges]");
        goto end;
    }
    OpenAPI_list_for_each(tsctsf_info->supi_ranges, node) {
        cJSON *itemLocal = OpenAPI_supi_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_tsctsf_info_convertToJSON() failed [supi_ranges]");
            goto end;
        }
        cJSON_AddItemToArray(supi_rangesList, itemLocal);
    }
    }

    if (tsctsf_info->gpsi_ranges) {
    cJSON *gpsi_rangesList = cJSON_AddArrayToObject(item, "gpsiRanges");
    if (gpsi_rangesList == NULL) {
        ogs_error("OpenAPI_tsctsf_info_convertToJSON() failed [gpsi_ranges]");
        goto end;
    }
    OpenAPI_list_for_each(tsctsf_info->gpsi_ranges, node) {
        cJSON *itemLocal = OpenAPI_identity_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_tsctsf_info_convertToJSON() failed [gpsi_ranges]");
            goto end;
        }
        cJSON_AddItemToArray(gpsi_rangesList, itemLocal);
    }
    }

    if (tsctsf_info->internal_group_identifiers_ranges) {
    cJSON *internal_group_identifiers_rangesList = cJSON_AddArrayToObject(item, "internalGroupIdentifiersRanges");
    if (internal_group_identifiers_rangesList == NULL) {
        ogs_error("OpenAPI_tsctsf_info_convertToJSON() failed [internal_group_identifiers_ranges]");
        goto end;
    }
    OpenAPI_list_for_each(tsctsf_info->internal_group_identifiers_ranges, node) {
        cJSON *itemLocal = OpenAPI_internal_group_id_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_tsctsf_info_convertToJSON() failed [internal_group_identifiers_ranges]");
            goto end;
        }
        cJSON_AddItemToArray(internal_group_identifiers_rangesList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_tsctsf_info_t *OpenAPI_tsctsf_info_parseFromJSON(cJSON *tsctsf_infoJSON)
{
    OpenAPI_tsctsf_info_t *tsctsf_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *s_nssai_info_list = NULL;
    OpenAPI_list_t *s_nssai_info_listList = NULL;
    cJSON *external_group_identifiers_ranges = NULL;
    OpenAPI_list_t *external_group_identifiers_rangesList = NULL;
    cJSON *supi_ranges = NULL;
    OpenAPI_list_t *supi_rangesList = NULL;
    cJSON *gpsi_ranges = NULL;
    OpenAPI_list_t *gpsi_rangesList = NULL;
    cJSON *internal_group_identifiers_ranges = NULL;
    OpenAPI_list_t *internal_group_identifiers_rangesList = NULL;
    s_nssai_info_list = cJSON_GetObjectItemCaseSensitive(tsctsf_infoJSON, "sNssaiInfoList");
    if (s_nssai_info_list) {
        cJSON *s_nssai_info_list_local_map = NULL;
        if (!cJSON_IsObject(s_nssai_info_list) && !cJSON_IsNull(s_nssai_info_list)) {
            ogs_error("OpenAPI_tsctsf_info_parseFromJSON() failed [s_nssai_info_list]");
            goto end;
        }
        if (cJSON_IsObject(s_nssai_info_list)) {
            s_nssai_info_listList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(s_nssai_info_list_local_map, s_nssai_info_list) {
                cJSON *localMapObject = s_nssai_info_list_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_snssai_tsctsf_info_item_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_tsctsf_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(s_nssai_info_listList, localMapKeyPair);
            }
        }
    }

    external_group_identifiers_ranges = cJSON_GetObjectItemCaseSensitive(tsctsf_infoJSON, "externalGroupIdentifiersRanges");
    if (external_group_identifiers_ranges) {
        cJSON *external_group_identifiers_ranges_local = NULL;
        if (!cJSON_IsArray(external_group_identifiers_ranges)) {
            ogs_error("OpenAPI_tsctsf_info_parseFromJSON() failed [external_group_identifiers_ranges]");
            goto end;
        }

        external_group_identifiers_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(external_group_identifiers_ranges_local, external_group_identifiers_ranges) {
            if (!cJSON_IsObject(external_group_identifiers_ranges_local)) {
                ogs_error("OpenAPI_tsctsf_info_parseFromJSON() failed [external_group_identifiers_ranges]");
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

    supi_ranges = cJSON_GetObjectItemCaseSensitive(tsctsf_infoJSON, "supiRanges");
    if (supi_ranges) {
        cJSON *supi_ranges_local = NULL;
        if (!cJSON_IsArray(supi_ranges)) {
            ogs_error("OpenAPI_tsctsf_info_parseFromJSON() failed [supi_ranges]");
            goto end;
        }

        supi_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(supi_ranges_local, supi_ranges) {
            if (!cJSON_IsObject(supi_ranges_local)) {
                ogs_error("OpenAPI_tsctsf_info_parseFromJSON() failed [supi_ranges]");
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

    gpsi_ranges = cJSON_GetObjectItemCaseSensitive(tsctsf_infoJSON, "gpsiRanges");
    if (gpsi_ranges) {
        cJSON *gpsi_ranges_local = NULL;
        if (!cJSON_IsArray(gpsi_ranges)) {
            ogs_error("OpenAPI_tsctsf_info_parseFromJSON() failed [gpsi_ranges]");
            goto end;
        }

        gpsi_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(gpsi_ranges_local, gpsi_ranges) {
            if (!cJSON_IsObject(gpsi_ranges_local)) {
                ogs_error("OpenAPI_tsctsf_info_parseFromJSON() failed [gpsi_ranges]");
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

    internal_group_identifiers_ranges = cJSON_GetObjectItemCaseSensitive(tsctsf_infoJSON, "internalGroupIdentifiersRanges");
    if (internal_group_identifiers_ranges) {
        cJSON *internal_group_identifiers_ranges_local = NULL;
        if (!cJSON_IsArray(internal_group_identifiers_ranges)) {
            ogs_error("OpenAPI_tsctsf_info_parseFromJSON() failed [internal_group_identifiers_ranges]");
            goto end;
        }

        internal_group_identifiers_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(internal_group_identifiers_ranges_local, internal_group_identifiers_ranges) {
            if (!cJSON_IsObject(internal_group_identifiers_ranges_local)) {
                ogs_error("OpenAPI_tsctsf_info_parseFromJSON() failed [internal_group_identifiers_ranges]");
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

    tsctsf_info_local_var = OpenAPI_tsctsf_info_create (
        s_nssai_info_list ? s_nssai_info_listList : NULL,
        external_group_identifiers_ranges ? external_group_identifiers_rangesList : NULL,
        supi_ranges ? supi_rangesList : NULL,
        gpsi_ranges ? gpsi_rangesList : NULL,
        internal_group_identifiers_ranges ? internal_group_identifiers_rangesList : NULL
    );

    return tsctsf_info_local_var;
end:
    if (s_nssai_info_listList) {
        OpenAPI_list_for_each(s_nssai_info_listList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_snssai_tsctsf_info_item_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(s_nssai_info_listList);
        s_nssai_info_listList = NULL;
    }
    if (external_group_identifiers_rangesList) {
        OpenAPI_list_for_each(external_group_identifiers_rangesList, node) {
            OpenAPI_identity_range_free(node->data);
        }
        OpenAPI_list_free(external_group_identifiers_rangesList);
        external_group_identifiers_rangesList = NULL;
    }
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
    if (internal_group_identifiers_rangesList) {
        OpenAPI_list_for_each(internal_group_identifiers_rangesList, node) {
            OpenAPI_internal_group_id_range_free(node->data);
        }
        OpenAPI_list_free(internal_group_identifiers_rangesList);
        internal_group_identifiers_rangesList = NULL;
    }
    return NULL;
}

OpenAPI_tsctsf_info_t *OpenAPI_tsctsf_info_copy(OpenAPI_tsctsf_info_t *dst, OpenAPI_tsctsf_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_tsctsf_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_tsctsf_info_convertToJSON() failed");
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

    OpenAPI_tsctsf_info_free(dst);
    dst = OpenAPI_tsctsf_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

