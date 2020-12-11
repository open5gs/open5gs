
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tsn_bridge_info.h"

OpenAPI_tsn_bridge_info_t *OpenAPI_tsn_bridge_info_create(
    char *bridge_name,
    char *bridge_mac,
    OpenAPI_list_t *nwtt_ports,
    OpenAPI_tsn_port_identifier_t *dstt_port,
    int dstt_resid_time
    )
{
    OpenAPI_tsn_bridge_info_t *tsn_bridge_info_local_var = OpenAPI_malloc(sizeof(OpenAPI_tsn_bridge_info_t));
    if (!tsn_bridge_info_local_var) {
        return NULL;
    }
    tsn_bridge_info_local_var->bridge_name = bridge_name;
    tsn_bridge_info_local_var->bridge_mac = bridge_mac;
    tsn_bridge_info_local_var->nwtt_ports = nwtt_ports;
    tsn_bridge_info_local_var->dstt_port = dstt_port;
    tsn_bridge_info_local_var->dstt_resid_time = dstt_resid_time;

    return tsn_bridge_info_local_var;
}

void OpenAPI_tsn_bridge_info_free(OpenAPI_tsn_bridge_info_t *tsn_bridge_info)
{
    if (NULL == tsn_bridge_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(tsn_bridge_info->bridge_name);
    ogs_free(tsn_bridge_info->bridge_mac);
    OpenAPI_list_for_each(tsn_bridge_info->nwtt_ports, node) {
        OpenAPI_tsn_port_identifier_free(node->data);
    }
    OpenAPI_list_free(tsn_bridge_info->nwtt_ports);
    OpenAPI_tsn_port_identifier_free(tsn_bridge_info->dstt_port);
    ogs_free(tsn_bridge_info);
}

cJSON *OpenAPI_tsn_bridge_info_convertToJSON(OpenAPI_tsn_bridge_info_t *tsn_bridge_info)
{
    cJSON *item = NULL;

    if (tsn_bridge_info == NULL) {
        ogs_error("OpenAPI_tsn_bridge_info_convertToJSON() failed [TsnBridgeInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (tsn_bridge_info->bridge_name) {
        if (cJSON_AddStringToObject(item, "bridgeName", tsn_bridge_info->bridge_name) == NULL) {
            ogs_error("OpenAPI_tsn_bridge_info_convertToJSON() failed [bridge_name]");
            goto end;
        }
    }

    if (tsn_bridge_info->bridge_mac) {
        if (cJSON_AddStringToObject(item, "bridgeMac", tsn_bridge_info->bridge_mac) == NULL) {
            ogs_error("OpenAPI_tsn_bridge_info_convertToJSON() failed [bridge_mac]");
            goto end;
        }
    }

    if (tsn_bridge_info->nwtt_ports) {
        cJSON *nwtt_portsList = cJSON_AddArrayToObject(item, "nwttPorts");
        if (nwtt_portsList == NULL) {
            ogs_error("OpenAPI_tsn_bridge_info_convertToJSON() failed [nwtt_ports]");
            goto end;
        }

        OpenAPI_lnode_t *nwtt_ports_node;
        if (tsn_bridge_info->nwtt_ports) {
            OpenAPI_list_for_each(tsn_bridge_info->nwtt_ports, nwtt_ports_node) {
                cJSON *itemLocal = OpenAPI_tsn_port_identifier_convertToJSON(nwtt_ports_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_tsn_bridge_info_convertToJSON() failed [nwtt_ports]");
                    goto end;
                }
                cJSON_AddItemToArray(nwtt_portsList, itemLocal);
            }
        }
    }

    if (tsn_bridge_info->dstt_port) {
        cJSON *dstt_port_local_JSON = OpenAPI_tsn_port_identifier_convertToJSON(tsn_bridge_info->dstt_port);
        if (dstt_port_local_JSON == NULL) {
            ogs_error("OpenAPI_tsn_bridge_info_convertToJSON() failed [dstt_port]");
            goto end;
        }
        cJSON_AddItemToObject(item, "dsttPort", dstt_port_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_tsn_bridge_info_convertToJSON() failed [dstt_port]");
            goto end;
        }
    }

    if (tsn_bridge_info->dstt_resid_time) {
        if (cJSON_AddNumberToObject(item, "dsttResidTime", tsn_bridge_info->dstt_resid_time) == NULL) {
            ogs_error("OpenAPI_tsn_bridge_info_convertToJSON() failed [dstt_resid_time]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_tsn_bridge_info_t *OpenAPI_tsn_bridge_info_parseFromJSON(cJSON *tsn_bridge_infoJSON)
{
    OpenAPI_tsn_bridge_info_t *tsn_bridge_info_local_var = NULL;
    cJSON *bridge_name = cJSON_GetObjectItemCaseSensitive(tsn_bridge_infoJSON, "bridgeName");

    if (bridge_name) {
        if (!cJSON_IsString(bridge_name)) {
            ogs_error("OpenAPI_tsn_bridge_info_parseFromJSON() failed [bridge_name]");
            goto end;
        }
    }

    cJSON *bridge_mac = cJSON_GetObjectItemCaseSensitive(tsn_bridge_infoJSON, "bridgeMac");

    if (bridge_mac) {
        if (!cJSON_IsString(bridge_mac)) {
            ogs_error("OpenAPI_tsn_bridge_info_parseFromJSON() failed [bridge_mac]");
            goto end;
        }
    }

    cJSON *nwtt_ports = cJSON_GetObjectItemCaseSensitive(tsn_bridge_infoJSON, "nwttPorts");

    OpenAPI_list_t *nwtt_portsList;
    if (nwtt_ports) {
        cJSON *nwtt_ports_local_nonprimitive;
        if (!cJSON_IsArray(nwtt_ports)) {
            ogs_error("OpenAPI_tsn_bridge_info_parseFromJSON() failed [nwtt_ports]");
            goto end;
        }

        nwtt_portsList = OpenAPI_list_create();

        cJSON_ArrayForEach(nwtt_ports_local_nonprimitive, nwtt_ports ) {
            if (!cJSON_IsObject(nwtt_ports_local_nonprimitive)) {
                ogs_error("OpenAPI_tsn_bridge_info_parseFromJSON() failed [nwtt_ports]");
                goto end;
            }
            OpenAPI_tsn_port_identifier_t *nwtt_portsItem = OpenAPI_tsn_port_identifier_parseFromJSON(nwtt_ports_local_nonprimitive);

            OpenAPI_list_add(nwtt_portsList, nwtt_portsItem);
        }
    }

    cJSON *dstt_port = cJSON_GetObjectItemCaseSensitive(tsn_bridge_infoJSON, "dsttPort");

    OpenAPI_tsn_port_identifier_t *dstt_port_local_nonprim = NULL;
    if (dstt_port) {
        dstt_port_local_nonprim = OpenAPI_tsn_port_identifier_parseFromJSON(dstt_port);
    }

    cJSON *dstt_resid_time = cJSON_GetObjectItemCaseSensitive(tsn_bridge_infoJSON, "dsttResidTime");

    if (dstt_resid_time) {
        if (!cJSON_IsNumber(dstt_resid_time)) {
            ogs_error("OpenAPI_tsn_bridge_info_parseFromJSON() failed [dstt_resid_time]");
            goto end;
        }
    }

    tsn_bridge_info_local_var = OpenAPI_tsn_bridge_info_create (
        bridge_name ? ogs_strdup(bridge_name->valuestring) : NULL,
        bridge_mac ? ogs_strdup(bridge_mac->valuestring) : NULL,
        nwtt_ports ? nwtt_portsList : NULL,
        dstt_port ? dstt_port_local_nonprim : NULL,
        dstt_resid_time ? dstt_resid_time->valuedouble : 0
        );

    return tsn_bridge_info_local_var;
end:
    return NULL;
}

OpenAPI_tsn_bridge_info_t *OpenAPI_tsn_bridge_info_copy(OpenAPI_tsn_bridge_info_t *dst, OpenAPI_tsn_bridge_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_tsn_bridge_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_tsn_bridge_info_convertToJSON() failed");
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

    OpenAPI_tsn_bridge_info_free(dst);
    dst = OpenAPI_tsn_bridge_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

