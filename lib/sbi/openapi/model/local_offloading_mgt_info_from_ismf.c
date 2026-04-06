
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "local_offloading_mgt_info_from_ismf.h"

OpenAPI_local_offloading_mgt_info_from_ismf_t *OpenAPI_local_offloading_mgt_info_from_ismf_create(
    bool is_local_offloading_mgt_allowed_ind,
    int local_offloading_mgt_allowed_ind,
    OpenAPI_ip_address_t *easdf_addr,
    OpenAPI_list_t *easdf_security_info,
    bool is_eas_rediscovery_ind,
    int eas_rediscovery_ind,
    OpenAPI_eas_info_to_refresh_t *eas_info_to_refresh,
    OpenAPI_list_t *stored_offload_ids
)
{
    OpenAPI_local_offloading_mgt_info_from_ismf_t *local_offloading_mgt_info_from_ismf_local_var = ogs_malloc(sizeof(OpenAPI_local_offloading_mgt_info_from_ismf_t));
    ogs_assert(local_offloading_mgt_info_from_ismf_local_var);

    local_offloading_mgt_info_from_ismf_local_var->is_local_offloading_mgt_allowed_ind = is_local_offloading_mgt_allowed_ind;
    local_offloading_mgt_info_from_ismf_local_var->local_offloading_mgt_allowed_ind = local_offloading_mgt_allowed_ind;
    local_offloading_mgt_info_from_ismf_local_var->easdf_addr = easdf_addr;
    local_offloading_mgt_info_from_ismf_local_var->easdf_security_info = easdf_security_info;
    local_offloading_mgt_info_from_ismf_local_var->is_eas_rediscovery_ind = is_eas_rediscovery_ind;
    local_offloading_mgt_info_from_ismf_local_var->eas_rediscovery_ind = eas_rediscovery_ind;
    local_offloading_mgt_info_from_ismf_local_var->eas_info_to_refresh = eas_info_to_refresh;
    local_offloading_mgt_info_from_ismf_local_var->stored_offload_ids = stored_offload_ids;

    return local_offloading_mgt_info_from_ismf_local_var;
}

void OpenAPI_local_offloading_mgt_info_from_ismf_free(OpenAPI_local_offloading_mgt_info_from_ismf_t *local_offloading_mgt_info_from_ismf)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == local_offloading_mgt_info_from_ismf) {
        return;
    }
    if (local_offloading_mgt_info_from_ismf->easdf_addr) {
        OpenAPI_ip_address_free(local_offloading_mgt_info_from_ismf->easdf_addr);
        local_offloading_mgt_info_from_ismf->easdf_addr = NULL;
    }
    if (local_offloading_mgt_info_from_ismf->easdf_security_info) {
        OpenAPI_list_for_each(local_offloading_mgt_info_from_ismf->easdf_security_info, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(local_offloading_mgt_info_from_ismf->easdf_security_info);
        local_offloading_mgt_info_from_ismf->easdf_security_info = NULL;
    }
    if (local_offloading_mgt_info_from_ismf->eas_info_to_refresh) {
        OpenAPI_eas_info_to_refresh_free(local_offloading_mgt_info_from_ismf->eas_info_to_refresh);
        local_offloading_mgt_info_from_ismf->eas_info_to_refresh = NULL;
    }
    if (local_offloading_mgt_info_from_ismf->stored_offload_ids) {
        OpenAPI_list_for_each(local_offloading_mgt_info_from_ismf->stored_offload_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(local_offloading_mgt_info_from_ismf->stored_offload_ids);
        local_offloading_mgt_info_from_ismf->stored_offload_ids = NULL;
    }
    ogs_free(local_offloading_mgt_info_from_ismf);
}

cJSON *OpenAPI_local_offloading_mgt_info_from_ismf_convertToJSON(OpenAPI_local_offloading_mgt_info_from_ismf_t *local_offloading_mgt_info_from_ismf)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (local_offloading_mgt_info_from_ismf == NULL) {
        ogs_error("OpenAPI_local_offloading_mgt_info_from_ismf_convertToJSON() failed [LocalOffloadingMgtInfoFromIsmf]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (local_offloading_mgt_info_from_ismf->is_local_offloading_mgt_allowed_ind) {
    if (cJSON_AddBoolToObject(item, "localOffloadingMgtAllowedInd", local_offloading_mgt_info_from_ismf->local_offloading_mgt_allowed_ind) == NULL) {
        ogs_error("OpenAPI_local_offloading_mgt_info_from_ismf_convertToJSON() failed [local_offloading_mgt_allowed_ind]");
        goto end;
    }
    }

    if (local_offloading_mgt_info_from_ismf->easdf_addr) {
    cJSON *easdf_addr_local_JSON = OpenAPI_ip_address_convertToJSON(local_offloading_mgt_info_from_ismf->easdf_addr);
    if (easdf_addr_local_JSON == NULL) {
        ogs_error("OpenAPI_local_offloading_mgt_info_from_ismf_convertToJSON() failed [easdf_addr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "easdfAddr", easdf_addr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_local_offloading_mgt_info_from_ismf_convertToJSON() failed [easdf_addr]");
        goto end;
    }
    }

    if (local_offloading_mgt_info_from_ismf->easdf_security_info) {
    cJSON *easdf_security_infoList = cJSON_AddArrayToObject(item, "easdfSecurityInfo");
    if (easdf_security_infoList == NULL) {
        ogs_error("OpenAPI_local_offloading_mgt_info_from_ismf_convertToJSON() failed [easdf_security_info]");
        goto end;
    }
    OpenAPI_list_for_each(local_offloading_mgt_info_from_ismf->easdf_security_info, node) {
        if (cJSON_AddStringToObject(easdf_security_infoList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_local_offloading_mgt_info_from_ismf_convertToJSON() failed [easdf_security_info]");
            goto end;
        }
    }
    }

    if (local_offloading_mgt_info_from_ismf->is_eas_rediscovery_ind) {
    if (cJSON_AddBoolToObject(item, "easRediscoveryInd", local_offloading_mgt_info_from_ismf->eas_rediscovery_ind) == NULL) {
        ogs_error("OpenAPI_local_offloading_mgt_info_from_ismf_convertToJSON() failed [eas_rediscovery_ind]");
        goto end;
    }
    }

    if (local_offloading_mgt_info_from_ismf->eas_info_to_refresh) {
    cJSON *eas_info_to_refresh_local_JSON = OpenAPI_eas_info_to_refresh_convertToJSON(local_offloading_mgt_info_from_ismf->eas_info_to_refresh);
    if (eas_info_to_refresh_local_JSON == NULL) {
        ogs_error("OpenAPI_local_offloading_mgt_info_from_ismf_convertToJSON() failed [eas_info_to_refresh]");
        goto end;
    }
    cJSON_AddItemToObject(item, "easInfoToRefresh", eas_info_to_refresh_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_local_offloading_mgt_info_from_ismf_convertToJSON() failed [eas_info_to_refresh]");
        goto end;
    }
    }

    if (local_offloading_mgt_info_from_ismf->stored_offload_ids) {
    cJSON *stored_offload_idsList = cJSON_AddArrayToObject(item, "storedOffloadIds");
    if (stored_offload_idsList == NULL) {
        ogs_error("OpenAPI_local_offloading_mgt_info_from_ismf_convertToJSON() failed [stored_offload_ids]");
        goto end;
    }
    OpenAPI_list_for_each(local_offloading_mgt_info_from_ismf->stored_offload_ids, node) {
        if (cJSON_AddStringToObject(stored_offload_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_local_offloading_mgt_info_from_ismf_convertToJSON() failed [stored_offload_ids]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_local_offloading_mgt_info_from_ismf_t *OpenAPI_local_offloading_mgt_info_from_ismf_parseFromJSON(cJSON *local_offloading_mgt_info_from_ismfJSON)
{
    OpenAPI_local_offloading_mgt_info_from_ismf_t *local_offloading_mgt_info_from_ismf_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *local_offloading_mgt_allowed_ind = NULL;
    cJSON *easdf_addr = NULL;
    OpenAPI_ip_address_t *easdf_addr_local_nonprim = NULL;
    cJSON *easdf_security_info = NULL;
    OpenAPI_list_t *easdf_security_infoList = NULL;
    cJSON *eas_rediscovery_ind = NULL;
    cJSON *eas_info_to_refresh = NULL;
    OpenAPI_eas_info_to_refresh_t *eas_info_to_refresh_local_nonprim = NULL;
    cJSON *stored_offload_ids = NULL;
    OpenAPI_list_t *stored_offload_idsList = NULL;
    local_offloading_mgt_allowed_ind = cJSON_GetObjectItemCaseSensitive(local_offloading_mgt_info_from_ismfJSON, "localOffloadingMgtAllowedInd");
    if (local_offloading_mgt_allowed_ind) {
    if (!cJSON_IsBool(local_offloading_mgt_allowed_ind)) {
        ogs_error("OpenAPI_local_offloading_mgt_info_from_ismf_parseFromJSON() failed [local_offloading_mgt_allowed_ind]");
        goto end;
    }
    }

    easdf_addr = cJSON_GetObjectItemCaseSensitive(local_offloading_mgt_info_from_ismfJSON, "easdfAddr");
    if (easdf_addr) {
    easdf_addr_local_nonprim = OpenAPI_ip_address_parseFromJSON(easdf_addr);
    if (!easdf_addr_local_nonprim) {
        ogs_error("OpenAPI_ip_address_parseFromJSON failed [easdf_addr]");
        goto end;
    }
    }

    easdf_security_info = cJSON_GetObjectItemCaseSensitive(local_offloading_mgt_info_from_ismfJSON, "easdfSecurityInfo");
    if (easdf_security_info) {
        cJSON *easdf_security_info_local = NULL;
        if (!cJSON_IsArray(easdf_security_info)) {
            ogs_error("OpenAPI_local_offloading_mgt_info_from_ismf_parseFromJSON() failed [easdf_security_info]");
            goto end;
        }

        easdf_security_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(easdf_security_info_local, easdf_security_info) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(easdf_security_info_local)) {
                ogs_error("OpenAPI_local_offloading_mgt_info_from_ismf_parseFromJSON() failed [easdf_security_info]");
                goto end;
            }
            OpenAPI_list_add(easdf_security_infoList, ogs_strdup(easdf_security_info_local->valuestring));
        }
    }

    eas_rediscovery_ind = cJSON_GetObjectItemCaseSensitive(local_offloading_mgt_info_from_ismfJSON, "easRediscoveryInd");
    if (eas_rediscovery_ind) {
    if (!cJSON_IsBool(eas_rediscovery_ind)) {
        ogs_error("OpenAPI_local_offloading_mgt_info_from_ismf_parseFromJSON() failed [eas_rediscovery_ind]");
        goto end;
    }
    }

    eas_info_to_refresh = cJSON_GetObjectItemCaseSensitive(local_offloading_mgt_info_from_ismfJSON, "easInfoToRefresh");
    if (eas_info_to_refresh) {
    eas_info_to_refresh_local_nonprim = OpenAPI_eas_info_to_refresh_parseFromJSON(eas_info_to_refresh);
    if (!eas_info_to_refresh_local_nonprim) {
        ogs_error("OpenAPI_eas_info_to_refresh_parseFromJSON failed [eas_info_to_refresh]");
        goto end;
    }
    }

    stored_offload_ids = cJSON_GetObjectItemCaseSensitive(local_offloading_mgt_info_from_ismfJSON, "storedOffloadIds");
    if (stored_offload_ids) {
        cJSON *stored_offload_ids_local = NULL;
        if (!cJSON_IsArray(stored_offload_ids)) {
            ogs_error("OpenAPI_local_offloading_mgt_info_from_ismf_parseFromJSON() failed [stored_offload_ids]");
            goto end;
        }

        stored_offload_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(stored_offload_ids_local, stored_offload_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(stored_offload_ids_local)) {
                ogs_error("OpenAPI_local_offloading_mgt_info_from_ismf_parseFromJSON() failed [stored_offload_ids]");
                goto end;
            }
            OpenAPI_list_add(stored_offload_idsList, ogs_strdup(stored_offload_ids_local->valuestring));
        }
    }

    local_offloading_mgt_info_from_ismf_local_var = OpenAPI_local_offloading_mgt_info_from_ismf_create (
        local_offloading_mgt_allowed_ind ? true : false,
        local_offloading_mgt_allowed_ind ? local_offloading_mgt_allowed_ind->valueint : 0,
        easdf_addr ? easdf_addr_local_nonprim : NULL,
        easdf_security_info ? easdf_security_infoList : NULL,
        eas_rediscovery_ind ? true : false,
        eas_rediscovery_ind ? eas_rediscovery_ind->valueint : 0,
        eas_info_to_refresh ? eas_info_to_refresh_local_nonprim : NULL,
        stored_offload_ids ? stored_offload_idsList : NULL
    );

    return local_offloading_mgt_info_from_ismf_local_var;
end:
    if (easdf_addr_local_nonprim) {
        OpenAPI_ip_address_free(easdf_addr_local_nonprim);
        easdf_addr_local_nonprim = NULL;
    }
    if (easdf_security_infoList) {
        OpenAPI_list_for_each(easdf_security_infoList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(easdf_security_infoList);
        easdf_security_infoList = NULL;
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
    return NULL;
}

OpenAPI_local_offloading_mgt_info_from_ismf_t *OpenAPI_local_offloading_mgt_info_from_ismf_copy(OpenAPI_local_offloading_mgt_info_from_ismf_t *dst, OpenAPI_local_offloading_mgt_info_from_ismf_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_local_offloading_mgt_info_from_ismf_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_local_offloading_mgt_info_from_ismf_convertToJSON() failed");
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

    OpenAPI_local_offloading_mgt_info_from_ismf_free(dst);
    dst = OpenAPI_local_offloading_mgt_info_from_ismf_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

