
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sm_context_updated_data.h"

OpenAPI_sm_context_updated_data_t *OpenAPI_sm_context_updated_data_create(
    OpenAPI_up_cnx_state_e up_cnx_state,
    OpenAPI_ho_state_e ho_state,
    OpenAPI_list_t *release_ebi_list,
    OpenAPI_list_t *allocated_ebi_list,
    OpenAPI_list_t *modified_ebi_list,
    OpenAPI_ref_to_binary_data_t *n1_sm_msg,
    OpenAPI_ref_to_binary_data_t *n2_sm_info,
    OpenAPI_n2_sm_info_type_e n2_sm_info_type,
    OpenAPI_list_t *eps_bearer_setup,
    bool is_data_forwarding,
    int data_forwarding,
    OpenAPI_list_t *n3_dl_forwarding_tnl_list,
    OpenAPI_list_t *n3_ul_forwarding_tnl_list,
    OpenAPI_tunnel_info_t *n9_ul_forwarding_tunnel,
    OpenAPI_cause_e cause,
    bool is_ma_accepted_ind,
    int ma_accepted_ind,
    char *supported_features,
    char *forwarding_f_teid,
    OpenAPI_list_t *forwarding_bearer_contexts,
    char *selected_smf_id,
    char *selected_old_smf_id,
    char *inter_plmn_api_root,
    OpenAPI_anchor_smf_features_t *anchor_smf_features
)
{
    OpenAPI_sm_context_updated_data_t *sm_context_updated_data_local_var = ogs_malloc(sizeof(OpenAPI_sm_context_updated_data_t));
    ogs_assert(sm_context_updated_data_local_var);

    sm_context_updated_data_local_var->up_cnx_state = up_cnx_state;
    sm_context_updated_data_local_var->ho_state = ho_state;
    sm_context_updated_data_local_var->release_ebi_list = release_ebi_list;
    sm_context_updated_data_local_var->allocated_ebi_list = allocated_ebi_list;
    sm_context_updated_data_local_var->modified_ebi_list = modified_ebi_list;
    sm_context_updated_data_local_var->n1_sm_msg = n1_sm_msg;
    sm_context_updated_data_local_var->n2_sm_info = n2_sm_info;
    sm_context_updated_data_local_var->n2_sm_info_type = n2_sm_info_type;
    sm_context_updated_data_local_var->eps_bearer_setup = eps_bearer_setup;
    sm_context_updated_data_local_var->is_data_forwarding = is_data_forwarding;
    sm_context_updated_data_local_var->data_forwarding = data_forwarding;
    sm_context_updated_data_local_var->n3_dl_forwarding_tnl_list = n3_dl_forwarding_tnl_list;
    sm_context_updated_data_local_var->n3_ul_forwarding_tnl_list = n3_ul_forwarding_tnl_list;
    sm_context_updated_data_local_var->n9_ul_forwarding_tunnel = n9_ul_forwarding_tunnel;
    sm_context_updated_data_local_var->cause = cause;
    sm_context_updated_data_local_var->is_ma_accepted_ind = is_ma_accepted_ind;
    sm_context_updated_data_local_var->ma_accepted_ind = ma_accepted_ind;
    sm_context_updated_data_local_var->supported_features = supported_features;
    sm_context_updated_data_local_var->forwarding_f_teid = forwarding_f_teid;
    sm_context_updated_data_local_var->forwarding_bearer_contexts = forwarding_bearer_contexts;
    sm_context_updated_data_local_var->selected_smf_id = selected_smf_id;
    sm_context_updated_data_local_var->selected_old_smf_id = selected_old_smf_id;
    sm_context_updated_data_local_var->inter_plmn_api_root = inter_plmn_api_root;
    sm_context_updated_data_local_var->anchor_smf_features = anchor_smf_features;

    return sm_context_updated_data_local_var;
}

void OpenAPI_sm_context_updated_data_free(OpenAPI_sm_context_updated_data_t *sm_context_updated_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == sm_context_updated_data) {
        return;
    }
    if (sm_context_updated_data->release_ebi_list) {
        OpenAPI_list_for_each(sm_context_updated_data->release_ebi_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(sm_context_updated_data->release_ebi_list);
        sm_context_updated_data->release_ebi_list = NULL;
    }
    if (sm_context_updated_data->allocated_ebi_list) {
        OpenAPI_list_for_each(sm_context_updated_data->allocated_ebi_list, node) {
            OpenAPI_ebi_arp_mapping_free(node->data);
        }
        OpenAPI_list_free(sm_context_updated_data->allocated_ebi_list);
        sm_context_updated_data->allocated_ebi_list = NULL;
    }
    if (sm_context_updated_data->modified_ebi_list) {
        OpenAPI_list_for_each(sm_context_updated_data->modified_ebi_list, node) {
            OpenAPI_ebi_arp_mapping_free(node->data);
        }
        OpenAPI_list_free(sm_context_updated_data->modified_ebi_list);
        sm_context_updated_data->modified_ebi_list = NULL;
    }
    if (sm_context_updated_data->n1_sm_msg) {
        OpenAPI_ref_to_binary_data_free(sm_context_updated_data->n1_sm_msg);
        sm_context_updated_data->n1_sm_msg = NULL;
    }
    if (sm_context_updated_data->n2_sm_info) {
        OpenAPI_ref_to_binary_data_free(sm_context_updated_data->n2_sm_info);
        sm_context_updated_data->n2_sm_info = NULL;
    }
    if (sm_context_updated_data->eps_bearer_setup) {
        OpenAPI_list_for_each(sm_context_updated_data->eps_bearer_setup, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(sm_context_updated_data->eps_bearer_setup);
        sm_context_updated_data->eps_bearer_setup = NULL;
    }
    if (sm_context_updated_data->n3_dl_forwarding_tnl_list) {
        OpenAPI_list_for_each(sm_context_updated_data->n3_dl_forwarding_tnl_list, node) {
            OpenAPI_indirect_data_forwarding_tunnel_info_free(node->data);
        }
        OpenAPI_list_free(sm_context_updated_data->n3_dl_forwarding_tnl_list);
        sm_context_updated_data->n3_dl_forwarding_tnl_list = NULL;
    }
    if (sm_context_updated_data->n3_ul_forwarding_tnl_list) {
        OpenAPI_list_for_each(sm_context_updated_data->n3_ul_forwarding_tnl_list, node) {
            OpenAPI_indirect_data_forwarding_tunnel_info_free(node->data);
        }
        OpenAPI_list_free(sm_context_updated_data->n3_ul_forwarding_tnl_list);
        sm_context_updated_data->n3_ul_forwarding_tnl_list = NULL;
    }
    if (sm_context_updated_data->n9_ul_forwarding_tunnel) {
        OpenAPI_tunnel_info_free(sm_context_updated_data->n9_ul_forwarding_tunnel);
        sm_context_updated_data->n9_ul_forwarding_tunnel = NULL;
    }
    if (sm_context_updated_data->supported_features) {
        ogs_free(sm_context_updated_data->supported_features);
        sm_context_updated_data->supported_features = NULL;
    }
    if (sm_context_updated_data->forwarding_f_teid) {
        ogs_free(sm_context_updated_data->forwarding_f_teid);
        sm_context_updated_data->forwarding_f_teid = NULL;
    }
    if (sm_context_updated_data->forwarding_bearer_contexts) {
        OpenAPI_list_for_each(sm_context_updated_data->forwarding_bearer_contexts, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(sm_context_updated_data->forwarding_bearer_contexts);
        sm_context_updated_data->forwarding_bearer_contexts = NULL;
    }
    if (sm_context_updated_data->selected_smf_id) {
        ogs_free(sm_context_updated_data->selected_smf_id);
        sm_context_updated_data->selected_smf_id = NULL;
    }
    if (sm_context_updated_data->selected_old_smf_id) {
        ogs_free(sm_context_updated_data->selected_old_smf_id);
        sm_context_updated_data->selected_old_smf_id = NULL;
    }
    if (sm_context_updated_data->inter_plmn_api_root) {
        ogs_free(sm_context_updated_data->inter_plmn_api_root);
        sm_context_updated_data->inter_plmn_api_root = NULL;
    }
    if (sm_context_updated_data->anchor_smf_features) {
        OpenAPI_anchor_smf_features_free(sm_context_updated_data->anchor_smf_features);
        sm_context_updated_data->anchor_smf_features = NULL;
    }
    ogs_free(sm_context_updated_data);
}

cJSON *OpenAPI_sm_context_updated_data_convertToJSON(OpenAPI_sm_context_updated_data_t *sm_context_updated_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (sm_context_updated_data == NULL) {
        ogs_error("OpenAPI_sm_context_updated_data_convertToJSON() failed [SmContextUpdatedData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (sm_context_updated_data->up_cnx_state != OpenAPI_up_cnx_state_NULL) {
    if (cJSON_AddStringToObject(item, "upCnxState", OpenAPI_up_cnx_state_ToString(sm_context_updated_data->up_cnx_state)) == NULL) {
        ogs_error("OpenAPI_sm_context_updated_data_convertToJSON() failed [up_cnx_state]");
        goto end;
    }
    }

    if (sm_context_updated_data->ho_state != OpenAPI_ho_state_NULL) {
    if (cJSON_AddStringToObject(item, "hoState", OpenAPI_ho_state_ToString(sm_context_updated_data->ho_state)) == NULL) {
        ogs_error("OpenAPI_sm_context_updated_data_convertToJSON() failed [ho_state]");
        goto end;
    }
    }

    if (sm_context_updated_data->release_ebi_list) {
    cJSON *release_ebi_listList = cJSON_AddArrayToObject(item, "releaseEbiList");
    if (release_ebi_listList == NULL) {
        ogs_error("OpenAPI_sm_context_updated_data_convertToJSON() failed [release_ebi_list]");
        goto end;
    }
    OpenAPI_list_for_each(sm_context_updated_data->release_ebi_list, node) {
        if (node->data == NULL) {
            ogs_error("OpenAPI_sm_context_updated_data_convertToJSON() failed [release_ebi_list]");
            goto end;
        }
        if (cJSON_AddNumberToObject(release_ebi_listList, "", *(double *)node->data) == NULL) {
            ogs_error("OpenAPI_sm_context_updated_data_convertToJSON() failed [release_ebi_list]");
            goto end;
        }
    }
    }

    if (sm_context_updated_data->allocated_ebi_list) {
    cJSON *allocated_ebi_listList = cJSON_AddArrayToObject(item, "allocatedEbiList");
    if (allocated_ebi_listList == NULL) {
        ogs_error("OpenAPI_sm_context_updated_data_convertToJSON() failed [allocated_ebi_list]");
        goto end;
    }
    OpenAPI_list_for_each(sm_context_updated_data->allocated_ebi_list, node) {
        cJSON *itemLocal = OpenAPI_ebi_arp_mapping_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sm_context_updated_data_convertToJSON() failed [allocated_ebi_list]");
            goto end;
        }
        cJSON_AddItemToArray(allocated_ebi_listList, itemLocal);
    }
    }

    if (sm_context_updated_data->modified_ebi_list) {
    cJSON *modified_ebi_listList = cJSON_AddArrayToObject(item, "modifiedEbiList");
    if (modified_ebi_listList == NULL) {
        ogs_error("OpenAPI_sm_context_updated_data_convertToJSON() failed [modified_ebi_list]");
        goto end;
    }
    OpenAPI_list_for_each(sm_context_updated_data->modified_ebi_list, node) {
        cJSON *itemLocal = OpenAPI_ebi_arp_mapping_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sm_context_updated_data_convertToJSON() failed [modified_ebi_list]");
            goto end;
        }
        cJSON_AddItemToArray(modified_ebi_listList, itemLocal);
    }
    }

    if (sm_context_updated_data->n1_sm_msg) {
    cJSON *n1_sm_msg_local_JSON = OpenAPI_ref_to_binary_data_convertToJSON(sm_context_updated_data->n1_sm_msg);
    if (n1_sm_msg_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_updated_data_convertToJSON() failed [n1_sm_msg]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n1SmMsg", n1_sm_msg_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_updated_data_convertToJSON() failed [n1_sm_msg]");
        goto end;
    }
    }

    if (sm_context_updated_data->n2_sm_info) {
    cJSON *n2_sm_info_local_JSON = OpenAPI_ref_to_binary_data_convertToJSON(sm_context_updated_data->n2_sm_info);
    if (n2_sm_info_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_updated_data_convertToJSON() failed [n2_sm_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n2SmInfo", n2_sm_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_updated_data_convertToJSON() failed [n2_sm_info]");
        goto end;
    }
    }

    if (sm_context_updated_data->n2_sm_info_type != OpenAPI_n2_sm_info_type_NULL) {
    if (cJSON_AddStringToObject(item, "n2SmInfoType", OpenAPI_n2_sm_info_type_ToString(sm_context_updated_data->n2_sm_info_type)) == NULL) {
        ogs_error("OpenAPI_sm_context_updated_data_convertToJSON() failed [n2_sm_info_type]");
        goto end;
    }
    }

    if (sm_context_updated_data->eps_bearer_setup) {
    cJSON *eps_bearer_setupList = cJSON_AddArrayToObject(item, "epsBearerSetup");
    if (eps_bearer_setupList == NULL) {
        ogs_error("OpenAPI_sm_context_updated_data_convertToJSON() failed [eps_bearer_setup]");
        goto end;
    }
    OpenAPI_list_for_each(sm_context_updated_data->eps_bearer_setup, node) {
        if (cJSON_AddStringToObject(eps_bearer_setupList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_sm_context_updated_data_convertToJSON() failed [eps_bearer_setup]");
            goto end;
        }
    }
    }

    if (sm_context_updated_data->is_data_forwarding) {
    if (cJSON_AddBoolToObject(item, "dataForwarding", sm_context_updated_data->data_forwarding) == NULL) {
        ogs_error("OpenAPI_sm_context_updated_data_convertToJSON() failed [data_forwarding]");
        goto end;
    }
    }

    if (sm_context_updated_data->n3_dl_forwarding_tnl_list) {
    cJSON *n3_dl_forwarding_tnl_listList = cJSON_AddArrayToObject(item, "n3DlForwardingTnlList");
    if (n3_dl_forwarding_tnl_listList == NULL) {
        ogs_error("OpenAPI_sm_context_updated_data_convertToJSON() failed [n3_dl_forwarding_tnl_list]");
        goto end;
    }
    OpenAPI_list_for_each(sm_context_updated_data->n3_dl_forwarding_tnl_list, node) {
        cJSON *itemLocal = OpenAPI_indirect_data_forwarding_tunnel_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sm_context_updated_data_convertToJSON() failed [n3_dl_forwarding_tnl_list]");
            goto end;
        }
        cJSON_AddItemToArray(n3_dl_forwarding_tnl_listList, itemLocal);
    }
    }

    if (sm_context_updated_data->n3_ul_forwarding_tnl_list) {
    cJSON *n3_ul_forwarding_tnl_listList = cJSON_AddArrayToObject(item, "n3UlForwardingTnlList");
    if (n3_ul_forwarding_tnl_listList == NULL) {
        ogs_error("OpenAPI_sm_context_updated_data_convertToJSON() failed [n3_ul_forwarding_tnl_list]");
        goto end;
    }
    OpenAPI_list_for_each(sm_context_updated_data->n3_ul_forwarding_tnl_list, node) {
        cJSON *itemLocal = OpenAPI_indirect_data_forwarding_tunnel_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sm_context_updated_data_convertToJSON() failed [n3_ul_forwarding_tnl_list]");
            goto end;
        }
        cJSON_AddItemToArray(n3_ul_forwarding_tnl_listList, itemLocal);
    }
    }

    if (sm_context_updated_data->n9_ul_forwarding_tunnel) {
    cJSON *n9_ul_forwarding_tunnel_local_JSON = OpenAPI_tunnel_info_convertToJSON(sm_context_updated_data->n9_ul_forwarding_tunnel);
    if (n9_ul_forwarding_tunnel_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_updated_data_convertToJSON() failed [n9_ul_forwarding_tunnel]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n9UlForwardingTunnel", n9_ul_forwarding_tunnel_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_updated_data_convertToJSON() failed [n9_ul_forwarding_tunnel]");
        goto end;
    }
    }

    if (sm_context_updated_data->cause != OpenAPI_cause_NULL) {
    if (cJSON_AddStringToObject(item, "cause", OpenAPI_cause_ToString(sm_context_updated_data->cause)) == NULL) {
        ogs_error("OpenAPI_sm_context_updated_data_convertToJSON() failed [cause]");
        goto end;
    }
    }

    if (sm_context_updated_data->is_ma_accepted_ind) {
    if (cJSON_AddBoolToObject(item, "maAcceptedInd", sm_context_updated_data->ma_accepted_ind) == NULL) {
        ogs_error("OpenAPI_sm_context_updated_data_convertToJSON() failed [ma_accepted_ind]");
        goto end;
    }
    }

    if (sm_context_updated_data->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", sm_context_updated_data->supported_features) == NULL) {
        ogs_error("OpenAPI_sm_context_updated_data_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (sm_context_updated_data->forwarding_f_teid) {
    if (cJSON_AddStringToObject(item, "forwardingFTeid", sm_context_updated_data->forwarding_f_teid) == NULL) {
        ogs_error("OpenAPI_sm_context_updated_data_convertToJSON() failed [forwarding_f_teid]");
        goto end;
    }
    }

    if (sm_context_updated_data->forwarding_bearer_contexts) {
    cJSON *forwarding_bearer_contextsList = cJSON_AddArrayToObject(item, "forwardingBearerContexts");
    if (forwarding_bearer_contextsList == NULL) {
        ogs_error("OpenAPI_sm_context_updated_data_convertToJSON() failed [forwarding_bearer_contexts]");
        goto end;
    }
    OpenAPI_list_for_each(sm_context_updated_data->forwarding_bearer_contexts, node) {
        if (cJSON_AddStringToObject(forwarding_bearer_contextsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_sm_context_updated_data_convertToJSON() failed [forwarding_bearer_contexts]");
            goto end;
        }
    }
    }

    if (sm_context_updated_data->selected_smf_id) {
    if (cJSON_AddStringToObject(item, "selectedSmfId", sm_context_updated_data->selected_smf_id) == NULL) {
        ogs_error("OpenAPI_sm_context_updated_data_convertToJSON() failed [selected_smf_id]");
        goto end;
    }
    }

    if (sm_context_updated_data->selected_old_smf_id) {
    if (cJSON_AddStringToObject(item, "selectedOldSmfId", sm_context_updated_data->selected_old_smf_id) == NULL) {
        ogs_error("OpenAPI_sm_context_updated_data_convertToJSON() failed [selected_old_smf_id]");
        goto end;
    }
    }

    if (sm_context_updated_data->inter_plmn_api_root) {
    if (cJSON_AddStringToObject(item, "interPlmnApiRoot", sm_context_updated_data->inter_plmn_api_root) == NULL) {
        ogs_error("OpenAPI_sm_context_updated_data_convertToJSON() failed [inter_plmn_api_root]");
        goto end;
    }
    }

    if (sm_context_updated_data->anchor_smf_features) {
    cJSON *anchor_smf_features_local_JSON = OpenAPI_anchor_smf_features_convertToJSON(sm_context_updated_data->anchor_smf_features);
    if (anchor_smf_features_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_updated_data_convertToJSON() failed [anchor_smf_features]");
        goto end;
    }
    cJSON_AddItemToObject(item, "anchorSmfFeatures", anchor_smf_features_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_updated_data_convertToJSON() failed [anchor_smf_features]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_sm_context_updated_data_t *OpenAPI_sm_context_updated_data_parseFromJSON(cJSON *sm_context_updated_dataJSON)
{
    OpenAPI_sm_context_updated_data_t *sm_context_updated_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *up_cnx_state = NULL;
    OpenAPI_up_cnx_state_e up_cnx_stateVariable = 0;
    cJSON *ho_state = NULL;
    OpenAPI_ho_state_e ho_stateVariable = 0;
    cJSON *release_ebi_list = NULL;
    OpenAPI_list_t *release_ebi_listList = NULL;
    cJSON *allocated_ebi_list = NULL;
    OpenAPI_list_t *allocated_ebi_listList = NULL;
    cJSON *modified_ebi_list = NULL;
    OpenAPI_list_t *modified_ebi_listList = NULL;
    cJSON *n1_sm_msg = NULL;
    OpenAPI_ref_to_binary_data_t *n1_sm_msg_local_nonprim = NULL;
    cJSON *n2_sm_info = NULL;
    OpenAPI_ref_to_binary_data_t *n2_sm_info_local_nonprim = NULL;
    cJSON *n2_sm_info_type = NULL;
    OpenAPI_n2_sm_info_type_e n2_sm_info_typeVariable = 0;
    cJSON *eps_bearer_setup = NULL;
    OpenAPI_list_t *eps_bearer_setupList = NULL;
    cJSON *data_forwarding = NULL;
    cJSON *n3_dl_forwarding_tnl_list = NULL;
    OpenAPI_list_t *n3_dl_forwarding_tnl_listList = NULL;
    cJSON *n3_ul_forwarding_tnl_list = NULL;
    OpenAPI_list_t *n3_ul_forwarding_tnl_listList = NULL;
    cJSON *n9_ul_forwarding_tunnel = NULL;
    OpenAPI_tunnel_info_t *n9_ul_forwarding_tunnel_local_nonprim = NULL;
    cJSON *cause = NULL;
    OpenAPI_cause_e causeVariable = 0;
    cJSON *ma_accepted_ind = NULL;
    cJSON *supported_features = NULL;
    cJSON *forwarding_f_teid = NULL;
    cJSON *forwarding_bearer_contexts = NULL;
    OpenAPI_list_t *forwarding_bearer_contextsList = NULL;
    cJSON *selected_smf_id = NULL;
    cJSON *selected_old_smf_id = NULL;
    cJSON *inter_plmn_api_root = NULL;
    cJSON *anchor_smf_features = NULL;
    OpenAPI_anchor_smf_features_t *anchor_smf_features_local_nonprim = NULL;
    up_cnx_state = cJSON_GetObjectItemCaseSensitive(sm_context_updated_dataJSON, "upCnxState");
    if (up_cnx_state) {
    if (!cJSON_IsString(up_cnx_state)) {
        ogs_error("OpenAPI_sm_context_updated_data_parseFromJSON() failed [up_cnx_state]");
        goto end;
    }
    up_cnx_stateVariable = OpenAPI_up_cnx_state_FromString(up_cnx_state->valuestring);
    }

    ho_state = cJSON_GetObjectItemCaseSensitive(sm_context_updated_dataJSON, "hoState");
    if (ho_state) {
    if (!cJSON_IsString(ho_state)) {
        ogs_error("OpenAPI_sm_context_updated_data_parseFromJSON() failed [ho_state]");
        goto end;
    }
    ho_stateVariable = OpenAPI_ho_state_FromString(ho_state->valuestring);
    }

    release_ebi_list = cJSON_GetObjectItemCaseSensitive(sm_context_updated_dataJSON, "releaseEbiList");
    if (release_ebi_list) {
        cJSON *release_ebi_list_local = NULL;
        if (!cJSON_IsArray(release_ebi_list)) {
            ogs_error("OpenAPI_sm_context_updated_data_parseFromJSON() failed [release_ebi_list]");
            goto end;
        }

        release_ebi_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(release_ebi_list_local, release_ebi_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsNumber(release_ebi_list_local)) {
                ogs_error("OpenAPI_sm_context_updated_data_parseFromJSON() failed [release_ebi_list]");
                goto end;
            }
            localDouble = (double *)ogs_calloc(1, sizeof(double));
            if (!localDouble) {
                ogs_error("OpenAPI_sm_context_updated_data_parseFromJSON() failed [release_ebi_list]");
                goto end;
            }
            *localDouble = release_ebi_list_local->valuedouble;
            OpenAPI_list_add(release_ebi_listList, localDouble);
        }
    }

    allocated_ebi_list = cJSON_GetObjectItemCaseSensitive(sm_context_updated_dataJSON, "allocatedEbiList");
    if (allocated_ebi_list) {
        cJSON *allocated_ebi_list_local = NULL;
        if (!cJSON_IsArray(allocated_ebi_list)) {
            ogs_error("OpenAPI_sm_context_updated_data_parseFromJSON() failed [allocated_ebi_list]");
            goto end;
        }

        allocated_ebi_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(allocated_ebi_list_local, allocated_ebi_list) {
            if (!cJSON_IsObject(allocated_ebi_list_local)) {
                ogs_error("OpenAPI_sm_context_updated_data_parseFromJSON() failed [allocated_ebi_list]");
                goto end;
            }
            OpenAPI_ebi_arp_mapping_t *allocated_ebi_listItem = OpenAPI_ebi_arp_mapping_parseFromJSON(allocated_ebi_list_local);
            if (!allocated_ebi_listItem) {
                ogs_error("No allocated_ebi_listItem");
                goto end;
            }
            OpenAPI_list_add(allocated_ebi_listList, allocated_ebi_listItem);
        }
    }

    modified_ebi_list = cJSON_GetObjectItemCaseSensitive(sm_context_updated_dataJSON, "modifiedEbiList");
    if (modified_ebi_list) {
        cJSON *modified_ebi_list_local = NULL;
        if (!cJSON_IsArray(modified_ebi_list)) {
            ogs_error("OpenAPI_sm_context_updated_data_parseFromJSON() failed [modified_ebi_list]");
            goto end;
        }

        modified_ebi_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(modified_ebi_list_local, modified_ebi_list) {
            if (!cJSON_IsObject(modified_ebi_list_local)) {
                ogs_error("OpenAPI_sm_context_updated_data_parseFromJSON() failed [modified_ebi_list]");
                goto end;
            }
            OpenAPI_ebi_arp_mapping_t *modified_ebi_listItem = OpenAPI_ebi_arp_mapping_parseFromJSON(modified_ebi_list_local);
            if (!modified_ebi_listItem) {
                ogs_error("No modified_ebi_listItem");
                goto end;
            }
            OpenAPI_list_add(modified_ebi_listList, modified_ebi_listItem);
        }
    }

    n1_sm_msg = cJSON_GetObjectItemCaseSensitive(sm_context_updated_dataJSON, "n1SmMsg");
    if (n1_sm_msg) {
    n1_sm_msg_local_nonprim = OpenAPI_ref_to_binary_data_parseFromJSON(n1_sm_msg);
    if (!n1_sm_msg_local_nonprim) {
        ogs_error("OpenAPI_ref_to_binary_data_parseFromJSON failed [n1_sm_msg]");
        goto end;
    }
    }

    n2_sm_info = cJSON_GetObjectItemCaseSensitive(sm_context_updated_dataJSON, "n2SmInfo");
    if (n2_sm_info) {
    n2_sm_info_local_nonprim = OpenAPI_ref_to_binary_data_parseFromJSON(n2_sm_info);
    if (!n2_sm_info_local_nonprim) {
        ogs_error("OpenAPI_ref_to_binary_data_parseFromJSON failed [n2_sm_info]");
        goto end;
    }
    }

    n2_sm_info_type = cJSON_GetObjectItemCaseSensitive(sm_context_updated_dataJSON, "n2SmInfoType");
    if (n2_sm_info_type) {
    if (!cJSON_IsString(n2_sm_info_type)) {
        ogs_error("OpenAPI_sm_context_updated_data_parseFromJSON() failed [n2_sm_info_type]");
        goto end;
    }
    n2_sm_info_typeVariable = OpenAPI_n2_sm_info_type_FromString(n2_sm_info_type->valuestring);
    }

    eps_bearer_setup = cJSON_GetObjectItemCaseSensitive(sm_context_updated_dataJSON, "epsBearerSetup");
    if (eps_bearer_setup) {
        cJSON *eps_bearer_setup_local = NULL;
        if (!cJSON_IsArray(eps_bearer_setup)) {
            ogs_error("OpenAPI_sm_context_updated_data_parseFromJSON() failed [eps_bearer_setup]");
            goto end;
        }

        eps_bearer_setupList = OpenAPI_list_create();

        cJSON_ArrayForEach(eps_bearer_setup_local, eps_bearer_setup) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(eps_bearer_setup_local)) {
                ogs_error("OpenAPI_sm_context_updated_data_parseFromJSON() failed [eps_bearer_setup]");
                goto end;
            }
            OpenAPI_list_add(eps_bearer_setupList, ogs_strdup(eps_bearer_setup_local->valuestring));
        }
    }

    data_forwarding = cJSON_GetObjectItemCaseSensitive(sm_context_updated_dataJSON, "dataForwarding");
    if (data_forwarding) {
    if (!cJSON_IsBool(data_forwarding)) {
        ogs_error("OpenAPI_sm_context_updated_data_parseFromJSON() failed [data_forwarding]");
        goto end;
    }
    }

    n3_dl_forwarding_tnl_list = cJSON_GetObjectItemCaseSensitive(sm_context_updated_dataJSON, "n3DlForwardingTnlList");
    if (n3_dl_forwarding_tnl_list) {
        cJSON *n3_dl_forwarding_tnl_list_local = NULL;
        if (!cJSON_IsArray(n3_dl_forwarding_tnl_list)) {
            ogs_error("OpenAPI_sm_context_updated_data_parseFromJSON() failed [n3_dl_forwarding_tnl_list]");
            goto end;
        }

        n3_dl_forwarding_tnl_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(n3_dl_forwarding_tnl_list_local, n3_dl_forwarding_tnl_list) {
            if (!cJSON_IsObject(n3_dl_forwarding_tnl_list_local)) {
                ogs_error("OpenAPI_sm_context_updated_data_parseFromJSON() failed [n3_dl_forwarding_tnl_list]");
                goto end;
            }
            OpenAPI_indirect_data_forwarding_tunnel_info_t *n3_dl_forwarding_tnl_listItem = OpenAPI_indirect_data_forwarding_tunnel_info_parseFromJSON(n3_dl_forwarding_tnl_list_local);
            if (!n3_dl_forwarding_tnl_listItem) {
                ogs_error("No n3_dl_forwarding_tnl_listItem");
                goto end;
            }
            OpenAPI_list_add(n3_dl_forwarding_tnl_listList, n3_dl_forwarding_tnl_listItem);
        }
    }

    n3_ul_forwarding_tnl_list = cJSON_GetObjectItemCaseSensitive(sm_context_updated_dataJSON, "n3UlForwardingTnlList");
    if (n3_ul_forwarding_tnl_list) {
        cJSON *n3_ul_forwarding_tnl_list_local = NULL;
        if (!cJSON_IsArray(n3_ul_forwarding_tnl_list)) {
            ogs_error("OpenAPI_sm_context_updated_data_parseFromJSON() failed [n3_ul_forwarding_tnl_list]");
            goto end;
        }

        n3_ul_forwarding_tnl_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(n3_ul_forwarding_tnl_list_local, n3_ul_forwarding_tnl_list) {
            if (!cJSON_IsObject(n3_ul_forwarding_tnl_list_local)) {
                ogs_error("OpenAPI_sm_context_updated_data_parseFromJSON() failed [n3_ul_forwarding_tnl_list]");
                goto end;
            }
            OpenAPI_indirect_data_forwarding_tunnel_info_t *n3_ul_forwarding_tnl_listItem = OpenAPI_indirect_data_forwarding_tunnel_info_parseFromJSON(n3_ul_forwarding_tnl_list_local);
            if (!n3_ul_forwarding_tnl_listItem) {
                ogs_error("No n3_ul_forwarding_tnl_listItem");
                goto end;
            }
            OpenAPI_list_add(n3_ul_forwarding_tnl_listList, n3_ul_forwarding_tnl_listItem);
        }
    }

    n9_ul_forwarding_tunnel = cJSON_GetObjectItemCaseSensitive(sm_context_updated_dataJSON, "n9UlForwardingTunnel");
    if (n9_ul_forwarding_tunnel) {
    n9_ul_forwarding_tunnel_local_nonprim = OpenAPI_tunnel_info_parseFromJSON(n9_ul_forwarding_tunnel);
    if (!n9_ul_forwarding_tunnel_local_nonprim) {
        ogs_error("OpenAPI_tunnel_info_parseFromJSON failed [n9_ul_forwarding_tunnel]");
        goto end;
    }
    }

    cause = cJSON_GetObjectItemCaseSensitive(sm_context_updated_dataJSON, "cause");
    if (cause) {
    if (!cJSON_IsString(cause)) {
        ogs_error("OpenAPI_sm_context_updated_data_parseFromJSON() failed [cause]");
        goto end;
    }
    causeVariable = OpenAPI_cause_FromString(cause->valuestring);
    }

    ma_accepted_ind = cJSON_GetObjectItemCaseSensitive(sm_context_updated_dataJSON, "maAcceptedInd");
    if (ma_accepted_ind) {
    if (!cJSON_IsBool(ma_accepted_ind)) {
        ogs_error("OpenAPI_sm_context_updated_data_parseFromJSON() failed [ma_accepted_ind]");
        goto end;
    }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(sm_context_updated_dataJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_sm_context_updated_data_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    forwarding_f_teid = cJSON_GetObjectItemCaseSensitive(sm_context_updated_dataJSON, "forwardingFTeid");
    if (forwarding_f_teid) {
    if (!cJSON_IsString(forwarding_f_teid) && !cJSON_IsNull(forwarding_f_teid)) {
        ogs_error("OpenAPI_sm_context_updated_data_parseFromJSON() failed [forwarding_f_teid]");
        goto end;
    }
    }

    forwarding_bearer_contexts = cJSON_GetObjectItemCaseSensitive(sm_context_updated_dataJSON, "forwardingBearerContexts");
    if (forwarding_bearer_contexts) {
        cJSON *forwarding_bearer_contexts_local = NULL;
        if (!cJSON_IsArray(forwarding_bearer_contexts)) {
            ogs_error("OpenAPI_sm_context_updated_data_parseFromJSON() failed [forwarding_bearer_contexts]");
            goto end;
        }

        forwarding_bearer_contextsList = OpenAPI_list_create();

        cJSON_ArrayForEach(forwarding_bearer_contexts_local, forwarding_bearer_contexts) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(forwarding_bearer_contexts_local)) {
                ogs_error("OpenAPI_sm_context_updated_data_parseFromJSON() failed [forwarding_bearer_contexts]");
                goto end;
            }
            OpenAPI_list_add(forwarding_bearer_contextsList, ogs_strdup(forwarding_bearer_contexts_local->valuestring));
        }
    }

    selected_smf_id = cJSON_GetObjectItemCaseSensitive(sm_context_updated_dataJSON, "selectedSmfId");
    if (selected_smf_id) {
    if (!cJSON_IsString(selected_smf_id) && !cJSON_IsNull(selected_smf_id)) {
        ogs_error("OpenAPI_sm_context_updated_data_parseFromJSON() failed [selected_smf_id]");
        goto end;
    }
    }

    selected_old_smf_id = cJSON_GetObjectItemCaseSensitive(sm_context_updated_dataJSON, "selectedOldSmfId");
    if (selected_old_smf_id) {
    if (!cJSON_IsString(selected_old_smf_id) && !cJSON_IsNull(selected_old_smf_id)) {
        ogs_error("OpenAPI_sm_context_updated_data_parseFromJSON() failed [selected_old_smf_id]");
        goto end;
    }
    }

    inter_plmn_api_root = cJSON_GetObjectItemCaseSensitive(sm_context_updated_dataJSON, "interPlmnApiRoot");
    if (inter_plmn_api_root) {
    if (!cJSON_IsString(inter_plmn_api_root) && !cJSON_IsNull(inter_plmn_api_root)) {
        ogs_error("OpenAPI_sm_context_updated_data_parseFromJSON() failed [inter_plmn_api_root]");
        goto end;
    }
    }

    anchor_smf_features = cJSON_GetObjectItemCaseSensitive(sm_context_updated_dataJSON, "anchorSmfFeatures");
    if (anchor_smf_features) {
    anchor_smf_features_local_nonprim = OpenAPI_anchor_smf_features_parseFromJSON(anchor_smf_features);
    if (!anchor_smf_features_local_nonprim) {
        ogs_error("OpenAPI_anchor_smf_features_parseFromJSON failed [anchor_smf_features]");
        goto end;
    }
    }

    sm_context_updated_data_local_var = OpenAPI_sm_context_updated_data_create (
        up_cnx_state ? up_cnx_stateVariable : 0,
        ho_state ? ho_stateVariable : 0,
        release_ebi_list ? release_ebi_listList : NULL,
        allocated_ebi_list ? allocated_ebi_listList : NULL,
        modified_ebi_list ? modified_ebi_listList : NULL,
        n1_sm_msg ? n1_sm_msg_local_nonprim : NULL,
        n2_sm_info ? n2_sm_info_local_nonprim : NULL,
        n2_sm_info_type ? n2_sm_info_typeVariable : 0,
        eps_bearer_setup ? eps_bearer_setupList : NULL,
        data_forwarding ? true : false,
        data_forwarding ? data_forwarding->valueint : 0,
        n3_dl_forwarding_tnl_list ? n3_dl_forwarding_tnl_listList : NULL,
        n3_ul_forwarding_tnl_list ? n3_ul_forwarding_tnl_listList : NULL,
        n9_ul_forwarding_tunnel ? n9_ul_forwarding_tunnel_local_nonprim : NULL,
        cause ? causeVariable : 0,
        ma_accepted_ind ? true : false,
        ma_accepted_ind ? ma_accepted_ind->valueint : 0,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL,
        forwarding_f_teid && !cJSON_IsNull(forwarding_f_teid) ? ogs_strdup(forwarding_f_teid->valuestring) : NULL,
        forwarding_bearer_contexts ? forwarding_bearer_contextsList : NULL,
        selected_smf_id && !cJSON_IsNull(selected_smf_id) ? ogs_strdup(selected_smf_id->valuestring) : NULL,
        selected_old_smf_id && !cJSON_IsNull(selected_old_smf_id) ? ogs_strdup(selected_old_smf_id->valuestring) : NULL,
        inter_plmn_api_root && !cJSON_IsNull(inter_plmn_api_root) ? ogs_strdup(inter_plmn_api_root->valuestring) : NULL,
        anchor_smf_features ? anchor_smf_features_local_nonprim : NULL
    );

    return sm_context_updated_data_local_var;
end:
    if (release_ebi_listList) {
        OpenAPI_list_for_each(release_ebi_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(release_ebi_listList);
        release_ebi_listList = NULL;
    }
    if (allocated_ebi_listList) {
        OpenAPI_list_for_each(allocated_ebi_listList, node) {
            OpenAPI_ebi_arp_mapping_free(node->data);
        }
        OpenAPI_list_free(allocated_ebi_listList);
        allocated_ebi_listList = NULL;
    }
    if (modified_ebi_listList) {
        OpenAPI_list_for_each(modified_ebi_listList, node) {
            OpenAPI_ebi_arp_mapping_free(node->data);
        }
        OpenAPI_list_free(modified_ebi_listList);
        modified_ebi_listList = NULL;
    }
    if (n1_sm_msg_local_nonprim) {
        OpenAPI_ref_to_binary_data_free(n1_sm_msg_local_nonprim);
        n1_sm_msg_local_nonprim = NULL;
    }
    if (n2_sm_info_local_nonprim) {
        OpenAPI_ref_to_binary_data_free(n2_sm_info_local_nonprim);
        n2_sm_info_local_nonprim = NULL;
    }
    if (eps_bearer_setupList) {
        OpenAPI_list_for_each(eps_bearer_setupList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(eps_bearer_setupList);
        eps_bearer_setupList = NULL;
    }
    if (n3_dl_forwarding_tnl_listList) {
        OpenAPI_list_for_each(n3_dl_forwarding_tnl_listList, node) {
            OpenAPI_indirect_data_forwarding_tunnel_info_free(node->data);
        }
        OpenAPI_list_free(n3_dl_forwarding_tnl_listList);
        n3_dl_forwarding_tnl_listList = NULL;
    }
    if (n3_ul_forwarding_tnl_listList) {
        OpenAPI_list_for_each(n3_ul_forwarding_tnl_listList, node) {
            OpenAPI_indirect_data_forwarding_tunnel_info_free(node->data);
        }
        OpenAPI_list_free(n3_ul_forwarding_tnl_listList);
        n3_ul_forwarding_tnl_listList = NULL;
    }
    if (n9_ul_forwarding_tunnel_local_nonprim) {
        OpenAPI_tunnel_info_free(n9_ul_forwarding_tunnel_local_nonprim);
        n9_ul_forwarding_tunnel_local_nonprim = NULL;
    }
    if (forwarding_bearer_contextsList) {
        OpenAPI_list_for_each(forwarding_bearer_contextsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(forwarding_bearer_contextsList);
        forwarding_bearer_contextsList = NULL;
    }
    if (anchor_smf_features_local_nonprim) {
        OpenAPI_anchor_smf_features_free(anchor_smf_features_local_nonprim);
        anchor_smf_features_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_sm_context_updated_data_t *OpenAPI_sm_context_updated_data_copy(OpenAPI_sm_context_updated_data_t *dst, OpenAPI_sm_context_updated_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sm_context_updated_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sm_context_updated_data_convertToJSON() failed");
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

    OpenAPI_sm_context_updated_data_free(dst);
    dst = OpenAPI_sm_context_updated_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

