
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "hrsbo_info_from_vplmn.h"

OpenAPI_hrsbo_info_from_vplmn_t *OpenAPI_hrsbo_info_from_vplmn_create(
    bool is_hrsbo_auth_req_ind,
    int hrsbo_auth_req_ind,
    OpenAPI_ip_address_t *v_easdf_addr,
    OpenAPI_ip_address_t *local_dns_addr,
    bool is_eas_rediscovery_ind,
    int eas_rediscovery_ind,
    OpenAPI_eas_info_to_refresh_t *eas_info_to_refresh,
    OpenAPI_list_t *stored_offload_ids,
    bool is_eas_ip_repl_support_ind,
    int eas_ip_repl_support_ind,
    OpenAPI_list_t *v_easdf_security_info,
    OpenAPI_list_t *local_dns_security_info
)
{
    OpenAPI_hrsbo_info_from_vplmn_t *hrsbo_info_from_vplmn_local_var = ogs_malloc(sizeof(OpenAPI_hrsbo_info_from_vplmn_t));
    ogs_assert(hrsbo_info_from_vplmn_local_var);

    hrsbo_info_from_vplmn_local_var->is_hrsbo_auth_req_ind = is_hrsbo_auth_req_ind;
    hrsbo_info_from_vplmn_local_var->hrsbo_auth_req_ind = hrsbo_auth_req_ind;
    hrsbo_info_from_vplmn_local_var->v_easdf_addr = v_easdf_addr;
    hrsbo_info_from_vplmn_local_var->local_dns_addr = local_dns_addr;
    hrsbo_info_from_vplmn_local_var->is_eas_rediscovery_ind = is_eas_rediscovery_ind;
    hrsbo_info_from_vplmn_local_var->eas_rediscovery_ind = eas_rediscovery_ind;
    hrsbo_info_from_vplmn_local_var->eas_info_to_refresh = eas_info_to_refresh;
    hrsbo_info_from_vplmn_local_var->stored_offload_ids = stored_offload_ids;
    hrsbo_info_from_vplmn_local_var->is_eas_ip_repl_support_ind = is_eas_ip_repl_support_ind;
    hrsbo_info_from_vplmn_local_var->eas_ip_repl_support_ind = eas_ip_repl_support_ind;
    hrsbo_info_from_vplmn_local_var->v_easdf_security_info = v_easdf_security_info;
    hrsbo_info_from_vplmn_local_var->local_dns_security_info = local_dns_security_info;

    return hrsbo_info_from_vplmn_local_var;
}

void OpenAPI_hrsbo_info_from_vplmn_free(OpenAPI_hrsbo_info_from_vplmn_t *hrsbo_info_from_vplmn)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == hrsbo_info_from_vplmn) {
        return;
    }
    if (hrsbo_info_from_vplmn->v_easdf_addr) {
        OpenAPI_ip_address_free(hrsbo_info_from_vplmn->v_easdf_addr);
        hrsbo_info_from_vplmn->v_easdf_addr = NULL;
    }
    if (hrsbo_info_from_vplmn->local_dns_addr) {
        OpenAPI_ip_address_free(hrsbo_info_from_vplmn->local_dns_addr);
        hrsbo_info_from_vplmn->local_dns_addr = NULL;
    }
    if (hrsbo_info_from_vplmn->eas_info_to_refresh) {
        OpenAPI_eas_info_to_refresh_free(hrsbo_info_from_vplmn->eas_info_to_refresh);
        hrsbo_info_from_vplmn->eas_info_to_refresh = NULL;
    }
    if (hrsbo_info_from_vplmn->stored_offload_ids) {
        OpenAPI_list_for_each(hrsbo_info_from_vplmn->stored_offload_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(hrsbo_info_from_vplmn->stored_offload_ids);
        hrsbo_info_from_vplmn->stored_offload_ids = NULL;
    }
    if (hrsbo_info_from_vplmn->v_easdf_security_info) {
        OpenAPI_list_for_each(hrsbo_info_from_vplmn->v_easdf_security_info, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(hrsbo_info_from_vplmn->v_easdf_security_info);
        hrsbo_info_from_vplmn->v_easdf_security_info = NULL;
    }
    if (hrsbo_info_from_vplmn->local_dns_security_info) {
        OpenAPI_list_for_each(hrsbo_info_from_vplmn->local_dns_security_info, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(hrsbo_info_from_vplmn->local_dns_security_info);
        hrsbo_info_from_vplmn->local_dns_security_info = NULL;
    }
    ogs_free(hrsbo_info_from_vplmn);
}

cJSON *OpenAPI_hrsbo_info_from_vplmn_convertToJSON(OpenAPI_hrsbo_info_from_vplmn_t *hrsbo_info_from_vplmn)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (hrsbo_info_from_vplmn == NULL) {
        ogs_error("OpenAPI_hrsbo_info_from_vplmn_convertToJSON() failed [HrsboInfoFromVplmn]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (hrsbo_info_from_vplmn->is_hrsbo_auth_req_ind) {
    if (cJSON_AddBoolToObject(item, "hrsboAuthReqInd", hrsbo_info_from_vplmn->hrsbo_auth_req_ind) == NULL) {
        ogs_error("OpenAPI_hrsbo_info_from_vplmn_convertToJSON() failed [hrsbo_auth_req_ind]");
        goto end;
    }
    }

    if (hrsbo_info_from_vplmn->v_easdf_addr) {
    cJSON *v_easdf_addr_local_JSON = OpenAPI_ip_address_convertToJSON(hrsbo_info_from_vplmn->v_easdf_addr);
    if (v_easdf_addr_local_JSON == NULL) {
        ogs_error("OpenAPI_hrsbo_info_from_vplmn_convertToJSON() failed [v_easdf_addr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "vEasdfAddr", v_easdf_addr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_hrsbo_info_from_vplmn_convertToJSON() failed [v_easdf_addr]");
        goto end;
    }
    }

    if (hrsbo_info_from_vplmn->local_dns_addr) {
    cJSON *local_dns_addr_local_JSON = OpenAPI_ip_address_convertToJSON(hrsbo_info_from_vplmn->local_dns_addr);
    if (local_dns_addr_local_JSON == NULL) {
        ogs_error("OpenAPI_hrsbo_info_from_vplmn_convertToJSON() failed [local_dns_addr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "localDnsAddr", local_dns_addr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_hrsbo_info_from_vplmn_convertToJSON() failed [local_dns_addr]");
        goto end;
    }
    }

    if (hrsbo_info_from_vplmn->is_eas_rediscovery_ind) {
    if (cJSON_AddBoolToObject(item, "easRediscoveryInd", hrsbo_info_from_vplmn->eas_rediscovery_ind) == NULL) {
        ogs_error("OpenAPI_hrsbo_info_from_vplmn_convertToJSON() failed [eas_rediscovery_ind]");
        goto end;
    }
    }

    if (hrsbo_info_from_vplmn->eas_info_to_refresh) {
    cJSON *eas_info_to_refresh_local_JSON = OpenAPI_eas_info_to_refresh_convertToJSON(hrsbo_info_from_vplmn->eas_info_to_refresh);
    if (eas_info_to_refresh_local_JSON == NULL) {
        ogs_error("OpenAPI_hrsbo_info_from_vplmn_convertToJSON() failed [eas_info_to_refresh]");
        goto end;
    }
    cJSON_AddItemToObject(item, "easInfoToRefresh", eas_info_to_refresh_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_hrsbo_info_from_vplmn_convertToJSON() failed [eas_info_to_refresh]");
        goto end;
    }
    }

    if (hrsbo_info_from_vplmn->stored_offload_ids) {
    cJSON *stored_offload_idsList = cJSON_AddArrayToObject(item, "storedOffloadIds");
    if (stored_offload_idsList == NULL) {
        ogs_error("OpenAPI_hrsbo_info_from_vplmn_convertToJSON() failed [stored_offload_ids]");
        goto end;
    }
    OpenAPI_list_for_each(hrsbo_info_from_vplmn->stored_offload_ids, node) {
        if (cJSON_AddStringToObject(stored_offload_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_hrsbo_info_from_vplmn_convertToJSON() failed [stored_offload_ids]");
            goto end;
        }
    }
    }

    if (hrsbo_info_from_vplmn->is_eas_ip_repl_support_ind) {
    if (cJSON_AddBoolToObject(item, "easIpReplSupportInd", hrsbo_info_from_vplmn->eas_ip_repl_support_ind) == NULL) {
        ogs_error("OpenAPI_hrsbo_info_from_vplmn_convertToJSON() failed [eas_ip_repl_support_ind]");
        goto end;
    }
    }

    if (hrsbo_info_from_vplmn->v_easdf_security_info) {
    cJSON *v_easdf_security_infoList = cJSON_AddArrayToObject(item, "vEasdfSecurityInfo");
    if (v_easdf_security_infoList == NULL) {
        ogs_error("OpenAPI_hrsbo_info_from_vplmn_convertToJSON() failed [v_easdf_security_info]");
        goto end;
    }
    OpenAPI_list_for_each(hrsbo_info_from_vplmn->v_easdf_security_info, node) {
        if (cJSON_AddStringToObject(v_easdf_security_infoList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_hrsbo_info_from_vplmn_convertToJSON() failed [v_easdf_security_info]");
            goto end;
        }
    }
    }

    if (hrsbo_info_from_vplmn->local_dns_security_info) {
    cJSON *local_dns_security_infoList = cJSON_AddArrayToObject(item, "localDnsSecurityInfo");
    if (local_dns_security_infoList == NULL) {
        ogs_error("OpenAPI_hrsbo_info_from_vplmn_convertToJSON() failed [local_dns_security_info]");
        goto end;
    }
    OpenAPI_list_for_each(hrsbo_info_from_vplmn->local_dns_security_info, node) {
        if (cJSON_AddStringToObject(local_dns_security_infoList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_hrsbo_info_from_vplmn_convertToJSON() failed [local_dns_security_info]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_hrsbo_info_from_vplmn_t *OpenAPI_hrsbo_info_from_vplmn_parseFromJSON(cJSON *hrsbo_info_from_vplmnJSON)
{
    OpenAPI_hrsbo_info_from_vplmn_t *hrsbo_info_from_vplmn_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *hrsbo_auth_req_ind = NULL;
    cJSON *v_easdf_addr = NULL;
    OpenAPI_ip_address_t *v_easdf_addr_local_nonprim = NULL;
    cJSON *local_dns_addr = NULL;
    OpenAPI_ip_address_t *local_dns_addr_local_nonprim = NULL;
    cJSON *eas_rediscovery_ind = NULL;
    cJSON *eas_info_to_refresh = NULL;
    OpenAPI_eas_info_to_refresh_t *eas_info_to_refresh_local_nonprim = NULL;
    cJSON *stored_offload_ids = NULL;
    OpenAPI_list_t *stored_offload_idsList = NULL;
    cJSON *eas_ip_repl_support_ind = NULL;
    cJSON *v_easdf_security_info = NULL;
    OpenAPI_list_t *v_easdf_security_infoList = NULL;
    cJSON *local_dns_security_info = NULL;
    OpenAPI_list_t *local_dns_security_infoList = NULL;
    hrsbo_auth_req_ind = cJSON_GetObjectItemCaseSensitive(hrsbo_info_from_vplmnJSON, "hrsboAuthReqInd");
    if (hrsbo_auth_req_ind) {
    if (!cJSON_IsBool(hrsbo_auth_req_ind)) {
        ogs_error("OpenAPI_hrsbo_info_from_vplmn_parseFromJSON() failed [hrsbo_auth_req_ind]");
        goto end;
    }
    }

    v_easdf_addr = cJSON_GetObjectItemCaseSensitive(hrsbo_info_from_vplmnJSON, "vEasdfAddr");
    if (v_easdf_addr) {
    v_easdf_addr_local_nonprim = OpenAPI_ip_address_parseFromJSON(v_easdf_addr);
    if (!v_easdf_addr_local_nonprim) {
        ogs_error("OpenAPI_ip_address_parseFromJSON failed [v_easdf_addr]");
        goto end;
    }
    }

    local_dns_addr = cJSON_GetObjectItemCaseSensitive(hrsbo_info_from_vplmnJSON, "localDnsAddr");
    if (local_dns_addr) {
    local_dns_addr_local_nonprim = OpenAPI_ip_address_parseFromJSON(local_dns_addr);
    if (!local_dns_addr_local_nonprim) {
        ogs_error("OpenAPI_ip_address_parseFromJSON failed [local_dns_addr]");
        goto end;
    }
    }

    eas_rediscovery_ind = cJSON_GetObjectItemCaseSensitive(hrsbo_info_from_vplmnJSON, "easRediscoveryInd");
    if (eas_rediscovery_ind) {
    if (!cJSON_IsBool(eas_rediscovery_ind)) {
        ogs_error("OpenAPI_hrsbo_info_from_vplmn_parseFromJSON() failed [eas_rediscovery_ind]");
        goto end;
    }
    }

    eas_info_to_refresh = cJSON_GetObjectItemCaseSensitive(hrsbo_info_from_vplmnJSON, "easInfoToRefresh");
    if (eas_info_to_refresh) {
    eas_info_to_refresh_local_nonprim = OpenAPI_eas_info_to_refresh_parseFromJSON(eas_info_to_refresh);
    if (!eas_info_to_refresh_local_nonprim) {
        ogs_error("OpenAPI_eas_info_to_refresh_parseFromJSON failed [eas_info_to_refresh]");
        goto end;
    }
    }

    stored_offload_ids = cJSON_GetObjectItemCaseSensitive(hrsbo_info_from_vplmnJSON, "storedOffloadIds");
    if (stored_offload_ids) {
        cJSON *stored_offload_ids_local = NULL;
        if (!cJSON_IsArray(stored_offload_ids)) {
            ogs_error("OpenAPI_hrsbo_info_from_vplmn_parseFromJSON() failed [stored_offload_ids]");
            goto end;
        }

        stored_offload_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(stored_offload_ids_local, stored_offload_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(stored_offload_ids_local)) {
                ogs_error("OpenAPI_hrsbo_info_from_vplmn_parseFromJSON() failed [stored_offload_ids]");
                goto end;
            }
            OpenAPI_list_add(stored_offload_idsList, ogs_strdup(stored_offload_ids_local->valuestring));
        }
    }

    eas_ip_repl_support_ind = cJSON_GetObjectItemCaseSensitive(hrsbo_info_from_vplmnJSON, "easIpReplSupportInd");
    if (eas_ip_repl_support_ind) {
    if (!cJSON_IsBool(eas_ip_repl_support_ind)) {
        ogs_error("OpenAPI_hrsbo_info_from_vplmn_parseFromJSON() failed [eas_ip_repl_support_ind]");
        goto end;
    }
    }

    v_easdf_security_info = cJSON_GetObjectItemCaseSensitive(hrsbo_info_from_vplmnJSON, "vEasdfSecurityInfo");
    if (v_easdf_security_info) {
        cJSON *v_easdf_security_info_local = NULL;
        if (!cJSON_IsArray(v_easdf_security_info)) {
            ogs_error("OpenAPI_hrsbo_info_from_vplmn_parseFromJSON() failed [v_easdf_security_info]");
            goto end;
        }

        v_easdf_security_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(v_easdf_security_info_local, v_easdf_security_info) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(v_easdf_security_info_local)) {
                ogs_error("OpenAPI_hrsbo_info_from_vplmn_parseFromJSON() failed [v_easdf_security_info]");
                goto end;
            }
            OpenAPI_list_add(v_easdf_security_infoList, ogs_strdup(v_easdf_security_info_local->valuestring));
        }
    }

    local_dns_security_info = cJSON_GetObjectItemCaseSensitive(hrsbo_info_from_vplmnJSON, "localDnsSecurityInfo");
    if (local_dns_security_info) {
        cJSON *local_dns_security_info_local = NULL;
        if (!cJSON_IsArray(local_dns_security_info)) {
            ogs_error("OpenAPI_hrsbo_info_from_vplmn_parseFromJSON() failed [local_dns_security_info]");
            goto end;
        }

        local_dns_security_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(local_dns_security_info_local, local_dns_security_info) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(local_dns_security_info_local)) {
                ogs_error("OpenAPI_hrsbo_info_from_vplmn_parseFromJSON() failed [local_dns_security_info]");
                goto end;
            }
            OpenAPI_list_add(local_dns_security_infoList, ogs_strdup(local_dns_security_info_local->valuestring));
        }
    }

    hrsbo_info_from_vplmn_local_var = OpenAPI_hrsbo_info_from_vplmn_create (
        hrsbo_auth_req_ind ? true : false,
        hrsbo_auth_req_ind ? hrsbo_auth_req_ind->valueint : 0,
        v_easdf_addr ? v_easdf_addr_local_nonprim : NULL,
        local_dns_addr ? local_dns_addr_local_nonprim : NULL,
        eas_rediscovery_ind ? true : false,
        eas_rediscovery_ind ? eas_rediscovery_ind->valueint : 0,
        eas_info_to_refresh ? eas_info_to_refresh_local_nonprim : NULL,
        stored_offload_ids ? stored_offload_idsList : NULL,
        eas_ip_repl_support_ind ? true : false,
        eas_ip_repl_support_ind ? eas_ip_repl_support_ind->valueint : 0,
        v_easdf_security_info ? v_easdf_security_infoList : NULL,
        local_dns_security_info ? local_dns_security_infoList : NULL
    );

    return hrsbo_info_from_vplmn_local_var;
end:
    if (v_easdf_addr_local_nonprim) {
        OpenAPI_ip_address_free(v_easdf_addr_local_nonprim);
        v_easdf_addr_local_nonprim = NULL;
    }
    if (local_dns_addr_local_nonprim) {
        OpenAPI_ip_address_free(local_dns_addr_local_nonprim);
        local_dns_addr_local_nonprim = NULL;
    }
    if (eas_info_to_refresh_local_nonprim) {
        OpenAPI_eas_info_to_refresh_free(eas_info_to_refresh_local_nonprim);
        eas_info_to_refresh_local_nonprim = NULL;
    }
    if (stored_offload_idsList) {
        OpenAPI_list_for_each(stored_offload_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(stored_offload_idsList);
        stored_offload_idsList = NULL;
    }
    if (v_easdf_security_infoList) {
        OpenAPI_list_for_each(v_easdf_security_infoList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(v_easdf_security_infoList);
        v_easdf_security_infoList = NULL;
    }
    if (local_dns_security_infoList) {
        OpenAPI_list_for_each(local_dns_security_infoList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(local_dns_security_infoList);
        local_dns_security_infoList = NULL;
    }
    return NULL;
}

OpenAPI_hrsbo_info_from_vplmn_t *OpenAPI_hrsbo_info_from_vplmn_copy(OpenAPI_hrsbo_info_from_vplmn_t *dst, OpenAPI_hrsbo_info_from_vplmn_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_hrsbo_info_from_vplmn_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_hrsbo_info_from_vplmn_convertToJSON() failed");
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

    OpenAPI_hrsbo_info_from_vplmn_free(dst);
    dst = OpenAPI_hrsbo_info_from_vplmn_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

