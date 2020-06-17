
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pc5_qo_s_para.h"

OpenAPI_pc5_qo_s_para_t *OpenAPI_pc5_qo_s_para_create(
    OpenAPI_list_t *pc5_qos_flow_list,
    char *pc5_link_ambr
    )
{
    OpenAPI_pc5_qo_s_para_t *pc5_qo_s_para_local_var = OpenAPI_malloc(sizeof(OpenAPI_pc5_qo_s_para_t));
    if (!pc5_qo_s_para_local_var) {
        return NULL;
    }
    pc5_qo_s_para_local_var->pc5_qos_flow_list = pc5_qos_flow_list;
    pc5_qo_s_para_local_var->pc5_link_ambr = pc5_link_ambr;

    return pc5_qo_s_para_local_var;
}

void OpenAPI_pc5_qo_s_para_free(OpenAPI_pc5_qo_s_para_t *pc5_qo_s_para)
{
    if (NULL == pc5_qo_s_para) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(pc5_qo_s_para->pc5_qos_flow_list, node) {
        OpenAPI_pc5_qos_flow_item_free(node->data);
    }
    OpenAPI_list_free(pc5_qo_s_para->pc5_qos_flow_list);
    ogs_free(pc5_qo_s_para->pc5_link_ambr);
    ogs_free(pc5_qo_s_para);
}

cJSON *OpenAPI_pc5_qo_s_para_convertToJSON(OpenAPI_pc5_qo_s_para_t *pc5_qo_s_para)
{
    cJSON *item = NULL;

    if (pc5_qo_s_para == NULL) {
        ogs_error("OpenAPI_pc5_qo_s_para_convertToJSON() failed [Pc5QoSPara]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!pc5_qo_s_para->pc5_qos_flow_list) {
        ogs_error("OpenAPI_pc5_qo_s_para_convertToJSON() failed [pc5_qos_flow_list]");
        goto end;
    }
    cJSON *pc5_qos_flow_listList = cJSON_AddArrayToObject(item, "pc5QosFlowList");
    if (pc5_qos_flow_listList == NULL) {
        ogs_error("OpenAPI_pc5_qo_s_para_convertToJSON() failed [pc5_qos_flow_list]");
        goto end;
    }

    OpenAPI_lnode_t *pc5_qos_flow_list_node;
    if (pc5_qo_s_para->pc5_qos_flow_list) {
        OpenAPI_list_for_each(pc5_qo_s_para->pc5_qos_flow_list, pc5_qos_flow_list_node) {
            cJSON *itemLocal = OpenAPI_pc5_qos_flow_item_convertToJSON(pc5_qos_flow_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_pc5_qo_s_para_convertToJSON() failed [pc5_qos_flow_list]");
                goto end;
            }
            cJSON_AddItemToArray(pc5_qos_flow_listList, itemLocal);
        }
    }

    if (pc5_qo_s_para->pc5_link_ambr) {
        if (cJSON_AddStringToObject(item, "pc5LinkAmbr", pc5_qo_s_para->pc5_link_ambr) == NULL) {
            ogs_error("OpenAPI_pc5_qo_s_para_convertToJSON() failed [pc5_link_ambr]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_pc5_qo_s_para_t *OpenAPI_pc5_qo_s_para_parseFromJSON(cJSON *pc5_qo_s_paraJSON)
{
    OpenAPI_pc5_qo_s_para_t *pc5_qo_s_para_local_var = NULL;
    cJSON *pc5_qos_flow_list = cJSON_GetObjectItemCaseSensitive(pc5_qo_s_paraJSON, "pc5QosFlowList");
    if (!pc5_qos_flow_list) {
        ogs_error("OpenAPI_pc5_qo_s_para_parseFromJSON() failed [pc5_qos_flow_list]");
        goto end;
    }

    OpenAPI_list_t *pc5_qos_flow_listList;

    cJSON *pc5_qos_flow_list_local_nonprimitive;
    if (!cJSON_IsArray(pc5_qos_flow_list)) {
        ogs_error("OpenAPI_pc5_qo_s_para_parseFromJSON() failed [pc5_qos_flow_list]");
        goto end;
    }

    pc5_qos_flow_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(pc5_qos_flow_list_local_nonprimitive, pc5_qos_flow_list ) {
        if (!cJSON_IsObject(pc5_qos_flow_list_local_nonprimitive)) {
            ogs_error("OpenAPI_pc5_qo_s_para_parseFromJSON() failed [pc5_qos_flow_list]");
            goto end;
        }
        OpenAPI_pc5_qos_flow_item_t *pc5_qos_flow_listItem = OpenAPI_pc5_qos_flow_item_parseFromJSON(pc5_qos_flow_list_local_nonprimitive);

        OpenAPI_list_add(pc5_qos_flow_listList, pc5_qos_flow_listItem);
    }

    cJSON *pc5_link_ambr = cJSON_GetObjectItemCaseSensitive(pc5_qo_s_paraJSON, "pc5LinkAmbr");

    if (pc5_link_ambr) {
        if (!cJSON_IsString(pc5_link_ambr)) {
            ogs_error("OpenAPI_pc5_qo_s_para_parseFromJSON() failed [pc5_link_ambr]");
            goto end;
        }
    }

    pc5_qo_s_para_local_var = OpenAPI_pc5_qo_s_para_create (
        pc5_qos_flow_listList,
        pc5_link_ambr ? ogs_strdup(pc5_link_ambr->valuestring) : NULL
        );

    return pc5_qo_s_para_local_var;
end:
    return NULL;
}

OpenAPI_pc5_qo_s_para_t *OpenAPI_pc5_qo_s_para_copy(OpenAPI_pc5_qo_s_para_t *dst, OpenAPI_pc5_qo_s_para_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pc5_qo_s_para_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pc5_qo_s_para_convertToJSON() failed");
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

    OpenAPI_pc5_qo_s_para_free(dst);
    dst = OpenAPI_pc5_qo_s_para_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

