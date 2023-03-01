
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nrf_info_served_aanf_info_list_value_value.h"

OpenAPI_nrf_info_served_aanf_info_list_value_value_t *OpenAPI_nrf_info_served_aanf_info_list_value_value_create(
    OpenAPI_list_t *routing_indicators
)
{
    OpenAPI_nrf_info_served_aanf_info_list_value_value_t *nrf_info_served_aanf_info_list_value_value_local_var = ogs_malloc(sizeof(OpenAPI_nrf_info_served_aanf_info_list_value_value_t));
    ogs_assert(nrf_info_served_aanf_info_list_value_value_local_var);

    nrf_info_served_aanf_info_list_value_value_local_var->routing_indicators = routing_indicators;

    return nrf_info_served_aanf_info_list_value_value_local_var;
}

void OpenAPI_nrf_info_served_aanf_info_list_value_value_free(OpenAPI_nrf_info_served_aanf_info_list_value_value_t *nrf_info_served_aanf_info_list_value_value)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nrf_info_served_aanf_info_list_value_value) {
        return;
    }
    if (nrf_info_served_aanf_info_list_value_value->routing_indicators) {
        OpenAPI_list_for_each(nrf_info_served_aanf_info_list_value_value->routing_indicators, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(nrf_info_served_aanf_info_list_value_value->routing_indicators);
        nrf_info_served_aanf_info_list_value_value->routing_indicators = NULL;
    }
    ogs_free(nrf_info_served_aanf_info_list_value_value);
}

cJSON *OpenAPI_nrf_info_served_aanf_info_list_value_value_convertToJSON(OpenAPI_nrf_info_served_aanf_info_list_value_value_t *nrf_info_served_aanf_info_list_value_value)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nrf_info_served_aanf_info_list_value_value == NULL) {
        ogs_error("OpenAPI_nrf_info_served_aanf_info_list_value_value_convertToJSON() failed [NrfInfo_servedAanfInfoList_value_value]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (nrf_info_served_aanf_info_list_value_value->routing_indicators) {
    cJSON *routing_indicatorsList = cJSON_AddArrayToObject(item, "routingIndicators");
    if (routing_indicatorsList == NULL) {
        ogs_error("OpenAPI_nrf_info_served_aanf_info_list_value_value_convertToJSON() failed [routing_indicators]");
        goto end;
    }
    OpenAPI_list_for_each(nrf_info_served_aanf_info_list_value_value->routing_indicators, node) {
        if (cJSON_AddStringToObject(routing_indicatorsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_nrf_info_served_aanf_info_list_value_value_convertToJSON() failed [routing_indicators]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_nrf_info_served_aanf_info_list_value_value_t *OpenAPI_nrf_info_served_aanf_info_list_value_value_parseFromJSON(cJSON *nrf_info_served_aanf_info_list_value_valueJSON)
{
    OpenAPI_nrf_info_served_aanf_info_list_value_value_t *nrf_info_served_aanf_info_list_value_value_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *routing_indicators = NULL;
    OpenAPI_list_t *routing_indicatorsList = NULL;
    routing_indicators = cJSON_GetObjectItemCaseSensitive(nrf_info_served_aanf_info_list_value_valueJSON, "routingIndicators");
    if (routing_indicators) {
        cJSON *routing_indicators_local = NULL;
        if (!cJSON_IsArray(routing_indicators)) {
            ogs_error("OpenAPI_nrf_info_served_aanf_info_list_value_value_parseFromJSON() failed [routing_indicators]");
            goto end;
        }

        routing_indicatorsList = OpenAPI_list_create();

        cJSON_ArrayForEach(routing_indicators_local, routing_indicators) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(routing_indicators_local)) {
                ogs_error("OpenAPI_nrf_info_served_aanf_info_list_value_value_parseFromJSON() failed [routing_indicators]");
                goto end;
            }
            OpenAPI_list_add(routing_indicatorsList, ogs_strdup(routing_indicators_local->valuestring));
        }
    }

    nrf_info_served_aanf_info_list_value_value_local_var = OpenAPI_nrf_info_served_aanf_info_list_value_value_create (
        routing_indicators ? routing_indicatorsList : NULL
    );

    return nrf_info_served_aanf_info_list_value_value_local_var;
end:
    if (routing_indicatorsList) {
        OpenAPI_list_for_each(routing_indicatorsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(routing_indicatorsList);
        routing_indicatorsList = NULL;
    }
    return NULL;
}

OpenAPI_nrf_info_served_aanf_info_list_value_value_t *OpenAPI_nrf_info_served_aanf_info_list_value_value_copy(OpenAPI_nrf_info_served_aanf_info_list_value_value_t *dst, OpenAPI_nrf_info_served_aanf_info_list_value_value_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nrf_info_served_aanf_info_list_value_value_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nrf_info_served_aanf_info_list_value_value_convertToJSON() failed");
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

    OpenAPI_nrf_info_served_aanf_info_list_value_value_free(dst);
    dst = OpenAPI_nrf_info_served_aanf_info_list_value_value_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

