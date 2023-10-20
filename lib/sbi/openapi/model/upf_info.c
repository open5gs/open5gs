
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "upf_info.h"

OpenAPI_upf_info_t *OpenAPI_upf_info_create(
    OpenAPI_list_t *s_nssai_upf_info_list,
    OpenAPI_list_t *smf_serving_area,
    OpenAPI_list_t *interface_upf_info_list,
    bool is_iwk_eps_ind,
    int iwk_eps_ind,
    OpenAPI_list_t *pdu_session_types,
    OpenAPI_atsss_capability_t *atsss_capability,
    bool is_ue_ip_addr_ind,
    int ue_ip_addr_ind,
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *tai_range_list,
    OpenAPI_w_agf_info_t *w_agf_info,
    OpenAPI_tngf_info_t *tngf_info,
    OpenAPI_twif_info_t *twif_info,
    bool is_priority,
    int priority,
    bool is_redundant_gtpu,
    int redundant_gtpu,
    bool is_ipups,
    int ipups,
    bool is_data_forwarding,
    int data_forwarding,
    char *supported_pfcp_features
)
{
    OpenAPI_upf_info_t *upf_info_local_var = ogs_malloc(sizeof(OpenAPI_upf_info_t));
    ogs_assert(upf_info_local_var);

    upf_info_local_var->s_nssai_upf_info_list = s_nssai_upf_info_list;
    upf_info_local_var->smf_serving_area = smf_serving_area;
    upf_info_local_var->interface_upf_info_list = interface_upf_info_list;
    upf_info_local_var->is_iwk_eps_ind = is_iwk_eps_ind;
    upf_info_local_var->iwk_eps_ind = iwk_eps_ind;
    upf_info_local_var->pdu_session_types = pdu_session_types;
    upf_info_local_var->atsss_capability = atsss_capability;
    upf_info_local_var->is_ue_ip_addr_ind = is_ue_ip_addr_ind;
    upf_info_local_var->ue_ip_addr_ind = ue_ip_addr_ind;
    upf_info_local_var->tai_list = tai_list;
    upf_info_local_var->tai_range_list = tai_range_list;
    upf_info_local_var->w_agf_info = w_agf_info;
    upf_info_local_var->tngf_info = tngf_info;
    upf_info_local_var->twif_info = twif_info;
    upf_info_local_var->is_priority = is_priority;
    upf_info_local_var->priority = priority;
    upf_info_local_var->is_redundant_gtpu = is_redundant_gtpu;
    upf_info_local_var->redundant_gtpu = redundant_gtpu;
    upf_info_local_var->is_ipups = is_ipups;
    upf_info_local_var->ipups = ipups;
    upf_info_local_var->is_data_forwarding = is_data_forwarding;
    upf_info_local_var->data_forwarding = data_forwarding;
    upf_info_local_var->supported_pfcp_features = supported_pfcp_features;

    return upf_info_local_var;
}

void OpenAPI_upf_info_free(OpenAPI_upf_info_t *upf_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == upf_info) {
        return;
    }
    if (upf_info->s_nssai_upf_info_list) {
        OpenAPI_list_for_each(upf_info->s_nssai_upf_info_list, node) {
            OpenAPI_snssai_upf_info_item_free(node->data);
        }
        OpenAPI_list_free(upf_info->s_nssai_upf_info_list);
        upf_info->s_nssai_upf_info_list = NULL;
    }
    if (upf_info->smf_serving_area) {
        OpenAPI_list_for_each(upf_info->smf_serving_area, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(upf_info->smf_serving_area);
        upf_info->smf_serving_area = NULL;
    }
    if (upf_info->interface_upf_info_list) {
        OpenAPI_list_for_each(upf_info->interface_upf_info_list, node) {
            OpenAPI_interface_upf_info_item_free(node->data);
        }
        OpenAPI_list_free(upf_info->interface_upf_info_list);
        upf_info->interface_upf_info_list = NULL;
    }
    if (upf_info->pdu_session_types) {
        OpenAPI_list_free(upf_info->pdu_session_types);
        upf_info->pdu_session_types = NULL;
    }
    if (upf_info->atsss_capability) {
        OpenAPI_atsss_capability_free(upf_info->atsss_capability);
        upf_info->atsss_capability = NULL;
    }
    if (upf_info->tai_list) {
        OpenAPI_list_for_each(upf_info->tai_list, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(upf_info->tai_list);
        upf_info->tai_list = NULL;
    }
    if (upf_info->tai_range_list) {
        OpenAPI_list_for_each(upf_info->tai_range_list, node) {
            OpenAPI_tai_range_free(node->data);
        }
        OpenAPI_list_free(upf_info->tai_range_list);
        upf_info->tai_range_list = NULL;
    }
    if (upf_info->w_agf_info) {
        OpenAPI_w_agf_info_free(upf_info->w_agf_info);
        upf_info->w_agf_info = NULL;
    }
    if (upf_info->tngf_info) {
        OpenAPI_tngf_info_free(upf_info->tngf_info);
        upf_info->tngf_info = NULL;
    }
    if (upf_info->twif_info) {
        OpenAPI_twif_info_free(upf_info->twif_info);
        upf_info->twif_info = NULL;
    }
    if (upf_info->supported_pfcp_features) {
        ogs_free(upf_info->supported_pfcp_features);
        upf_info->supported_pfcp_features = NULL;
    }
    ogs_free(upf_info);
}

cJSON *OpenAPI_upf_info_convertToJSON(OpenAPI_upf_info_t *upf_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (upf_info == NULL) {
        ogs_error("OpenAPI_upf_info_convertToJSON() failed [UpfInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!upf_info->s_nssai_upf_info_list) {
        ogs_error("OpenAPI_upf_info_convertToJSON() failed [s_nssai_upf_info_list]");
        return NULL;
    }
    cJSON *s_nssai_upf_info_listList = cJSON_AddArrayToObject(item, "sNssaiUpfInfoList");
    if (s_nssai_upf_info_listList == NULL) {
        ogs_error("OpenAPI_upf_info_convertToJSON() failed [s_nssai_upf_info_list]");
        goto end;
    }
    OpenAPI_list_for_each(upf_info->s_nssai_upf_info_list, node) {
        cJSON *itemLocal = OpenAPI_snssai_upf_info_item_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_upf_info_convertToJSON() failed [s_nssai_upf_info_list]");
            goto end;
        }
        cJSON_AddItemToArray(s_nssai_upf_info_listList, itemLocal);
    }

    if (upf_info->smf_serving_area) {
    cJSON *smf_serving_areaList = cJSON_AddArrayToObject(item, "smfServingArea");
    if (smf_serving_areaList == NULL) {
        ogs_error("OpenAPI_upf_info_convertToJSON() failed [smf_serving_area]");
        goto end;
    }
    OpenAPI_list_for_each(upf_info->smf_serving_area, node) {
        if (cJSON_AddStringToObject(smf_serving_areaList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_upf_info_convertToJSON() failed [smf_serving_area]");
            goto end;
        }
    }
    }

    if (upf_info->interface_upf_info_list) {
    cJSON *interface_upf_info_listList = cJSON_AddArrayToObject(item, "interfaceUpfInfoList");
    if (interface_upf_info_listList == NULL) {
        ogs_error("OpenAPI_upf_info_convertToJSON() failed [interface_upf_info_list]");
        goto end;
    }
    OpenAPI_list_for_each(upf_info->interface_upf_info_list, node) {
        cJSON *itemLocal = OpenAPI_interface_upf_info_item_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_upf_info_convertToJSON() failed [interface_upf_info_list]");
            goto end;
        }
        cJSON_AddItemToArray(interface_upf_info_listList, itemLocal);
    }
    }

    if (upf_info->is_iwk_eps_ind) {
    if (cJSON_AddBoolToObject(item, "iwkEpsInd", upf_info->iwk_eps_ind) == NULL) {
        ogs_error("OpenAPI_upf_info_convertToJSON() failed [iwk_eps_ind]");
        goto end;
    }
    }

    if (upf_info->pdu_session_types != OpenAPI_pdu_session_type_NULL) {
    cJSON *pdu_session_typesList = cJSON_AddArrayToObject(item, "pduSessionTypes");
    if (pdu_session_typesList == NULL) {
        ogs_error("OpenAPI_upf_info_convertToJSON() failed [pdu_session_types]");
        goto end;
    }
    OpenAPI_list_for_each(upf_info->pdu_session_types, node) {
        if (cJSON_AddStringToObject(pdu_session_typesList, "", OpenAPI_pdu_session_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_upf_info_convertToJSON() failed [pdu_session_types]");
            goto end;
        }
    }
    }

    if (upf_info->atsss_capability) {
    cJSON *atsss_capability_local_JSON = OpenAPI_atsss_capability_convertToJSON(upf_info->atsss_capability);
    if (atsss_capability_local_JSON == NULL) {
        ogs_error("OpenAPI_upf_info_convertToJSON() failed [atsss_capability]");
        goto end;
    }
    cJSON_AddItemToObject(item, "atsssCapability", atsss_capability_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_upf_info_convertToJSON() failed [atsss_capability]");
        goto end;
    }
    }

    if (upf_info->is_ue_ip_addr_ind) {
    if (cJSON_AddBoolToObject(item, "ueIpAddrInd", upf_info->ue_ip_addr_ind) == NULL) {
        ogs_error("OpenAPI_upf_info_convertToJSON() failed [ue_ip_addr_ind]");
        goto end;
    }
    }

    if (upf_info->tai_list) {
    cJSON *tai_listList = cJSON_AddArrayToObject(item, "taiList");
    if (tai_listList == NULL) {
        ogs_error("OpenAPI_upf_info_convertToJSON() failed [tai_list]");
        goto end;
    }
    OpenAPI_list_for_each(upf_info->tai_list, node) {
        cJSON *itemLocal = OpenAPI_tai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_upf_info_convertToJSON() failed [tai_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_listList, itemLocal);
    }
    }

    if (upf_info->tai_range_list) {
    cJSON *tai_range_listList = cJSON_AddArrayToObject(item, "taiRangeList");
    if (tai_range_listList == NULL) {
        ogs_error("OpenAPI_upf_info_convertToJSON() failed [tai_range_list]");
        goto end;
    }
    OpenAPI_list_for_each(upf_info->tai_range_list, node) {
        cJSON *itemLocal = OpenAPI_tai_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_upf_info_convertToJSON() failed [tai_range_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_range_listList, itemLocal);
    }
    }

    if (upf_info->w_agf_info) {
    cJSON *w_agf_info_local_JSON = OpenAPI_w_agf_info_convertToJSON(upf_info->w_agf_info);
    if (w_agf_info_local_JSON == NULL) {
        ogs_error("OpenAPI_upf_info_convertToJSON() failed [w_agf_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "wAgfInfo", w_agf_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_upf_info_convertToJSON() failed [w_agf_info]");
        goto end;
    }
    }

    if (upf_info->tngf_info) {
    cJSON *tngf_info_local_JSON = OpenAPI_tngf_info_convertToJSON(upf_info->tngf_info);
    if (tngf_info_local_JSON == NULL) {
        ogs_error("OpenAPI_upf_info_convertToJSON() failed [tngf_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "tngfInfo", tngf_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_upf_info_convertToJSON() failed [tngf_info]");
        goto end;
    }
    }

    if (upf_info->twif_info) {
    cJSON *twif_info_local_JSON = OpenAPI_twif_info_convertToJSON(upf_info->twif_info);
    if (twif_info_local_JSON == NULL) {
        ogs_error("OpenAPI_upf_info_convertToJSON() failed [twif_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "twifInfo", twif_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_upf_info_convertToJSON() failed [twif_info]");
        goto end;
    }
    }

    if (upf_info->is_priority) {
    if (cJSON_AddNumberToObject(item, "priority", upf_info->priority) == NULL) {
        ogs_error("OpenAPI_upf_info_convertToJSON() failed [priority]");
        goto end;
    }
    }

    if (upf_info->is_redundant_gtpu) {
    if (cJSON_AddBoolToObject(item, "redundantGtpu", upf_info->redundant_gtpu) == NULL) {
        ogs_error("OpenAPI_upf_info_convertToJSON() failed [redundant_gtpu]");
        goto end;
    }
    }

    if (upf_info->is_ipups) {
    if (cJSON_AddBoolToObject(item, "ipups", upf_info->ipups) == NULL) {
        ogs_error("OpenAPI_upf_info_convertToJSON() failed [ipups]");
        goto end;
    }
    }

    if (upf_info->is_data_forwarding) {
    if (cJSON_AddBoolToObject(item, "dataForwarding", upf_info->data_forwarding) == NULL) {
        ogs_error("OpenAPI_upf_info_convertToJSON() failed [data_forwarding]");
        goto end;
    }
    }

    if (upf_info->supported_pfcp_features) {
    if (cJSON_AddStringToObject(item, "supportedPfcpFeatures", upf_info->supported_pfcp_features) == NULL) {
        ogs_error("OpenAPI_upf_info_convertToJSON() failed [supported_pfcp_features]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_upf_info_t *OpenAPI_upf_info_parseFromJSON(cJSON *upf_infoJSON)
{
    OpenAPI_upf_info_t *upf_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *s_nssai_upf_info_list = NULL;
    OpenAPI_list_t *s_nssai_upf_info_listList = NULL;
    cJSON *smf_serving_area = NULL;
    OpenAPI_list_t *smf_serving_areaList = NULL;
    cJSON *interface_upf_info_list = NULL;
    OpenAPI_list_t *interface_upf_info_listList = NULL;
    cJSON *iwk_eps_ind = NULL;
    cJSON *pdu_session_types = NULL;
    OpenAPI_list_t *pdu_session_typesList = NULL;
    cJSON *atsss_capability = NULL;
    OpenAPI_atsss_capability_t *atsss_capability_local_nonprim = NULL;
    cJSON *ue_ip_addr_ind = NULL;
    cJSON *tai_list = NULL;
    OpenAPI_list_t *tai_listList = NULL;
    cJSON *tai_range_list = NULL;
    OpenAPI_list_t *tai_range_listList = NULL;
    cJSON *w_agf_info = NULL;
    OpenAPI_w_agf_info_t *w_agf_info_local_nonprim = NULL;
    cJSON *tngf_info = NULL;
    OpenAPI_tngf_info_t *tngf_info_local_nonprim = NULL;
    cJSON *twif_info = NULL;
    OpenAPI_twif_info_t *twif_info_local_nonprim = NULL;
    cJSON *priority = NULL;
    cJSON *redundant_gtpu = NULL;
    cJSON *ipups = NULL;
    cJSON *data_forwarding = NULL;
    cJSON *supported_pfcp_features = NULL;
    s_nssai_upf_info_list = cJSON_GetObjectItemCaseSensitive(upf_infoJSON, "sNssaiUpfInfoList");
    if (!s_nssai_upf_info_list) {
        ogs_error("OpenAPI_upf_info_parseFromJSON() failed [s_nssai_upf_info_list]");
        goto end;
    }
        cJSON *s_nssai_upf_info_list_local = NULL;
        if (!cJSON_IsArray(s_nssai_upf_info_list)) {
            ogs_error("OpenAPI_upf_info_parseFromJSON() failed [s_nssai_upf_info_list]");
            goto end;
        }

        s_nssai_upf_info_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(s_nssai_upf_info_list_local, s_nssai_upf_info_list) {
            if (!cJSON_IsObject(s_nssai_upf_info_list_local)) {
                ogs_error("OpenAPI_upf_info_parseFromJSON() failed [s_nssai_upf_info_list]");
                goto end;
            }
            OpenAPI_snssai_upf_info_item_t *s_nssai_upf_info_listItem = OpenAPI_snssai_upf_info_item_parseFromJSON(s_nssai_upf_info_list_local);
            if (!s_nssai_upf_info_listItem) {
                ogs_error("No s_nssai_upf_info_listItem");
                goto end;
            }
            OpenAPI_list_add(s_nssai_upf_info_listList, s_nssai_upf_info_listItem);
        }

    smf_serving_area = cJSON_GetObjectItemCaseSensitive(upf_infoJSON, "smfServingArea");
    if (smf_serving_area) {
        cJSON *smf_serving_area_local = NULL;
        if (!cJSON_IsArray(smf_serving_area)) {
            ogs_error("OpenAPI_upf_info_parseFromJSON() failed [smf_serving_area]");
            goto end;
        }

        smf_serving_areaList = OpenAPI_list_create();

        cJSON_ArrayForEach(smf_serving_area_local, smf_serving_area) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(smf_serving_area_local)) {
                ogs_error("OpenAPI_upf_info_parseFromJSON() failed [smf_serving_area]");
                goto end;
            }
            OpenAPI_list_add(smf_serving_areaList, ogs_strdup(smf_serving_area_local->valuestring));
        }
    }

    interface_upf_info_list = cJSON_GetObjectItemCaseSensitive(upf_infoJSON, "interfaceUpfInfoList");
    if (interface_upf_info_list) {
        cJSON *interface_upf_info_list_local = NULL;
        if (!cJSON_IsArray(interface_upf_info_list)) {
            ogs_error("OpenAPI_upf_info_parseFromJSON() failed [interface_upf_info_list]");
            goto end;
        }

        interface_upf_info_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(interface_upf_info_list_local, interface_upf_info_list) {
            if (!cJSON_IsObject(interface_upf_info_list_local)) {
                ogs_error("OpenAPI_upf_info_parseFromJSON() failed [interface_upf_info_list]");
                goto end;
            }
            OpenAPI_interface_upf_info_item_t *interface_upf_info_listItem = OpenAPI_interface_upf_info_item_parseFromJSON(interface_upf_info_list_local);
            if (!interface_upf_info_listItem) {
                ogs_error("No interface_upf_info_listItem");
                goto end;
            }
            OpenAPI_list_add(interface_upf_info_listList, interface_upf_info_listItem);
        }
    }

    iwk_eps_ind = cJSON_GetObjectItemCaseSensitive(upf_infoJSON, "iwkEpsInd");
    if (iwk_eps_ind) {
    if (!cJSON_IsBool(iwk_eps_ind)) {
        ogs_error("OpenAPI_upf_info_parseFromJSON() failed [iwk_eps_ind]");
        goto end;
    }
    }

    pdu_session_types = cJSON_GetObjectItemCaseSensitive(upf_infoJSON, "pduSessionTypes");
    if (pdu_session_types) {
        cJSON *pdu_session_types_local = NULL;
        if (!cJSON_IsArray(pdu_session_types)) {
            ogs_error("OpenAPI_upf_info_parseFromJSON() failed [pdu_session_types]");
            goto end;
        }

        pdu_session_typesList = OpenAPI_list_create();

        cJSON_ArrayForEach(pdu_session_types_local, pdu_session_types) {
            OpenAPI_pdu_session_type_e localEnum = OpenAPI_pdu_session_type_NULL;
            if (!cJSON_IsString(pdu_session_types_local)) {
                ogs_error("OpenAPI_upf_info_parseFromJSON() failed [pdu_session_types]");
                goto end;
            }
            localEnum = OpenAPI_pdu_session_type_FromString(pdu_session_types_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"pdu_session_types\" is not supported. Ignoring it ...",
                         pdu_session_types_local->valuestring);
            } else {
                OpenAPI_list_add(pdu_session_typesList, (void *)localEnum);
            }
        }
        if (pdu_session_typesList->count == 0) {
            ogs_error("OpenAPI_upf_info_parseFromJSON() failed: Expected pdu_session_typesList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    atsss_capability = cJSON_GetObjectItemCaseSensitive(upf_infoJSON, "atsssCapability");
    if (atsss_capability) {
    atsss_capability_local_nonprim = OpenAPI_atsss_capability_parseFromJSON(atsss_capability);
    if (!atsss_capability_local_nonprim) {
        ogs_error("OpenAPI_atsss_capability_parseFromJSON failed [atsss_capability]");
        goto end;
    }
    }

    ue_ip_addr_ind = cJSON_GetObjectItemCaseSensitive(upf_infoJSON, "ueIpAddrInd");
    if (ue_ip_addr_ind) {
    if (!cJSON_IsBool(ue_ip_addr_ind)) {
        ogs_error("OpenAPI_upf_info_parseFromJSON() failed [ue_ip_addr_ind]");
        goto end;
    }
    }

    tai_list = cJSON_GetObjectItemCaseSensitive(upf_infoJSON, "taiList");
    if (tai_list) {
        cJSON *tai_list_local = NULL;
        if (!cJSON_IsArray(tai_list)) {
            ogs_error("OpenAPI_upf_info_parseFromJSON() failed [tai_list]");
            goto end;
        }

        tai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_list_local, tai_list) {
            if (!cJSON_IsObject(tai_list_local)) {
                ogs_error("OpenAPI_upf_info_parseFromJSON() failed [tai_list]");
                goto end;
            }
            OpenAPI_tai_t *tai_listItem = OpenAPI_tai_parseFromJSON(tai_list_local);
            if (!tai_listItem) {
                ogs_error("No tai_listItem");
                goto end;
            }
            OpenAPI_list_add(tai_listList, tai_listItem);
        }
    }

    tai_range_list = cJSON_GetObjectItemCaseSensitive(upf_infoJSON, "taiRangeList");
    if (tai_range_list) {
        cJSON *tai_range_list_local = NULL;
        if (!cJSON_IsArray(tai_range_list)) {
            ogs_error("OpenAPI_upf_info_parseFromJSON() failed [tai_range_list]");
            goto end;
        }

        tai_range_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_range_list_local, tai_range_list) {
            if (!cJSON_IsObject(tai_range_list_local)) {
                ogs_error("OpenAPI_upf_info_parseFromJSON() failed [tai_range_list]");
                goto end;
            }
            OpenAPI_tai_range_t *tai_range_listItem = OpenAPI_tai_range_parseFromJSON(tai_range_list_local);
            if (!tai_range_listItem) {
                ogs_error("No tai_range_listItem");
                goto end;
            }
            OpenAPI_list_add(tai_range_listList, tai_range_listItem);
        }
    }

    w_agf_info = cJSON_GetObjectItemCaseSensitive(upf_infoJSON, "wAgfInfo");
    if (w_agf_info) {
    w_agf_info_local_nonprim = OpenAPI_w_agf_info_parseFromJSON(w_agf_info);
    if (!w_agf_info_local_nonprim) {
        ogs_error("OpenAPI_w_agf_info_parseFromJSON failed [w_agf_info]");
        goto end;
    }
    }

    tngf_info = cJSON_GetObjectItemCaseSensitive(upf_infoJSON, "tngfInfo");
    if (tngf_info) {
    tngf_info_local_nonprim = OpenAPI_tngf_info_parseFromJSON(tngf_info);
    if (!tngf_info_local_nonprim) {
        ogs_error("OpenAPI_tngf_info_parseFromJSON failed [tngf_info]");
        goto end;
    }
    }

    twif_info = cJSON_GetObjectItemCaseSensitive(upf_infoJSON, "twifInfo");
    if (twif_info) {
    twif_info_local_nonprim = OpenAPI_twif_info_parseFromJSON(twif_info);
    if (!twif_info_local_nonprim) {
        ogs_error("OpenAPI_twif_info_parseFromJSON failed [twif_info]");
        goto end;
    }
    }

    priority = cJSON_GetObjectItemCaseSensitive(upf_infoJSON, "priority");
    if (priority) {
    if (!cJSON_IsNumber(priority)) {
        ogs_error("OpenAPI_upf_info_parseFromJSON() failed [priority]");
        goto end;
    }
    }

    redundant_gtpu = cJSON_GetObjectItemCaseSensitive(upf_infoJSON, "redundantGtpu");
    if (redundant_gtpu) {
    if (!cJSON_IsBool(redundant_gtpu)) {
        ogs_error("OpenAPI_upf_info_parseFromJSON() failed [redundant_gtpu]");
        goto end;
    }
    }

    ipups = cJSON_GetObjectItemCaseSensitive(upf_infoJSON, "ipups");
    if (ipups) {
    if (!cJSON_IsBool(ipups)) {
        ogs_error("OpenAPI_upf_info_parseFromJSON() failed [ipups]");
        goto end;
    }
    }

    data_forwarding = cJSON_GetObjectItemCaseSensitive(upf_infoJSON, "dataForwarding");
    if (data_forwarding) {
    if (!cJSON_IsBool(data_forwarding)) {
        ogs_error("OpenAPI_upf_info_parseFromJSON() failed [data_forwarding]");
        goto end;
    }
    }

    supported_pfcp_features = cJSON_GetObjectItemCaseSensitive(upf_infoJSON, "supportedPfcpFeatures");
    if (supported_pfcp_features) {
    if (!cJSON_IsString(supported_pfcp_features) && !cJSON_IsNull(supported_pfcp_features)) {
        ogs_error("OpenAPI_upf_info_parseFromJSON() failed [supported_pfcp_features]");
        goto end;
    }
    }

    upf_info_local_var = OpenAPI_upf_info_create (
        s_nssai_upf_info_listList,
        smf_serving_area ? smf_serving_areaList : NULL,
        interface_upf_info_list ? interface_upf_info_listList : NULL,
        iwk_eps_ind ? true : false,
        iwk_eps_ind ? iwk_eps_ind->valueint : 0,
        pdu_session_types ? pdu_session_typesList : NULL,
        atsss_capability ? atsss_capability_local_nonprim : NULL,
        ue_ip_addr_ind ? true : false,
        ue_ip_addr_ind ? ue_ip_addr_ind->valueint : 0,
        tai_list ? tai_listList : NULL,
        tai_range_list ? tai_range_listList : NULL,
        w_agf_info ? w_agf_info_local_nonprim : NULL,
        tngf_info ? tngf_info_local_nonprim : NULL,
        twif_info ? twif_info_local_nonprim : NULL,
        priority ? true : false,
        priority ? priority->valuedouble : 0,
        redundant_gtpu ? true : false,
        redundant_gtpu ? redundant_gtpu->valueint : 0,
        ipups ? true : false,
        ipups ? ipups->valueint : 0,
        data_forwarding ? true : false,
        data_forwarding ? data_forwarding->valueint : 0,
        supported_pfcp_features && !cJSON_IsNull(supported_pfcp_features) ? ogs_strdup(supported_pfcp_features->valuestring) : NULL
    );

    return upf_info_local_var;
end:
    if (s_nssai_upf_info_listList) {
        OpenAPI_list_for_each(s_nssai_upf_info_listList, node) {
            OpenAPI_snssai_upf_info_item_free(node->data);
        }
        OpenAPI_list_free(s_nssai_upf_info_listList);
        s_nssai_upf_info_listList = NULL;
    }
    if (smf_serving_areaList) {
        OpenAPI_list_for_each(smf_serving_areaList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(smf_serving_areaList);
        smf_serving_areaList = NULL;
    }
    if (interface_upf_info_listList) {
        OpenAPI_list_for_each(interface_upf_info_listList, node) {
            OpenAPI_interface_upf_info_item_free(node->data);
        }
        OpenAPI_list_free(interface_upf_info_listList);
        interface_upf_info_listList = NULL;
    }
    if (pdu_session_typesList) {
        OpenAPI_list_free(pdu_session_typesList);
        pdu_session_typesList = NULL;
    }
    if (atsss_capability_local_nonprim) {
        OpenAPI_atsss_capability_free(atsss_capability_local_nonprim);
        atsss_capability_local_nonprim = NULL;
    }
    if (tai_listList) {
        OpenAPI_list_for_each(tai_listList, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(tai_listList);
        tai_listList = NULL;
    }
    if (tai_range_listList) {
        OpenAPI_list_for_each(tai_range_listList, node) {
            OpenAPI_tai_range_free(node->data);
        }
        OpenAPI_list_free(tai_range_listList);
        tai_range_listList = NULL;
    }
    if (w_agf_info_local_nonprim) {
        OpenAPI_w_agf_info_free(w_agf_info_local_nonprim);
        w_agf_info_local_nonprim = NULL;
    }
    if (tngf_info_local_nonprim) {
        OpenAPI_tngf_info_free(tngf_info_local_nonprim);
        tngf_info_local_nonprim = NULL;
    }
    if (twif_info_local_nonprim) {
        OpenAPI_twif_info_free(twif_info_local_nonprim);
        twif_info_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_upf_info_t *OpenAPI_upf_info_copy(OpenAPI_upf_info_t *dst, OpenAPI_upf_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_upf_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_upf_info_convertToJSON() failed");
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

    OpenAPI_upf_info_free(dst);
    dst = OpenAPI_upf_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

