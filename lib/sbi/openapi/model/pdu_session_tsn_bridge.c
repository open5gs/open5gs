
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pdu_session_tsn_bridge.h"

OpenAPI_pdu_session_tsn_bridge_t *OpenAPI_pdu_session_tsn_bridge_create(
    OpenAPI_tsn_bridge_info_t *tsn_bridge_info,
    OpenAPI_bridge_management_container_t *tsn_bridge_man_cont,
    OpenAPI_port_management_container_t *tsn_port_man_cont_dstt,
    OpenAPI_list_t *tsn_port_man_cont_nwtts,
    char *ue_ipv4_addr,
    char *dnn,
    OpenAPI_snssai_t *snssai,
    char *ip_domain,
    char *ue_ipv6_addr_prefix
)
{
    OpenAPI_pdu_session_tsn_bridge_t *pdu_session_tsn_bridge_local_var = ogs_malloc(sizeof(OpenAPI_pdu_session_tsn_bridge_t));
    ogs_assert(pdu_session_tsn_bridge_local_var);

    pdu_session_tsn_bridge_local_var->tsn_bridge_info = tsn_bridge_info;
    pdu_session_tsn_bridge_local_var->tsn_bridge_man_cont = tsn_bridge_man_cont;
    pdu_session_tsn_bridge_local_var->tsn_port_man_cont_dstt = tsn_port_man_cont_dstt;
    pdu_session_tsn_bridge_local_var->tsn_port_man_cont_nwtts = tsn_port_man_cont_nwtts;
    pdu_session_tsn_bridge_local_var->ue_ipv4_addr = ue_ipv4_addr;
    pdu_session_tsn_bridge_local_var->dnn = dnn;
    pdu_session_tsn_bridge_local_var->snssai = snssai;
    pdu_session_tsn_bridge_local_var->ip_domain = ip_domain;
    pdu_session_tsn_bridge_local_var->ue_ipv6_addr_prefix = ue_ipv6_addr_prefix;

    return pdu_session_tsn_bridge_local_var;
}

void OpenAPI_pdu_session_tsn_bridge_free(OpenAPI_pdu_session_tsn_bridge_t *pdu_session_tsn_bridge)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pdu_session_tsn_bridge) {
        return;
    }
    if (pdu_session_tsn_bridge->tsn_bridge_info) {
        OpenAPI_tsn_bridge_info_free(pdu_session_tsn_bridge->tsn_bridge_info);
        pdu_session_tsn_bridge->tsn_bridge_info = NULL;
    }
    if (pdu_session_tsn_bridge->tsn_bridge_man_cont) {
        OpenAPI_bridge_management_container_free(pdu_session_tsn_bridge->tsn_bridge_man_cont);
        pdu_session_tsn_bridge->tsn_bridge_man_cont = NULL;
    }
    if (pdu_session_tsn_bridge->tsn_port_man_cont_dstt) {
        OpenAPI_port_management_container_free(pdu_session_tsn_bridge->tsn_port_man_cont_dstt);
        pdu_session_tsn_bridge->tsn_port_man_cont_dstt = NULL;
    }
    if (pdu_session_tsn_bridge->tsn_port_man_cont_nwtts) {
        OpenAPI_list_for_each(pdu_session_tsn_bridge->tsn_port_man_cont_nwtts, node) {
            OpenAPI_port_management_container_free(node->data);
        }
        OpenAPI_list_free(pdu_session_tsn_bridge->tsn_port_man_cont_nwtts);
        pdu_session_tsn_bridge->tsn_port_man_cont_nwtts = NULL;
    }
    if (pdu_session_tsn_bridge->ue_ipv4_addr) {
        ogs_free(pdu_session_tsn_bridge->ue_ipv4_addr);
        pdu_session_tsn_bridge->ue_ipv4_addr = NULL;
    }
    if (pdu_session_tsn_bridge->dnn) {
        ogs_free(pdu_session_tsn_bridge->dnn);
        pdu_session_tsn_bridge->dnn = NULL;
    }
    if (pdu_session_tsn_bridge->snssai) {
        OpenAPI_snssai_free(pdu_session_tsn_bridge->snssai);
        pdu_session_tsn_bridge->snssai = NULL;
    }
    if (pdu_session_tsn_bridge->ip_domain) {
        ogs_free(pdu_session_tsn_bridge->ip_domain);
        pdu_session_tsn_bridge->ip_domain = NULL;
    }
    if (pdu_session_tsn_bridge->ue_ipv6_addr_prefix) {
        ogs_free(pdu_session_tsn_bridge->ue_ipv6_addr_prefix);
        pdu_session_tsn_bridge->ue_ipv6_addr_prefix = NULL;
    }
    ogs_free(pdu_session_tsn_bridge);
}

cJSON *OpenAPI_pdu_session_tsn_bridge_convertToJSON(OpenAPI_pdu_session_tsn_bridge_t *pdu_session_tsn_bridge)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pdu_session_tsn_bridge == NULL) {
        ogs_error("OpenAPI_pdu_session_tsn_bridge_convertToJSON() failed [PduSessionTsnBridge]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!pdu_session_tsn_bridge->tsn_bridge_info) {
        ogs_error("OpenAPI_pdu_session_tsn_bridge_convertToJSON() failed [tsn_bridge_info]");
        return NULL;
    }
    cJSON *tsn_bridge_info_local_JSON = OpenAPI_tsn_bridge_info_convertToJSON(pdu_session_tsn_bridge->tsn_bridge_info);
    if (tsn_bridge_info_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_tsn_bridge_convertToJSON() failed [tsn_bridge_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "tsnBridgeInfo", tsn_bridge_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_tsn_bridge_convertToJSON() failed [tsn_bridge_info]");
        goto end;
    }

    if (pdu_session_tsn_bridge->tsn_bridge_man_cont) {
    cJSON *tsn_bridge_man_cont_local_JSON = OpenAPI_bridge_management_container_convertToJSON(pdu_session_tsn_bridge->tsn_bridge_man_cont);
    if (tsn_bridge_man_cont_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_tsn_bridge_convertToJSON() failed [tsn_bridge_man_cont]");
        goto end;
    }
    cJSON_AddItemToObject(item, "tsnBridgeManCont", tsn_bridge_man_cont_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_tsn_bridge_convertToJSON() failed [tsn_bridge_man_cont]");
        goto end;
    }
    }

    if (pdu_session_tsn_bridge->tsn_port_man_cont_dstt) {
    cJSON *tsn_port_man_cont_dstt_local_JSON = OpenAPI_port_management_container_convertToJSON(pdu_session_tsn_bridge->tsn_port_man_cont_dstt);
    if (tsn_port_man_cont_dstt_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_tsn_bridge_convertToJSON() failed [tsn_port_man_cont_dstt]");
        goto end;
    }
    cJSON_AddItemToObject(item, "tsnPortManContDstt", tsn_port_man_cont_dstt_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_tsn_bridge_convertToJSON() failed [tsn_port_man_cont_dstt]");
        goto end;
    }
    }

    if (pdu_session_tsn_bridge->tsn_port_man_cont_nwtts) {
    cJSON *tsn_port_man_cont_nwttsList = cJSON_AddArrayToObject(item, "tsnPortManContNwtts");
    if (tsn_port_man_cont_nwttsList == NULL) {
        ogs_error("OpenAPI_pdu_session_tsn_bridge_convertToJSON() failed [tsn_port_man_cont_nwtts]");
        goto end;
    }
    OpenAPI_list_for_each(pdu_session_tsn_bridge->tsn_port_man_cont_nwtts, node) {
        cJSON *itemLocal = OpenAPI_port_management_container_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_pdu_session_tsn_bridge_convertToJSON() failed [tsn_port_man_cont_nwtts]");
            goto end;
        }
        cJSON_AddItemToArray(tsn_port_man_cont_nwttsList, itemLocal);
    }
    }

    if (pdu_session_tsn_bridge->ue_ipv4_addr) {
    if (cJSON_AddStringToObject(item, "ueIpv4Addr", pdu_session_tsn_bridge->ue_ipv4_addr) == NULL) {
        ogs_error("OpenAPI_pdu_session_tsn_bridge_convertToJSON() failed [ue_ipv4_addr]");
        goto end;
    }
    }

    if (pdu_session_tsn_bridge->dnn) {
    if (cJSON_AddStringToObject(item, "dnn", pdu_session_tsn_bridge->dnn) == NULL) {
        ogs_error("OpenAPI_pdu_session_tsn_bridge_convertToJSON() failed [dnn]");
        goto end;
    }
    }

    if (pdu_session_tsn_bridge->snssai) {
    cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(pdu_session_tsn_bridge->snssai);
    if (snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_tsn_bridge_convertToJSON() failed [snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_tsn_bridge_convertToJSON() failed [snssai]");
        goto end;
    }
    }

    if (pdu_session_tsn_bridge->ip_domain) {
    if (cJSON_AddStringToObject(item, "ipDomain", pdu_session_tsn_bridge->ip_domain) == NULL) {
        ogs_error("OpenAPI_pdu_session_tsn_bridge_convertToJSON() failed [ip_domain]");
        goto end;
    }
    }

    if (pdu_session_tsn_bridge->ue_ipv6_addr_prefix) {
    if (cJSON_AddStringToObject(item, "ueIpv6AddrPrefix", pdu_session_tsn_bridge->ue_ipv6_addr_prefix) == NULL) {
        ogs_error("OpenAPI_pdu_session_tsn_bridge_convertToJSON() failed [ue_ipv6_addr_prefix]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pdu_session_tsn_bridge_t *OpenAPI_pdu_session_tsn_bridge_parseFromJSON(cJSON *pdu_session_tsn_bridgeJSON)
{
    OpenAPI_pdu_session_tsn_bridge_t *pdu_session_tsn_bridge_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *tsn_bridge_info = NULL;
    OpenAPI_tsn_bridge_info_t *tsn_bridge_info_local_nonprim = NULL;
    cJSON *tsn_bridge_man_cont = NULL;
    OpenAPI_bridge_management_container_t *tsn_bridge_man_cont_local_nonprim = NULL;
    cJSON *tsn_port_man_cont_dstt = NULL;
    OpenAPI_port_management_container_t *tsn_port_man_cont_dstt_local_nonprim = NULL;
    cJSON *tsn_port_man_cont_nwtts = NULL;
    OpenAPI_list_t *tsn_port_man_cont_nwttsList = NULL;
    cJSON *ue_ipv4_addr = NULL;
    cJSON *dnn = NULL;
    cJSON *snssai = NULL;
    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    cJSON *ip_domain = NULL;
    cJSON *ue_ipv6_addr_prefix = NULL;
    tsn_bridge_info = cJSON_GetObjectItemCaseSensitive(pdu_session_tsn_bridgeJSON, "tsnBridgeInfo");
    if (!tsn_bridge_info) {
        ogs_error("OpenAPI_pdu_session_tsn_bridge_parseFromJSON() failed [tsn_bridge_info]");
        goto end;
    }
    tsn_bridge_info_local_nonprim = OpenAPI_tsn_bridge_info_parseFromJSON(tsn_bridge_info);
    if (!tsn_bridge_info_local_nonprim) {
        ogs_error("OpenAPI_tsn_bridge_info_parseFromJSON failed [tsn_bridge_info]");
        goto end;
    }

    tsn_bridge_man_cont = cJSON_GetObjectItemCaseSensitive(pdu_session_tsn_bridgeJSON, "tsnBridgeManCont");
    if (tsn_bridge_man_cont) {
    tsn_bridge_man_cont_local_nonprim = OpenAPI_bridge_management_container_parseFromJSON(tsn_bridge_man_cont);
    if (!tsn_bridge_man_cont_local_nonprim) {
        ogs_error("OpenAPI_bridge_management_container_parseFromJSON failed [tsn_bridge_man_cont]");
        goto end;
    }
    }

    tsn_port_man_cont_dstt = cJSON_GetObjectItemCaseSensitive(pdu_session_tsn_bridgeJSON, "tsnPortManContDstt");
    if (tsn_port_man_cont_dstt) {
    tsn_port_man_cont_dstt_local_nonprim = OpenAPI_port_management_container_parseFromJSON(tsn_port_man_cont_dstt);
    if (!tsn_port_man_cont_dstt_local_nonprim) {
        ogs_error("OpenAPI_port_management_container_parseFromJSON failed [tsn_port_man_cont_dstt]");
        goto end;
    }
    }

    tsn_port_man_cont_nwtts = cJSON_GetObjectItemCaseSensitive(pdu_session_tsn_bridgeJSON, "tsnPortManContNwtts");
    if (tsn_port_man_cont_nwtts) {
        cJSON *tsn_port_man_cont_nwtts_local = NULL;
        if (!cJSON_IsArray(tsn_port_man_cont_nwtts)) {
            ogs_error("OpenAPI_pdu_session_tsn_bridge_parseFromJSON() failed [tsn_port_man_cont_nwtts]");
            goto end;
        }

        tsn_port_man_cont_nwttsList = OpenAPI_list_create();

        cJSON_ArrayForEach(tsn_port_man_cont_nwtts_local, tsn_port_man_cont_nwtts) {
            if (!cJSON_IsObject(tsn_port_man_cont_nwtts_local)) {
                ogs_error("OpenAPI_pdu_session_tsn_bridge_parseFromJSON() failed [tsn_port_man_cont_nwtts]");
                goto end;
            }
            OpenAPI_port_management_container_t *tsn_port_man_cont_nwttsItem = OpenAPI_port_management_container_parseFromJSON(tsn_port_man_cont_nwtts_local);
            if (!tsn_port_man_cont_nwttsItem) {
                ogs_error("No tsn_port_man_cont_nwttsItem");
                goto end;
            }
            OpenAPI_list_add(tsn_port_man_cont_nwttsList, tsn_port_man_cont_nwttsItem);
        }
    }

    ue_ipv4_addr = cJSON_GetObjectItemCaseSensitive(pdu_session_tsn_bridgeJSON, "ueIpv4Addr");
    if (ue_ipv4_addr) {
    if (!cJSON_IsString(ue_ipv4_addr) && !cJSON_IsNull(ue_ipv4_addr)) {
        ogs_error("OpenAPI_pdu_session_tsn_bridge_parseFromJSON() failed [ue_ipv4_addr]");
        goto end;
    }
    }

    dnn = cJSON_GetObjectItemCaseSensitive(pdu_session_tsn_bridgeJSON, "dnn");
    if (dnn) {
    if (!cJSON_IsString(dnn) && !cJSON_IsNull(dnn)) {
        ogs_error("OpenAPI_pdu_session_tsn_bridge_parseFromJSON() failed [dnn]");
        goto end;
    }
    }

    snssai = cJSON_GetObjectItemCaseSensitive(pdu_session_tsn_bridgeJSON, "snssai");
    if (snssai) {
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    if (!snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [snssai]");
        goto end;
    }
    }

    ip_domain = cJSON_GetObjectItemCaseSensitive(pdu_session_tsn_bridgeJSON, "ipDomain");
    if (ip_domain) {
    if (!cJSON_IsString(ip_domain) && !cJSON_IsNull(ip_domain)) {
        ogs_error("OpenAPI_pdu_session_tsn_bridge_parseFromJSON() failed [ip_domain]");
        goto end;
    }
    }

    ue_ipv6_addr_prefix = cJSON_GetObjectItemCaseSensitive(pdu_session_tsn_bridgeJSON, "ueIpv6AddrPrefix");
    if (ue_ipv6_addr_prefix) {
    if (!cJSON_IsString(ue_ipv6_addr_prefix) && !cJSON_IsNull(ue_ipv6_addr_prefix)) {
        ogs_error("OpenAPI_pdu_session_tsn_bridge_parseFromJSON() failed [ue_ipv6_addr_prefix]");
        goto end;
    }
    }

    pdu_session_tsn_bridge_local_var = OpenAPI_pdu_session_tsn_bridge_create (
        tsn_bridge_info_local_nonprim,
        tsn_bridge_man_cont ? tsn_bridge_man_cont_local_nonprim : NULL,
        tsn_port_man_cont_dstt ? tsn_port_man_cont_dstt_local_nonprim : NULL,
        tsn_port_man_cont_nwtts ? tsn_port_man_cont_nwttsList : NULL,
        ue_ipv4_addr && !cJSON_IsNull(ue_ipv4_addr) ? ogs_strdup(ue_ipv4_addr->valuestring) : NULL,
        dnn && !cJSON_IsNull(dnn) ? ogs_strdup(dnn->valuestring) : NULL,
        snssai ? snssai_local_nonprim : NULL,
        ip_domain && !cJSON_IsNull(ip_domain) ? ogs_strdup(ip_domain->valuestring) : NULL,
        ue_ipv6_addr_prefix && !cJSON_IsNull(ue_ipv6_addr_prefix) ? ogs_strdup(ue_ipv6_addr_prefix->valuestring) : NULL
    );

    return pdu_session_tsn_bridge_local_var;
end:
    if (tsn_bridge_info_local_nonprim) {
        OpenAPI_tsn_bridge_info_free(tsn_bridge_info_local_nonprim);
        tsn_bridge_info_local_nonprim = NULL;
    }
    if (tsn_bridge_man_cont_local_nonprim) {
        OpenAPI_bridge_management_container_free(tsn_bridge_man_cont_local_nonprim);
        tsn_bridge_man_cont_local_nonprim = NULL;
    }
    if (tsn_port_man_cont_dstt_local_nonprim) {
        OpenAPI_port_management_container_free(tsn_port_man_cont_dstt_local_nonprim);
        tsn_port_man_cont_dstt_local_nonprim = NULL;
    }
    if (tsn_port_man_cont_nwttsList) {
        OpenAPI_list_for_each(tsn_port_man_cont_nwttsList, node) {
            OpenAPI_port_management_container_free(node->data);
        }
        OpenAPI_list_free(tsn_port_man_cont_nwttsList);
        tsn_port_man_cont_nwttsList = NULL;
    }
    if (snssai_local_nonprim) {
        OpenAPI_snssai_free(snssai_local_nonprim);
        snssai_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_pdu_session_tsn_bridge_t *OpenAPI_pdu_session_tsn_bridge_copy(OpenAPI_pdu_session_tsn_bridge_t *dst, OpenAPI_pdu_session_tsn_bridge_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pdu_session_tsn_bridge_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pdu_session_tsn_bridge_convertToJSON() failed");
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

    OpenAPI_pdu_session_tsn_bridge_free(dst);
    dst = OpenAPI_pdu_session_tsn_bridge_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

