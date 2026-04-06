
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "hrsbo_info_from_hplmn.h"

OpenAPI_hrsbo_info_from_hplmn_t *OpenAPI_hrsbo_info_from_hplmn_create(
    bool is_hrsbo_auth_result,
    int hrsbo_auth_result,
    OpenAPI_ip_address_t *h_dns_addr,
    OpenAPI_ip_address_t *h_plmn_addr,
    OpenAPI_list_t *vplmn_offloading_info_list,
    bool is_vplmn_dl_ambr_null,
    OpenAPI_vplmn_dl_ambr_t *vplmn_dl_ambr,
    OpenAPI_list_t *offload_ids,
    OpenAPI_list_t *internal_group_ids,
    char *target_dnai,
    OpenAPI_traffic_influence_info_t *traffic_influ_info
)
{
    OpenAPI_hrsbo_info_from_hplmn_t *hrsbo_info_from_hplmn_local_var = ogs_malloc(sizeof(OpenAPI_hrsbo_info_from_hplmn_t));
    ogs_assert(hrsbo_info_from_hplmn_local_var);

    hrsbo_info_from_hplmn_local_var->is_hrsbo_auth_result = is_hrsbo_auth_result;
    hrsbo_info_from_hplmn_local_var->hrsbo_auth_result = hrsbo_auth_result;
    hrsbo_info_from_hplmn_local_var->h_dns_addr = h_dns_addr;
    hrsbo_info_from_hplmn_local_var->h_plmn_addr = h_plmn_addr;
    hrsbo_info_from_hplmn_local_var->vplmn_offloading_info_list = vplmn_offloading_info_list;
    hrsbo_info_from_hplmn_local_var->is_vplmn_dl_ambr_null = is_vplmn_dl_ambr_null;
    hrsbo_info_from_hplmn_local_var->vplmn_dl_ambr = vplmn_dl_ambr;
    hrsbo_info_from_hplmn_local_var->offload_ids = offload_ids;
    hrsbo_info_from_hplmn_local_var->internal_group_ids = internal_group_ids;
    hrsbo_info_from_hplmn_local_var->target_dnai = target_dnai;
    hrsbo_info_from_hplmn_local_var->traffic_influ_info = traffic_influ_info;

    return hrsbo_info_from_hplmn_local_var;
}

void OpenAPI_hrsbo_info_from_hplmn_free(OpenAPI_hrsbo_info_from_hplmn_t *hrsbo_info_from_hplmn)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == hrsbo_info_from_hplmn) {
        return;
    }
    if (hrsbo_info_from_hplmn->h_dns_addr) {
        OpenAPI_ip_address_free(hrsbo_info_from_hplmn->h_dns_addr);
        hrsbo_info_from_hplmn->h_dns_addr = NULL;
    }
    if (hrsbo_info_from_hplmn->h_plmn_addr) {
        OpenAPI_ip_address_free(hrsbo_info_from_hplmn->h_plmn_addr);
        hrsbo_info_from_hplmn->h_plmn_addr = NULL;
    }
    if (hrsbo_info_from_hplmn->vplmn_offloading_info_list) {
        OpenAPI_list_for_each(hrsbo_info_from_hplmn->vplmn_offloading_info_list, node) {
            OpenAPI_vplmn_offloading_info_free(node->data);
        }
        OpenAPI_list_free(hrsbo_info_from_hplmn->vplmn_offloading_info_list);
        hrsbo_info_from_hplmn->vplmn_offloading_info_list = NULL;
    }
    if (hrsbo_info_from_hplmn->vplmn_dl_ambr) {
        OpenAPI_vplmn_dl_ambr_free(hrsbo_info_from_hplmn->vplmn_dl_ambr);
        hrsbo_info_from_hplmn->vplmn_dl_ambr = NULL;
    }
    if (hrsbo_info_from_hplmn->offload_ids) {
        OpenAPI_list_for_each(hrsbo_info_from_hplmn->offload_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(hrsbo_info_from_hplmn->offload_ids);
        hrsbo_info_from_hplmn->offload_ids = NULL;
    }
    if (hrsbo_info_from_hplmn->internal_group_ids) {
        OpenAPI_list_for_each(hrsbo_info_from_hplmn->internal_group_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(hrsbo_info_from_hplmn->internal_group_ids);
        hrsbo_info_from_hplmn->internal_group_ids = NULL;
    }
    if (hrsbo_info_from_hplmn->target_dnai) {
        ogs_free(hrsbo_info_from_hplmn->target_dnai);
        hrsbo_info_from_hplmn->target_dnai = NULL;
    }
    if (hrsbo_info_from_hplmn->traffic_influ_info) {
        OpenAPI_traffic_influence_info_free(hrsbo_info_from_hplmn->traffic_influ_info);
        hrsbo_info_from_hplmn->traffic_influ_info = NULL;
    }
    ogs_free(hrsbo_info_from_hplmn);
}

cJSON *OpenAPI_hrsbo_info_from_hplmn_convertToJSON(OpenAPI_hrsbo_info_from_hplmn_t *hrsbo_info_from_hplmn)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (hrsbo_info_from_hplmn == NULL) {
        ogs_error("OpenAPI_hrsbo_info_from_hplmn_convertToJSON() failed [HrsboInfoFromHplmn]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (hrsbo_info_from_hplmn->is_hrsbo_auth_result) {
    if (cJSON_AddBoolToObject(item, "hrsboAuthResult", hrsbo_info_from_hplmn->hrsbo_auth_result) == NULL) {
        ogs_error("OpenAPI_hrsbo_info_from_hplmn_convertToJSON() failed [hrsbo_auth_result]");
        goto end;
    }
    }

    if (hrsbo_info_from_hplmn->h_dns_addr) {
    cJSON *h_dns_addr_local_JSON = OpenAPI_ip_address_convertToJSON(hrsbo_info_from_hplmn->h_dns_addr);
    if (h_dns_addr_local_JSON == NULL) {
        ogs_error("OpenAPI_hrsbo_info_from_hplmn_convertToJSON() failed [h_dns_addr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "hDnsAddr", h_dns_addr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_hrsbo_info_from_hplmn_convertToJSON() failed [h_dns_addr]");
        goto end;
    }
    }

    if (hrsbo_info_from_hplmn->h_plmn_addr) {
    cJSON *h_plmn_addr_local_JSON = OpenAPI_ip_address_convertToJSON(hrsbo_info_from_hplmn->h_plmn_addr);
    if (h_plmn_addr_local_JSON == NULL) {
        ogs_error("OpenAPI_hrsbo_info_from_hplmn_convertToJSON() failed [h_plmn_addr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "hPlmnAddr", h_plmn_addr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_hrsbo_info_from_hplmn_convertToJSON() failed [h_plmn_addr]");
        goto end;
    }
    }

    if (hrsbo_info_from_hplmn->vplmn_offloading_info_list) {
    cJSON *vplmn_offloading_info_listList = cJSON_AddArrayToObject(item, "vplmnOffloadingInfoList");
    if (vplmn_offloading_info_listList == NULL) {
        ogs_error("OpenAPI_hrsbo_info_from_hplmn_convertToJSON() failed [vplmn_offloading_info_list]");
        goto end;
    }
    OpenAPI_list_for_each(hrsbo_info_from_hplmn->vplmn_offloading_info_list, node) {
        cJSON *itemLocal = OpenAPI_vplmn_offloading_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_hrsbo_info_from_hplmn_convertToJSON() failed [vplmn_offloading_info_list]");
            goto end;
        }
        cJSON_AddItemToArray(vplmn_offloading_info_listList, itemLocal);
    }
    }

    if (hrsbo_info_from_hplmn->vplmn_dl_ambr) {
    cJSON *vplmn_dl_ambr_local_JSON = OpenAPI_vplmn_dl_ambr_convertToJSON(hrsbo_info_from_hplmn->vplmn_dl_ambr);
    if (vplmn_dl_ambr_local_JSON == NULL) {
        ogs_error("OpenAPI_hrsbo_info_from_hplmn_convertToJSON() failed [vplmn_dl_ambr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "vplmnDlAmbr", vplmn_dl_ambr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_hrsbo_info_from_hplmn_convertToJSON() failed [vplmn_dl_ambr]");
        goto end;
    }
    } else if (hrsbo_info_from_hplmn->is_vplmn_dl_ambr_null) {
        if (cJSON_AddNullToObject(item, "vplmnDlAmbr") == NULL) {
            ogs_error("OpenAPI_hrsbo_info_from_hplmn_convertToJSON() failed [vplmn_dl_ambr]");
            goto end;
        }
    }

    if (hrsbo_info_from_hplmn->offload_ids) {
    cJSON *offload_idsList = cJSON_AddArrayToObject(item, "offloadIds");
    if (offload_idsList == NULL) {
        ogs_error("OpenAPI_hrsbo_info_from_hplmn_convertToJSON() failed [offload_ids]");
        goto end;
    }
    OpenAPI_list_for_each(hrsbo_info_from_hplmn->offload_ids, node) {
        if (cJSON_AddStringToObject(offload_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_hrsbo_info_from_hplmn_convertToJSON() failed [offload_ids]");
            goto end;
        }
    }
    }

    if (hrsbo_info_from_hplmn->internal_group_ids) {
    cJSON *internal_group_idsList = cJSON_AddArrayToObject(item, "internalGroupIds");
    if (internal_group_idsList == NULL) {
        ogs_error("OpenAPI_hrsbo_info_from_hplmn_convertToJSON() failed [internal_group_ids]");
        goto end;
    }
    OpenAPI_list_for_each(hrsbo_info_from_hplmn->internal_group_ids, node) {
        if (cJSON_AddStringToObject(internal_group_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_hrsbo_info_from_hplmn_convertToJSON() failed [internal_group_ids]");
            goto end;
        }
    }
    }

    if (hrsbo_info_from_hplmn->target_dnai) {
    if (cJSON_AddStringToObject(item, "targetDnai", hrsbo_info_from_hplmn->target_dnai) == NULL) {
        ogs_error("OpenAPI_hrsbo_info_from_hplmn_convertToJSON() failed [target_dnai]");
        goto end;
    }
    }

    if (hrsbo_info_from_hplmn->traffic_influ_info) {
    cJSON *traffic_influ_info_local_JSON = OpenAPI_traffic_influence_info_convertToJSON(hrsbo_info_from_hplmn->traffic_influ_info);
    if (traffic_influ_info_local_JSON == NULL) {
        ogs_error("OpenAPI_hrsbo_info_from_hplmn_convertToJSON() failed [traffic_influ_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "trafficInfluInfo", traffic_influ_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_hrsbo_info_from_hplmn_convertToJSON() failed [traffic_influ_info]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_hrsbo_info_from_hplmn_t *OpenAPI_hrsbo_info_from_hplmn_parseFromJSON(cJSON *hrsbo_info_from_hplmnJSON)
{
    OpenAPI_hrsbo_info_from_hplmn_t *hrsbo_info_from_hplmn_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *hrsbo_auth_result = NULL;
    cJSON *h_dns_addr = NULL;
    OpenAPI_ip_address_t *h_dns_addr_local_nonprim = NULL;
    cJSON *h_plmn_addr = NULL;
    OpenAPI_ip_address_t *h_plmn_addr_local_nonprim = NULL;
    cJSON *vplmn_offloading_info_list = NULL;
    OpenAPI_list_t *vplmn_offloading_info_listList = NULL;
    cJSON *vplmn_dl_ambr = NULL;
    OpenAPI_vplmn_dl_ambr_t *vplmn_dl_ambr_local_nonprim = NULL;
    cJSON *offload_ids = NULL;
    OpenAPI_list_t *offload_idsList = NULL;
    cJSON *internal_group_ids = NULL;
    OpenAPI_list_t *internal_group_idsList = NULL;
    cJSON *target_dnai = NULL;
    cJSON *traffic_influ_info = NULL;
    OpenAPI_traffic_influence_info_t *traffic_influ_info_local_nonprim = NULL;
    hrsbo_auth_result = cJSON_GetObjectItemCaseSensitive(hrsbo_info_from_hplmnJSON, "hrsboAuthResult");
    if (hrsbo_auth_result) {
    if (!cJSON_IsBool(hrsbo_auth_result)) {
        ogs_error("OpenAPI_hrsbo_info_from_hplmn_parseFromJSON() failed [hrsbo_auth_result]");
        goto end;
    }
    }

    h_dns_addr = cJSON_GetObjectItemCaseSensitive(hrsbo_info_from_hplmnJSON, "hDnsAddr");
    if (h_dns_addr) {
    h_dns_addr_local_nonprim = OpenAPI_ip_address_parseFromJSON(h_dns_addr);
    if (!h_dns_addr_local_nonprim) {
        ogs_error("OpenAPI_ip_address_parseFromJSON failed [h_dns_addr]");
        goto end;
    }
    }

    h_plmn_addr = cJSON_GetObjectItemCaseSensitive(hrsbo_info_from_hplmnJSON, "hPlmnAddr");
    if (h_plmn_addr) {
    h_plmn_addr_local_nonprim = OpenAPI_ip_address_parseFromJSON(h_plmn_addr);
    if (!h_plmn_addr_local_nonprim) {
        ogs_error("OpenAPI_ip_address_parseFromJSON failed [h_plmn_addr]");
        goto end;
    }
    }

    vplmn_offloading_info_list = cJSON_GetObjectItemCaseSensitive(hrsbo_info_from_hplmnJSON, "vplmnOffloadingInfoList");
    if (vplmn_offloading_info_list) {
        cJSON *vplmn_offloading_info_list_local = NULL;
        if (!cJSON_IsArray(vplmn_offloading_info_list)) {
            ogs_error("OpenAPI_hrsbo_info_from_hplmn_parseFromJSON() failed [vplmn_offloading_info_list]");
            goto end;
        }

        vplmn_offloading_info_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(vplmn_offloading_info_list_local, vplmn_offloading_info_list) {
            if (!cJSON_IsObject(vplmn_offloading_info_list_local)) {
                ogs_error("OpenAPI_hrsbo_info_from_hplmn_parseFromJSON() failed [vplmn_offloading_info_list]");
                goto end;
            }
            OpenAPI_vplmn_offloading_info_t *vplmn_offloading_info_listItem = OpenAPI_vplmn_offloading_info_parseFromJSON(vplmn_offloading_info_list_local);
            if (!vplmn_offloading_info_listItem) {
                ogs_error("No vplmn_offloading_info_listItem");
                goto end;
            }
            OpenAPI_list_add(vplmn_offloading_info_listList, vplmn_offloading_info_listItem);
        }
    }

    vplmn_dl_ambr = cJSON_GetObjectItemCaseSensitive(hrsbo_info_from_hplmnJSON, "vplmnDlAmbr");
    if (vplmn_dl_ambr) {
    if (!cJSON_IsNull(vplmn_dl_ambr)) {
    vplmn_dl_ambr_local_nonprim = OpenAPI_vplmn_dl_ambr_parseFromJSON(vplmn_dl_ambr);
    if (!vplmn_dl_ambr_local_nonprim) {
        ogs_error("OpenAPI_vplmn_dl_ambr_parseFromJSON failed [vplmn_dl_ambr]");
        goto end;
    }
    }
    }

    offload_ids = cJSON_GetObjectItemCaseSensitive(hrsbo_info_from_hplmnJSON, "offloadIds");
    if (offload_ids) {
        cJSON *offload_ids_local = NULL;
        if (!cJSON_IsArray(offload_ids)) {
            ogs_error("OpenAPI_hrsbo_info_from_hplmn_parseFromJSON() failed [offload_ids]");
            goto end;
        }

        offload_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(offload_ids_local, offload_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(offload_ids_local)) {
                ogs_error("OpenAPI_hrsbo_info_from_hplmn_parseFromJSON() failed [offload_ids]");
                goto end;
            }
            OpenAPI_list_add(offload_idsList, ogs_strdup(offload_ids_local->valuestring));
        }
    }

    internal_group_ids = cJSON_GetObjectItemCaseSensitive(hrsbo_info_from_hplmnJSON, "internalGroupIds");
    if (internal_group_ids) {
        cJSON *internal_group_ids_local = NULL;
        if (!cJSON_IsArray(internal_group_ids)) {
            ogs_error("OpenAPI_hrsbo_info_from_hplmn_parseFromJSON() failed [internal_group_ids]");
            goto end;
        }

        internal_group_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(internal_group_ids_local, internal_group_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(internal_group_ids_local)) {
                ogs_error("OpenAPI_hrsbo_info_from_hplmn_parseFromJSON() failed [internal_group_ids]");
                goto end;
            }
            OpenAPI_list_add(internal_group_idsList, ogs_strdup(internal_group_ids_local->valuestring));
        }
    }

    target_dnai = cJSON_GetObjectItemCaseSensitive(hrsbo_info_from_hplmnJSON, "targetDnai");
    if (target_dnai) {
    if (!cJSON_IsString(target_dnai) && !cJSON_IsNull(target_dnai)) {
        ogs_error("OpenAPI_hrsbo_info_from_hplmn_parseFromJSON() failed [target_dnai]");
        goto end;
    }
    }

    traffic_influ_info = cJSON_GetObjectItemCaseSensitive(hrsbo_info_from_hplmnJSON, "trafficInfluInfo");
    if (traffic_influ_info) {
    traffic_influ_info_local_nonprim = OpenAPI_traffic_influence_info_parseFromJSON(traffic_influ_info);
    if (!traffic_influ_info_local_nonprim) {
        ogs_error("OpenAPI_traffic_influence_info_parseFromJSON failed [traffic_influ_info]");
        goto end;
    }
    }

    hrsbo_info_from_hplmn_local_var = OpenAPI_hrsbo_info_from_hplmn_create (
        hrsbo_auth_result ? true : false,
        hrsbo_auth_result ? hrsbo_auth_result->valueint : 0,
        h_dns_addr ? h_dns_addr_local_nonprim : NULL,
        h_plmn_addr ? h_plmn_addr_local_nonprim : NULL,
        vplmn_offloading_info_list ? vplmn_offloading_info_listList : NULL,
        vplmn_dl_ambr && cJSON_IsNull(vplmn_dl_ambr) ? true : false,
        vplmn_dl_ambr ? vplmn_dl_ambr_local_nonprim : NULL,
        offload_ids ? offload_idsList : NULL,
        internal_group_ids ? internal_group_idsList : NULL,
        target_dnai && !cJSON_IsNull(target_dnai) ? ogs_strdup(target_dnai->valuestring) : NULL,
        traffic_influ_info ? traffic_influ_info_local_nonprim : NULL
    );

    return hrsbo_info_from_hplmn_local_var;
end:
    if (h_dns_addr_local_nonprim) {
        OpenAPI_ip_address_free(h_dns_addr_local_nonprim);
        h_dns_addr_local_nonprim = NULL;
    }
    if (h_plmn_addr_local_nonprim) {
        OpenAPI_ip_address_free(h_plmn_addr_local_nonprim);
        h_plmn_addr_local_nonprim = NULL;
    }
    if (vplmn_offloading_info_listList) {
        OpenAPI_list_for_each(vplmn_offloading_info_listList, node) {
            OpenAPI_vplmn_offloading_info_free(node->data);
        }
        OpenAPI_list_free(vplmn_offloading_info_listList);
        vplmn_offloading_info_listList = NULL;
    }
    if (vplmn_dl_ambr_local_nonprim) {
        OpenAPI_vplmn_dl_ambr_free(vplmn_dl_ambr_local_nonprim);
        vplmn_dl_ambr_local_nonprim = NULL;
    }
    if (offload_idsList) {
        OpenAPI_list_for_each(offload_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(offload_idsList);
        offload_idsList = NULL;
    }
    if (internal_group_idsList) {
        OpenAPI_list_for_each(internal_group_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(internal_group_idsList);
        internal_group_idsList = NULL;
    }
    if (traffic_influ_info_local_nonprim) {
        OpenAPI_traffic_influence_info_free(traffic_influ_info_local_nonprim);
        traffic_influ_info_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_hrsbo_info_from_hplmn_t *OpenAPI_hrsbo_info_from_hplmn_copy(OpenAPI_hrsbo_info_from_hplmn_t *dst, OpenAPI_hrsbo_info_from_hplmn_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_hrsbo_info_from_hplmn_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_hrsbo_info_from_hplmn_convertToJSON() failed");
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

    OpenAPI_hrsbo_info_from_hplmn_free(dst);
    dst = OpenAPI_hrsbo_info_from_hplmn_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

