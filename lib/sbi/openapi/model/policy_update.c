
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "policy_update.h"

OpenAPI_policy_update_t *OpenAPI_policy_update_create(
    char *resource_uri,
    bool is_triggers_null,
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
    bool is_pras_null,
    OpenAPI_list_t* pras,
    bool is_pcf_ue_info_null,
    OpenAPI_pcf_ue_callback_info_t *pcf_ue_info,
    bool is_match_pdus_null,
    OpenAPI_list_t *match_pdus,
    bool is_as_time_dis_param_null,
    OpenAPI_as_time_distribution_param_t *as_time_dis_param,
    bool is_snssai_repl_infos_null,
    OpenAPI_list_t* snssai_repl_infos,
    OpenAPI_list_t* slice_usg_ctrl_info_sets,
    OpenAPI_slice_repl_req_t *slice_repl_req,
    char *supp_feat
)
{
    OpenAPI_policy_update_t *policy_update_local_var = ogs_malloc(sizeof(OpenAPI_policy_update_t));
    ogs_assert(policy_update_local_var);

    policy_update_local_var->resource_uri = resource_uri;
    policy_update_local_var->is_triggers_null = is_triggers_null;
    policy_update_local_var->triggers = triggers;
    policy_update_local_var->serv_area_res = serv_area_res;
    policy_update_local_var->wl_serv_area_res = wl_serv_area_res;
    policy_update_local_var->is_rfsp = is_rfsp;
    policy_update_local_var->rfsp = rfsp;
    policy_update_local_var->is_rfsp_val_time = is_rfsp_val_time;
    policy_update_local_var->rfsp_val_time = rfsp_val_time;
    policy_update_local_var->is_target_rfsp = is_target_rfsp;
    policy_update_local_var->target_rfsp = target_rfsp;
    policy_update_local_var->is_smf_sel_info_null = is_smf_sel_info_null;
    policy_update_local_var->smf_sel_info = smf_sel_info;
    policy_update_local_var->ue_ambr = ue_ambr;
    policy_update_local_var->ue_slice_mbrs = ue_slice_mbrs;
    policy_update_local_var->is_pras_null = is_pras_null;
    policy_update_local_var->pras = pras;
    policy_update_local_var->is_pcf_ue_info_null = is_pcf_ue_info_null;
    policy_update_local_var->pcf_ue_info = pcf_ue_info;
    policy_update_local_var->is_match_pdus_null = is_match_pdus_null;
    policy_update_local_var->match_pdus = match_pdus;
    policy_update_local_var->is_as_time_dis_param_null = is_as_time_dis_param_null;
    policy_update_local_var->as_time_dis_param = as_time_dis_param;
    policy_update_local_var->is_snssai_repl_infos_null = is_snssai_repl_infos_null;
    policy_update_local_var->snssai_repl_infos = snssai_repl_infos;
    policy_update_local_var->slice_usg_ctrl_info_sets = slice_usg_ctrl_info_sets;
    policy_update_local_var->slice_repl_req = slice_repl_req;
    policy_update_local_var->supp_feat = supp_feat;

    return policy_update_local_var;
}

void OpenAPI_policy_update_free(OpenAPI_policy_update_t *policy_update)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == policy_update) {
        return;
    }
    if (policy_update->resource_uri) {
        ogs_free(policy_update->resource_uri);
        policy_update->resource_uri = NULL;
    }
    if (policy_update->triggers) {
        OpenAPI_list_free(policy_update->triggers);
        policy_update->triggers = NULL;
    }
    if (policy_update->serv_area_res) {
        OpenAPI_service_area_restriction_free(policy_update->serv_area_res);
        policy_update->serv_area_res = NULL;
    }
    if (policy_update->wl_serv_area_res) {
        OpenAPI_wireline_service_area_restriction_free(policy_update->wl_serv_area_res);
        policy_update->wl_serv_area_res = NULL;
    }
    if (policy_update->smf_sel_info) {
        OpenAPI_smf_selection_data_free(policy_update->smf_sel_info);
        policy_update->smf_sel_info = NULL;
    }
    if (policy_update->ue_ambr) {
        OpenAPI_ambr_free(policy_update->ue_ambr);
        policy_update->ue_ambr = NULL;
    }
    if (policy_update->ue_slice_mbrs) {
        OpenAPI_list_for_each(policy_update->ue_slice_mbrs, node) {
            OpenAPI_ue_slice_mbr_free(node->data);
        }
        OpenAPI_list_free(policy_update->ue_slice_mbrs);
        policy_update->ue_slice_mbrs = NULL;
    }
    if (policy_update->pras) {
        OpenAPI_list_for_each(policy_update->pras, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_presence_info_rm_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(policy_update->pras);
        policy_update->pras = NULL;
    }
    if (policy_update->pcf_ue_info) {
        OpenAPI_pcf_ue_callback_info_free(policy_update->pcf_ue_info);
        policy_update->pcf_ue_info = NULL;
    }
    if (policy_update->match_pdus) {
        OpenAPI_list_for_each(policy_update->match_pdus, node) {
            OpenAPI_pdu_session_info_free(node->data);
        }
        OpenAPI_list_free(policy_update->match_pdus);
        policy_update->match_pdus = NULL;
    }
    if (policy_update->as_time_dis_param) {
        OpenAPI_as_time_distribution_param_free(policy_update->as_time_dis_param);
        policy_update->as_time_dis_param = NULL;
    }
    if (policy_update->snssai_repl_infos) {
        OpenAPI_list_for_each(policy_update->snssai_repl_infos, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_snssai_replace_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(policy_update->snssai_repl_infos);
        policy_update->snssai_repl_infos = NULL;
    }
    if (policy_update->slice_usg_ctrl_info_sets) {
        OpenAPI_list_for_each(policy_update->slice_usg_ctrl_info_sets, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_slice_usg_ctrl_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(policy_update->slice_usg_ctrl_info_sets);
        policy_update->slice_usg_ctrl_info_sets = NULL;
    }
    if (policy_update->slice_repl_req) {
        OpenAPI_slice_repl_req_free(policy_update->slice_repl_req);
        policy_update->slice_repl_req = NULL;
    }
    if (policy_update->supp_feat) {
        ogs_free(policy_update->supp_feat);
        policy_update->supp_feat = NULL;
    }
    ogs_free(policy_update);
}

cJSON *OpenAPI_policy_update_convertToJSON(OpenAPI_policy_update_t *policy_update)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (policy_update == NULL) {
        ogs_error("OpenAPI_policy_update_convertToJSON() failed [PolicyUpdate]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!policy_update->resource_uri) {
        ogs_error("OpenAPI_policy_update_convertToJSON() failed [resource_uri]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "resourceUri", policy_update->resource_uri) == NULL) {
        ogs_error("OpenAPI_policy_update_convertToJSON() failed [resource_uri]");
        goto end;
    }

    if (policy_update->triggers != OpenAPI_request_trigger_NULL) {
    cJSON *triggersList = cJSON_AddArrayToObject(item, "triggers");
    if (triggersList == NULL) {
        ogs_error("OpenAPI_policy_update_convertToJSON() failed [triggers]");
        goto end;
    }
    OpenAPI_list_for_each(policy_update->triggers, node) {
        if (cJSON_AddStringToObject(triggersList, "", OpenAPI_request_trigger_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_policy_update_convertToJSON() failed [triggers]");
            goto end;
        }
    }
    } else if (policy_update->is_triggers_null) {
        if (cJSON_AddNullToObject(item, "triggers") == NULL) {
            ogs_error("OpenAPI_policy_update_convertToJSON() failed [triggers]");
            goto end;
        }
    }

    if (policy_update->serv_area_res) {
    cJSON *serv_area_res_local_JSON = OpenAPI_service_area_restriction_convertToJSON(policy_update->serv_area_res);
    if (serv_area_res_local_JSON == NULL) {
        ogs_error("OpenAPI_policy_update_convertToJSON() failed [serv_area_res]");
        goto end;
    }
    cJSON_AddItemToObject(item, "servAreaRes", serv_area_res_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_policy_update_convertToJSON() failed [serv_area_res]");
        goto end;
    }
    }

    if (policy_update->wl_serv_area_res) {
    cJSON *wl_serv_area_res_local_JSON = OpenAPI_wireline_service_area_restriction_convertToJSON(policy_update->wl_serv_area_res);
    if (wl_serv_area_res_local_JSON == NULL) {
        ogs_error("OpenAPI_policy_update_convertToJSON() failed [wl_serv_area_res]");
        goto end;
    }
    cJSON_AddItemToObject(item, "wlServAreaRes", wl_serv_area_res_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_policy_update_convertToJSON() failed [wl_serv_area_res]");
        goto end;
    }
    }

    if (policy_update->is_rfsp) {
    if (cJSON_AddNumberToObject(item, "rfsp", policy_update->rfsp) == NULL) {
        ogs_error("OpenAPI_policy_update_convertToJSON() failed [rfsp]");
        goto end;
    }
    }

    if (policy_update->is_rfsp_val_time) {
    if (cJSON_AddNumberToObject(item, "rfspValTime", policy_update->rfsp_val_time) == NULL) {
        ogs_error("OpenAPI_policy_update_convertToJSON() failed [rfsp_val_time]");
        goto end;
    }
    }

    if (policy_update->is_target_rfsp) {
    if (cJSON_AddNumberToObject(item, "targetRfsp", policy_update->target_rfsp) == NULL) {
        ogs_error("OpenAPI_policy_update_convertToJSON() failed [target_rfsp]");
        goto end;
    }
    }

    if (policy_update->smf_sel_info) {
    cJSON *smf_sel_info_local_JSON = OpenAPI_smf_selection_data_convertToJSON(policy_update->smf_sel_info);
    if (smf_sel_info_local_JSON == NULL) {
        ogs_error("OpenAPI_policy_update_convertToJSON() failed [smf_sel_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "smfSelInfo", smf_sel_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_policy_update_convertToJSON() failed [smf_sel_info]");
        goto end;
    }
    } else if (policy_update->is_smf_sel_info_null) {
        if (cJSON_AddNullToObject(item, "smfSelInfo") == NULL) {
            ogs_error("OpenAPI_policy_update_convertToJSON() failed [smf_sel_info]");
            goto end;
        }
    }

    if (policy_update->ue_ambr) {
    cJSON *ue_ambr_local_JSON = OpenAPI_ambr_convertToJSON(policy_update->ue_ambr);
    if (ue_ambr_local_JSON == NULL) {
        ogs_error("OpenAPI_policy_update_convertToJSON() failed [ue_ambr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ueAmbr", ue_ambr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_policy_update_convertToJSON() failed [ue_ambr]");
        goto end;
    }
    }

    if (policy_update->ue_slice_mbrs) {
    cJSON *ue_slice_mbrsList = cJSON_AddArrayToObject(item, "ueSliceMbrs");
    if (ue_slice_mbrsList == NULL) {
        ogs_error("OpenAPI_policy_update_convertToJSON() failed [ue_slice_mbrs]");
        goto end;
    }
    OpenAPI_list_for_each(policy_update->ue_slice_mbrs, node) {
        cJSON *itemLocal = OpenAPI_ue_slice_mbr_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_policy_update_convertToJSON() failed [ue_slice_mbrs]");
            goto end;
        }
        cJSON_AddItemToArray(ue_slice_mbrsList, itemLocal);
    }
    }

    if (policy_update->pras) {
    cJSON *pras = cJSON_AddObjectToObject(item, "pras");
    if (pras == NULL) {
        ogs_error("OpenAPI_policy_update_convertToJSON() failed [pras]");
        goto end;
    }
    cJSON *localMapObject = pras;
    if (policy_update->pras) {
        OpenAPI_list_for_each(policy_update->pras, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_policy_update_convertToJSON() failed [pras]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_policy_update_convertToJSON() failed [pras]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_presence_info_rm_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_policy_update_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    } else if (policy_update->is_pras_null) {
        if (cJSON_AddNullToObject(item, "pras") == NULL) {
            ogs_error("OpenAPI_policy_update_convertToJSON() failed [pras]");
            goto end;
        }
    }

    if (policy_update->pcf_ue_info) {
    cJSON *pcf_ue_info_local_JSON = OpenAPI_pcf_ue_callback_info_convertToJSON(policy_update->pcf_ue_info);
    if (pcf_ue_info_local_JSON == NULL) {
        ogs_error("OpenAPI_policy_update_convertToJSON() failed [pcf_ue_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pcfUeInfo", pcf_ue_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_policy_update_convertToJSON() failed [pcf_ue_info]");
        goto end;
    }
    } else if (policy_update->is_pcf_ue_info_null) {
        if (cJSON_AddNullToObject(item, "pcfUeInfo") == NULL) {
            ogs_error("OpenAPI_policy_update_convertToJSON() failed [pcf_ue_info]");
            goto end;
        }
    }

    if (policy_update->match_pdus) {
    cJSON *match_pdusList = cJSON_AddArrayToObject(item, "matchPdus");
    if (match_pdusList == NULL) {
        ogs_error("OpenAPI_policy_update_convertToJSON() failed [match_pdus]");
        goto end;
    }
    OpenAPI_list_for_each(policy_update->match_pdus, node) {
        cJSON *itemLocal = OpenAPI_pdu_session_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_policy_update_convertToJSON() failed [match_pdus]");
            goto end;
        }
        cJSON_AddItemToArray(match_pdusList, itemLocal);
    }
    } else if (policy_update->is_match_pdus_null) {
        if (cJSON_AddNullToObject(item, "matchPdus") == NULL) {
            ogs_error("OpenAPI_policy_update_convertToJSON() failed [match_pdus]");
            goto end;
        }
    }

    if (policy_update->as_time_dis_param) {
    cJSON *as_time_dis_param_local_JSON = OpenAPI_as_time_distribution_param_convertToJSON(policy_update->as_time_dis_param);
    if (as_time_dis_param_local_JSON == NULL) {
        ogs_error("OpenAPI_policy_update_convertToJSON() failed [as_time_dis_param]");
        goto end;
    }
    cJSON_AddItemToObject(item, "asTimeDisParam", as_time_dis_param_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_policy_update_convertToJSON() failed [as_time_dis_param]");
        goto end;
    }
    } else if (policy_update->is_as_time_dis_param_null) {
        if (cJSON_AddNullToObject(item, "asTimeDisParam") == NULL) {
            ogs_error("OpenAPI_policy_update_convertToJSON() failed [as_time_dis_param]");
            goto end;
        }
    }

    if (policy_update->snssai_repl_infos) {
    cJSON *snssai_repl_infos = cJSON_AddObjectToObject(item, "snssaiReplInfos");
    if (snssai_repl_infos == NULL) {
        ogs_error("OpenAPI_policy_update_convertToJSON() failed [snssai_repl_infos]");
        goto end;
    }
    cJSON *localMapObject = snssai_repl_infos;
    if (policy_update->snssai_repl_infos) {
        OpenAPI_list_for_each(policy_update->snssai_repl_infos, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_policy_update_convertToJSON() failed [snssai_repl_infos]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_policy_update_convertToJSON() failed [snssai_repl_infos]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_snssai_replace_info_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_policy_update_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    } else if (policy_update->is_snssai_repl_infos_null) {
        if (cJSON_AddNullToObject(item, "snssaiReplInfos") == NULL) {
            ogs_error("OpenAPI_policy_update_convertToJSON() failed [snssai_repl_infos]");
            goto end;
        }
    }

    if (policy_update->slice_usg_ctrl_info_sets) {
    cJSON *slice_usg_ctrl_info_sets = cJSON_AddObjectToObject(item, "sliceUsgCtrlInfoSets");
    if (slice_usg_ctrl_info_sets == NULL) {
        ogs_error("OpenAPI_policy_update_convertToJSON() failed [slice_usg_ctrl_info_sets]");
        goto end;
    }
    cJSON *localMapObject = slice_usg_ctrl_info_sets;
    if (policy_update->slice_usg_ctrl_info_sets) {
        OpenAPI_list_for_each(policy_update->slice_usg_ctrl_info_sets, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_policy_update_convertToJSON() failed [slice_usg_ctrl_info_sets]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_policy_update_convertToJSON() failed [slice_usg_ctrl_info_sets]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_slice_usg_ctrl_info_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_policy_update_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (policy_update->slice_repl_req) {
    cJSON *slice_repl_req_local_JSON = OpenAPI_slice_repl_req_convertToJSON(policy_update->slice_repl_req);
    if (slice_repl_req_local_JSON == NULL) {
        ogs_error("OpenAPI_policy_update_convertToJSON() failed [slice_repl_req]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sliceReplReq", slice_repl_req_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_policy_update_convertToJSON() failed [slice_repl_req]");
        goto end;
    }
    }

    if (policy_update->supp_feat) {
    if (cJSON_AddStringToObject(item, "suppFeat", policy_update->supp_feat) == NULL) {
        ogs_error("OpenAPI_policy_update_convertToJSON() failed [supp_feat]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_policy_update_t *OpenAPI_policy_update_parseFromJSON(cJSON *policy_updateJSON)
{
    OpenAPI_policy_update_t *policy_update_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *resource_uri = NULL;
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
    cJSON *snssai_repl_infos = NULL;
    OpenAPI_list_t *snssai_repl_infosList = NULL;
    cJSON *slice_usg_ctrl_info_sets = NULL;
    OpenAPI_list_t *slice_usg_ctrl_info_setsList = NULL;
    cJSON *slice_repl_req = NULL;
    OpenAPI_slice_repl_req_t *slice_repl_req_local_nonprim = NULL;
    cJSON *supp_feat = NULL;
    resource_uri = cJSON_GetObjectItemCaseSensitive(policy_updateJSON, "resourceUri");
    if (!resource_uri) {
        ogs_error("OpenAPI_policy_update_parseFromJSON() failed [resource_uri]");
        goto end;
    }
    if (!cJSON_IsString(resource_uri)) {
        ogs_error("OpenAPI_policy_update_parseFromJSON() failed [resource_uri]");
        goto end;
    }

    triggers = cJSON_GetObjectItemCaseSensitive(policy_updateJSON, "triggers");
    if (triggers) {
    if (!cJSON_IsNull(triggers)) {
        cJSON *triggers_local = NULL;
        if (!cJSON_IsArray(triggers)) {
            ogs_error("OpenAPI_policy_update_parseFromJSON() failed [triggers]");
            goto end;
        }

        triggersList = OpenAPI_list_create();

        cJSON_ArrayForEach(triggers_local, triggers) {
            OpenAPI_request_trigger_e localEnum = OpenAPI_request_trigger_NULL;
            if (!cJSON_IsString(triggers_local)) {
                ogs_error("OpenAPI_policy_update_parseFromJSON() failed [triggers]");
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
            ogs_error("OpenAPI_policy_update_parseFromJSON() failed: Expected triggersList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }
    }

    serv_area_res = cJSON_GetObjectItemCaseSensitive(policy_updateJSON, "servAreaRes");
    if (serv_area_res) {
    serv_area_res_local_nonprim = OpenAPI_service_area_restriction_parseFromJSON(serv_area_res);
    if (!serv_area_res_local_nonprim) {
        ogs_error("OpenAPI_service_area_restriction_parseFromJSON failed [serv_area_res]");
        goto end;
    }
    }

    wl_serv_area_res = cJSON_GetObjectItemCaseSensitive(policy_updateJSON, "wlServAreaRes");
    if (wl_serv_area_res) {
    wl_serv_area_res_local_nonprim = OpenAPI_wireline_service_area_restriction_parseFromJSON(wl_serv_area_res);
    if (!wl_serv_area_res_local_nonprim) {
        ogs_error("OpenAPI_wireline_service_area_restriction_parseFromJSON failed [wl_serv_area_res]");
        goto end;
    }
    }

    rfsp = cJSON_GetObjectItemCaseSensitive(policy_updateJSON, "rfsp");
    if (rfsp) {
    if (!cJSON_IsNumber(rfsp)) {
        ogs_error("OpenAPI_policy_update_parseFromJSON() failed [rfsp]");
        goto end;
    }
    }

    rfsp_val_time = cJSON_GetObjectItemCaseSensitive(policy_updateJSON, "rfspValTime");
    if (rfsp_val_time) {
    if (!cJSON_IsNumber(rfsp_val_time)) {
        ogs_error("OpenAPI_policy_update_parseFromJSON() failed [rfsp_val_time]");
        goto end;
    }
    }

    target_rfsp = cJSON_GetObjectItemCaseSensitive(policy_updateJSON, "targetRfsp");
    if (target_rfsp) {
    if (!cJSON_IsNumber(target_rfsp)) {
        ogs_error("OpenAPI_policy_update_parseFromJSON() failed [target_rfsp]");
        goto end;
    }
    }

    smf_sel_info = cJSON_GetObjectItemCaseSensitive(policy_updateJSON, "smfSelInfo");
    if (smf_sel_info) {
    if (!cJSON_IsNull(smf_sel_info)) {
    smf_sel_info_local_nonprim = OpenAPI_smf_selection_data_parseFromJSON(smf_sel_info);
    if (!smf_sel_info_local_nonprim) {
        ogs_error("OpenAPI_smf_selection_data_parseFromJSON failed [smf_sel_info]");
        goto end;
    }
    }
    }

    ue_ambr = cJSON_GetObjectItemCaseSensitive(policy_updateJSON, "ueAmbr");
    if (ue_ambr) {
    ue_ambr_local_nonprim = OpenAPI_ambr_parseFromJSON(ue_ambr);
    if (!ue_ambr_local_nonprim) {
        ogs_error("OpenAPI_ambr_parseFromJSON failed [ue_ambr]");
        goto end;
    }
    }

    ue_slice_mbrs = cJSON_GetObjectItemCaseSensitive(policy_updateJSON, "ueSliceMbrs");
    if (ue_slice_mbrs) {
        cJSON *ue_slice_mbrs_local = NULL;
        if (!cJSON_IsArray(ue_slice_mbrs)) {
            ogs_error("OpenAPI_policy_update_parseFromJSON() failed [ue_slice_mbrs]");
            goto end;
        }

        ue_slice_mbrsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ue_slice_mbrs_local, ue_slice_mbrs) {
            if (!cJSON_IsObject(ue_slice_mbrs_local)) {
                ogs_error("OpenAPI_policy_update_parseFromJSON() failed [ue_slice_mbrs]");
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

    pras = cJSON_GetObjectItemCaseSensitive(policy_updateJSON, "pras");
    if (pras) {
    if (!cJSON_IsNull(pras)) {
        cJSON *pras_local_map = NULL;
        if (!cJSON_IsObject(pras) && !cJSON_IsNull(pras)) {
            ogs_error("OpenAPI_policy_update_parseFromJSON() failed [pras]");
            goto end;
        }
        if (cJSON_IsObject(pras)) {
            prasList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(pras_local_map, pras) {
                cJSON *localMapObject = pras_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_presence_info_rm_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_policy_update_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(prasList, localMapKeyPair);
            }
        }
    }
    }

    pcf_ue_info = cJSON_GetObjectItemCaseSensitive(policy_updateJSON, "pcfUeInfo");
    if (pcf_ue_info) {
    if (!cJSON_IsNull(pcf_ue_info)) {
    pcf_ue_info_local_nonprim = OpenAPI_pcf_ue_callback_info_parseFromJSON(pcf_ue_info);
    if (!pcf_ue_info_local_nonprim) {
        ogs_error("OpenAPI_pcf_ue_callback_info_parseFromJSON failed [pcf_ue_info]");
        goto end;
    }
    }
    }

    match_pdus = cJSON_GetObjectItemCaseSensitive(policy_updateJSON, "matchPdus");
    if (match_pdus) {
    if (!cJSON_IsNull(match_pdus)) {
        cJSON *match_pdus_local = NULL;
        if (!cJSON_IsArray(match_pdus)) {
            ogs_error("OpenAPI_policy_update_parseFromJSON() failed [match_pdus]");
            goto end;
        }

        match_pdusList = OpenAPI_list_create();

        cJSON_ArrayForEach(match_pdus_local, match_pdus) {
            if (!cJSON_IsObject(match_pdus_local)) {
                ogs_error("OpenAPI_policy_update_parseFromJSON() failed [match_pdus]");
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

    as_time_dis_param = cJSON_GetObjectItemCaseSensitive(policy_updateJSON, "asTimeDisParam");
    if (as_time_dis_param) {
    if (!cJSON_IsNull(as_time_dis_param)) {
    as_time_dis_param_local_nonprim = OpenAPI_as_time_distribution_param_parseFromJSON(as_time_dis_param);
    if (!as_time_dis_param_local_nonprim) {
        ogs_error("OpenAPI_as_time_distribution_param_parseFromJSON failed [as_time_dis_param]");
        goto end;
    }
    }
    }

    snssai_repl_infos = cJSON_GetObjectItemCaseSensitive(policy_updateJSON, "snssaiReplInfos");
    if (snssai_repl_infos) {
    if (!cJSON_IsNull(snssai_repl_infos)) {
        cJSON *snssai_repl_infos_local_map = NULL;
        if (!cJSON_IsObject(snssai_repl_infos) && !cJSON_IsNull(snssai_repl_infos)) {
            ogs_error("OpenAPI_policy_update_parseFromJSON() failed [snssai_repl_infos]");
            goto end;
        }
        if (cJSON_IsObject(snssai_repl_infos)) {
            snssai_repl_infosList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(snssai_repl_infos_local_map, snssai_repl_infos) {
                cJSON *localMapObject = snssai_repl_infos_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_snssai_replace_info_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_policy_update_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(snssai_repl_infosList, localMapKeyPair);
            }
        }
    }
    }

    slice_usg_ctrl_info_sets = cJSON_GetObjectItemCaseSensitive(policy_updateJSON, "sliceUsgCtrlInfoSets");
    if (slice_usg_ctrl_info_sets) {
        cJSON *slice_usg_ctrl_info_sets_local_map = NULL;
        if (!cJSON_IsObject(slice_usg_ctrl_info_sets) && !cJSON_IsNull(slice_usg_ctrl_info_sets)) {
            ogs_error("OpenAPI_policy_update_parseFromJSON() failed [slice_usg_ctrl_info_sets]");
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
                    ogs_error("OpenAPI_policy_update_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(slice_usg_ctrl_info_setsList, localMapKeyPair);
            }
        }
    }

    slice_repl_req = cJSON_GetObjectItemCaseSensitive(policy_updateJSON, "sliceReplReq");
    if (slice_repl_req) {
    slice_repl_req_local_nonprim = OpenAPI_slice_repl_req_parseFromJSON(slice_repl_req);
    if (!slice_repl_req_local_nonprim) {
        ogs_error("OpenAPI_slice_repl_req_parseFromJSON failed [slice_repl_req]");
        goto end;
    }
    }

    supp_feat = cJSON_GetObjectItemCaseSensitive(policy_updateJSON, "suppFeat");
    if (supp_feat) {
    if (!cJSON_IsString(supp_feat) && !cJSON_IsNull(supp_feat)) {
        ogs_error("OpenAPI_policy_update_parseFromJSON() failed [supp_feat]");
        goto end;
    }
    }

    policy_update_local_var = OpenAPI_policy_update_create (
        ogs_strdup(resource_uri->valuestring),
        triggers && cJSON_IsNull(triggers) ? true : false,
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
        pras && cJSON_IsNull(pras) ? true : false,
        pras ? prasList : NULL,
        pcf_ue_info && cJSON_IsNull(pcf_ue_info) ? true : false,
        pcf_ue_info ? pcf_ue_info_local_nonprim : NULL,
        match_pdus && cJSON_IsNull(match_pdus) ? true : false,
        match_pdus ? match_pdusList : NULL,
        as_time_dis_param && cJSON_IsNull(as_time_dis_param) ? true : false,
        as_time_dis_param ? as_time_dis_param_local_nonprim : NULL,
        snssai_repl_infos && cJSON_IsNull(snssai_repl_infos) ? true : false,
        snssai_repl_infos ? snssai_repl_infosList : NULL,
        slice_usg_ctrl_info_sets ? slice_usg_ctrl_info_setsList : NULL,
        slice_repl_req ? slice_repl_req_local_nonprim : NULL,
        supp_feat && !cJSON_IsNull(supp_feat) ? ogs_strdup(supp_feat->valuestring) : NULL
    );

    return policy_update_local_var;
end:
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
            OpenAPI_presence_info_rm_free(localKeyValue->value);
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
    if (snssai_repl_infosList) {
        OpenAPI_list_for_each(snssai_repl_infosList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_snssai_replace_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(snssai_repl_infosList);
        snssai_repl_infosList = NULL;
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
    if (slice_repl_req_local_nonprim) {
        OpenAPI_slice_repl_req_free(slice_repl_req_local_nonprim);
        slice_repl_req_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_policy_update_t *OpenAPI_policy_update_copy(OpenAPI_policy_update_t *dst, OpenAPI_policy_update_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_policy_update_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_policy_update_convertToJSON() failed");
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

    OpenAPI_policy_update_free(dst);
    dst = OpenAPI_policy_update_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

