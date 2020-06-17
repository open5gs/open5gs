
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pc5_qos_mapping.h"

OpenAPI_pc5_qos_mapping_t *OpenAPI_pc5_qos_mapping_create(
    OpenAPI_list_t *ser_ids,
    OpenAPI_pc5_qos_flow_item_t *pc5_qo_s_flow_param
    )
{
    OpenAPI_pc5_qos_mapping_t *pc5_qos_mapping_local_var = OpenAPI_malloc(sizeof(OpenAPI_pc5_qos_mapping_t));
    if (!pc5_qos_mapping_local_var) {
        return NULL;
    }
    pc5_qos_mapping_local_var->ser_ids = ser_ids;
    pc5_qos_mapping_local_var->pc5_qo_s_flow_param = pc5_qo_s_flow_param;

    return pc5_qos_mapping_local_var;
}

void OpenAPI_pc5_qos_mapping_free(OpenAPI_pc5_qos_mapping_t *pc5_qos_mapping)
{
    if (NULL == pc5_qos_mapping) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(pc5_qos_mapping->ser_ids, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(pc5_qos_mapping->ser_ids);
    OpenAPI_pc5_qos_flow_item_free(pc5_qos_mapping->pc5_qo_s_flow_param);
    ogs_free(pc5_qos_mapping);
}

cJSON *OpenAPI_pc5_qos_mapping_convertToJSON(OpenAPI_pc5_qos_mapping_t *pc5_qos_mapping)
{
    cJSON *item = NULL;

    if (pc5_qos_mapping == NULL) {
        ogs_error("OpenAPI_pc5_qos_mapping_convertToJSON() failed [Pc5QosMapping]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!pc5_qos_mapping->ser_ids) {
        ogs_error("OpenAPI_pc5_qos_mapping_convertToJSON() failed [ser_ids]");
        goto end;
    }
    cJSON *ser_ids = cJSON_AddArrayToObject(item, "serIds");
    if (ser_ids == NULL) {
        ogs_error("OpenAPI_pc5_qos_mapping_convertToJSON() failed [ser_ids]");
        goto end;
    }

    OpenAPI_lnode_t *ser_ids_node;
    OpenAPI_list_for_each(pc5_qos_mapping->ser_ids, ser_ids_node)  {
        if (cJSON_AddStringToObject(ser_ids, "", (char*)ser_ids_node->data) == NULL) {
            ogs_error("OpenAPI_pc5_qos_mapping_convertToJSON() failed [ser_ids]");
            goto end;
        }
    }

    if (pc5_qos_mapping->pc5_qo_s_flow_param) {
        cJSON *pc5_qo_s_flow_param_local_JSON = OpenAPI_pc5_qos_flow_item_convertToJSON(pc5_qos_mapping->pc5_qo_s_flow_param);
        if (pc5_qo_s_flow_param_local_JSON == NULL) {
            ogs_error("OpenAPI_pc5_qos_mapping_convertToJSON() failed [pc5_qo_s_flow_param]");
            goto end;
        }
        cJSON_AddItemToObject(item, "pc5QoSFlowParam", pc5_qo_s_flow_param_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_pc5_qos_mapping_convertToJSON() failed [pc5_qo_s_flow_param]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_pc5_qos_mapping_t *OpenAPI_pc5_qos_mapping_parseFromJSON(cJSON *pc5_qos_mappingJSON)
{
    OpenAPI_pc5_qos_mapping_t *pc5_qos_mapping_local_var = NULL;
    cJSON *ser_ids = cJSON_GetObjectItemCaseSensitive(pc5_qos_mappingJSON, "serIds");
    if (!ser_ids) {
        ogs_error("OpenAPI_pc5_qos_mapping_parseFromJSON() failed [ser_ids]");
        goto end;
    }

    OpenAPI_list_t *ser_idsList;

    cJSON *ser_ids_local;
    if (!cJSON_IsArray(ser_ids)) {
        ogs_error("OpenAPI_pc5_qos_mapping_parseFromJSON() failed [ser_ids]");
        goto end;
    }
    ser_idsList = OpenAPI_list_create();

    cJSON_ArrayForEach(ser_ids_local, ser_ids) {
        if (!cJSON_IsString(ser_ids_local)) {
            ogs_error("OpenAPI_pc5_qos_mapping_parseFromJSON() failed [ser_ids]");
            goto end;
        }
        OpenAPI_list_add(ser_idsList, ogs_strdup(ser_ids_local->valuestring));
    }

    cJSON *pc5_qo_s_flow_param = cJSON_GetObjectItemCaseSensitive(pc5_qos_mappingJSON, "pc5QoSFlowParam");

    OpenAPI_pc5_qos_flow_item_t *pc5_qo_s_flow_param_local_nonprim = NULL;
    if (pc5_qo_s_flow_param) {
        pc5_qo_s_flow_param_local_nonprim = OpenAPI_pc5_qos_flow_item_parseFromJSON(pc5_qo_s_flow_param);
    }

    pc5_qos_mapping_local_var = OpenAPI_pc5_qos_mapping_create (
        ser_idsList,
        pc5_qo_s_flow_param ? pc5_qo_s_flow_param_local_nonprim : NULL
        );

    return pc5_qos_mapping_local_var;
end:
    return NULL;
}

OpenAPI_pc5_qos_mapping_t *OpenAPI_pc5_qos_mapping_copy(OpenAPI_pc5_qos_mapping_t *dst, OpenAPI_pc5_qos_mapping_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pc5_qos_mapping_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pc5_qos_mapping_convertToJSON() failed");
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

    OpenAPI_pc5_qos_mapping_free(dst);
    dst = OpenAPI_pc5_qos_mapping_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

