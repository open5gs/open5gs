
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ausf_info.h"

OpenAPI_ausf_info_t *OpenAPI_ausf_info_create(
    char *group_id,
    OpenAPI_list_t *supi_ranges,
    OpenAPI_list_t *routing_indicators,
    OpenAPI_list_t *suci_infos
)
{
    OpenAPI_ausf_info_t *ausf_info_local_var = ogs_malloc(sizeof(OpenAPI_ausf_info_t));
    ogs_assert(ausf_info_local_var);

    ausf_info_local_var->group_id = group_id;
    ausf_info_local_var->supi_ranges = supi_ranges;
    ausf_info_local_var->routing_indicators = routing_indicators;
    ausf_info_local_var->suci_infos = suci_infos;

    return ausf_info_local_var;
}

void OpenAPI_ausf_info_free(OpenAPI_ausf_info_t *ausf_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ausf_info) {
        return;
    }
    if (ausf_info->group_id) {
        ogs_free(ausf_info->group_id);
        ausf_info->group_id = NULL;
    }
    if (ausf_info->supi_ranges) {
        OpenAPI_list_for_each(ausf_info->supi_ranges, node) {
            OpenAPI_supi_range_free(node->data);
        }
        OpenAPI_list_free(ausf_info->supi_ranges);
        ausf_info->supi_ranges = NULL;
    }
    if (ausf_info->routing_indicators) {
        OpenAPI_list_for_each(ausf_info->routing_indicators, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ausf_info->routing_indicators);
        ausf_info->routing_indicators = NULL;
    }
    if (ausf_info->suci_infos) {
        OpenAPI_list_for_each(ausf_info->suci_infos, node) {
            OpenAPI_suci_info_free(node->data);
        }
        OpenAPI_list_free(ausf_info->suci_infos);
        ausf_info->suci_infos = NULL;
    }
    ogs_free(ausf_info);
}

cJSON *OpenAPI_ausf_info_convertToJSON(OpenAPI_ausf_info_t *ausf_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ausf_info == NULL) {
        ogs_error("OpenAPI_ausf_info_convertToJSON() failed [AusfInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ausf_info->group_id) {
    if (cJSON_AddStringToObject(item, "groupId", ausf_info->group_id) == NULL) {
        ogs_error("OpenAPI_ausf_info_convertToJSON() failed [group_id]");
        goto end;
    }
    }

    if (ausf_info->supi_ranges) {
    cJSON *supi_rangesList = cJSON_AddArrayToObject(item, "supiRanges");
    if (supi_rangesList == NULL) {
        ogs_error("OpenAPI_ausf_info_convertToJSON() failed [supi_ranges]");
        goto end;
    }
    OpenAPI_list_for_each(ausf_info->supi_ranges, node) {
        cJSON *itemLocal = OpenAPI_supi_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ausf_info_convertToJSON() failed [supi_ranges]");
            goto end;
        }
        cJSON_AddItemToArray(supi_rangesList, itemLocal);
    }
    }

    if (ausf_info->routing_indicators) {
    cJSON *routing_indicatorsList = cJSON_AddArrayToObject(item, "routingIndicators");
    if (routing_indicatorsList == NULL) {
        ogs_error("OpenAPI_ausf_info_convertToJSON() failed [routing_indicators]");
        goto end;
    }
    OpenAPI_list_for_each(ausf_info->routing_indicators, node) {
        if (cJSON_AddStringToObject(routing_indicatorsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_ausf_info_convertToJSON() failed [routing_indicators]");
            goto end;
        }
    }
    }

    if (ausf_info->suci_infos) {
    cJSON *suci_infosList = cJSON_AddArrayToObject(item, "suciInfos");
    if (suci_infosList == NULL) {
        ogs_error("OpenAPI_ausf_info_convertToJSON() failed [suci_infos]");
        goto end;
    }
    OpenAPI_list_for_each(ausf_info->suci_infos, node) {
        cJSON *itemLocal = OpenAPI_suci_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ausf_info_convertToJSON() failed [suci_infos]");
            goto end;
        }
        cJSON_AddItemToArray(suci_infosList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_ausf_info_t *OpenAPI_ausf_info_parseFromJSON(cJSON *ausf_infoJSON)
{
    OpenAPI_ausf_info_t *ausf_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *group_id = NULL;
    cJSON *supi_ranges = NULL;
    OpenAPI_list_t *supi_rangesList = NULL;
    cJSON *routing_indicators = NULL;
    OpenAPI_list_t *routing_indicatorsList = NULL;
    cJSON *suci_infos = NULL;
    OpenAPI_list_t *suci_infosList = NULL;
    group_id = cJSON_GetObjectItemCaseSensitive(ausf_infoJSON, "groupId");
    if (group_id) {
    if (!cJSON_IsString(group_id) && !cJSON_IsNull(group_id)) {
        ogs_error("OpenAPI_ausf_info_parseFromJSON() failed [group_id]");
        goto end;
    }
    }

    supi_ranges = cJSON_GetObjectItemCaseSensitive(ausf_infoJSON, "supiRanges");
    if (supi_ranges) {
        cJSON *supi_ranges_local = NULL;
        if (!cJSON_IsArray(supi_ranges)) {
            ogs_error("OpenAPI_ausf_info_parseFromJSON() failed [supi_ranges]");
            goto end;
        }

        supi_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(supi_ranges_local, supi_ranges) {
            if (!cJSON_IsObject(supi_ranges_local)) {
                ogs_error("OpenAPI_ausf_info_parseFromJSON() failed [supi_ranges]");
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

    routing_indicators = cJSON_GetObjectItemCaseSensitive(ausf_infoJSON, "routingIndicators");
    if (routing_indicators) {
        cJSON *routing_indicators_local = NULL;
        if (!cJSON_IsArray(routing_indicators)) {
            ogs_error("OpenAPI_ausf_info_parseFromJSON() failed [routing_indicators]");
            goto end;
        }

        routing_indicatorsList = OpenAPI_list_create();

        cJSON_ArrayForEach(routing_indicators_local, routing_indicators) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(routing_indicators_local)) {
                ogs_error("OpenAPI_ausf_info_parseFromJSON() failed [routing_indicators]");
                goto end;
            }
            OpenAPI_list_add(routing_indicatorsList, ogs_strdup(routing_indicators_local->valuestring));
        }
    }

    suci_infos = cJSON_GetObjectItemCaseSensitive(ausf_infoJSON, "suciInfos");
    if (suci_infos) {
        cJSON *suci_infos_local = NULL;
        if (!cJSON_IsArray(suci_infos)) {
            ogs_error("OpenAPI_ausf_info_parseFromJSON() failed [suci_infos]");
            goto end;
        }

        suci_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(suci_infos_local, suci_infos) {
            if (!cJSON_IsObject(suci_infos_local)) {
                ogs_error("OpenAPI_ausf_info_parseFromJSON() failed [suci_infos]");
                goto end;
            }
            OpenAPI_suci_info_t *suci_infosItem = OpenAPI_suci_info_parseFromJSON(suci_infos_local);
            if (!suci_infosItem) {
                ogs_error("No suci_infosItem");
                goto end;
            }
            OpenAPI_list_add(suci_infosList, suci_infosItem);
        }
    }

    ausf_info_local_var = OpenAPI_ausf_info_create (
        group_id && !cJSON_IsNull(group_id) ? ogs_strdup(group_id->valuestring) : NULL,
        supi_ranges ? supi_rangesList : NULL,
        routing_indicators ? routing_indicatorsList : NULL,
        suci_infos ? suci_infosList : NULL
    );

    return ausf_info_local_var;
end:
    if (supi_rangesList) {
        OpenAPI_list_for_each(supi_rangesList, node) {
            OpenAPI_supi_range_free(node->data);
        }
        OpenAPI_list_free(supi_rangesList);
        supi_rangesList = NULL;
    }
    if (routing_indicatorsList) {
        OpenAPI_list_for_each(routing_indicatorsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(routing_indicatorsList);
        routing_indicatorsList = NULL;
    }
    if (suci_infosList) {
        OpenAPI_list_for_each(suci_infosList, node) {
            OpenAPI_suci_info_free(node->data);
        }
        OpenAPI_list_free(suci_infosList);
        suci_infosList = NULL;
    }
    return NULL;
}

OpenAPI_ausf_info_t *OpenAPI_ausf_info_copy(OpenAPI_ausf_info_t *dst, OpenAPI_ausf_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ausf_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ausf_info_convertToJSON() failed");
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

    OpenAPI_ausf_info_free(dst);
    dst = OpenAPI_ausf_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

