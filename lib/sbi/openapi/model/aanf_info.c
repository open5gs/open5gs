
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "aanf_info.h"

OpenAPI_aanf_info_t *OpenAPI_aanf_info_create(
    OpenAPI_list_t *routing_indicators
)
{
    OpenAPI_aanf_info_t *aanf_info_local_var = ogs_malloc(sizeof(OpenAPI_aanf_info_t));
    ogs_assert(aanf_info_local_var);

    aanf_info_local_var->routing_indicators = routing_indicators;

    return aanf_info_local_var;
}

void OpenAPI_aanf_info_free(OpenAPI_aanf_info_t *aanf_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == aanf_info) {
        return;
    }
    if (aanf_info->routing_indicators) {
        OpenAPI_list_for_each(aanf_info->routing_indicators, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(aanf_info->routing_indicators);
        aanf_info->routing_indicators = NULL;
    }
    ogs_free(aanf_info);
}

cJSON *OpenAPI_aanf_info_convertToJSON(OpenAPI_aanf_info_t *aanf_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (aanf_info == NULL) {
        ogs_error("OpenAPI_aanf_info_convertToJSON() failed [AanfInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (aanf_info->routing_indicators) {
    cJSON *routing_indicatorsList = cJSON_AddArrayToObject(item, "routingIndicators");
    if (routing_indicatorsList == NULL) {
        ogs_error("OpenAPI_aanf_info_convertToJSON() failed [routing_indicators]");
        goto end;
    }
    OpenAPI_list_for_each(aanf_info->routing_indicators, node) {
        if (cJSON_AddStringToObject(routing_indicatorsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_aanf_info_convertToJSON() failed [routing_indicators]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_aanf_info_t *OpenAPI_aanf_info_parseFromJSON(cJSON *aanf_infoJSON)
{
    OpenAPI_aanf_info_t *aanf_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *routing_indicators = NULL;
    OpenAPI_list_t *routing_indicatorsList = NULL;
    routing_indicators = cJSON_GetObjectItemCaseSensitive(aanf_infoJSON, "routingIndicators");
    if (routing_indicators) {
        cJSON *routing_indicators_local = NULL;
        if (!cJSON_IsArray(routing_indicators)) {
            ogs_error("OpenAPI_aanf_info_parseFromJSON() failed [routing_indicators]");
            goto end;
        }

        routing_indicatorsList = OpenAPI_list_create();

        cJSON_ArrayForEach(routing_indicators_local, routing_indicators) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(routing_indicators_local)) {
                ogs_error("OpenAPI_aanf_info_parseFromJSON() failed [routing_indicators]");
                goto end;
            }
            OpenAPI_list_add(routing_indicatorsList, ogs_strdup(routing_indicators_local->valuestring));
        }
    }

    aanf_info_local_var = OpenAPI_aanf_info_create (
        routing_indicators ? routing_indicatorsList : NULL
    );

    return aanf_info_local_var;
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

OpenAPI_aanf_info_t *OpenAPI_aanf_info_copy(OpenAPI_aanf_info_t *dst, OpenAPI_aanf_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_aanf_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_aanf_info_convertToJSON() failed");
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

    OpenAPI_aanf_info_free(dst);
    dst = OpenAPI_aanf_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

