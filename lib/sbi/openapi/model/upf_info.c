
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "upf_info.h"

OpenAPI_upf_info_t *OpenAPI_upf_info_create(
    OpenAPI_list_t *s_nssai_upf_info_list,
    OpenAPI_list_t *smf_serving_area,
    OpenAPI_list_t *interface_upf_info_list,
    int iwk_eps_ind,
    OpenAPI_list_t *pdu_session_types,
    OpenAPI_atsss_capability_t *atsss_capability,
    int ue_ip_addr_ind
    )
{
    OpenAPI_upf_info_t *upf_info_local_var = OpenAPI_malloc(sizeof(OpenAPI_upf_info_t));
    if (!upf_info_local_var) {
        return NULL;
    }
    upf_info_local_var->s_nssai_upf_info_list = s_nssai_upf_info_list;
    upf_info_local_var->smf_serving_area = smf_serving_area;
    upf_info_local_var->interface_upf_info_list = interface_upf_info_list;
    upf_info_local_var->iwk_eps_ind = iwk_eps_ind;
    upf_info_local_var->pdu_session_types = pdu_session_types;
    upf_info_local_var->atsss_capability = atsss_capability;
    upf_info_local_var->ue_ip_addr_ind = ue_ip_addr_ind;

    return upf_info_local_var;
}

void OpenAPI_upf_info_free(OpenAPI_upf_info_t *upf_info)
{
    if (NULL == upf_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(upf_info->s_nssai_upf_info_list, node) {
        OpenAPI_snssai_upf_info_item_free(node->data);
    }
    OpenAPI_list_free(upf_info->s_nssai_upf_info_list);
    OpenAPI_list_for_each(upf_info->smf_serving_area, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(upf_info->smf_serving_area);
    OpenAPI_list_for_each(upf_info->interface_upf_info_list, node) {
        OpenAPI_interface_upf_info_item_free(node->data);
    }
    OpenAPI_list_free(upf_info->interface_upf_info_list);
    OpenAPI_list_for_each(upf_info->pdu_session_types, node) {
        OpenAPI_pdu_session_type_free(node->data);
    }
    OpenAPI_list_free(upf_info->pdu_session_types);
    OpenAPI_atsss_capability_free(upf_info->atsss_capability);
    ogs_free(upf_info);
}

cJSON *OpenAPI_upf_info_convertToJSON(OpenAPI_upf_info_t *upf_info)
{
    cJSON *item = NULL;

    if (upf_info == NULL) {
        ogs_error("OpenAPI_upf_info_convertToJSON() failed [UpfInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!upf_info->s_nssai_upf_info_list) {
        ogs_error("OpenAPI_upf_info_convertToJSON() failed [s_nssai_upf_info_list]");
        goto end;
    }
    cJSON *s_nssai_upf_info_listList = cJSON_AddArrayToObject(item, "sNssaiUpfInfoList");
    if (s_nssai_upf_info_listList == NULL) {
        ogs_error("OpenAPI_upf_info_convertToJSON() failed [s_nssai_upf_info_list]");
        goto end;
    }

    OpenAPI_lnode_t *s_nssai_upf_info_list_node;
    if (upf_info->s_nssai_upf_info_list) {
        OpenAPI_list_for_each(upf_info->s_nssai_upf_info_list, s_nssai_upf_info_list_node) {
            cJSON *itemLocal = OpenAPI_snssai_upf_info_item_convertToJSON(s_nssai_upf_info_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_upf_info_convertToJSON() failed [s_nssai_upf_info_list]");
                goto end;
            }
            cJSON_AddItemToArray(s_nssai_upf_info_listList, itemLocal);
        }
    }

    if (upf_info->smf_serving_area) {
        cJSON *smf_serving_area = cJSON_AddArrayToObject(item, "smfServingArea");
        if (smf_serving_area == NULL) {
            ogs_error("OpenAPI_upf_info_convertToJSON() failed [smf_serving_area]");
            goto end;
        }

        OpenAPI_lnode_t *smf_serving_area_node;
        OpenAPI_list_for_each(upf_info->smf_serving_area, smf_serving_area_node)  {
            if (cJSON_AddStringToObject(smf_serving_area, "", (char*)smf_serving_area_node->data) == NULL) {
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

        OpenAPI_lnode_t *interface_upf_info_list_node;
        if (upf_info->interface_upf_info_list) {
            OpenAPI_list_for_each(upf_info->interface_upf_info_list, interface_upf_info_list_node) {
                cJSON *itemLocal = OpenAPI_interface_upf_info_item_convertToJSON(interface_upf_info_list_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_upf_info_convertToJSON() failed [interface_upf_info_list]");
                    goto end;
                }
                cJSON_AddItemToArray(interface_upf_info_listList, itemLocal);
            }
        }
    }

    if (upf_info->iwk_eps_ind) {
        if (cJSON_AddBoolToObject(item, "iwkEpsInd", upf_info->iwk_eps_ind) == NULL) {
            ogs_error("OpenAPI_upf_info_convertToJSON() failed [iwk_eps_ind]");
            goto end;
        }
    }

    if (upf_info->pdu_session_types) {
        cJSON *pdu_session_typesList = cJSON_AddArrayToObject(item, "pduSessionTypes");
        if (pdu_session_typesList == NULL) {
            ogs_error("OpenAPI_upf_info_convertToJSON() failed [pdu_session_types]");
            goto end;
        }

        OpenAPI_lnode_t *pdu_session_types_node;
        if (upf_info->pdu_session_types) {
            OpenAPI_list_for_each(upf_info->pdu_session_types, pdu_session_types_node) {
                cJSON *itemLocal = OpenAPI_pdu_session_type_convertToJSON(pdu_session_types_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_upf_info_convertToJSON() failed [pdu_session_types]");
                    goto end;
                }
                cJSON_AddItemToArray(pdu_session_typesList, itemLocal);
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

    if (upf_info->ue_ip_addr_ind) {
        if (cJSON_AddBoolToObject(item, "ueIpAddrInd", upf_info->ue_ip_addr_ind) == NULL) {
            ogs_error("OpenAPI_upf_info_convertToJSON() failed [ue_ip_addr_ind]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_upf_info_t *OpenAPI_upf_info_parseFromJSON(cJSON *upf_infoJSON)
{
    OpenAPI_upf_info_t *upf_info_local_var = NULL;
    cJSON *s_nssai_upf_info_list = cJSON_GetObjectItemCaseSensitive(upf_infoJSON, "sNssaiUpfInfoList");
    if (!s_nssai_upf_info_list) {
        ogs_error("OpenAPI_upf_info_parseFromJSON() failed [s_nssai_upf_info_list]");
        goto end;
    }

    OpenAPI_list_t *s_nssai_upf_info_listList;

    cJSON *s_nssai_upf_info_list_local_nonprimitive;
    if (!cJSON_IsArray(s_nssai_upf_info_list)) {
        ogs_error("OpenAPI_upf_info_parseFromJSON() failed [s_nssai_upf_info_list]");
        goto end;
    }

    s_nssai_upf_info_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(s_nssai_upf_info_list_local_nonprimitive, s_nssai_upf_info_list ) {
        if (!cJSON_IsObject(s_nssai_upf_info_list_local_nonprimitive)) {
            ogs_error("OpenAPI_upf_info_parseFromJSON() failed [s_nssai_upf_info_list]");
            goto end;
        }
        OpenAPI_snssai_upf_info_item_t *s_nssai_upf_info_listItem = OpenAPI_snssai_upf_info_item_parseFromJSON(s_nssai_upf_info_list_local_nonprimitive);

        OpenAPI_list_add(s_nssai_upf_info_listList, s_nssai_upf_info_listItem);
    }

    cJSON *smf_serving_area = cJSON_GetObjectItemCaseSensitive(upf_infoJSON, "smfServingArea");

    OpenAPI_list_t *smf_serving_areaList;
    if (smf_serving_area) {
        cJSON *smf_serving_area_local;
        if (!cJSON_IsArray(smf_serving_area)) {
            ogs_error("OpenAPI_upf_info_parseFromJSON() failed [smf_serving_area]");
            goto end;
        }
        smf_serving_areaList = OpenAPI_list_create();

        cJSON_ArrayForEach(smf_serving_area_local, smf_serving_area) {
            if (!cJSON_IsString(smf_serving_area_local)) {
                ogs_error("OpenAPI_upf_info_parseFromJSON() failed [smf_serving_area]");
                goto end;
            }
            OpenAPI_list_add(smf_serving_areaList, ogs_strdup(smf_serving_area_local->valuestring));
        }
    }

    cJSON *interface_upf_info_list = cJSON_GetObjectItemCaseSensitive(upf_infoJSON, "interfaceUpfInfoList");

    OpenAPI_list_t *interface_upf_info_listList;
    if (interface_upf_info_list) {
        cJSON *interface_upf_info_list_local_nonprimitive;
        if (!cJSON_IsArray(interface_upf_info_list)) {
            ogs_error("OpenAPI_upf_info_parseFromJSON() failed [interface_upf_info_list]");
            goto end;
        }

        interface_upf_info_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(interface_upf_info_list_local_nonprimitive, interface_upf_info_list ) {
            if (!cJSON_IsObject(interface_upf_info_list_local_nonprimitive)) {
                ogs_error("OpenAPI_upf_info_parseFromJSON() failed [interface_upf_info_list]");
                goto end;
            }
            OpenAPI_interface_upf_info_item_t *interface_upf_info_listItem = OpenAPI_interface_upf_info_item_parseFromJSON(interface_upf_info_list_local_nonprimitive);

            OpenAPI_list_add(interface_upf_info_listList, interface_upf_info_listItem);
        }
    }

    cJSON *iwk_eps_ind = cJSON_GetObjectItemCaseSensitive(upf_infoJSON, "iwkEpsInd");

    if (iwk_eps_ind) {
        if (!cJSON_IsBool(iwk_eps_ind)) {
            ogs_error("OpenAPI_upf_info_parseFromJSON() failed [iwk_eps_ind]");
            goto end;
        }
    }

    cJSON *pdu_session_types = cJSON_GetObjectItemCaseSensitive(upf_infoJSON, "pduSessionTypes");

    OpenAPI_list_t *pdu_session_typesList;
    if (pdu_session_types) {
        cJSON *pdu_session_types_local_nonprimitive;
        if (!cJSON_IsArray(pdu_session_types)) {
            ogs_error("OpenAPI_upf_info_parseFromJSON() failed [pdu_session_types]");
            goto end;
        }

        pdu_session_typesList = OpenAPI_list_create();

        cJSON_ArrayForEach(pdu_session_types_local_nonprimitive, pdu_session_types ) {
            if (!cJSON_IsObject(pdu_session_types_local_nonprimitive)) {
                ogs_error("OpenAPI_upf_info_parseFromJSON() failed [pdu_session_types]");
                goto end;
            }
            OpenAPI_pdu_session_type_t *pdu_session_typesItem = OpenAPI_pdu_session_type_parseFromJSON(pdu_session_types_local_nonprimitive);

            OpenAPI_list_add(pdu_session_typesList, pdu_session_typesItem);
        }
    }

    cJSON *atsss_capability = cJSON_GetObjectItemCaseSensitive(upf_infoJSON, "atsssCapability");

    OpenAPI_atsss_capability_t *atsss_capability_local_nonprim = NULL;
    if (atsss_capability) {
        atsss_capability_local_nonprim = OpenAPI_atsss_capability_parseFromJSON(atsss_capability);
    }

    cJSON *ue_ip_addr_ind = cJSON_GetObjectItemCaseSensitive(upf_infoJSON, "ueIpAddrInd");

    if (ue_ip_addr_ind) {
        if (!cJSON_IsBool(ue_ip_addr_ind)) {
            ogs_error("OpenAPI_upf_info_parseFromJSON() failed [ue_ip_addr_ind]");
            goto end;
        }
    }

    upf_info_local_var = OpenAPI_upf_info_create (
        s_nssai_upf_info_listList,
        smf_serving_area ? smf_serving_areaList : NULL,
        interface_upf_info_list ? interface_upf_info_listList : NULL,
        iwk_eps_ind ? iwk_eps_ind->valueint : 0,
        pdu_session_types ? pdu_session_typesList : NULL,
        atsss_capability ? atsss_capability_local_nonprim : NULL,
        ue_ip_addr_ind ? ue_ip_addr_ind->valueint : 0
        );

    return upf_info_local_var;
end:
    return NULL;
}

