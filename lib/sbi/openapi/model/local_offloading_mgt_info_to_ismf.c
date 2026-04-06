
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "local_offloading_mgt_info_to_ismf.h"

OpenAPI_local_offloading_mgt_info_to_ismf_t *OpenAPI_local_offloading_mgt_info_to_ismf_create(
    bool is_local_offloading_mgt_allowed_ind,
    int local_offloading_mgt_allowed_ind,
    OpenAPI_ip_address_t *dns_addr,
    OpenAPI_ip_address_t *psa_upf_addr,
    OpenAPI_list_t *local_offloading_info_list,
    OpenAPI_list_t *offload_ids,
    OpenAPI_traffic_influence_info_t *traffic_influ_info
)
{
    OpenAPI_local_offloading_mgt_info_to_ismf_t *local_offloading_mgt_info_to_ismf_local_var = ogs_malloc(sizeof(OpenAPI_local_offloading_mgt_info_to_ismf_t));
    ogs_assert(local_offloading_mgt_info_to_ismf_local_var);

    local_offloading_mgt_info_to_ismf_local_var->is_local_offloading_mgt_allowed_ind = is_local_offloading_mgt_allowed_ind;
    local_offloading_mgt_info_to_ismf_local_var->local_offloading_mgt_allowed_ind = local_offloading_mgt_allowed_ind;
    local_offloading_mgt_info_to_ismf_local_var->dns_addr = dns_addr;
    local_offloading_mgt_info_to_ismf_local_var->psa_upf_addr = psa_upf_addr;
    local_offloading_mgt_info_to_ismf_local_var->local_offloading_info_list = local_offloading_info_list;
    local_offloading_mgt_info_to_ismf_local_var->offload_ids = offload_ids;
    local_offloading_mgt_info_to_ismf_local_var->traffic_influ_info = traffic_influ_info;

    return local_offloading_mgt_info_to_ismf_local_var;
}

void OpenAPI_local_offloading_mgt_info_to_ismf_free(OpenAPI_local_offloading_mgt_info_to_ismf_t *local_offloading_mgt_info_to_ismf)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == local_offloading_mgt_info_to_ismf) {
        return;
    }
    if (local_offloading_mgt_info_to_ismf->dns_addr) {
        OpenAPI_ip_address_free(local_offloading_mgt_info_to_ismf->dns_addr);
        local_offloading_mgt_info_to_ismf->dns_addr = NULL;
    }
    if (local_offloading_mgt_info_to_ismf->psa_upf_addr) {
        OpenAPI_ip_address_free(local_offloading_mgt_info_to_ismf->psa_upf_addr);
        local_offloading_mgt_info_to_ismf->psa_upf_addr = NULL;
    }
    if (local_offloading_mgt_info_to_ismf->local_offloading_info_list) {
        OpenAPI_list_for_each(local_offloading_mgt_info_to_ismf->local_offloading_info_list, node) {
            OpenAPI_local_offloading_management_info_free(node->data);
        }
        OpenAPI_list_free(local_offloading_mgt_info_to_ismf->local_offloading_info_list);
        local_offloading_mgt_info_to_ismf->local_offloading_info_list = NULL;
    }
    if (local_offloading_mgt_info_to_ismf->offload_ids) {
        OpenAPI_list_for_each(local_offloading_mgt_info_to_ismf->offload_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(local_offloading_mgt_info_to_ismf->offload_ids);
        local_offloading_mgt_info_to_ismf->offload_ids = NULL;
    }
    if (local_offloading_mgt_info_to_ismf->traffic_influ_info) {
        OpenAPI_traffic_influence_info_free(local_offloading_mgt_info_to_ismf->traffic_influ_info);
        local_offloading_mgt_info_to_ismf->traffic_influ_info = NULL;
    }
    ogs_free(local_offloading_mgt_info_to_ismf);
}

cJSON *OpenAPI_local_offloading_mgt_info_to_ismf_convertToJSON(OpenAPI_local_offloading_mgt_info_to_ismf_t *local_offloading_mgt_info_to_ismf)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (local_offloading_mgt_info_to_ismf == NULL) {
        ogs_error("OpenAPI_local_offloading_mgt_info_to_ismf_convertToJSON() failed [LocalOffloadingMgtInfoToIsmf]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (local_offloading_mgt_info_to_ismf->is_local_offloading_mgt_allowed_ind) {
    if (cJSON_AddBoolToObject(item, "localOffloadingMgtAllowedInd", local_offloading_mgt_info_to_ismf->local_offloading_mgt_allowed_ind) == NULL) {
        ogs_error("OpenAPI_local_offloading_mgt_info_to_ismf_convertToJSON() failed [local_offloading_mgt_allowed_ind]");
        goto end;
    }
    }

    if (local_offloading_mgt_info_to_ismf->dns_addr) {
    cJSON *dns_addr_local_JSON = OpenAPI_ip_address_convertToJSON(local_offloading_mgt_info_to_ismf->dns_addr);
    if (dns_addr_local_JSON == NULL) {
        ogs_error("OpenAPI_local_offloading_mgt_info_to_ismf_convertToJSON() failed [dns_addr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "dnsAddr", dns_addr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_local_offloading_mgt_info_to_ismf_convertToJSON() failed [dns_addr]");
        goto end;
    }
    }

    if (local_offloading_mgt_info_to_ismf->psa_upf_addr) {
    cJSON *psa_upf_addr_local_JSON = OpenAPI_ip_address_convertToJSON(local_offloading_mgt_info_to_ismf->psa_upf_addr);
    if (psa_upf_addr_local_JSON == NULL) {
        ogs_error("OpenAPI_local_offloading_mgt_info_to_ismf_convertToJSON() failed [psa_upf_addr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "psaUpfAddr", psa_upf_addr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_local_offloading_mgt_info_to_ismf_convertToJSON() failed [psa_upf_addr]");
        goto end;
    }
    }

    if (local_offloading_mgt_info_to_ismf->local_offloading_info_list) {
    cJSON *local_offloading_info_listList = cJSON_AddArrayToObject(item, "localOffloadingInfoList");
    if (local_offloading_info_listList == NULL) {
        ogs_error("OpenAPI_local_offloading_mgt_info_to_ismf_convertToJSON() failed [local_offloading_info_list]");
        goto end;
    }
    OpenAPI_list_for_each(local_offloading_mgt_info_to_ismf->local_offloading_info_list, node) {
        cJSON *itemLocal = OpenAPI_local_offloading_management_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_local_offloading_mgt_info_to_ismf_convertToJSON() failed [local_offloading_info_list]");
            goto end;
        }
        cJSON_AddItemToArray(local_offloading_info_listList, itemLocal);
    }
    }

    if (local_offloading_mgt_info_to_ismf->offload_ids) {
    cJSON *offload_idsList = cJSON_AddArrayToObject(item, "offloadIds");
    if (offload_idsList == NULL) {
        ogs_error("OpenAPI_local_offloading_mgt_info_to_ismf_convertToJSON() failed [offload_ids]");
        goto end;
    }
    OpenAPI_list_for_each(local_offloading_mgt_info_to_ismf->offload_ids, node) {
        if (cJSON_AddStringToObject(offload_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_local_offloading_mgt_info_to_ismf_convertToJSON() failed [offload_ids]");
            goto end;
        }
    }
    }

    if (local_offloading_mgt_info_to_ismf->traffic_influ_info) {
    cJSON *traffic_influ_info_local_JSON = OpenAPI_traffic_influence_info_convertToJSON(local_offloading_mgt_info_to_ismf->traffic_influ_info);
    if (traffic_influ_info_local_JSON == NULL) {
        ogs_error("OpenAPI_local_offloading_mgt_info_to_ismf_convertToJSON() failed [traffic_influ_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "trafficInfluInfo", traffic_influ_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_local_offloading_mgt_info_to_ismf_convertToJSON() failed [traffic_influ_info]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_local_offloading_mgt_info_to_ismf_t *OpenAPI_local_offloading_mgt_info_to_ismf_parseFromJSON(cJSON *local_offloading_mgt_info_to_ismfJSON)
{
    OpenAPI_local_offloading_mgt_info_to_ismf_t *local_offloading_mgt_info_to_ismf_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *local_offloading_mgt_allowed_ind = NULL;
    cJSON *dns_addr = NULL;
    OpenAPI_ip_address_t *dns_addr_local_nonprim = NULL;
    cJSON *psa_upf_addr = NULL;
    OpenAPI_ip_address_t *psa_upf_addr_local_nonprim = NULL;
    cJSON *local_offloading_info_list = NULL;
    OpenAPI_list_t *local_offloading_info_listList = NULL;
    cJSON *offload_ids = NULL;
    OpenAPI_list_t *offload_idsList = NULL;
    cJSON *traffic_influ_info = NULL;
    OpenAPI_traffic_influence_info_t *traffic_influ_info_local_nonprim = NULL;
    local_offloading_mgt_allowed_ind = cJSON_GetObjectItemCaseSensitive(local_offloading_mgt_info_to_ismfJSON, "localOffloadingMgtAllowedInd");
    if (local_offloading_mgt_allowed_ind) {
    if (!cJSON_IsBool(local_offloading_mgt_allowed_ind)) {
        ogs_error("OpenAPI_local_offloading_mgt_info_to_ismf_parseFromJSON() failed [local_offloading_mgt_allowed_ind]");
        goto end;
    }
    }

    dns_addr = cJSON_GetObjectItemCaseSensitive(local_offloading_mgt_info_to_ismfJSON, "dnsAddr");
    if (dns_addr) {
    dns_addr_local_nonprim = OpenAPI_ip_address_parseFromJSON(dns_addr);
    if (!dns_addr_local_nonprim) {
        ogs_error("OpenAPI_ip_address_parseFromJSON failed [dns_addr]");
        goto end;
    }
    }

    psa_upf_addr = cJSON_GetObjectItemCaseSensitive(local_offloading_mgt_info_to_ismfJSON, "psaUpfAddr");
    if (psa_upf_addr) {
    psa_upf_addr_local_nonprim = OpenAPI_ip_address_parseFromJSON(psa_upf_addr);
    if (!psa_upf_addr_local_nonprim) {
        ogs_error("OpenAPI_ip_address_parseFromJSON failed [psa_upf_addr]");
        goto end;
    }
    }

    local_offloading_info_list = cJSON_GetObjectItemCaseSensitive(local_offloading_mgt_info_to_ismfJSON, "localOffloadingInfoList");
    if (local_offloading_info_list) {
        cJSON *local_offloading_info_list_local = NULL;
        if (!cJSON_IsArray(local_offloading_info_list)) {
            ogs_error("OpenAPI_local_offloading_mgt_info_to_ismf_parseFromJSON() failed [local_offloading_info_list]");
            goto end;
        }

        local_offloading_info_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(local_offloading_info_list_local, local_offloading_info_list) {
            if (!cJSON_IsObject(local_offloading_info_list_local)) {
                ogs_error("OpenAPI_local_offloading_mgt_info_to_ismf_parseFromJSON() failed [local_offloading_info_list]");
                goto end;
            }
            OpenAPI_local_offloading_management_info_t *local_offloading_info_listItem = OpenAPI_local_offloading_management_info_parseFromJSON(local_offloading_info_list_local);
            if (!local_offloading_info_listItem) {
                ogs_error("No local_offloading_info_listItem");
                goto end;
            }
            OpenAPI_list_add(local_offloading_info_listList, local_offloading_info_listItem);
        }
    }

    offload_ids = cJSON_GetObjectItemCaseSensitive(local_offloading_mgt_info_to_ismfJSON, "offloadIds");
    if (offload_ids) {
        cJSON *offload_ids_local = NULL;
        if (!cJSON_IsArray(offload_ids)) {
            ogs_error("OpenAPI_local_offloading_mgt_info_to_ismf_parseFromJSON() failed [offload_ids]");
            goto end;
        }

        offload_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(offload_ids_local, offload_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(offload_ids_local)) {
                ogs_error("OpenAPI_local_offloading_mgt_info_to_ismf_parseFromJSON() failed [offload_ids]");
                goto end;
            }
            OpenAPI_list_add(offload_idsList, ogs_strdup(offload_ids_local->valuestring));
        }
    }

    traffic_influ_info = cJSON_GetObjectItemCaseSensitive(local_offloading_mgt_info_to_ismfJSON, "trafficInfluInfo");
    if (traffic_influ_info) {
    traffic_influ_info_local_nonprim = OpenAPI_traffic_influence_info_parseFromJSON(traffic_influ_info);
    if (!traffic_influ_info_local_nonprim) {
        ogs_error("OpenAPI_traffic_influence_info_parseFromJSON failed [traffic_influ_info]");
        goto end;
    }
    }

    local_offloading_mgt_info_to_ismf_local_var = OpenAPI_local_offloading_mgt_info_to_ismf_create (
        local_offloading_mgt_allowed_ind ? true : false,
        local_offloading_mgt_allowed_ind ? local_offloading_mgt_allowed_ind->valueint : 0,
        dns_addr ? dns_addr_local_nonprim : NULL,
        psa_upf_addr ? psa_upf_addr_local_nonprim : NULL,
        local_offloading_info_list ? local_offloading_info_listList : NULL,
        offload_ids ? offload_idsList : NULL,
        traffic_influ_info ? traffic_influ_info_local_nonprim : NULL
    );

    return local_offloading_mgt_info_to_ismf_local_var;
end:
    if (dns_addr_local_nonprim) {
        OpenAPI_ip_address_free(dns_addr_local_nonprim);
        dns_addr_local_nonprim = NULL;
    }
    if (psa_upf_addr_local_nonprim) {
        OpenAPI_ip_address_free(psa_upf_addr_local_nonprim);
        psa_upf_addr_local_nonprim = NULL;
    }
    if (local_offloading_info_listList) {
        OpenAPI_list_for_each(local_offloading_info_listList, node) {
            OpenAPI_local_offloading_management_info_free(node->data);
        }
        OpenAPI_list_free(local_offloading_info_listList);
        local_offloading_info_listList = NULL;
    }
    if (offload_idsList) {
        OpenAPI_list_for_each(offload_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(offload_idsList);
        offload_idsList = NULL;
    }
    if (traffic_influ_info_local_nonprim) {
        OpenAPI_traffic_influence_info_free(traffic_influ_info_local_nonprim);
        traffic_influ_info_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_local_offloading_mgt_info_to_ismf_t *OpenAPI_local_offloading_mgt_info_to_ismf_copy(OpenAPI_local_offloading_mgt_info_to_ismf_t *dst, OpenAPI_local_offloading_mgt_info_to_ismf_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_local_offloading_mgt_info_to_ismf_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_local_offloading_mgt_info_to_ismf_convertToJSON() failed");
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

    OpenAPI_local_offloading_mgt_info_to_ismf_free(dst);
    dst = OpenAPI_local_offloading_mgt_info_to_ismf_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

