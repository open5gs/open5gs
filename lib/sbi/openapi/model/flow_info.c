
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "flow_info.h"

OpenAPI_flow_info_t *OpenAPI_flow_info_create(
    int flow_id,
    OpenAPI_list_t *flow_descriptions
    )
{
    OpenAPI_flow_info_t *flow_info_local_var = OpenAPI_malloc(sizeof(OpenAPI_flow_info_t));
    if (!flow_info_local_var) {
        return NULL;
    }
    flow_info_local_var->flow_id = flow_id;
    flow_info_local_var->flow_descriptions = flow_descriptions;

    return flow_info_local_var;
}

void OpenAPI_flow_info_free(OpenAPI_flow_info_t *flow_info)
{
    if (NULL == flow_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(flow_info->flow_descriptions, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(flow_info->flow_descriptions);
    ogs_free(flow_info);
}

cJSON *OpenAPI_flow_info_convertToJSON(OpenAPI_flow_info_t *flow_info)
{
    cJSON *item = NULL;

    if (flow_info == NULL) {
        ogs_error("OpenAPI_flow_info_convertToJSON() failed [FlowInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!flow_info->flow_id) {
        ogs_error("OpenAPI_flow_info_convertToJSON() failed [flow_id]");
        goto end;
    }
    if (cJSON_AddNumberToObject(item, "flowId", flow_info->flow_id) == NULL) {
        ogs_error("OpenAPI_flow_info_convertToJSON() failed [flow_id]");
        goto end;
    }

    if (flow_info->flow_descriptions) {
        cJSON *flow_descriptions = cJSON_AddArrayToObject(item, "flowDescriptions");
        if (flow_descriptions == NULL) {
            ogs_error("OpenAPI_flow_info_convertToJSON() failed [flow_descriptions]");
            goto end;
        }

        OpenAPI_lnode_t *flow_descriptions_node;
        OpenAPI_list_for_each(flow_info->flow_descriptions, flow_descriptions_node)  {
            if (cJSON_AddStringToObject(flow_descriptions, "", (char*)flow_descriptions_node->data) == NULL) {
                ogs_error("OpenAPI_flow_info_convertToJSON() failed [flow_descriptions]");
                goto end;
            }
        }
    }

end:
    return item;
}

OpenAPI_flow_info_t *OpenAPI_flow_info_parseFromJSON(cJSON *flow_infoJSON)
{
    OpenAPI_flow_info_t *flow_info_local_var = NULL;
    cJSON *flow_id = cJSON_GetObjectItemCaseSensitive(flow_infoJSON, "flowId");
    if (!flow_id) {
        ogs_error("OpenAPI_flow_info_parseFromJSON() failed [flow_id]");
        goto end;
    }


    if (!cJSON_IsNumber(flow_id)) {
        ogs_error("OpenAPI_flow_info_parseFromJSON() failed [flow_id]");
        goto end;
    }

    cJSON *flow_descriptions = cJSON_GetObjectItemCaseSensitive(flow_infoJSON, "flowDescriptions");

    OpenAPI_list_t *flow_descriptionsList;
    if (flow_descriptions) {
        cJSON *flow_descriptions_local;
        if (!cJSON_IsArray(flow_descriptions)) {
            ogs_error("OpenAPI_flow_info_parseFromJSON() failed [flow_descriptions]");
            goto end;
        }
        flow_descriptionsList = OpenAPI_list_create();

        cJSON_ArrayForEach(flow_descriptions_local, flow_descriptions) {
            if (!cJSON_IsString(flow_descriptions_local)) {
                ogs_error("OpenAPI_flow_info_parseFromJSON() failed [flow_descriptions]");
                goto end;
            }
            OpenAPI_list_add(flow_descriptionsList, ogs_strdup(flow_descriptions_local->valuestring));
        }
    }

    flow_info_local_var = OpenAPI_flow_info_create (
        flow_id->valuedouble,
        flow_descriptions ? flow_descriptionsList : NULL
        );

    return flow_info_local_var;
end:
    return NULL;
}

OpenAPI_flow_info_t *OpenAPI_flow_info_copy(OpenAPI_flow_info_t *dst, OpenAPI_flow_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_flow_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_flow_info_convertToJSON() failed");
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

    OpenAPI_flow_info_free(dst);
    dst = OpenAPI_flow_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

