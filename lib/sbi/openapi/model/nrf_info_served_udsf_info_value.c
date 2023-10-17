
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nrf_info_served_udsf_info_value.h"

OpenAPI_nrf_info_served_udsf_info_value_t *OpenAPI_nrf_info_served_udsf_info_value_create(
    char *group_id,
    OpenAPI_list_t *supi_ranges,
    OpenAPI_list_t* storage_id_ranges
)
{
    OpenAPI_nrf_info_served_udsf_info_value_t *nrf_info_served_udsf_info_value_local_var = ogs_malloc(sizeof(OpenAPI_nrf_info_served_udsf_info_value_t));
    ogs_assert(nrf_info_served_udsf_info_value_local_var);

    nrf_info_served_udsf_info_value_local_var->group_id = group_id;
    nrf_info_served_udsf_info_value_local_var->supi_ranges = supi_ranges;
    nrf_info_served_udsf_info_value_local_var->storage_id_ranges = storage_id_ranges;

    return nrf_info_served_udsf_info_value_local_var;
}

void OpenAPI_nrf_info_served_udsf_info_value_free(OpenAPI_nrf_info_served_udsf_info_value_t *nrf_info_served_udsf_info_value)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nrf_info_served_udsf_info_value) {
        return;
    }
    if (nrf_info_served_udsf_info_value->group_id) {
        ogs_free(nrf_info_served_udsf_info_value->group_id);
        nrf_info_served_udsf_info_value->group_id = NULL;
    }
    if (nrf_info_served_udsf_info_value->supi_ranges) {
        OpenAPI_list_for_each(nrf_info_served_udsf_info_value->supi_ranges, node) {
            OpenAPI_supi_range_free(node->data);
        }
        OpenAPI_list_free(nrf_info_served_udsf_info_value->supi_ranges);
        nrf_info_served_udsf_info_value->supi_ranges = NULL;
    }
    if (nrf_info_served_udsf_info_value->storage_id_ranges) {
        OpenAPI_list_for_each(nrf_info_served_udsf_info_value->storage_id_ranges, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nrf_info_served_udsf_info_value->storage_id_ranges);
        nrf_info_served_udsf_info_value->storage_id_ranges = NULL;
    }
    ogs_free(nrf_info_served_udsf_info_value);
}

cJSON *OpenAPI_nrf_info_served_udsf_info_value_convertToJSON(OpenAPI_nrf_info_served_udsf_info_value_t *nrf_info_served_udsf_info_value)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nrf_info_served_udsf_info_value == NULL) {
        ogs_error("OpenAPI_nrf_info_served_udsf_info_value_convertToJSON() failed [NrfInfo_servedUdsfInfo_value]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (nrf_info_served_udsf_info_value->group_id) {
    if (cJSON_AddStringToObject(item, "groupId", nrf_info_served_udsf_info_value->group_id) == NULL) {
        ogs_error("OpenAPI_nrf_info_served_udsf_info_value_convertToJSON() failed [group_id]");
        goto end;
    }
    }

    if (nrf_info_served_udsf_info_value->supi_ranges) {
    cJSON *supi_rangesList = cJSON_AddArrayToObject(item, "supiRanges");
    if (supi_rangesList == NULL) {
        ogs_error("OpenAPI_nrf_info_served_udsf_info_value_convertToJSON() failed [supi_ranges]");
        goto end;
    }
    OpenAPI_list_for_each(nrf_info_served_udsf_info_value->supi_ranges, node) {
        cJSON *itemLocal = OpenAPI_supi_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nrf_info_served_udsf_info_value_convertToJSON() failed [supi_ranges]");
            goto end;
        }
        cJSON_AddItemToArray(supi_rangesList, itemLocal);
    }
    }

    if (nrf_info_served_udsf_info_value->storage_id_ranges) {
    cJSON *storage_id_ranges = cJSON_AddObjectToObject(item, "storageIdRanges");
    if (storage_id_ranges == NULL) {
        ogs_error("OpenAPI_nrf_info_served_udsf_info_value_convertToJSON() failed [storage_id_ranges]");
        goto end;
    }
    cJSON *localMapObject = storage_id_ranges;
    if (nrf_info_served_udsf_info_value->storage_id_ranges) {
        OpenAPI_list_for_each(nrf_info_served_udsf_info_value->storage_id_ranges, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_nrf_info_served_udsf_info_value_convertToJSON() failed [storage_id_ranges]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_nrf_info_served_udsf_info_value_convertToJSON() failed [storage_id_ranges]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_identity_range_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nrf_info_served_udsf_info_value_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

end:
    return item;
}

OpenAPI_nrf_info_served_udsf_info_value_t *OpenAPI_nrf_info_served_udsf_info_value_parseFromJSON(cJSON *nrf_info_served_udsf_info_valueJSON)
{
    OpenAPI_nrf_info_served_udsf_info_value_t *nrf_info_served_udsf_info_value_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *group_id = NULL;
    cJSON *supi_ranges = NULL;
    OpenAPI_list_t *supi_rangesList = NULL;
    cJSON *storage_id_ranges = NULL;
    OpenAPI_list_t *storage_id_rangesList = NULL;
    group_id = cJSON_GetObjectItemCaseSensitive(nrf_info_served_udsf_info_valueJSON, "groupId");
    if (group_id) {
    if (!cJSON_IsString(group_id) && !cJSON_IsNull(group_id)) {
        ogs_error("OpenAPI_nrf_info_served_udsf_info_value_parseFromJSON() failed [group_id]");
        goto end;
    }
    }

    supi_ranges = cJSON_GetObjectItemCaseSensitive(nrf_info_served_udsf_info_valueJSON, "supiRanges");
    if (supi_ranges) {
        cJSON *supi_ranges_local = NULL;
        if (!cJSON_IsArray(supi_ranges)) {
            ogs_error("OpenAPI_nrf_info_served_udsf_info_value_parseFromJSON() failed [supi_ranges]");
            goto end;
        }

        supi_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(supi_ranges_local, supi_ranges) {
            if (!cJSON_IsObject(supi_ranges_local)) {
                ogs_error("OpenAPI_nrf_info_served_udsf_info_value_parseFromJSON() failed [supi_ranges]");
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

    storage_id_ranges = cJSON_GetObjectItemCaseSensitive(nrf_info_served_udsf_info_valueJSON, "storageIdRanges");
    if (storage_id_ranges) {
        cJSON *storage_id_ranges_local_map = NULL;
        if (!cJSON_IsObject(storage_id_ranges) && !cJSON_IsNull(storage_id_ranges)) {
            ogs_error("OpenAPI_nrf_info_served_udsf_info_value_parseFromJSON() failed [storage_id_ranges]");
            goto end;
        }
        if (cJSON_IsObject(storage_id_ranges)) {
            storage_id_rangesList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(storage_id_ranges_local_map, storage_id_ranges) {
                cJSON *localMapObject = storage_id_ranges_local_map;
                double *localDouble = NULL;
                int *localInt = NULL;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_identity_range_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_nrf_info_served_udsf_info_value_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(storage_id_rangesList, localMapKeyPair);
            }
        }
    }

    nrf_info_served_udsf_info_value_local_var = OpenAPI_nrf_info_served_udsf_info_value_create (
        group_id && !cJSON_IsNull(group_id) ? ogs_strdup(group_id->valuestring) : NULL,
        supi_ranges ? supi_rangesList : NULL,
        storage_id_ranges ? storage_id_rangesList : NULL
    );

    return nrf_info_served_udsf_info_value_local_var;
end:
    if (supi_rangesList) {
        OpenAPI_list_for_each(supi_rangesList, node) {
            OpenAPI_supi_range_free(node->data);
        }
        OpenAPI_list_free(supi_rangesList);
        supi_rangesList = NULL;
    }
    if (storage_id_rangesList) {
        OpenAPI_list_for_each(storage_id_rangesList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(storage_id_rangesList);
        storage_id_rangesList = NULL;
    }
    return NULL;
}

OpenAPI_nrf_info_served_udsf_info_value_t *OpenAPI_nrf_info_served_udsf_info_value_copy(OpenAPI_nrf_info_served_udsf_info_value_t *dst, OpenAPI_nrf_info_served_udsf_info_value_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nrf_info_served_udsf_info_value_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nrf_info_served_udsf_info_value_convertToJSON() failed");
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

    OpenAPI_nrf_info_served_udsf_info_value_free(dst);
    dst = OpenAPI_nrf_info_served_udsf_info_value_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

