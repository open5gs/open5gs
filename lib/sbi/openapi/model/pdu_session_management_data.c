
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pdu_session_management_data.h"

OpenAPI_pdu_session_management_data_t *OpenAPI_pdu_session_management_data_create(
    OpenAPI_pdu_session_status_e pdu_session_status,
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
    bool is_pdu_session_id,
    int pdu_session_id,
    char *supp_feat,
    OpenAPI_list_t *reset_ids
)
{
    OpenAPI_pdu_session_management_data_t *pdu_session_management_data_local_var = ogs_malloc(sizeof(OpenAPI_pdu_session_management_data_t));
    ogs_assert(pdu_session_management_data_local_var);

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
    pdu_session_management_data_local_var->is_pdu_session_id = is_pdu_session_id;
    pdu_session_management_data_local_var->pdu_session_id = pdu_session_id;
    pdu_session_management_data_local_var->supp_feat = supp_feat;
    pdu_session_management_data_local_var->reset_ids = reset_ids;

    return pdu_session_management_data_local_var;
}

void OpenAPI_pdu_session_management_data_free(OpenAPI_pdu_session_management_data_t *pdu_session_management_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pdu_session_management_data) {
        return;
    }
    if (pdu_session_management_data->pdu_session_status_ts) {
        ogs_free(pdu_session_management_data->pdu_session_status_ts);
        pdu_session_management_data->pdu_session_status_ts = NULL;
    }
    if (pdu_session_management_data->dnai) {
        ogs_free(pdu_session_management_data->dnai);
        pdu_session_management_data->dnai = NULL;
    }
    if (pdu_session_management_data->dnai_ts) {
        ogs_free(pdu_session_management_data->dnai_ts);
        pdu_session_management_data->dnai_ts = NULL;
    }
    if (pdu_session_management_data->n6_traffic_routing_info) {
        OpenAPI_list_for_each(pdu_session_management_data->n6_traffic_routing_info, node) {
            OpenAPI_route_to_location_free(node->data);
        }
        OpenAPI_list_free(pdu_session_management_data->n6_traffic_routing_info);
        pdu_session_management_data->n6_traffic_routing_info = NULL;
    }
    if (pdu_session_management_data->n6_traffic_routing_info_ts) {
        ogs_free(pdu_session_management_data->n6_traffic_routing_info_ts);
        pdu_session_management_data->n6_traffic_routing_info_ts = NULL;
    }
    if (pdu_session_management_data->ipv4_addr) {
        ogs_free(pdu_session_management_data->ipv4_addr);
        pdu_session_management_data->ipv4_addr = NULL;
    }
    if (pdu_session_management_data->ipv6_prefix) {
        OpenAPI_list_for_each(pdu_session_management_data->ipv6_prefix, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(pdu_session_management_data->ipv6_prefix);
        pdu_session_management_data->ipv6_prefix = NULL;
    }
    if (pdu_session_management_data->ipv6_addrs) {
        OpenAPI_list_for_each(pdu_session_management_data->ipv6_addrs, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(pdu_session_management_data->ipv6_addrs);
        pdu_session_management_data->ipv6_addrs = NULL;
    }
    if (pdu_session_management_data->ip_addr_ts) {
        ogs_free(pdu_session_management_data->ip_addr_ts);
        pdu_session_management_data->ip_addr_ts = NULL;
    }
    if (pdu_session_management_data->dnn) {
        ogs_free(pdu_session_management_data->dnn);
        pdu_session_management_data->dnn = NULL;
    }
    if (pdu_session_management_data->supp_feat) {
        ogs_free(pdu_session_management_data->supp_feat);
        pdu_session_management_data->supp_feat = NULL;
    }
    if (pdu_session_management_data->reset_ids) {
        OpenAPI_list_for_each(pdu_session_management_data->reset_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(pdu_session_management_data->reset_ids);
        pdu_session_management_data->reset_ids = NULL;
    }
    ogs_free(pdu_session_management_data);
}

cJSON *OpenAPI_pdu_session_management_data_convertToJSON(OpenAPI_pdu_session_management_data_t *pdu_session_management_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pdu_session_management_data == NULL) {
        ogs_error("OpenAPI_pdu_session_management_data_convertToJSON() failed [PduSessionManagementData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (pdu_session_management_data->pdu_session_status != OpenAPI_pdu_session_status_NULL) {
    if (cJSON_AddStringToObject(item, "pduSessionStatus", OpenAPI_pdu_session_status_ToString(pdu_session_management_data->pdu_session_status)) == NULL) {
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
    OpenAPI_list_for_each(pdu_session_management_data->n6_traffic_routing_info, node) {
        cJSON *itemLocal = OpenAPI_route_to_location_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_pdu_session_management_data_convertToJSON() failed [n6_traffic_routing_info]");
            goto end;
        }
        cJSON_AddItemToArray(n6_traffic_routing_infoList, itemLocal);
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
    cJSON *ipv6_prefixList = cJSON_AddArrayToObject(item, "ipv6Prefix");
    if (ipv6_prefixList == NULL) {
        ogs_error("OpenAPI_pdu_session_management_data_convertToJSON() failed [ipv6_prefix]");
        goto end;
    }
    OpenAPI_list_for_each(pdu_session_management_data->ipv6_prefix, node) {
        if (cJSON_AddStringToObject(ipv6_prefixList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_pdu_session_management_data_convertToJSON() failed [ipv6_prefix]");
            goto end;
        }
    }
    }

    if (pdu_session_management_data->ipv6_addrs) {
    cJSON *ipv6_addrsList = cJSON_AddArrayToObject(item, "ipv6Addrs");
    if (ipv6_addrsList == NULL) {
        ogs_error("OpenAPI_pdu_session_management_data_convertToJSON() failed [ipv6_addrs]");
        goto end;
    }
    OpenAPI_list_for_each(pdu_session_management_data->ipv6_addrs, node) {
        if (cJSON_AddStringToObject(ipv6_addrsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_pdu_session_management_data_convertToJSON() failed [ipv6_addrs]");
            goto end;
        }
    }
    }

    if (pdu_session_management_data->pdu_sess_type != OpenAPI_pdu_session_type_NULL) {
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

    if (pdu_session_management_data->is_pdu_session_id) {
    if (cJSON_AddNumberToObject(item, "pduSessionId", pdu_session_management_data->pdu_session_id) == NULL) {
        ogs_error("OpenAPI_pdu_session_management_data_convertToJSON() failed [pdu_session_id]");
        goto end;
    }
    }

    if (pdu_session_management_data->supp_feat) {
    if (cJSON_AddStringToObject(item, "suppFeat", pdu_session_management_data->supp_feat) == NULL) {
        ogs_error("OpenAPI_pdu_session_management_data_convertToJSON() failed [supp_feat]");
        goto end;
    }
    }

    if (pdu_session_management_data->reset_ids) {
    cJSON *reset_idsList = cJSON_AddArrayToObject(item, "resetIds");
    if (reset_idsList == NULL) {
        ogs_error("OpenAPI_pdu_session_management_data_convertToJSON() failed [reset_ids]");
        goto end;
    }
    OpenAPI_list_for_each(pdu_session_management_data->reset_ids, node) {
        if (cJSON_AddStringToObject(reset_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_pdu_session_management_data_convertToJSON() failed [reset_ids]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_pdu_session_management_data_t *OpenAPI_pdu_session_management_data_parseFromJSON(cJSON *pdu_session_management_dataJSON)
{
    OpenAPI_pdu_session_management_data_t *pdu_session_management_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *pdu_session_status = NULL;
    OpenAPI_pdu_session_status_e pdu_session_statusVariable = 0;
    cJSON *pdu_session_status_ts = NULL;
    cJSON *dnai = NULL;
    cJSON *dnai_ts = NULL;
    cJSON *n6_traffic_routing_info = NULL;
    OpenAPI_list_t *n6_traffic_routing_infoList = NULL;
    cJSON *n6_traffic_routing_info_ts = NULL;
    cJSON *ipv4_addr = NULL;
    cJSON *ipv6_prefix = NULL;
    OpenAPI_list_t *ipv6_prefixList = NULL;
    cJSON *ipv6_addrs = NULL;
    OpenAPI_list_t *ipv6_addrsList = NULL;
    cJSON *pdu_sess_type = NULL;
    OpenAPI_pdu_session_type_e pdu_sess_typeVariable = 0;
    cJSON *ip_addr_ts = NULL;
    cJSON *dnn = NULL;
    cJSON *pdu_session_id = NULL;
    cJSON *supp_feat = NULL;
    cJSON *reset_ids = NULL;
    OpenAPI_list_t *reset_idsList = NULL;
    pdu_session_status = cJSON_GetObjectItemCaseSensitive(pdu_session_management_dataJSON, "pduSessionStatus");
    if (pdu_session_status) {
    if (!cJSON_IsString(pdu_session_status)) {
        ogs_error("OpenAPI_pdu_session_management_data_parseFromJSON() failed [pdu_session_status]");
        goto end;
    }
    pdu_session_statusVariable = OpenAPI_pdu_session_status_FromString(pdu_session_status->valuestring);
    }

    pdu_session_status_ts = cJSON_GetObjectItemCaseSensitive(pdu_session_management_dataJSON, "pduSessionStatusTs");
    if (pdu_session_status_ts) {
    if (!cJSON_IsString(pdu_session_status_ts) && !cJSON_IsNull(pdu_session_status_ts)) {
        ogs_error("OpenAPI_pdu_session_management_data_parseFromJSON() failed [pdu_session_status_ts]");
        goto end;
    }
    }

    dnai = cJSON_GetObjectItemCaseSensitive(pdu_session_management_dataJSON, "dnai");
    if (dnai) {
    if (!cJSON_IsString(dnai) && !cJSON_IsNull(dnai)) {
        ogs_error("OpenAPI_pdu_session_management_data_parseFromJSON() failed [dnai]");
        goto end;
    }
    }

    dnai_ts = cJSON_GetObjectItemCaseSensitive(pdu_session_management_dataJSON, "dnaiTs");
    if (dnai_ts) {
    if (!cJSON_IsString(dnai_ts) && !cJSON_IsNull(dnai_ts)) {
        ogs_error("OpenAPI_pdu_session_management_data_parseFromJSON() failed [dnai_ts]");
        goto end;
    }
    }

    n6_traffic_routing_info = cJSON_GetObjectItemCaseSensitive(pdu_session_management_dataJSON, "n6TrafficRoutingInfo");
    if (n6_traffic_routing_info) {
        cJSON *n6_traffic_routing_info_local = NULL;
        if (!cJSON_IsArray(n6_traffic_routing_info)) {
            ogs_error("OpenAPI_pdu_session_management_data_parseFromJSON() failed [n6_traffic_routing_info]");
            goto end;
        }

        n6_traffic_routing_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(n6_traffic_routing_info_local, n6_traffic_routing_info) {
            if (!cJSON_IsObject(n6_traffic_routing_info_local)) {
                ogs_error("OpenAPI_pdu_session_management_data_parseFromJSON() failed [n6_traffic_routing_info]");
                goto end;
            }
            OpenAPI_route_to_location_t *n6_traffic_routing_infoItem = OpenAPI_route_to_location_parseFromJSON(n6_traffic_routing_info_local);
            if (!n6_traffic_routing_infoItem) {
                ogs_error("No n6_traffic_routing_infoItem");
                goto end;
            }
            OpenAPI_list_add(n6_traffic_routing_infoList, n6_traffic_routing_infoItem);
        }
    }

    n6_traffic_routing_info_ts = cJSON_GetObjectItemCaseSensitive(pdu_session_management_dataJSON, "n6TrafficRoutingInfoTs");
    if (n6_traffic_routing_info_ts) {
    if (!cJSON_IsString(n6_traffic_routing_info_ts) && !cJSON_IsNull(n6_traffic_routing_info_ts)) {
        ogs_error("OpenAPI_pdu_session_management_data_parseFromJSON() failed [n6_traffic_routing_info_ts]");
        goto end;
    }
    }

    ipv4_addr = cJSON_GetObjectItemCaseSensitive(pdu_session_management_dataJSON, "ipv4Addr");
    if (ipv4_addr) {
    if (!cJSON_IsString(ipv4_addr) && !cJSON_IsNull(ipv4_addr)) {
        ogs_error("OpenAPI_pdu_session_management_data_parseFromJSON() failed [ipv4_addr]");
        goto end;
    }
    }

    ipv6_prefix = cJSON_GetObjectItemCaseSensitive(pdu_session_management_dataJSON, "ipv6Prefix");
    if (ipv6_prefix) {
        cJSON *ipv6_prefix_local = NULL;
        if (!cJSON_IsArray(ipv6_prefix)) {
            ogs_error("OpenAPI_pdu_session_management_data_parseFromJSON() failed [ipv6_prefix]");
            goto end;
        }

        ipv6_prefixList = OpenAPI_list_create();

        cJSON_ArrayForEach(ipv6_prefix_local, ipv6_prefix) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ipv6_prefix_local)) {
                ogs_error("OpenAPI_pdu_session_management_data_parseFromJSON() failed [ipv6_prefix]");
                goto end;
            }
            OpenAPI_list_add(ipv6_prefixList, ogs_strdup(ipv6_prefix_local->valuestring));
        }
    }

    ipv6_addrs = cJSON_GetObjectItemCaseSensitive(pdu_session_management_dataJSON, "ipv6Addrs");
    if (ipv6_addrs) {
        cJSON *ipv6_addrs_local = NULL;
        if (!cJSON_IsArray(ipv6_addrs)) {
            ogs_error("OpenAPI_pdu_session_management_data_parseFromJSON() failed [ipv6_addrs]");
            goto end;
        }

        ipv6_addrsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ipv6_addrs_local, ipv6_addrs) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ipv6_addrs_local)) {
                ogs_error("OpenAPI_pdu_session_management_data_parseFromJSON() failed [ipv6_addrs]");
                goto end;
            }
            OpenAPI_list_add(ipv6_addrsList, ogs_strdup(ipv6_addrs_local->valuestring));
        }
    }

    pdu_sess_type = cJSON_GetObjectItemCaseSensitive(pdu_session_management_dataJSON, "pduSessType");
    if (pdu_sess_type) {
    if (!cJSON_IsString(pdu_sess_type)) {
        ogs_error("OpenAPI_pdu_session_management_data_parseFromJSON() failed [pdu_sess_type]");
        goto end;
    }
    pdu_sess_typeVariable = OpenAPI_pdu_session_type_FromString(pdu_sess_type->valuestring);
    }

    ip_addr_ts = cJSON_GetObjectItemCaseSensitive(pdu_session_management_dataJSON, "ipAddrTs");
    if (ip_addr_ts) {
    if (!cJSON_IsString(ip_addr_ts) && !cJSON_IsNull(ip_addr_ts)) {
        ogs_error("OpenAPI_pdu_session_management_data_parseFromJSON() failed [ip_addr_ts]");
        goto end;
    }
    }

    dnn = cJSON_GetObjectItemCaseSensitive(pdu_session_management_dataJSON, "dnn");
    if (dnn) {
    if (!cJSON_IsString(dnn) && !cJSON_IsNull(dnn)) {
        ogs_error("OpenAPI_pdu_session_management_data_parseFromJSON() failed [dnn]");
        goto end;
    }
    }

    pdu_session_id = cJSON_GetObjectItemCaseSensitive(pdu_session_management_dataJSON, "pduSessionId");
    if (pdu_session_id) {
    if (!cJSON_IsNumber(pdu_session_id)) {
        ogs_error("OpenAPI_pdu_session_management_data_parseFromJSON() failed [pdu_session_id]");
        goto end;
    }
    }

    supp_feat = cJSON_GetObjectItemCaseSensitive(pdu_session_management_dataJSON, "suppFeat");
    if (supp_feat) {
    if (!cJSON_IsString(supp_feat) && !cJSON_IsNull(supp_feat)) {
        ogs_error("OpenAPI_pdu_session_management_data_parseFromJSON() failed [supp_feat]");
        goto end;
    }
    }

    reset_ids = cJSON_GetObjectItemCaseSensitive(pdu_session_management_dataJSON, "resetIds");
    if (reset_ids) {
        cJSON *reset_ids_local = NULL;
        if (!cJSON_IsArray(reset_ids)) {
            ogs_error("OpenAPI_pdu_session_management_data_parseFromJSON() failed [reset_ids]");
            goto end;
        }

        reset_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(reset_ids_local, reset_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(reset_ids_local)) {
                ogs_error("OpenAPI_pdu_session_management_data_parseFromJSON() failed [reset_ids]");
                goto end;
            }
            OpenAPI_list_add(reset_idsList, ogs_strdup(reset_ids_local->valuestring));
        }
    }

    pdu_session_management_data_local_var = OpenAPI_pdu_session_management_data_create (
        pdu_session_status ? pdu_session_statusVariable : 0,
        pdu_session_status_ts && !cJSON_IsNull(pdu_session_status_ts) ? ogs_strdup(pdu_session_status_ts->valuestring) : NULL,
        dnai && !cJSON_IsNull(dnai) ? ogs_strdup(dnai->valuestring) : NULL,
        dnai_ts && !cJSON_IsNull(dnai_ts) ? ogs_strdup(dnai_ts->valuestring) : NULL,
        n6_traffic_routing_info ? n6_traffic_routing_infoList : NULL,
        n6_traffic_routing_info_ts && !cJSON_IsNull(n6_traffic_routing_info_ts) ? ogs_strdup(n6_traffic_routing_info_ts->valuestring) : NULL,
        ipv4_addr && !cJSON_IsNull(ipv4_addr) ? ogs_strdup(ipv4_addr->valuestring) : NULL,
        ipv6_prefix ? ipv6_prefixList : NULL,
        ipv6_addrs ? ipv6_addrsList : NULL,
        pdu_sess_type ? pdu_sess_typeVariable : 0,
        ip_addr_ts && !cJSON_IsNull(ip_addr_ts) ? ogs_strdup(ip_addr_ts->valuestring) : NULL,
        dnn && !cJSON_IsNull(dnn) ? ogs_strdup(dnn->valuestring) : NULL,
        pdu_session_id ? true : false,
        pdu_session_id ? pdu_session_id->valuedouble : 0,
        supp_feat && !cJSON_IsNull(supp_feat) ? ogs_strdup(supp_feat->valuestring) : NULL,
        reset_ids ? reset_idsList : NULL
    );

    return pdu_session_management_data_local_var;
end:
    if (n6_traffic_routing_infoList) {
        OpenAPI_list_for_each(n6_traffic_routing_infoList, node) {
            OpenAPI_route_to_location_free(node->data);
        }
        OpenAPI_list_free(n6_traffic_routing_infoList);
        n6_traffic_routing_infoList = NULL;
    }
    if (ipv6_prefixList) {
        OpenAPI_list_for_each(ipv6_prefixList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ipv6_prefixList);
        ipv6_prefixList = NULL;
    }
    if (ipv6_addrsList) {
        OpenAPI_list_for_each(ipv6_addrsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ipv6_addrsList);
        ipv6_addrsList = NULL;
    }
    if (reset_idsList) {
        OpenAPI_list_for_each(reset_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(reset_idsList);
        reset_idsList = NULL;
    }
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

