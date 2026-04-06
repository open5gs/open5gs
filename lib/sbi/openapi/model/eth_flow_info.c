
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "eth_flow_info.h"

OpenAPI_eth_flow_info_t *OpenAPI_eth_flow_info_create(
    int flow_id,
    OpenAPI_list_t *eth_flow_descriptions
)
{
    OpenAPI_eth_flow_info_t *eth_flow_info_local_var = ogs_malloc(sizeof(OpenAPI_eth_flow_info_t));
    ogs_assert(eth_flow_info_local_var);

    eth_flow_info_local_var->flow_id = flow_id;
    eth_flow_info_local_var->eth_flow_descriptions = eth_flow_descriptions;

    return eth_flow_info_local_var;
}

void OpenAPI_eth_flow_info_free(OpenAPI_eth_flow_info_t *eth_flow_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == eth_flow_info) {
        return;
    }
    if (eth_flow_info->eth_flow_descriptions) {
        OpenAPI_list_for_each(eth_flow_info->eth_flow_descriptions, node) {
            OpenAPI_eth_flow_description_1_free(node->data);
        }
        OpenAPI_list_free(eth_flow_info->eth_flow_descriptions);
        eth_flow_info->eth_flow_descriptions = NULL;
    }
    ogs_free(eth_flow_info);
}

cJSON *OpenAPI_eth_flow_info_convertToJSON(OpenAPI_eth_flow_info_t *eth_flow_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (eth_flow_info == NULL) {
        ogs_error("OpenAPI_eth_flow_info_convertToJSON() failed [EthFlowInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "flowId", eth_flow_info->flow_id) == NULL) {
        ogs_error("OpenAPI_eth_flow_info_convertToJSON() failed [flow_id]");
        goto end;
    }

    if (eth_flow_info->eth_flow_descriptions) {
    cJSON *eth_flow_descriptionsList = cJSON_AddArrayToObject(item, "ethFlowDescriptions");
    if (eth_flow_descriptionsList == NULL) {
        ogs_error("OpenAPI_eth_flow_info_convertToJSON() failed [eth_flow_descriptions]");
        goto end;
    }
    OpenAPI_list_for_each(eth_flow_info->eth_flow_descriptions, node) {
        cJSON *itemLocal = OpenAPI_eth_flow_description_1_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_eth_flow_info_convertToJSON() failed [eth_flow_descriptions]");
            goto end;
        }
        cJSON_AddItemToArray(eth_flow_descriptionsList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_eth_flow_info_t *OpenAPI_eth_flow_info_parseFromJSON(cJSON *eth_flow_infoJSON)
{
    OpenAPI_eth_flow_info_t *eth_flow_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *flow_id = NULL;
    cJSON *eth_flow_descriptions = NULL;
    OpenAPI_list_t *eth_flow_descriptionsList = NULL;
    flow_id = cJSON_GetObjectItemCaseSensitive(eth_flow_infoJSON, "flowId");
    if (!flow_id) {
        ogs_error("OpenAPI_eth_flow_info_parseFromJSON() failed [flow_id]");
        goto end;
    }
    if (!cJSON_IsNumber(flow_id)) {
        ogs_error("OpenAPI_eth_flow_info_parseFromJSON() failed [flow_id]");
        goto end;
    }

    eth_flow_descriptions = cJSON_GetObjectItemCaseSensitive(eth_flow_infoJSON, "ethFlowDescriptions");
    if (eth_flow_descriptions) {
        cJSON *eth_flow_descriptions_local = NULL;
        if (!cJSON_IsArray(eth_flow_descriptions)) {
            ogs_error("OpenAPI_eth_flow_info_parseFromJSON() failed [eth_flow_descriptions]");
            goto end;
        }

        eth_flow_descriptionsList = OpenAPI_list_create();

        cJSON_ArrayForEach(eth_flow_descriptions_local, eth_flow_descriptions) {
            if (!cJSON_IsObject(eth_flow_descriptions_local)) {
                ogs_error("OpenAPI_eth_flow_info_parseFromJSON() failed [eth_flow_descriptions]");
                goto end;
            }
            OpenAPI_eth_flow_description_1_t *eth_flow_descriptionsItem = OpenAPI_eth_flow_description_1_parseFromJSON(eth_flow_descriptions_local);
            if (!eth_flow_descriptionsItem) {
                ogs_error("No eth_flow_descriptionsItem");
                goto end;
            }
            OpenAPI_list_add(eth_flow_descriptionsList, eth_flow_descriptionsItem);
        }
    }

    eth_flow_info_local_var = OpenAPI_eth_flow_info_create (
        
        flow_id->valuedouble,
        eth_flow_descriptions ? eth_flow_descriptionsList : NULL
    );

    return eth_flow_info_local_var;
end:
    if (eth_flow_descriptionsList) {
        OpenAPI_list_for_each(eth_flow_descriptionsList, node) {
            OpenAPI_eth_flow_description_1_free(node->data);
        }
        OpenAPI_list_free(eth_flow_descriptionsList);
        eth_flow_descriptionsList = NULL;
    }
    return NULL;
}

OpenAPI_eth_flow_info_t *OpenAPI_eth_flow_info_copy(OpenAPI_eth_flow_info_t *dst, OpenAPI_eth_flow_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_eth_flow_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_eth_flow_info_convertToJSON() failed");
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

    OpenAPI_eth_flow_info_free(dst);
    dst = OpenAPI_eth_flow_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

