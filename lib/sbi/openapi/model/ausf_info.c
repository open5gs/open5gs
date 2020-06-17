
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ausf_info.h"

OpenAPI_ausf_info_t *OpenAPI_ausf_info_create(
    char *group_id,
    OpenAPI_list_t *supi_ranges,
    OpenAPI_list_t *routing_indicators
    )
{
    OpenAPI_ausf_info_t *ausf_info_local_var = OpenAPI_malloc(sizeof(OpenAPI_ausf_info_t));
    if (!ausf_info_local_var) {
        return NULL;
    }
    ausf_info_local_var->group_id = group_id;
    ausf_info_local_var->supi_ranges = supi_ranges;
    ausf_info_local_var->routing_indicators = routing_indicators;

    return ausf_info_local_var;
}

void OpenAPI_ausf_info_free(OpenAPI_ausf_info_t *ausf_info)
{
    if (NULL == ausf_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(ausf_info->group_id);
    OpenAPI_list_for_each(ausf_info->supi_ranges, node) {
        OpenAPI_supi_range_free(node->data);
    }
    OpenAPI_list_free(ausf_info->supi_ranges);
    OpenAPI_list_for_each(ausf_info->routing_indicators, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(ausf_info->routing_indicators);
    ogs_free(ausf_info);
}

cJSON *OpenAPI_ausf_info_convertToJSON(OpenAPI_ausf_info_t *ausf_info)
{
    cJSON *item = NULL;

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

        OpenAPI_lnode_t *supi_ranges_node;
        if (ausf_info->supi_ranges) {
            OpenAPI_list_for_each(ausf_info->supi_ranges, supi_ranges_node) {
                cJSON *itemLocal = OpenAPI_supi_range_convertToJSON(supi_ranges_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_ausf_info_convertToJSON() failed [supi_ranges]");
                    goto end;
                }
                cJSON_AddItemToArray(supi_rangesList, itemLocal);
            }
        }
    }

    if (ausf_info->routing_indicators) {
        cJSON *routing_indicators = cJSON_AddArrayToObject(item, "routingIndicators");
        if (routing_indicators == NULL) {
            ogs_error("OpenAPI_ausf_info_convertToJSON() failed [routing_indicators]");
            goto end;
        }

        OpenAPI_lnode_t *routing_indicators_node;
        OpenAPI_list_for_each(ausf_info->routing_indicators, routing_indicators_node)  {
            if (cJSON_AddStringToObject(routing_indicators, "", (char*)routing_indicators_node->data) == NULL) {
                ogs_error("OpenAPI_ausf_info_convertToJSON() failed [routing_indicators]");
                goto end;
            }
        }
    }

end:
    return item;
}

OpenAPI_ausf_info_t *OpenAPI_ausf_info_parseFromJSON(cJSON *ausf_infoJSON)
{
    OpenAPI_ausf_info_t *ausf_info_local_var = NULL;
    cJSON *group_id = cJSON_GetObjectItemCaseSensitive(ausf_infoJSON, "groupId");

    if (group_id) {
        if (!cJSON_IsString(group_id)) {
            ogs_error("OpenAPI_ausf_info_parseFromJSON() failed [group_id]");
            goto end;
        }
    }

    cJSON *supi_ranges = cJSON_GetObjectItemCaseSensitive(ausf_infoJSON, "supiRanges");

    OpenAPI_list_t *supi_rangesList;
    if (supi_ranges) {
        cJSON *supi_ranges_local_nonprimitive;
        if (!cJSON_IsArray(supi_ranges)) {
            ogs_error("OpenAPI_ausf_info_parseFromJSON() failed [supi_ranges]");
            goto end;
        }

        supi_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(supi_ranges_local_nonprimitive, supi_ranges ) {
            if (!cJSON_IsObject(supi_ranges_local_nonprimitive)) {
                ogs_error("OpenAPI_ausf_info_parseFromJSON() failed [supi_ranges]");
                goto end;
            }
            OpenAPI_supi_range_t *supi_rangesItem = OpenAPI_supi_range_parseFromJSON(supi_ranges_local_nonprimitive);

            OpenAPI_list_add(supi_rangesList, supi_rangesItem);
        }
    }

    cJSON *routing_indicators = cJSON_GetObjectItemCaseSensitive(ausf_infoJSON, "routingIndicators");

    OpenAPI_list_t *routing_indicatorsList;
    if (routing_indicators) {
        cJSON *routing_indicators_local;
        if (!cJSON_IsArray(routing_indicators)) {
            ogs_error("OpenAPI_ausf_info_parseFromJSON() failed [routing_indicators]");
            goto end;
        }
        routing_indicatorsList = OpenAPI_list_create();

        cJSON_ArrayForEach(routing_indicators_local, routing_indicators) {
            if (!cJSON_IsString(routing_indicators_local)) {
                ogs_error("OpenAPI_ausf_info_parseFromJSON() failed [routing_indicators]");
                goto end;
            }
            OpenAPI_list_add(routing_indicatorsList, ogs_strdup(routing_indicators_local->valuestring));
        }
    }

    ausf_info_local_var = OpenAPI_ausf_info_create (
        group_id ? ogs_strdup(group_id->valuestring) : NULL,
        supi_ranges ? supi_rangesList : NULL,
        routing_indicators ? routing_indicatorsList : NULL
        );

    return ausf_info_local_var;
end:
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

