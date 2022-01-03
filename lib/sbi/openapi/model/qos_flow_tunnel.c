
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "qos_flow_tunnel.h"

OpenAPI_qos_flow_tunnel_t *OpenAPI_qos_flow_tunnel_create(
    OpenAPI_list_t *qfi_list,
    OpenAPI_tunnel_info_t *tunnel_info
)
{
    OpenAPI_qos_flow_tunnel_t *qos_flow_tunnel_local_var = ogs_malloc(sizeof(OpenAPI_qos_flow_tunnel_t));
    ogs_assert(qos_flow_tunnel_local_var);

    qos_flow_tunnel_local_var->qfi_list = qfi_list;
    qos_flow_tunnel_local_var->tunnel_info = tunnel_info;

    return qos_flow_tunnel_local_var;
}

void OpenAPI_qos_flow_tunnel_free(OpenAPI_qos_flow_tunnel_t *qos_flow_tunnel)
{
    if (NULL == qos_flow_tunnel) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(qos_flow_tunnel->qfi_list, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(qos_flow_tunnel->qfi_list);
    OpenAPI_tunnel_info_free(qos_flow_tunnel->tunnel_info);
    ogs_free(qos_flow_tunnel);
}

cJSON *OpenAPI_qos_flow_tunnel_convertToJSON(OpenAPI_qos_flow_tunnel_t *qos_flow_tunnel)
{
    cJSON *item = NULL;

    if (qos_flow_tunnel == NULL) {
        ogs_error("OpenAPI_qos_flow_tunnel_convertToJSON() failed [QosFlowTunnel]");
        return NULL;
    }

    item = cJSON_CreateObject();
    cJSON *qfi_list = cJSON_AddArrayToObject(item, "qfiList");
    if (qfi_list == NULL) {
        ogs_error("OpenAPI_qos_flow_tunnel_convertToJSON() failed [qfi_list]");
        goto end;
    }

    OpenAPI_lnode_t *qfi_list_node;
    OpenAPI_list_for_each(qos_flow_tunnel->qfi_list, qfi_list_node)  {
    if (cJSON_AddNumberToObject(qfi_list, "", *(double *)qfi_list_node->data) == NULL) {
        ogs_error("OpenAPI_qos_flow_tunnel_convertToJSON() failed [qfi_list]");
        goto end;
    }
                    }

    cJSON *tunnel_info_local_JSON = OpenAPI_tunnel_info_convertToJSON(qos_flow_tunnel->tunnel_info);
    if (tunnel_info_local_JSON == NULL) {
        ogs_error("OpenAPI_qos_flow_tunnel_convertToJSON() failed [tunnel_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "tunnelInfo", tunnel_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_qos_flow_tunnel_convertToJSON() failed [tunnel_info]");
        goto end;
    }

end:
    return item;
}

OpenAPI_qos_flow_tunnel_t *OpenAPI_qos_flow_tunnel_parseFromJSON(cJSON *qos_flow_tunnelJSON)
{
    OpenAPI_qos_flow_tunnel_t *qos_flow_tunnel_local_var = NULL;
    cJSON *qfi_list = cJSON_GetObjectItemCaseSensitive(qos_flow_tunnelJSON, "qfiList");
    if (!qfi_list) {
        ogs_error("OpenAPI_qos_flow_tunnel_parseFromJSON() failed [qfi_list]");
        goto end;
    }

    OpenAPI_list_t *qfi_listList;
    cJSON *qfi_list_local;
    if (!cJSON_IsArray(qfi_list)) {
        ogs_error("OpenAPI_qos_flow_tunnel_parseFromJSON() failed [qfi_list]");
        goto end;
    }
    qfi_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(qfi_list_local, qfi_list) {
    if (!cJSON_IsNumber(qfi_list_local)) {
        ogs_error("OpenAPI_qos_flow_tunnel_parseFromJSON() failed [qfi_list]");
        goto end;
    }
    OpenAPI_list_add(qfi_listList , &qfi_list_local->valuedouble);
    }

    cJSON *tunnel_info = cJSON_GetObjectItemCaseSensitive(qos_flow_tunnelJSON, "tunnelInfo");
    if (!tunnel_info) {
        ogs_error("OpenAPI_qos_flow_tunnel_parseFromJSON() failed [tunnel_info]");
        goto end;
    }

    OpenAPI_tunnel_info_t *tunnel_info_local_nonprim = NULL;
    tunnel_info_local_nonprim = OpenAPI_tunnel_info_parseFromJSON(tunnel_info);

    qos_flow_tunnel_local_var = OpenAPI_qos_flow_tunnel_create (
        qfi_listList,
        tunnel_info_local_nonprim
    );

    return qos_flow_tunnel_local_var;
end:
    return NULL;
}

OpenAPI_qos_flow_tunnel_t *OpenAPI_qos_flow_tunnel_copy(OpenAPI_qos_flow_tunnel_t *dst, OpenAPI_qos_flow_tunnel_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_qos_flow_tunnel_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_qos_flow_tunnel_convertToJSON() failed");
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

    OpenAPI_qos_flow_tunnel_free(dst);
    dst = OpenAPI_qos_flow_tunnel_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

