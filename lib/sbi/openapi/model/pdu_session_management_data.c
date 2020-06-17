
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pdu_session_management_data.h"

OpenAPI_pdu_session_management_data_t *OpenAPI_pdu_session_management_data_create(
    OpenAPI_pdu_session_status_t *pdu_session_status,
    char *pdu_session_status_ts,
    char *dnai,
    char *dnai_ts,
    OpenAPI_list_t *n6_traffic_routing_info,
    char *n6_traffic_routing_info_ts,
    char *ipv4_addr,
    OpenAPI_list_t *ipv6_prefix,
    OpenAPI_list_t *ipv6_addrs,
    OpenAPI_pdu_session_type_e pdu_sess_type,
    char *ip_addr_ts,
    char *dnn,
    int pdu_session_id
    )
{
    OpenAPI_pdu_session_management_data_t *pdu_session_management_data_local_var = OpenAPI_malloc(sizeof(OpenAPI_pdu_session_management_data_t));
    if (!pdu_session_management_data_local_var) {
        return NULL;
    }
    pdu_session_management_data_local_var->pdu_session_status = pdu_session_status;
    pdu_session_management_data_local_var->pdu_session_status_ts = pdu_session_status_ts;
    pdu_session_management_data_local_var->dnai = dnai;
    pdu_session_management_data_local_var->dnai_ts = dnai_ts;
    pdu_session_management_data_local_var->n6_traffic_routing_info = n6_traffic_routing_info;
    pdu_session_management_data_local_var->n6_traffic_routing_info_ts = n6_traffic_routing_info_ts;
    pdu_session_management_data_local_var->ipv4_addr = ipv4_addr;
    pdu_session_management_data_local_var->ipv6_prefix = ipv6_prefix;
    pdu_session_management_data_local_var->ipv6_addrs = ipv6_addrs;
    pdu_session_management_data_local_var->pdu_sess_type = pdu_sess_type;
    pdu_session_management_data_local_var->ip_addr_ts = ip_addr_ts;
    pdu_session_management_data_local_var->dnn = dnn;
    pdu_session_management_data_local_var->pdu_session_id = pdu_session_id;

    return pdu_session_management_data_local_var;
}

void OpenAPI_pdu_session_management_data_free(OpenAPI_pdu_session_management_data_t *pdu_session_management_data)
{
    if (NULL == pdu_session_management_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_pdu_session_status_free(pdu_session_management_data->pdu_session_status);
    ogs_free(pdu_session_management_data->pdu_session_status_ts);
    ogs_free(pdu_session_management_data->dnai);
    ogs_free(pdu_session_management_data->dnai_ts);
    OpenAPI_list_for_each(pdu_session_management_data->n6_traffic_routing_info, node) {
        OpenAPI_route_to_location_free(node->data);
    }
    OpenAPI_list_free(pdu_session_management_data->n6_traffic_routing_info);
    ogs_free(pdu_session_management_data->n6_traffic_routing_info_ts);
    ogs_free(pdu_session_management_data->ipv4_addr);
    OpenAPI_list_for_each(pdu_session_management_data->ipv6_prefix, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(pdu_session_management_data->ipv6_prefix);
    OpenAPI_list_for_each(pdu_session_management_data->ipv6_addrs, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(pdu_session_management_data->ipv6_addrs);
    ogs_free(pdu_session_management_data->ip_addr_ts);
    ogs_free(pdu_session_management_data->dnn);
    ogs_free(pdu_session_management_data);
}

cJSON *OpenAPI_pdu_session_management_data_convertToJSON(OpenAPI_pdu_session_management_data_t *pdu_session_management_data)
{
    cJSON *item = NULL;

    if (pdu_session_management_data == NULL) {
        ogs_error("OpenAPI_pdu_session_management_data_convertToJSON() failed [PduSessionManagementData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (pdu_session_management_data->pdu_session_status) {
        cJSON *pdu_session_status_local_JSON = OpenAPI_pdu_session_status_convertToJSON(pdu_session_management_data->pdu_session_status);
        if (pdu_session_status_local_JSON == NULL) {
            ogs_error("OpenAPI_pdu_session_management_data_convertToJSON() failed [pdu_session_status]");
            goto end;
        }
        cJSON_AddItemToObject(item, "pduSessionStatus", pdu_session_status_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_pdu_session_management_data_convertToJSON() failed [pdu_session_status]");
            goto end;
        }
    }

    if (pdu_session_management_data->pdu_session_status_ts) {
        if (cJSON_AddStringToObject(item, "pduSessionStatusTs", pdu_session_management_data->pdu_session_status_ts) == NULL) {
            ogs_error("OpenAPI_pdu_session_management_data_convertToJSON() failed [pdu_session_status_ts]");
            goto end;
        }
    }

    if (pdu_session_management_data->dnai) {
        if (cJSON_AddStringToObject(item, "dnai", pdu_session_management_data->dnai) == NULL) {
            ogs_error("OpenAPI_pdu_session_management_data_convertToJSON() failed [dnai]");
            goto end;
        }
    }

    if (pdu_session_management_data->dnai_ts) {
        if (cJSON_AddStringToObject(item, "dnaiTs", pdu_session_management_data->dnai_ts) == NULL) {
            ogs_error("OpenAPI_pdu_session_management_data_convertToJSON() failed [dnai_ts]");
            goto end;
        }
    }

    if (pdu_session_management_data->n6_traffic_routing_info) {
        cJSON *n6_traffic_routing_infoList = cJSON_AddArrayToObject(item, "n6TrafficRoutingInfo");
        if (n6_traffic_routing_infoList == NULL) {
            ogs_error("OpenAPI_pdu_session_management_data_convertToJSON() failed [n6_traffic_routing_info]");
            goto end;
        }

        OpenAPI_lnode_t *n6_traffic_routing_info_node;
        if (pdu_session_management_data->n6_traffic_routing_info) {
            OpenAPI_list_for_each(pdu_session_management_data->n6_traffic_routing_info, n6_traffic_routing_info_node) {
                cJSON *itemLocal = OpenAPI_route_to_location_convertToJSON(n6_traffic_routing_info_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_pdu_session_management_data_convertToJSON() failed [n6_traffic_routing_info]");
                    goto end;
                }
                cJSON_AddItemToArray(n6_traffic_routing_infoList, itemLocal);
            }
        }
    }

    if (pdu_session_management_data->n6_traffic_routing_info_ts) {
        if (cJSON_AddStringToObject(item, "n6TrafficRoutingInfoTs", pdu_session_management_data->n6_traffic_routing_info_ts) == NULL) {
            ogs_error("OpenAPI_pdu_session_management_data_convertToJSON() failed [n6_traffic_routing_info_ts]");
            goto end;
        }
    }

    if (pdu_session_management_data->ipv4_addr) {
        if (cJSON_AddStringToObject(item, "ipv4Addr", pdu_session_management_data->ipv4_addr) == NULL) {
            ogs_error("OpenAPI_pdu_session_management_data_convertToJSON() failed [ipv4_addr]");
            goto end;
        }
    }

    if (pdu_session_management_data->ipv6_prefix) {
        cJSON *ipv6_prefix = cJSON_AddArrayToObject(item, "ipv6Prefix");
        if (ipv6_prefix == NULL) {
            ogs_error("OpenAPI_pdu_session_management_data_convertToJSON() failed [ipv6_prefix]");
            goto end;
        }

        OpenAPI_lnode_t *ipv6_prefix_node;
        OpenAPI_list_for_each(pdu_session_management_data->ipv6_prefix, ipv6_prefix_node)  {
            if (cJSON_AddStringToObject(ipv6_prefix, "", (char*)ipv6_prefix_node->data) == NULL) {
                ogs_error("OpenAPI_pdu_session_management_data_convertToJSON() failed [ipv6_prefix]");
                goto end;
            }
        }
    }

    if (pdu_session_management_data->ipv6_addrs) {
        cJSON *ipv6_addrs = cJSON_AddArrayToObject(item, "ipv6Addrs");
        if (ipv6_addrs == NULL) {
            ogs_error("OpenAPI_pdu_session_management_data_convertToJSON() failed [ipv6_addrs]");
            goto end;
        }

        OpenAPI_lnode_t *ipv6_addrs_node;
        OpenAPI_list_for_each(pdu_session_management_data->ipv6_addrs, ipv6_addrs_node)  {
            if (cJSON_AddStringToObject(ipv6_addrs, "", (char*)ipv6_addrs_node->data) == NULL) {
                ogs_error("OpenAPI_pdu_session_management_data_convertToJSON() failed [ipv6_addrs]");
                goto end;
            }
        }
    }

    if (pdu_session_management_data->pdu_sess_type) {
        if (cJSON_AddStringToObject(item, "pduSessType", OpenAPI_pdu_session_type_ToString(pdu_session_management_data->pdu_sess_type)) == NULL) {
            ogs_error("OpenAPI_pdu_session_management_data_convertToJSON() failed [pdu_sess_type]");
            goto end;
        }
    }

    if (pdu_session_management_data->ip_addr_ts) {
        if (cJSON_AddStringToObject(item, "ipAddrTs", pdu_session_management_data->ip_addr_ts) == NULL) {
            ogs_error("OpenAPI_pdu_session_management_data_convertToJSON() failed [ip_addr_ts]");
            goto end;
        }
    }

    if (pdu_session_management_data->dnn) {
        if (cJSON_AddStringToObject(item, "dnn", pdu_session_management_data->dnn) == NULL) {
            ogs_error("OpenAPI_pdu_session_management_data_convertToJSON() failed [dnn]");
            goto end;
        }
    }

    if (pdu_session_management_data->pdu_session_id) {
        if (cJSON_AddNumberToObject(item, "pduSessionId", pdu_session_management_data->pdu_session_id) == NULL) {
            ogs_error("OpenAPI_pdu_session_management_data_convertToJSON() failed [pdu_session_id]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_pdu_session_management_data_t *OpenAPI_pdu_session_management_data_parseFromJSON(cJSON *pdu_session_management_dataJSON)
{
    OpenAPI_pdu_session_management_data_t *pdu_session_management_data_local_var = NULL;
    cJSON *pdu_session_status = cJSON_GetObjectItemCaseSensitive(pdu_session_management_dataJSON, "pduSessionStatus");

    OpenAPI_pdu_session_status_t *pdu_session_status_local_nonprim = NULL;
    if (pdu_session_status) {
        pdu_session_status_local_nonprim = OpenAPI_pdu_session_status_parseFromJSON(pdu_session_status);
    }

    cJSON *pdu_session_status_ts = cJSON_GetObjectItemCaseSensitive(pdu_session_management_dataJSON, "pduSessionStatusTs");

    if (pdu_session_status_ts) {
        if (!cJSON_IsString(pdu_session_status_ts)) {
            ogs_error("OpenAPI_pdu_session_management_data_parseFromJSON() failed [pdu_session_status_ts]");
            goto end;
        }
    }

    cJSON *dnai = cJSON_GetObjectItemCaseSensitive(pdu_session_management_dataJSON, "dnai");

    if (dnai) {
        if (!cJSON_IsString(dnai)) {
            ogs_error("OpenAPI_pdu_session_management_data_parseFromJSON() failed [dnai]");
            goto end;
        }
    }

    cJSON *dnai_ts = cJSON_GetObjectItemCaseSensitive(pdu_session_management_dataJSON, "dnaiTs");

    if (dnai_ts) {
        if (!cJSON_IsString(dnai_ts)) {
            ogs_error("OpenAPI_pdu_session_management_data_parseFromJSON() failed [dnai_ts]");
            goto end;
        }
    }

    cJSON *n6_traffic_routing_info = cJSON_GetObjectItemCaseSensitive(pdu_session_management_dataJSON, "n6TrafficRoutingInfo");

    OpenAPI_list_t *n6_traffic_routing_infoList;
    if (n6_traffic_routing_info) {
        cJSON *n6_traffic_routing_info_local_nonprimitive;
        if (!cJSON_IsArray(n6_traffic_routing_info)) {
            ogs_error("OpenAPI_pdu_session_management_data_parseFromJSON() failed [n6_traffic_routing_info]");
            goto end;
        }

        n6_traffic_routing_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(n6_traffic_routing_info_local_nonprimitive, n6_traffic_routing_info ) {
            if (!cJSON_IsObject(n6_traffic_routing_info_local_nonprimitive)) {
                ogs_error("OpenAPI_pdu_session_management_data_parseFromJSON() failed [n6_traffic_routing_info]");
                goto end;
            }
            OpenAPI_route_to_location_t *n6_traffic_routing_infoItem = OpenAPI_route_to_location_parseFromJSON(n6_traffic_routing_info_local_nonprimitive);

            OpenAPI_list_add(n6_traffic_routing_infoList, n6_traffic_routing_infoItem);
        }
    }

    cJSON *n6_traffic_routing_info_ts = cJSON_GetObjectItemCaseSensitive(pdu_session_management_dataJSON, "n6TrafficRoutingInfoTs");

    if (n6_traffic_routing_info_ts) {
        if (!cJSON_IsString(n6_traffic_routing_info_ts)) {
            ogs_error("OpenAPI_pdu_session_management_data_parseFromJSON() failed [n6_traffic_routing_info_ts]");
            goto end;
        }
    }

    cJSON *ipv4_addr = cJSON_GetObjectItemCaseSensitive(pdu_session_management_dataJSON, "ipv4Addr");

    if (ipv4_addr) {
        if (!cJSON_IsString(ipv4_addr)) {
            ogs_error("OpenAPI_pdu_session_management_data_parseFromJSON() failed [ipv4_addr]");
            goto end;
        }
    }

    cJSON *ipv6_prefix = cJSON_GetObjectItemCaseSensitive(pdu_session_management_dataJSON, "ipv6Prefix");

    OpenAPI_list_t *ipv6_prefixList;
    if (ipv6_prefix) {
        cJSON *ipv6_prefix_local;
        if (!cJSON_IsArray(ipv6_prefix)) {
            ogs_error("OpenAPI_pdu_session_management_data_parseFromJSON() failed [ipv6_prefix]");
            goto end;
        }
        ipv6_prefixList = OpenAPI_list_create();

        cJSON_ArrayForEach(ipv6_prefix_local, ipv6_prefix) {
            if (!cJSON_IsString(ipv6_prefix_local)) {
                ogs_error("OpenAPI_pdu_session_management_data_parseFromJSON() failed [ipv6_prefix]");
                goto end;
            }
            OpenAPI_list_add(ipv6_prefixList, ogs_strdup(ipv6_prefix_local->valuestring));
        }
    }

    cJSON *ipv6_addrs = cJSON_GetObjectItemCaseSensitive(pdu_session_management_dataJSON, "ipv6Addrs");

    OpenAPI_list_t *ipv6_addrsList;
    if (ipv6_addrs) {
        cJSON *ipv6_addrs_local;
        if (!cJSON_IsArray(ipv6_addrs)) {
            ogs_error("OpenAPI_pdu_session_management_data_parseFromJSON() failed [ipv6_addrs]");
            goto end;
        }
        ipv6_addrsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ipv6_addrs_local, ipv6_addrs) {
            if (!cJSON_IsString(ipv6_addrs_local)) {
                ogs_error("OpenAPI_pdu_session_management_data_parseFromJSON() failed [ipv6_addrs]");
                goto end;
            }
            OpenAPI_list_add(ipv6_addrsList, ogs_strdup(ipv6_addrs_local->valuestring));
        }
    }

    cJSON *pdu_sess_type = cJSON_GetObjectItemCaseSensitive(pdu_session_management_dataJSON, "pduSessType");

    OpenAPI_pdu_session_type_e pdu_sess_typeVariable;
    if (pdu_sess_type) {
        if (!cJSON_IsString(pdu_sess_type)) {
            ogs_error("OpenAPI_pdu_session_management_data_parseFromJSON() failed [pdu_sess_type]");
            goto end;
        }
        pdu_sess_typeVariable = OpenAPI_pdu_session_type_FromString(pdu_sess_type->valuestring);
    }

    cJSON *ip_addr_ts = cJSON_GetObjectItemCaseSensitive(pdu_session_management_dataJSON, "ipAddrTs");

    if (ip_addr_ts) {
        if (!cJSON_IsString(ip_addr_ts)) {
            ogs_error("OpenAPI_pdu_session_management_data_parseFromJSON() failed [ip_addr_ts]");
            goto end;
        }
    }

    cJSON *dnn = cJSON_GetObjectItemCaseSensitive(pdu_session_management_dataJSON, "dnn");

    if (dnn) {
        if (!cJSON_IsString(dnn)) {
            ogs_error("OpenAPI_pdu_session_management_data_parseFromJSON() failed [dnn]");
            goto end;
        }
    }

    cJSON *pdu_session_id = cJSON_GetObjectItemCaseSensitive(pdu_session_management_dataJSON, "pduSessionId");

    if (pdu_session_id) {
        if (!cJSON_IsNumber(pdu_session_id)) {
            ogs_error("OpenAPI_pdu_session_management_data_parseFromJSON() failed [pdu_session_id]");
            goto end;
        }
    }

    pdu_session_management_data_local_var = OpenAPI_pdu_session_management_data_create (
        pdu_session_status ? pdu_session_status_local_nonprim : NULL,
        pdu_session_status_ts ? ogs_strdup(pdu_session_status_ts->valuestring) : NULL,
        dnai ? ogs_strdup(dnai->valuestring) : NULL,
        dnai_ts ? ogs_strdup(dnai_ts->valuestring) : NULL,
        n6_traffic_routing_info ? n6_traffic_routing_infoList : NULL,
        n6_traffic_routing_info_ts ? ogs_strdup(n6_traffic_routing_info_ts->valuestring) : NULL,
        ipv4_addr ? ogs_strdup(ipv4_addr->valuestring) : NULL,
        ipv6_prefix ? ipv6_prefixList : NULL,
        ipv6_addrs ? ipv6_addrsList : NULL,
        pdu_sess_type ? pdu_sess_typeVariable : 0,
        ip_addr_ts ? ogs_strdup(ip_addr_ts->valuestring) : NULL,
        dnn ? ogs_strdup(dnn->valuestring) : NULL,
        pdu_session_id ? pdu_session_id->valuedouble : 0
        );

    return pdu_session_management_data_local_var;
end:
    return NULL;
}

OpenAPI_pdu_session_management_data_t *OpenAPI_pdu_session_management_data_copy(OpenAPI_pdu_session_management_data_t *dst, OpenAPI_pdu_session_management_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pdu_session_management_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pdu_session_management_data_convertToJSON() failed");
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

    OpenAPI_pdu_session_management_data_free(dst);
    dst = OpenAPI_pdu_session_management_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

