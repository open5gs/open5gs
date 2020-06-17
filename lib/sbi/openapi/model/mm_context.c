
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mm_context.h"

OpenAPI_mm_context_t *OpenAPI_mm_context_create(
    OpenAPI_access_type_e access_type,
    OpenAPI_nas_security_mode_t *nas_security_mode,
    int nas_downlink_count,
    int nas_uplink_count,
    char ue_security_capability,
    char s1_ue_network_capability,
    OpenAPI_list_t *allowed_nssai,
    OpenAPI_list_t *nssai_mapping_list,
    OpenAPI_list_t *ns_instance_list,
    OpenAPI_expected_ue_behavior_t *expected_u_ebehavior,
    char plmn_assi_ue_radio_cap_id,
    char man_assi_ue_radio_cap_id,
    char *ucmf_dic_entry_id
    )
{
    OpenAPI_mm_context_t *mm_context_local_var = OpenAPI_malloc(sizeof(OpenAPI_mm_context_t));
    if (!mm_context_local_var) {
        return NULL;
    }
    mm_context_local_var->access_type = access_type;
    mm_context_local_var->nas_security_mode = nas_security_mode;
    mm_context_local_var->nas_downlink_count = nas_downlink_count;
    mm_context_local_var->nas_uplink_count = nas_uplink_count;
    mm_context_local_var->ue_security_capability = ue_security_capability;
    mm_context_local_var->s1_ue_network_capability = s1_ue_network_capability;
    mm_context_local_var->allowed_nssai = allowed_nssai;
    mm_context_local_var->nssai_mapping_list = nssai_mapping_list;
    mm_context_local_var->ns_instance_list = ns_instance_list;
    mm_context_local_var->expected_u_ebehavior = expected_u_ebehavior;
    mm_context_local_var->plmn_assi_ue_radio_cap_id = plmn_assi_ue_radio_cap_id;
    mm_context_local_var->man_assi_ue_radio_cap_id = man_assi_ue_radio_cap_id;
    mm_context_local_var->ucmf_dic_entry_id = ucmf_dic_entry_id;

    return mm_context_local_var;
}

void OpenAPI_mm_context_free(OpenAPI_mm_context_t *mm_context)
{
    if (NULL == mm_context) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_nas_security_mode_free(mm_context->nas_security_mode);
    OpenAPI_list_for_each(mm_context->allowed_nssai, node) {
        OpenAPI_snssai_free(node->data);
    }
    OpenAPI_list_free(mm_context->allowed_nssai);
    OpenAPI_list_for_each(mm_context->nssai_mapping_list, node) {
        OpenAPI_nssai_mapping_free(node->data);
    }
    OpenAPI_list_free(mm_context->nssai_mapping_list);
    OpenAPI_list_for_each(mm_context->ns_instance_list, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(mm_context->ns_instance_list);
    OpenAPI_expected_ue_behavior_free(mm_context->expected_u_ebehavior);
    ogs_free(mm_context->ucmf_dic_entry_id);
    ogs_free(mm_context);
}

cJSON *OpenAPI_mm_context_convertToJSON(OpenAPI_mm_context_t *mm_context)
{
    cJSON *item = NULL;

    if (mm_context == NULL) {
        ogs_error("OpenAPI_mm_context_convertToJSON() failed [MmContext]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!mm_context->access_type) {
        ogs_error("OpenAPI_mm_context_convertToJSON() failed [access_type]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "accessType", OpenAPI_access_type_ToString(mm_context->access_type)) == NULL) {
        ogs_error("OpenAPI_mm_context_convertToJSON() failed [access_type]");
        goto end;
    }

    if (mm_context->nas_security_mode) {
        cJSON *nas_security_mode_local_JSON = OpenAPI_nas_security_mode_convertToJSON(mm_context->nas_security_mode);
        if (nas_security_mode_local_JSON == NULL) {
            ogs_error("OpenAPI_mm_context_convertToJSON() failed [nas_security_mode]");
            goto end;
        }
        cJSON_AddItemToObject(item, "nasSecurityMode", nas_security_mode_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_mm_context_convertToJSON() failed [nas_security_mode]");
            goto end;
        }
    }

    if (mm_context->nas_downlink_count) {
        if (cJSON_AddNumberToObject(item, "nasDownlinkCount", mm_context->nas_downlink_count) == NULL) {
            ogs_error("OpenAPI_mm_context_convertToJSON() failed [nas_downlink_count]");
            goto end;
        }
    }

    if (mm_context->nas_uplink_count) {
        if (cJSON_AddNumberToObject(item, "nasUplinkCount", mm_context->nas_uplink_count) == NULL) {
            ogs_error("OpenAPI_mm_context_convertToJSON() failed [nas_uplink_count]");
            goto end;
        }
    }

    if (mm_context->ue_security_capability) {
        if (cJSON_AddNumberToObject(item, "ueSecurityCapability", mm_context->ue_security_capability) == NULL) {
            ogs_error("OpenAPI_mm_context_convertToJSON() failed [ue_security_capability]");
            goto end;
        }
    }

    if (mm_context->s1_ue_network_capability) {
        if (cJSON_AddNumberToObject(item, "s1UeNetworkCapability", mm_context->s1_ue_network_capability) == NULL) {
            ogs_error("OpenAPI_mm_context_convertToJSON() failed [s1_ue_network_capability]");
            goto end;
        }
    }

    if (mm_context->allowed_nssai) {
        cJSON *allowed_nssaiList = cJSON_AddArrayToObject(item, "allowedNssai");
        if (allowed_nssaiList == NULL) {
            ogs_error("OpenAPI_mm_context_convertToJSON() failed [allowed_nssai]");
            goto end;
        }

        OpenAPI_lnode_t *allowed_nssai_node;
        if (mm_context->allowed_nssai) {
            OpenAPI_list_for_each(mm_context->allowed_nssai, allowed_nssai_node) {
                cJSON *itemLocal = OpenAPI_snssai_convertToJSON(allowed_nssai_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_mm_context_convertToJSON() failed [allowed_nssai]");
                    goto end;
                }
                cJSON_AddItemToArray(allowed_nssaiList, itemLocal);
            }
        }
    }

    if (mm_context->nssai_mapping_list) {
        cJSON *nssai_mapping_listList = cJSON_AddArrayToObject(item, "nssaiMappingList");
        if (nssai_mapping_listList == NULL) {
            ogs_error("OpenAPI_mm_context_convertToJSON() failed [nssai_mapping_list]");
            goto end;
        }

        OpenAPI_lnode_t *nssai_mapping_list_node;
        if (mm_context->nssai_mapping_list) {
            OpenAPI_list_for_each(mm_context->nssai_mapping_list, nssai_mapping_list_node) {
                cJSON *itemLocal = OpenAPI_nssai_mapping_convertToJSON(nssai_mapping_list_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_mm_context_convertToJSON() failed [nssai_mapping_list]");
                    goto end;
                }
                cJSON_AddItemToArray(nssai_mapping_listList, itemLocal);
            }
        }
    }

    if (mm_context->ns_instance_list) {
        cJSON *ns_instance_list = cJSON_AddArrayToObject(item, "nsInstanceList");
        if (ns_instance_list == NULL) {
            ogs_error("OpenAPI_mm_context_convertToJSON() failed [ns_instance_list]");
            goto end;
        }

        OpenAPI_lnode_t *ns_instance_list_node;
        OpenAPI_list_for_each(mm_context->ns_instance_list, ns_instance_list_node)  {
            if (cJSON_AddStringToObject(ns_instance_list, "", (char*)ns_instance_list_node->data) == NULL) {
                ogs_error("OpenAPI_mm_context_convertToJSON() failed [ns_instance_list]");
                goto end;
            }
        }
    }

    if (mm_context->expected_u_ebehavior) {
        cJSON *expected_u_ebehavior_local_JSON = OpenAPI_expected_ue_behavior_convertToJSON(mm_context->expected_u_ebehavior);
        if (expected_u_ebehavior_local_JSON == NULL) {
            ogs_error("OpenAPI_mm_context_convertToJSON() failed [expected_u_ebehavior]");
            goto end;
        }
        cJSON_AddItemToObject(item, "expectedUEbehavior", expected_u_ebehavior_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_mm_context_convertToJSON() failed [expected_u_ebehavior]");
            goto end;
        }
    }

    if (mm_context->plmn_assi_ue_radio_cap_id) {
        if (cJSON_AddNumberToObject(item, "plmnAssiUeRadioCapId", mm_context->plmn_assi_ue_radio_cap_id) == NULL) {
            ogs_error("OpenAPI_mm_context_convertToJSON() failed [plmn_assi_ue_radio_cap_id]");
            goto end;
        }
    }

    if (mm_context->man_assi_ue_radio_cap_id) {
        if (cJSON_AddNumberToObject(item, "manAssiUeRadioCapId", mm_context->man_assi_ue_radio_cap_id) == NULL) {
            ogs_error("OpenAPI_mm_context_convertToJSON() failed [man_assi_ue_radio_cap_id]");
            goto end;
        }
    }

    if (mm_context->ucmf_dic_entry_id) {
        if (cJSON_AddStringToObject(item, "ucmfDicEntryId", mm_context->ucmf_dic_entry_id) == NULL) {
            ogs_error("OpenAPI_mm_context_convertToJSON() failed [ucmf_dic_entry_id]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_mm_context_t *OpenAPI_mm_context_parseFromJSON(cJSON *mm_contextJSON)
{
    OpenAPI_mm_context_t *mm_context_local_var = NULL;
    cJSON *access_type = cJSON_GetObjectItemCaseSensitive(mm_contextJSON, "accessType");
    if (!access_type) {
        ogs_error("OpenAPI_mm_context_parseFromJSON() failed [access_type]");
        goto end;
    }

    OpenAPI_access_type_e access_typeVariable;

    if (!cJSON_IsString(access_type)) {
        ogs_error("OpenAPI_mm_context_parseFromJSON() failed [access_type]");
        goto end;
    }
    access_typeVariable = OpenAPI_access_type_FromString(access_type->valuestring);

    cJSON *nas_security_mode = cJSON_GetObjectItemCaseSensitive(mm_contextJSON, "nasSecurityMode");

    OpenAPI_nas_security_mode_t *nas_security_mode_local_nonprim = NULL;
    if (nas_security_mode) {
        nas_security_mode_local_nonprim = OpenAPI_nas_security_mode_parseFromJSON(nas_security_mode);
    }

    cJSON *nas_downlink_count = cJSON_GetObjectItemCaseSensitive(mm_contextJSON, "nasDownlinkCount");

    if (nas_downlink_count) {
        if (!cJSON_IsNumber(nas_downlink_count)) {
            ogs_error("OpenAPI_mm_context_parseFromJSON() failed [nas_downlink_count]");
            goto end;
        }
    }

    cJSON *nas_uplink_count = cJSON_GetObjectItemCaseSensitive(mm_contextJSON, "nasUplinkCount");

    if (nas_uplink_count) {
        if (!cJSON_IsNumber(nas_uplink_count)) {
            ogs_error("OpenAPI_mm_context_parseFromJSON() failed [nas_uplink_count]");
            goto end;
        }
    }

    cJSON *ue_security_capability = cJSON_GetObjectItemCaseSensitive(mm_contextJSON, "ueSecurityCapability");

    if (ue_security_capability) {
        if (!cJSON_IsNumber(ue_security_capability)) {
            ogs_error("OpenAPI_mm_context_parseFromJSON() failed [ue_security_capability]");
            goto end;
        }
    }

    cJSON *s1_ue_network_capability = cJSON_GetObjectItemCaseSensitive(mm_contextJSON, "s1UeNetworkCapability");

    if (s1_ue_network_capability) {
        if (!cJSON_IsNumber(s1_ue_network_capability)) {
            ogs_error("OpenAPI_mm_context_parseFromJSON() failed [s1_ue_network_capability]");
            goto end;
        }
    }

    cJSON *allowed_nssai = cJSON_GetObjectItemCaseSensitive(mm_contextJSON, "allowedNssai");

    OpenAPI_list_t *allowed_nssaiList;
    if (allowed_nssai) {
        cJSON *allowed_nssai_local_nonprimitive;
        if (!cJSON_IsArray(allowed_nssai)) {
            ogs_error("OpenAPI_mm_context_parseFromJSON() failed [allowed_nssai]");
            goto end;
        }

        allowed_nssaiList = OpenAPI_list_create();

        cJSON_ArrayForEach(allowed_nssai_local_nonprimitive, allowed_nssai ) {
            if (!cJSON_IsObject(allowed_nssai_local_nonprimitive)) {
                ogs_error("OpenAPI_mm_context_parseFromJSON() failed [allowed_nssai]");
                goto end;
            }
            OpenAPI_snssai_t *allowed_nssaiItem = OpenAPI_snssai_parseFromJSON(allowed_nssai_local_nonprimitive);

            OpenAPI_list_add(allowed_nssaiList, allowed_nssaiItem);
        }
    }

    cJSON *nssai_mapping_list = cJSON_GetObjectItemCaseSensitive(mm_contextJSON, "nssaiMappingList");

    OpenAPI_list_t *nssai_mapping_listList;
    if (nssai_mapping_list) {
        cJSON *nssai_mapping_list_local_nonprimitive;
        if (!cJSON_IsArray(nssai_mapping_list)) {
            ogs_error("OpenAPI_mm_context_parseFromJSON() failed [nssai_mapping_list]");
            goto end;
        }

        nssai_mapping_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(nssai_mapping_list_local_nonprimitive, nssai_mapping_list ) {
            if (!cJSON_IsObject(nssai_mapping_list_local_nonprimitive)) {
                ogs_error("OpenAPI_mm_context_parseFromJSON() failed [nssai_mapping_list]");
                goto end;
            }
            OpenAPI_nssai_mapping_t *nssai_mapping_listItem = OpenAPI_nssai_mapping_parseFromJSON(nssai_mapping_list_local_nonprimitive);

            OpenAPI_list_add(nssai_mapping_listList, nssai_mapping_listItem);
        }
    }

    cJSON *ns_instance_list = cJSON_GetObjectItemCaseSensitive(mm_contextJSON, "nsInstanceList");

    OpenAPI_list_t *ns_instance_listList;
    if (ns_instance_list) {
        cJSON *ns_instance_list_local;
        if (!cJSON_IsArray(ns_instance_list)) {
            ogs_error("OpenAPI_mm_context_parseFromJSON() failed [ns_instance_list]");
            goto end;
        }
        ns_instance_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(ns_instance_list_local, ns_instance_list) {
            if (!cJSON_IsString(ns_instance_list_local)) {
                ogs_error("OpenAPI_mm_context_parseFromJSON() failed [ns_instance_list]");
                goto end;
            }
            OpenAPI_list_add(ns_instance_listList, ogs_strdup(ns_instance_list_local->valuestring));
        }
    }

    cJSON *expected_u_ebehavior = cJSON_GetObjectItemCaseSensitive(mm_contextJSON, "expectedUEbehavior");

    OpenAPI_expected_ue_behavior_t *expected_u_ebehavior_local_nonprim = NULL;
    if (expected_u_ebehavior) {
        expected_u_ebehavior_local_nonprim = OpenAPI_expected_ue_behavior_parseFromJSON(expected_u_ebehavior);
    }

    cJSON *plmn_assi_ue_radio_cap_id = cJSON_GetObjectItemCaseSensitive(mm_contextJSON, "plmnAssiUeRadioCapId");

    if (plmn_assi_ue_radio_cap_id) {
        if (!cJSON_IsNumber(plmn_assi_ue_radio_cap_id)) {
            ogs_error("OpenAPI_mm_context_parseFromJSON() failed [plmn_assi_ue_radio_cap_id]");
            goto end;
        }
    }

    cJSON *man_assi_ue_radio_cap_id = cJSON_GetObjectItemCaseSensitive(mm_contextJSON, "manAssiUeRadioCapId");

    if (man_assi_ue_radio_cap_id) {
        if (!cJSON_IsNumber(man_assi_ue_radio_cap_id)) {
            ogs_error("OpenAPI_mm_context_parseFromJSON() failed [man_assi_ue_radio_cap_id]");
            goto end;
        }
    }

    cJSON *ucmf_dic_entry_id = cJSON_GetObjectItemCaseSensitive(mm_contextJSON, "ucmfDicEntryId");

    if (ucmf_dic_entry_id) {
        if (!cJSON_IsString(ucmf_dic_entry_id)) {
            ogs_error("OpenAPI_mm_context_parseFromJSON() failed [ucmf_dic_entry_id]");
            goto end;
        }
    }

    mm_context_local_var = OpenAPI_mm_context_create (
        access_typeVariable,
        nas_security_mode ? nas_security_mode_local_nonprim : NULL,
        nas_downlink_count ? nas_downlink_count->valuedouble : 0,
        nas_uplink_count ? nas_uplink_count->valuedouble : 0,
        ue_security_capability ? ue_security_capability->valueint : 0,
        s1_ue_network_capability ? s1_ue_network_capability->valueint : 0,
        allowed_nssai ? allowed_nssaiList : NULL,
        nssai_mapping_list ? nssai_mapping_listList : NULL,
        ns_instance_list ? ns_instance_listList : NULL,
        expected_u_ebehavior ? expected_u_ebehavior_local_nonprim : NULL,
        plmn_assi_ue_radio_cap_id ? plmn_assi_ue_radio_cap_id->valueint : 0,
        man_assi_ue_radio_cap_id ? man_assi_ue_radio_cap_id->valueint : 0,
        ucmf_dic_entry_id ? ogs_strdup(ucmf_dic_entry_id->valuestring) : NULL
        );

    return mm_context_local_var;
end:
    return NULL;
}

OpenAPI_mm_context_t *OpenAPI_mm_context_copy(OpenAPI_mm_context_t *dst, OpenAPI_mm_context_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_mm_context_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_mm_context_convertToJSON() failed");
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

    OpenAPI_mm_context_free(dst);
    dst = OpenAPI_mm_context_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

