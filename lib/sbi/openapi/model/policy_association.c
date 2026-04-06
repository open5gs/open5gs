
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "policy_association.h"

OpenAPI_policy_association_t *OpenAPI_policy_association_create(
    OpenAPI_policy_association_request_t *request,
    OpenAPI_list_t *triggers,
    OpenAPI_service_area_restriction_t *serv_area_res,
    OpenAPI_wireline_service_area_restriction_t *wl_serv_area_res,
    bool is_rfsp,
    int rfsp,
    bool is_rfsp_val_time,
    int rfsp_val_time,
    bool is_target_rfsp,
    int target_rfsp,
    bool is_smf_sel_info_null,
    OpenAPI_smf_selection_data_t *smf_sel_info,
    OpenAPI_ambr_t *ue_ambr,
    OpenAPI_list_t *ue_slice_mbrs,
    OpenAPI_list_t* pras,
    bool is_pcf_ue_info_null,
    OpenAPI_pcf_ue_callback_info_t *pcf_ue_info,
    bool is_match_pdus_null,
    OpenAPI_list_t *match_pdus,
    bool is_as_time_dis_param_null,
    OpenAPI_as_time_distribution_param_t *as_time_dis_param,
    OpenAPI_list_t* slice_usg_ctrl_info_sets,
    OpenAPI_charging_information_t *chf_info,
    char *chf_group_id,
    OpenAPI_slice_repl_req_t *slice_repl_req,
    char *supp_feat
)
{
    OpenAPI_policy_association_t *policy_association_local_var = ogs_malloc(sizeof(OpenAPI_policy_association_t));
    ogs_assert(policy_association_local_var);

    policy_association_local_var->request = request;
    policy_association_local_var->triggers = triggers;
    policy_association_local_var->serv_area_res = serv_area_res;
    policy_association_local_var->wl_serv_area_res = wl_serv_area_res;
    policy_association_local_var->is_rfsp = is_rfsp;
    policy_association_local_var->rfsp = rfsp;
    policy_association_local_var->is_rfsp_val_time = is_rfsp_val_time;
    policy_association_local_var->rfsp_val_time = rfsp_val_time;
    policy_association_local_var->is_target_rfsp = is_target_rfsp;
    policy_association_local_var->target_rfsp = target_rfsp;
    policy_association_local_var->is_smf_sel_info_null = is_smf_sel_info_null;
    policy_association_local_var->smf_sel_info = smf_sel_info;
    policy_association_local_var->ue_ambr = ue_ambr;
    policy_association_local_var->ue_slice_mbrs = ue_slice_mbrs;
    policy_association_local_var->pras = pras;
    policy_association_local_var->is_pcf_ue_info_null = is_pcf_ue_info_null;
    policy_association_local_var->pcf_ue_info = pcf_ue_info;
    policy_association_local_var->is_match_pdus_null = is_match_pdus_null;
    policy_association_local_var->match_pdus = match_pdus;
    policy_association_local_var->is_as_time_dis_param_null = is_as_time_dis_param_null;
    policy_association_local_var->as_time_dis_param = as_time_dis_param;
    policy_association_local_var->slice_usg_ctrl_info_sets = slice_usg_ctrl_info_sets;
    policy_association_local_var->chf_info = chf_info;
    policy_association_local_var->chf_group_id = chf_group_id;
    policy_association_local_var->slice_repl_req = slice_repl_req;
    policy_association_local_var->supp_feat = supp_feat;

    return policy_association_local_var;
}

void OpenAPI_policy_association_free(OpenAPI_policy_association_t *policy_association)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == policy_association) {
        return;
    }
    if (policy_association->request) {
        OpenAPI_policy_association_request_free(policy_association->request);
        policy_association->request = NULL;
    }
    if (policy_association->triggers) {
        OpenAPI_list_free(policy_association->triggers);
        policy_association->triggers = NULL;
    }
    if (policy_association->serv_area_res) {
        OpenAPI_service_area_restriction_free(policy_association->serv_area_res);
        policy_association->serv_area_res = NULL;
    }
    if (policy_association->wl_serv_area_res) {
        OpenAPI_wireline_service_area_restriction_free(policy_association->wl_serv_area_res);
        policy_association->wl_serv_area_res = NULL;
    }
    if (policy_association->smf_sel_info) {
        OpenAPI_smf_selection_data_free(policy_association->smf_sel_info);
        policy_association->smf_sel_info = NULL;
    }
    if (policy_association->ue_ambr) {
        OpenAPI_ambr_free(policy_association->ue_ambr);
        policy_association->ue_ambr = NULL;
    }
    if (policy_association->ue_slice_mbrs) {
        OpenAPI_list_for_each(policy_association->ue_slice_mbrs, node) {
            OpenAPI_ue_slice_mbr_free(node->data);
        }
        OpenAPI_list_free(policy_association->ue_slice_mbrs);
        policy_association->ue_slice_mbrs = NULL;
    }
    if (policy_association->pras) {
        OpenAPI_list_for_each(policy_association->pras, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_presence_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(policy_association->pras);
        policy_association->pras = NULL;
    }
    if (policy_association->pcf_ue_info) {
        OpenAPI_pcf_ue_callback_info_free(policy_association->pcf_ue_info);
        policy_association->pcf_ue_info = NULL;
    }
    if (policy_association->match_pdus) {
        OpenAPI_list_for_each(policy_association->match_pdus, node) {
            OpenAPI_pdu_session_info_free(node->data);
        }
        OpenAPI_list_free(policy_association->match_pdus);
        policy_association->match_pdus = NULL;
    }
    if (policy_association->as_time_dis_param) {
        OpenAPI_as_time_distribution_param_free(policy_association->as_time_dis_param);
        policy_association->as_time_dis_param = NULL;
    }
    if (policy_association->slice_usg_ctrl_info_sets) {
        OpenAPI_list_for_each(policy_association->slice_usg_ctrl_info_sets, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_slice_usg_ctrl_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(policy_association->slice_usg_ctrl_info_sets);
        policy_association->slice_usg_ctrl_info_sets = NULL;
    }
    if (policy_association->chf_info) {
        OpenAPI_charging_information_free(policy_association->chf_info);
        policy_association->chf_info = NULL;
    }
    if (policy_association->chf_group_id) {
        ogs_free(policy_association->chf_group_id);
        policy_association->chf_group_id = NULL;
    }
    if (policy_association->slice_repl_req) {
        OpenAPI_slice_repl_req_free(policy_association->slice_repl_req);
        policy_association->slice_repl_req = NULL;
    }
    if (policy_association->supp_feat) {
        ogs_free(policy_association->supp_feat);
        policy_association->supp_feat = NULL;
    }
    ogs_free(policy_association);
}

cJSON *OpenAPI_policy_association_convertToJSON(OpenAPI_policy_association_t *policy_association)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (policy_association == NULL) {
        ogs_error("OpenAPI_policy_association_convertToJSON() failed [PolicyAssociation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (policy_association->request) {
    cJSON *request_local_JSON = OpenAPI_policy_association_request_convertToJSON(policy_association->request);
    if (request_local_JSON == NULL) {
        ogs_error("OpenAPI_policy_association_convertToJSON() failed [request]");
        goto end;
    }
    cJSON_AddItemToObject(item, "request", request_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_policy_association_convertToJSON() failed [request]");
        goto end;
    }
    }

    if (policy_association->triggers != OpenAPI_request_trigger_NULL) {
    cJSON *triggersList = cJSON_AddArrayToObject(item, "triggers");
    if (triggersList == NULL) {
        ogs_error("OpenAPI_policy_association_convertToJSON() failed [triggers]");
        goto end;
    }
    OpenAPI_list_for_each(policy_association->triggers, node) {
        if (cJSON_AddStringToObject(triggersList, "", OpenAPI_request_trigger_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_policy_association_convertToJSON() failed [triggers]");
            goto end;
        }
    }
    }

    if (policy_association->serv_area_res) {
    cJSON *serv_area_res_local_JSON = OpenAPI_service_area_restriction_convertToJSON(policy_association->serv_area_res);
    if (serv_area_res_local_JSON == NULL) {
        ogs_error("OpenAPI_policy_association_convertToJSON() failed [serv_area_res]");
        goto end;
    }
    cJSON_AddItemToObject(item, "servAreaRes", serv_area_res_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_policy_association_convertToJSON() failed [serv_area_res]");
        goto end;
    }
    }

    if (policy_association->wl_serv_area_res) {
    cJSON *wl_serv_area_res_local_JSON = OpenAPI_wireline_service_area_restriction_convertToJSON(policy_association->wl_serv_area_res);
    if (wl_serv_area_res_local_JSON == NULL) {
        ogs_error("OpenAPI_policy_association_convertToJSON() failed [wl_serv_area_res]");
        goto end;
    }
    cJSON_AddItemToObject(item, "wlServAreaRes", wl_serv_area_res_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_policy_association_convertToJSON() failed [wl_serv_area_res]");
        goto end;
    }
    }

    if (policy_association->is_rfsp) {
    if (cJSON_AddNumberToObject(item, "rfsp", policy_association->rfsp) == NULL) {
        ogs_error("OpenAPI_policy_association_convertToJSON() failed [rfsp]");
        goto end;
    }
    }

    if (policy_association->is_rfsp_val_time) {
    if (cJSON_AddNumberToObject(item, "rfspValTime", policy_association->rfsp_val_time) == NULL) {
        ogs_error("OpenAPI_policy_association_convertToJSON() failed [rfsp_val_time]");
        goto end;
    }
    }

    if (policy_association->is_target_rfsp) {
    if (cJSON_AddNumberToObject(item, "targetRfsp", policy_association->target_rfsp) == NULL) {
        ogs_error("OpenAPI_policy_association_convertToJSON() failed [target_rfsp]");
        goto end;
    }
    }

    if (policy_association->smf_sel_info) {
    cJSON *smf_sel_info_local_JSON = OpenAPI_smf_selection_data_convertToJSON(policy_association->smf_sel_info);
    if (smf_sel_info_local_JSON == NULL) {
        ogs_error("OpenAPI_policy_association_convertToJSON() failed [smf_sel_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "smfSelInfo", smf_sel_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_policy_association_convertToJSON() failed [smf_sel_info]");
        goto end;
    }
    } else if (policy_association->is_smf_sel_info_null) {
        if (cJSON_AddNullToObject(item, "smfSelInfo") == NULL) {
            ogs_error("OpenAPI_policy_association_convertToJSON() failed [smf_sel_info]");
            goto end;
        }
    }

    if (policy_association->ue_ambr) {
    cJSON *ue_ambr_local_JSON = OpenAPI_ambr_convertToJSON(policy_association->ue_ambr);
    if (ue_ambr_local_JSON == NULL) {
        ogs_error("OpenAPI_policy_association_convertToJSON() failed [ue_ambr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ueAmbr", ue_ambr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_policy_association_convertToJSON() failed [ue_ambr]");
        goto end;
    }
    }

    if (policy_association->ue_slice_mbrs) {
    cJSON *ue_slice_mbrsList = cJSON_AddArrayToObject(item, "ueSliceMbrs");
    if (ue_slice_mbrsList == NULL) {
        ogs_error("OpenAPI_policy_association_convertToJSON() failed [ue_slice_mbrs]");
        goto end;
    }
    OpenAPI_list_for_each(policy_association->ue_slice_mbrs, node) {
        cJSON *itemLocal = OpenAPI_ue_slice_mbr_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_policy_association_convertToJSON() failed [ue_slice_mbrs]");
            goto end;
        }
        cJSON_AddItemToArray(ue_slice_mbrsList, itemLocal);
    }
    }

    if (policy_association->pras) {
    cJSON *pras = cJSON_AddObjectToObject(item, "pras");
    if (pras == NULL) {
        ogs_error("OpenAPI_policy_association_convertToJSON() failed [pras]");
        goto end;
    }
    cJSON *localMapObject = pras;
    if (policy_association->pras) {
        OpenAPI_list_for_each(policy_association->pras, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_policy_association_convertToJSON() failed [pras]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_policy_association_convertToJSON() failed [pras]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_presence_info_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_policy_association_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (policy_association->pcf_ue_info) {
    cJSON *pcf_ue_info_local_JSON = OpenAPI_pcf_ue_callback_info_convertToJSON(policy_association->pcf_ue_info);
    if (pcf_ue_info_local_JSON == NULL) {
        ogs_error("OpenAPI_policy_association_convertToJSON() failed [pcf_ue_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pcfUeInfo", pcf_ue_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_policy_association_convertToJSON() failed [pcf_ue_info]");
        goto end;
    }
    } else if (policy_association->is_pcf_ue_info_null) {
        if (cJSON_AddNullToObject(item, "pcfUeInfo") == NULL) {
            ogs_error("OpenAPI_policy_association_convertToJSON() failed [pcf_ue_info]");
            goto end;
        }
    }

    if (policy_association->match_pdus) {
    cJSON *match_pdusList = cJSON_AddArrayToObject(item, "matchPdus");
    if (match_pdusList == NULL) {
        ogs_error("OpenAPI_policy_association_convertToJSON() failed [match_pdus]");
        goto end;
    }
    OpenAPI_list_for_each(policy_association->match_pdus, node) {
        cJSON *itemLocal = OpenAPI_pdu_session_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_policy_association_convertToJSON() failed [match_pdus]");
            goto end;
        }
        cJSON_AddItemToArray(match_pdusList, itemLocal);
    }
    } else if (policy_association->is_match_pdus_null) {
        if (cJSON_AddNullToObject(item, "matchPdus") == NULL) {
            ogs_error("OpenAPI_policy_association_convertToJSON() failed [match_pdus]");
            goto end;
        }
    }

    if (policy_association->as_time_dis_param) {
    cJSON *as_time_dis_param_local_JSON = OpenAPI_as_time_distribution_param_convertToJSON(policy_association->as_time_dis_param);
    if (as_time_dis_param_local_JSON == NULL) {
        ogs_error("OpenAPI_policy_association_convertToJSON() failed [as_time_dis_param]");
        goto end;
    }
    cJSON_AddItemToObject(item, "asTimeDisParam", as_time_dis_param_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_policy_association_convertToJSON() failed [as_time_dis_param]");
        goto end;
    }
    } else if (policy_association->is_as_time_dis_param_null) {
        if (cJSON_AddNullToObject(item, "asTimeDisParam") == NULL) {
            ogs_error("OpenAPI_policy_association_convertToJSON() failed [as_time_dis_param]");
            goto end;
        }
    }

    if (policy_association->slice_usg_ctrl_info_sets) {
    cJSON *slice_usg_ctrl_info_sets = cJSON_AddObjectToObject(item, "sliceUsgCtrlInfoSets");
    if (slice_usg_ctrl_info_sets == NULL) {
        ogs_error("OpenAPI_policy_association_convertToJSON() failed [slice_usg_ctrl_info_sets]");
        goto end;
    }
    cJSON *localMapObject = slice_usg_ctrl_info_sets;
    if (policy_association->slice_usg_ctrl_info_sets) {
        OpenAPI_list_for_each(policy_association->slice_usg_ctrl_info_sets, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_policy_association_convertToJSON() failed [slice_usg_ctrl_info_sets]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_policy_association_convertToJSON() failed [slice_usg_ctrl_info_sets]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_slice_usg_ctrl_info_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_policy_association_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (policy_association->chf_info) {
    cJSON *chf_info_local_JSON = OpenAPI_charging_information_convertToJSON(policy_association->chf_info);
    if (chf_info_local_JSON == NULL) {
        ogs_error("OpenAPI_policy_association_convertToJSON() failed [chf_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "chfInfo", chf_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_policy_association_convertToJSON() failed [chf_info]");
        goto end;
    }
    }

    if (policy_association->chf_group_id) {
    if (cJSON_AddStringToObject(item, "chfGroupId", policy_association->chf_group_id) == NULL) {
        ogs_error("OpenAPI_policy_association_convertToJSON() failed [chf_group_id]");
        goto end;
    }
    }

    if (policy_association->slice_repl_req) {
    cJSON *slice_repl_req_local_JSON = OpenAPI_slice_repl_req_convertToJSON(policy_association->slice_repl_req);
    if (slice_repl_req_local_JSON == NULL) {
        ogs_error("OpenAPI_policy_association_convertToJSON() failed [slice_repl_req]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sliceReplReq", slice_repl_req_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_policy_association_convertToJSON() failed [slice_repl_req]");
        goto end;
    }
    }

    if (!policy_association->supp_feat) {
        ogs_error("OpenAPI_policy_association_convertToJSON() failed [supp_feat]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "suppFeat", policy_association->supp_feat) == NULL) {
        ogs_error("OpenAPI_policy_association_convertToJSON() failed [supp_feat]");
        goto end;
    }

end:
    return item;
}

OpenAPI_policy_association_t *OpenAPI_policy_association_parseFromJSON(cJSON *policy_associationJSON)
{
    OpenAPI_policy_association_t *policy_association_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *request = NULL;
    OpenAPI_policy_association_request_t *request_local_nonprim = NULL;
    cJSON *triggers = NULL;
    OpenAPI_list_t *triggersList = NULL;
    cJSON *serv_area_res = NULL;
    OpenAPI_service_area_restriction_t *serv_area_res_local_nonprim = NULL;
    cJSON *wl_serv_area_res = NULL;
    OpenAPI_wireline_service_area_restriction_t *wl_serv_area_res_local_nonprim = NULL;
    cJSON *rfsp = NULL;
    cJSON *rfsp_val_time = NULL;
    cJSON *target_rfsp = NULL;
    cJSON *smf_sel_info = NULL;
    OpenAPI_smf_selection_data_t *smf_sel_info_local_nonprim = NULL;
    cJSON *ue_ambr = NULL;
    OpenAPI_ambr_t *ue_ambr_local_nonprim = NULL;
    cJSON *ue_slice_mbrs = NULL;
    OpenAPI_list_t *ue_slice_mbrsList = NULL;
    cJSON *pras = NULL;
    OpenAPI_list_t *prasList = NULL;
    cJSON *pcf_ue_info = NULL;
    OpenAPI_pcf_ue_callback_info_t *pcf_ue_info_local_nonprim = NULL;
    cJSON *match_pdus = NULL;
    OpenAPI_list_t *match_pdusList = NULL;
    cJSON *as_time_dis_param = NULL;
    OpenAPI_as_time_distribution_param_t *as_time_dis_param_local_nonprim = NULL;
    cJSON *slice_usg_ctrl_info_sets = NULL;
    OpenAPI_list_t *slice_usg_ctrl_info_setsList = NULL;
    cJSON *chf_info = NULL;
    OpenAPI_charging_information_t *chf_info_local_nonprim = NULL;
    cJSON *chf_group_id = NULL;
    cJSON *slice_repl_req = NULL;
    OpenAPI_slice_repl_req_t *slice_repl_req_local_nonprim = NULL;
    cJSON *supp_feat = NULL;
    request = cJSON_GetObjectItemCaseSensitive(policy_associationJSON, "request");
    if (request) {
    request_local_nonprim = OpenAPI_policy_association_request_parseFromJSON(request);
    if (!request_local_nonprim) {
        ogs_error("OpenAPI_policy_association_request_parseFromJSON failed [request]");
        goto end;
    }
    }

    triggers = cJSON_GetObjectItemCaseSensitive(policy_associationJSON, "triggers");
    if (triggers) {
        cJSON *triggers_local = NULL;
        if (!cJSON_IsArray(triggers)) {
            ogs_error("OpenAPI_policy_association_parseFromJSON() failed [triggers]");
            goto end;
        }

        triggersList = OpenAPI_list_create();

        cJSON_ArrayForEach(triggers_local, triggers) {
            OpenAPI_request_trigger_e localEnum = OpenAPI_request_trigger_NULL;
            if (!cJSON_IsString(triggers_local)) {
                ogs_error("OpenAPI_policy_association_parseFromJSON() failed [triggers]");
                goto end;
            }
            localEnum = OpenAPI_request_trigger_FromString(triggers_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"triggers\" is not supported. Ignoring it ...",
                         triggers_local->valuestring);
            } else {
                OpenAPI_list_add(triggersList, (void *)localEnum);
            }
        }
        if (triggersList->count == 0) {
            ogs_error("OpenAPI_policy_association_parseFromJSON() failed: Expected triggersList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    serv_area_res = cJSON_GetObjectItemCaseSensitive(policy_associationJSON, "servAreaRes");
    if (serv_area_res) {
    serv_area_res_local_nonprim = OpenAPI_service_area_restriction_parseFromJSON(serv_area_res);
    if (!serv_area_res_local_nonprim) {
        ogs_error("OpenAPI_service_area_restriction_parseFromJSON failed [serv_area_res]");
        goto end;
    }
    }

    wl_serv_area_res = cJSON_GetObjectItemCaseSensitive(policy_associationJSON, "wlServAreaRes");
    if (wl_serv_area_res) {
    wl_serv_area_res_local_nonprim = OpenAPI_wireline_service_area_restriction_parseFromJSON(wl_serv_area_res);
    if (!wl_serv_area_res_local_nonprim) {
        ogs_error("OpenAPI_wireline_service_area_restriction_parseFromJSON failed [wl_serv_area_res]");
        goto end;
    }
    }

    rfsp = cJSON_GetObjectItemCaseSensitive(policy_associationJSON, "rfsp");
    if (rfsp) {
    if (!cJSON_IsNumber(rfsp)) {
        ogs_error("OpenAPI_policy_association_parseFromJSON() failed [rfsp]");
        goto end;
    }
    }

    rfsp_val_time = cJSON_GetObjectItemCaseSensitive(policy_associationJSON, "rfspValTime");
    if (rfsp_val_time) {
    if (!cJSON_IsNumber(rfsp_val_time)) {
        ogs_error("OpenAPI_policy_association_parseFromJSON() failed [rfsp_val_time]");
        goto end;
    }
    }

    target_rfsp = cJSON_GetObjectItemCaseSensitive(policy_associationJSON, "targetRfsp");
    if (target_rfsp) {
    if (!cJSON_IsNumber(target_rfsp)) {
        ogs_error("OpenAPI_policy_association_parseFromJSON() failed [target_rfsp]");
        goto end;
    }
    }

    smf_sel_info = cJSON_GetObjectItemCaseSensitive(policy_associationJSON, "smfSelInfo");
    if (smf_sel_info) {
    if (!cJSON_IsNull(smf_sel_info)) {
    smf_sel_info_local_nonprim = OpenAPI_smf_selection_data_parseFromJSON(smf_sel_info);
    if (!smf_sel_info_local_nonprim) {
        ogs_error("OpenAPI_smf_selection_data_parseFromJSON failed [smf_sel_info]");
        goto end;
    }
    }
    }

    ue_ambr = cJSON_GetObjectItemCaseSensitive(policy_associationJSON, "ueAmbr");
    if (ue_ambr) {
    ue_ambr_local_nonprim = OpenAPI_ambr_parseFromJSON(ue_ambr);
    if (!ue_ambr_local_nonprim) {
        ogs_error("OpenAPI_ambr_parseFromJSON failed [ue_ambr]");
        goto end;
    }
    }

    ue_slice_mbrs = cJSON_GetObjectItemCaseSensitive(policy_associationJSON, "ueSliceMbrs");
    if (ue_slice_mbrs) {
        cJSON *ue_slice_mbrs_local = NULL;
        if (!cJSON_IsArray(ue_slice_mbrs)) {
            ogs_error("OpenAPI_policy_association_parseFromJSON() failed [ue_slice_mbrs]");
            goto end;
        }

        ue_slice_mbrsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ue_slice_mbrs_local, ue_slice_mbrs) {
            if (!cJSON_IsObject(ue_slice_mbrs_local)) {
                ogs_error("OpenAPI_policy_association_parseFromJSON() failed [ue_slice_mbrs]");
                goto end;
            }
            OpenAPI_ue_slice_mbr_t *ue_slice_mbrsItem = OpenAPI_ue_slice_mbr_parseFromJSON(ue_slice_mbrs_local);
            if (!ue_slice_mbrsItem) {
                ogs_error("No ue_slice_mbrsItem");
                goto end;
            }
            OpenAPI_list_add(ue_slice_mbrsList, ue_slice_mbrsItem);
        }
    }

    pras = cJSON_GetObjectItemCaseSensitive(policy_associationJSON, "pras");
    if (pras) {
        cJSON *pras_local_map = NULL;
        if (!cJSON_IsObject(pras) && !cJSON_IsNull(pras)) {
            ogs_error("OpenAPI_policy_association_parseFromJSON() failed [pras]");
            goto end;
        }
        if (cJSON_IsObject(pras)) {
            prasList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(pras_local_map, pras) {
                cJSON *localMapObject = pras_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_presence_info_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_policy_association_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(prasList, localMapKeyPair);
            }
        }
    }

    pcf_ue_info = cJSON_GetObjectItemCaseSensitive(policy_associationJSON, "pcfUeInfo");
    if (pcf_ue_info) {
    if (!cJSON_IsNull(pcf_ue_info)) {
    pcf_ue_info_local_nonprim = OpenAPI_pcf_ue_callback_info_parseFromJSON(pcf_ue_info);
    if (!pcf_ue_info_local_nonprim) {
        ogs_error("OpenAPI_pcf_ue_callback_info_parseFromJSON failed [pcf_ue_info]");
        goto end;
    }
    }
    }

    match_pdus = cJSON_GetObjectItemCaseSensitive(policy_associationJSON, "matchPdus");
    if (match_pdus) {
    if (!cJSON_IsNull(match_pdus)) {
        cJSON *match_pdus_local = NULL;
        if (!cJSON_IsArray(match_pdus)) {
            ogs_error("OpenAPI_policy_association_parseFromJSON() failed [match_pdus]");
            goto end;
        }

        match_pdusList = OpenAPI_list_create();

        cJSON_ArrayForEach(match_pdus_local, match_pdus) {
            if (!cJSON_IsObject(match_pdus_local)) {
                ogs_error("OpenAPI_policy_association_parseFromJSON() failed [match_pdus]");
                goto end;
            }
            OpenAPI_pdu_session_info_t *match_pdusItem = OpenAPI_pdu_session_info_parseFromJSON(match_pdus_local);
            if (!match_pdusItem) {
                ogs_error("No match_pdusItem");
                goto end;
            }
            OpenAPI_list_add(match_pdusList, match_pdusItem);
        }
    }
    }

    as_time_dis_param = cJSON_GetObjectItemCaseSensitive(policy_associationJSON, "asTimeDisParam");
    if (as_time_dis_param) {
    if (!cJSON_IsNull(as_time_dis_param)) {
    as_time_dis_param_local_nonprim = OpenAPI_as_time_distribution_param_parseFromJSON(as_time_dis_param);
    if (!as_time_dis_param_local_nonprim) {
        ogs_error("OpenAPI_as_time_distribution_param_parseFromJSON failed [as_time_dis_param]");
        goto end;
    }
    }
    }

    slice_usg_ctrl_info_sets = cJSON_GetObjectItemCaseSensitive(policy_associationJSON, "sliceUsgCtrlInfoSets");
    if (slice_usg_ctrl_info_sets) {
        cJSON *slice_usg_ctrl_info_sets_local_map = NULL;
        if (!cJSON_IsObject(slice_usg_ctrl_info_sets) && !cJSON_IsNull(slice_usg_ctrl_info_sets)) {
            ogs_error("OpenAPI_policy_association_parseFromJSON() failed [slice_usg_ctrl_info_sets]");
            goto end;
        }
        if (cJSON_IsObject(slice_usg_ctrl_info_sets)) {
            slice_usg_ctrl_info_setsList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(slice_usg_ctrl_info_sets_local_map, slice_usg_ctrl_info_sets) {
                cJSON *localMapObject = slice_usg_ctrl_info_sets_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_slice_usg_ctrl_info_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_policy_association_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(slice_usg_ctrl_info_setsList, localMapKeyPair);
            }
        }
    }

    chf_info = cJSON_GetObjectItemCaseSensitive(policy_associationJSON, "chfInfo");
    if (chf_info) {
    chf_info_local_nonprim = OpenAPI_charging_information_parseFromJSON(chf_info);
    if (!chf_info_local_nonprim) {
        ogs_error("OpenAPI_charging_information_parseFromJSON failed [chf_info]");
        goto end;
    }
    }

    chf_group_id = cJSON_GetObjectItemCaseSensitive(policy_associationJSON, "chfGroupId");
    if (chf_group_id) {
    if (!cJSON_IsString(chf_group_id) && !cJSON_IsNull(chf_group_id)) {
        ogs_error("OpenAPI_policy_association_parseFromJSON() failed [chf_group_id]");
        goto end;
    }
    }

    slice_repl_req = cJSON_GetObjectItemCaseSensitive(policy_associationJSON, "sliceReplReq");
    if (slice_repl_req) {
    slice_repl_req_local_nonprim = OpenAPI_slice_repl_req_parseFromJSON(slice_repl_req);
    if (!slice_repl_req_local_nonprim) {
        ogs_error("OpenAPI_slice_repl_req_parseFromJSON failed [slice_repl_req]");
        goto end;
    }
    }

    supp_feat = cJSON_GetObjectItemCaseSensitive(policy_associationJSON, "suppFeat");
    if (!supp_feat) {
        ogs_error("OpenAPI_policy_association_parseFromJSON() failed [supp_feat]");
        goto end;
    }
    if (!cJSON_IsString(supp_feat)) {
        ogs_error("OpenAPI_policy_association_parseFromJSON() failed [supp_feat]");
        goto end;
    }

    policy_association_local_var = OpenAPI_policy_association_create (
        request ? request_local_nonprim : NULL,
        triggers ? triggersList : NULL,
        serv_area_res ? serv_area_res_local_nonprim : NULL,
        wl_serv_area_res ? wl_serv_area_res_local_nonprim : NULL,
        rfsp ? true : false,
        rfsp ? rfsp->valuedouble : 0,
        rfsp_val_time ? true : false,
        rfsp_val_time ? rfsp_val_time->valuedouble : 0,
        target_rfsp ? true : false,
        target_rfsp ? target_rfsp->valuedouble : 0,
        smf_sel_info && cJSON_IsNull(smf_sel_info) ? true : false,
        smf_sel_info ? smf_sel_info_local_nonprim : NULL,
        ue_ambr ? ue_ambr_local_nonprim : NULL,
        ue_slice_mbrs ? ue_slice_mbrsList : NULL,
        pras ? prasList : NULL,
        pcf_ue_info && cJSON_IsNull(pcf_ue_info) ? true : false,
        pcf_ue_info ? pcf_ue_info_local_nonprim : NULL,
        match_pdus && cJSON_IsNull(match_pdus) ? true : false,
        match_pdus ? match_pdusList : NULL,
        as_time_dis_param && cJSON_IsNull(as_time_dis_param) ? true : false,
        as_time_dis_param ? as_time_dis_param_local_nonprim : NULL,
        slice_usg_ctrl_info_sets ? slice_usg_ctrl_info_setsList : NULL,
        chf_info ? chf_info_local_nonprim : NULL,
        chf_group_id && !cJSON_IsNull(chf_group_id) ? ogs_strdup(chf_group_id->valuestring) : NULL,
        slice_repl_req ? slice_repl_req_local_nonprim : NULL,
        ogs_strdup(supp_feat->valuestring)
    );

    return policy_association_local_var;
end:
    if (request_local_nonprim) {
        OpenAPI_policy_association_request_free(request_local_nonprim);
        request_local_nonprim = NULL;
    }
    if (triggersList) {
        OpenAPI_list_free(triggersList);
        triggersList = NULL;
    }
    if (serv_area_res_local_nonprim) {
        OpenAPI_service_area_restriction_free(serv_area_res_local_nonprim);
        serv_area_res_local_nonprim = NULL;
    }
    if (wl_serv_area_res_local_nonprim) {
        OpenAPI_wireline_service_area_restriction_free(wl_serv_area_res_local_nonprim);
        wl_serv_area_res_local_nonprim = NULL;
    }
    if (smf_sel_info_local_nonprim) {
        OpenAPI_smf_selection_data_free(smf_sel_info_local_nonprim);
        smf_sel_info_local_nonprim = NULL;
    }
    if (ue_ambr_local_nonprim) {
        OpenAPI_ambr_free(ue_ambr_local_nonprim);
        ue_ambr_local_nonprim = NULL;
    }
    if (ue_slice_mbrsList) {
        OpenAPI_list_for_each(ue_slice_mbrsList, node) {
            OpenAPI_ue_slice_mbr_free(node->data);
        }
        OpenAPI_list_free(ue_slice_mbrsList);
        ue_slice_mbrsList = NULL;
    }
    if (prasList) {
        OpenAPI_list_for_each(prasList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_presence_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(prasList);
        prasList = NULL;
    }
    if (pcf_ue_info_local_nonprim) {
        OpenAPI_pcf_ue_callback_info_free(pcf_ue_info_local_nonprim);
        pcf_ue_info_local_nonprim = NULL;
    }
    if (match_pdusList) {
        OpenAPI_list_for_each(match_pdusList, node) {
            OpenAPI_pdu_session_info_free(node->data);
        }
        OpenAPI_list_free(match_pdusList);
        match_pdusList = NULL;
    }
    if (as_time_dis_param_local_nonprim) {
        OpenAPI_as_time_distribution_param_free(as_time_dis_param_local_nonprim);
        as_time_dis_param_local_nonprim = NULL;
    }
    if (slice_usg_ctrl_info_setsList) {
        OpenAPI_list_for_each(slice_usg_ctrl_info_setsList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_slice_usg_ctrl_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(slice_usg_ctrl_info_setsList);
        slice_usg_ctrl_info_setsList = NULL;
    }
    if (chf_info_local_nonprim) {
        OpenAPI_charging_information_free(chf_info_local_nonprim);
        chf_info_local_nonprim = NULL;
    }
    if (slice_repl_req_local_nonprim) {
        OpenAPI_slice_repl_req_free(slice_repl_req_local_nonprim);
        slice_repl_req_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_policy_association_t *OpenAPI_policy_association_copy(OpenAPI_policy_association_t *dst, OpenAPI_policy_association_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_policy_association_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_policy_association_convertToJSON() failed");
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

    OpenAPI_policy_association_free(dst);
    dst = OpenAPI_policy_association_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

