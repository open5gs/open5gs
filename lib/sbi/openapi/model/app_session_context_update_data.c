
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "app_session_context_update_data.h"

OpenAPI_app_session_context_update_data_t *OpenAPI_app_session_context_update_data_create(
    char *af_app_id,
    OpenAPI_af_routing_requirement_rm_t *af_rout_req,
    char *asp_id,
    char *bdt_ref_id,
    OpenAPI_events_subsc_req_data_rm_t *ev_subsc,
    char *mcptt_id,
    char *mc_video_id,
    OpenAPI_list_t* med_components,
    char *mps_id,
    char *mcs_id,
    OpenAPI_preemption_control_information_rm_t *preempt_control_info,
    OpenAPI_reserv_priority_e res_prio,
    OpenAPI_service_info_status_e serv_inf_status,
    OpenAPI_sip_forking_indication_e sip_fork_ind,
    char *spon_id,
    OpenAPI_sponsoring_status_e spon_status,
    OpenAPI_bridge_management_container_t *tsn_bridge_man_cont,
    OpenAPI_port_management_container_t *tsn_port_man_cont_dstt,
    OpenAPI_list_t *tsn_port_man_cont_nwtts
)
{
    OpenAPI_app_session_context_update_data_t *app_session_context_update_data_local_var = ogs_malloc(sizeof(OpenAPI_app_session_context_update_data_t));
    ogs_assert(app_session_context_update_data_local_var);

    app_session_context_update_data_local_var->af_app_id = af_app_id;
    app_session_context_update_data_local_var->af_rout_req = af_rout_req;
    app_session_context_update_data_local_var->asp_id = asp_id;
    app_session_context_update_data_local_var->bdt_ref_id = bdt_ref_id;
    app_session_context_update_data_local_var->ev_subsc = ev_subsc;
    app_session_context_update_data_local_var->mcptt_id = mcptt_id;
    app_session_context_update_data_local_var->mc_video_id = mc_video_id;
    app_session_context_update_data_local_var->med_components = med_components;
    app_session_context_update_data_local_var->mps_id = mps_id;
    app_session_context_update_data_local_var->mcs_id = mcs_id;
    app_session_context_update_data_local_var->preempt_control_info = preempt_control_info;
    app_session_context_update_data_local_var->res_prio = res_prio;
    app_session_context_update_data_local_var->serv_inf_status = serv_inf_status;
    app_session_context_update_data_local_var->sip_fork_ind = sip_fork_ind;
    app_session_context_update_data_local_var->spon_id = spon_id;
    app_session_context_update_data_local_var->spon_status = spon_status;
    app_session_context_update_data_local_var->tsn_bridge_man_cont = tsn_bridge_man_cont;
    app_session_context_update_data_local_var->tsn_port_man_cont_dstt = tsn_port_man_cont_dstt;
    app_session_context_update_data_local_var->tsn_port_man_cont_nwtts = tsn_port_man_cont_nwtts;

    return app_session_context_update_data_local_var;
}

void OpenAPI_app_session_context_update_data_free(OpenAPI_app_session_context_update_data_t *app_session_context_update_data)
{
    if (NULL == app_session_context_update_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(app_session_context_update_data->af_app_id);
    OpenAPI_af_routing_requirement_rm_free(app_session_context_update_data->af_rout_req);
    ogs_free(app_session_context_update_data->asp_id);
    ogs_free(app_session_context_update_data->bdt_ref_id);
    OpenAPI_events_subsc_req_data_rm_free(app_session_context_update_data->ev_subsc);
    ogs_free(app_session_context_update_data->mcptt_id);
    ogs_free(app_session_context_update_data->mc_video_id);
    OpenAPI_list_for_each(app_session_context_update_data->med_components, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        ogs_free(localKeyValue->key);
        OpenAPI_media_component_rm_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(app_session_context_update_data->med_components);
    ogs_free(app_session_context_update_data->mps_id);
    ogs_free(app_session_context_update_data->mcs_id);
    OpenAPI_preemption_control_information_rm_free(app_session_context_update_data->preempt_control_info);
    ogs_free(app_session_context_update_data->spon_id);
    OpenAPI_bridge_management_container_free(app_session_context_update_data->tsn_bridge_man_cont);
    OpenAPI_port_management_container_free(app_session_context_update_data->tsn_port_man_cont_dstt);
    OpenAPI_list_for_each(app_session_context_update_data->tsn_port_man_cont_nwtts, node) {
        OpenAPI_port_management_container_free(node->data);
    }
    OpenAPI_list_free(app_session_context_update_data->tsn_port_man_cont_nwtts);
    ogs_free(app_session_context_update_data);
}

cJSON *OpenAPI_app_session_context_update_data_convertToJSON(OpenAPI_app_session_context_update_data_t *app_session_context_update_data)
{
    cJSON *item = NULL;

    if (app_session_context_update_data == NULL) {
        ogs_error("OpenAPI_app_session_context_update_data_convertToJSON() failed [AppSessionContextUpdateData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (app_session_context_update_data->af_app_id) {
    if (cJSON_AddStringToObject(item, "afAppId", app_session_context_update_data->af_app_id) == NULL) {
        ogs_error("OpenAPI_app_session_context_update_data_convertToJSON() failed [af_app_id]");
        goto end;
    }
    }

    if (app_session_context_update_data->af_rout_req) {
    cJSON *af_rout_req_local_JSON = OpenAPI_af_routing_requirement_rm_convertToJSON(app_session_context_update_data->af_rout_req);
    if (af_rout_req_local_JSON == NULL) {
        ogs_error("OpenAPI_app_session_context_update_data_convertToJSON() failed [af_rout_req]");
        goto end;
    }
    cJSON_AddItemToObject(item, "afRoutReq", af_rout_req_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_app_session_context_update_data_convertToJSON() failed [af_rout_req]");
        goto end;
    }
    }

    if (app_session_context_update_data->asp_id) {
    if (cJSON_AddStringToObject(item, "aspId", app_session_context_update_data->asp_id) == NULL) {
        ogs_error("OpenAPI_app_session_context_update_data_convertToJSON() failed [asp_id]");
        goto end;
    }
    }

    if (app_session_context_update_data->bdt_ref_id) {
    if (cJSON_AddStringToObject(item, "bdtRefId", app_session_context_update_data->bdt_ref_id) == NULL) {
        ogs_error("OpenAPI_app_session_context_update_data_convertToJSON() failed [bdt_ref_id]");
        goto end;
    }
    }

    if (app_session_context_update_data->ev_subsc) {
    cJSON *ev_subsc_local_JSON = OpenAPI_events_subsc_req_data_rm_convertToJSON(app_session_context_update_data->ev_subsc);
    if (ev_subsc_local_JSON == NULL) {
        ogs_error("OpenAPI_app_session_context_update_data_convertToJSON() failed [ev_subsc]");
        goto end;
    }
    cJSON_AddItemToObject(item, "evSubsc", ev_subsc_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_app_session_context_update_data_convertToJSON() failed [ev_subsc]");
        goto end;
    }
    }

    if (app_session_context_update_data->mcptt_id) {
    if (cJSON_AddStringToObject(item, "mcpttId", app_session_context_update_data->mcptt_id) == NULL) {
        ogs_error("OpenAPI_app_session_context_update_data_convertToJSON() failed [mcptt_id]");
        goto end;
    }
    }

    if (app_session_context_update_data->mc_video_id) {
    if (cJSON_AddStringToObject(item, "mcVideoId", app_session_context_update_data->mc_video_id) == NULL) {
        ogs_error("OpenAPI_app_session_context_update_data_convertToJSON() failed [mc_video_id]");
        goto end;
    }
    }

    if (app_session_context_update_data->med_components) {
    cJSON *med_components = cJSON_AddObjectToObject(item, "medComponents");
    if (med_components == NULL) {
        ogs_error("OpenAPI_app_session_context_update_data_convertToJSON() failed [med_components]");
        goto end;
    }
    cJSON *localMapObject = med_components;
    OpenAPI_lnode_t *med_components_node;
    if (app_session_context_update_data->med_components) {
        OpenAPI_list_for_each(app_session_context_update_data->med_components, med_components_node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)med_components_node->data;
        cJSON *itemLocal = localKeyValue->value ?
            OpenAPI_media_component_rm_convertToJSON(localKeyValue->value) :
            cJSON_CreateNull();
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_app_session_context_update_data_convertToJSON() failed [med_components]");
            goto end;
        }
        cJSON_AddItemToObject(med_components, localKeyValue->key, itemLocal);
            }
        }
    }

    if (app_session_context_update_data->mps_id) {
    if (cJSON_AddStringToObject(item, "mpsId", app_session_context_update_data->mps_id) == NULL) {
        ogs_error("OpenAPI_app_session_context_update_data_convertToJSON() failed [mps_id]");
        goto end;
    }
    }

    if (app_session_context_update_data->mcs_id) {
    if (cJSON_AddStringToObject(item, "mcsId", app_session_context_update_data->mcs_id) == NULL) {
        ogs_error("OpenAPI_app_session_context_update_data_convertToJSON() failed [mcs_id]");
        goto end;
    }
    }

    if (app_session_context_update_data->preempt_control_info) {
    cJSON *preempt_control_info_local_JSON = OpenAPI_preemption_control_information_rm_convertToJSON(app_session_context_update_data->preempt_control_info);
    if (preempt_control_info_local_JSON == NULL) {
        ogs_error("OpenAPI_app_session_context_update_data_convertToJSON() failed [preempt_control_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "preemptControlInfo", preempt_control_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_app_session_context_update_data_convertToJSON() failed [preempt_control_info]");
        goto end;
    }
    }

    if (app_session_context_update_data->res_prio) {
    if (cJSON_AddStringToObject(item, "resPrio", OpenAPI_reserv_priority_ToString(app_session_context_update_data->res_prio)) == NULL) {
        ogs_error("OpenAPI_app_session_context_update_data_convertToJSON() failed [res_prio]");
        goto end;
    }
    }

    if (app_session_context_update_data->serv_inf_status) {
    if (cJSON_AddStringToObject(item, "servInfStatus", OpenAPI_service_info_status_ToString(app_session_context_update_data->serv_inf_status)) == NULL) {
        ogs_error("OpenAPI_app_session_context_update_data_convertToJSON() failed [serv_inf_status]");
        goto end;
    }
    }

    if (app_session_context_update_data->sip_fork_ind) {
    if (cJSON_AddStringToObject(item, "sipForkInd", OpenAPI_sip_forking_indication_ToString(app_session_context_update_data->sip_fork_ind)) == NULL) {
        ogs_error("OpenAPI_app_session_context_update_data_convertToJSON() failed [sip_fork_ind]");
        goto end;
    }
    }

    if (app_session_context_update_data->spon_id) {
    if (cJSON_AddStringToObject(item, "sponId", app_session_context_update_data->spon_id) == NULL) {
        ogs_error("OpenAPI_app_session_context_update_data_convertToJSON() failed [spon_id]");
        goto end;
    }
    }

    if (app_session_context_update_data->spon_status) {
    if (cJSON_AddStringToObject(item, "sponStatus", OpenAPI_sponsoring_status_ToString(app_session_context_update_data->spon_status)) == NULL) {
        ogs_error("OpenAPI_app_session_context_update_data_convertToJSON() failed [spon_status]");
        goto end;
    }
    }

    if (app_session_context_update_data->tsn_bridge_man_cont) {
    cJSON *tsn_bridge_man_cont_local_JSON = OpenAPI_bridge_management_container_convertToJSON(app_session_context_update_data->tsn_bridge_man_cont);
    if (tsn_bridge_man_cont_local_JSON == NULL) {
        ogs_error("OpenAPI_app_session_context_update_data_convertToJSON() failed [tsn_bridge_man_cont]");
        goto end;
    }
    cJSON_AddItemToObject(item, "tsnBridgeManCont", tsn_bridge_man_cont_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_app_session_context_update_data_convertToJSON() failed [tsn_bridge_man_cont]");
        goto end;
    }
    }

    if (app_session_context_update_data->tsn_port_man_cont_dstt) {
    cJSON *tsn_port_man_cont_dstt_local_JSON = OpenAPI_port_management_container_convertToJSON(app_session_context_update_data->tsn_port_man_cont_dstt);
    if (tsn_port_man_cont_dstt_local_JSON == NULL) {
        ogs_error("OpenAPI_app_session_context_update_data_convertToJSON() failed [tsn_port_man_cont_dstt]");
        goto end;
    }
    cJSON_AddItemToObject(item, "tsnPortManContDstt", tsn_port_man_cont_dstt_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_app_session_context_update_data_convertToJSON() failed [tsn_port_man_cont_dstt]");
        goto end;
    }
    }

    if (app_session_context_update_data->tsn_port_man_cont_nwtts) {
    cJSON *tsn_port_man_cont_nwttsList = cJSON_AddArrayToObject(item, "tsnPortManContNwtts");
    if (tsn_port_man_cont_nwttsList == NULL) {
        ogs_error("OpenAPI_app_session_context_update_data_convertToJSON() failed [tsn_port_man_cont_nwtts]");
        goto end;
    }

    OpenAPI_lnode_t *tsn_port_man_cont_nwtts_node;
    if (app_session_context_update_data->tsn_port_man_cont_nwtts) {
        OpenAPI_list_for_each(app_session_context_update_data->tsn_port_man_cont_nwtts, tsn_port_man_cont_nwtts_node) {
            cJSON *itemLocal = OpenAPI_port_management_container_convertToJSON(tsn_port_man_cont_nwtts_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_app_session_context_update_data_convertToJSON() failed [tsn_port_man_cont_nwtts]");
                goto end;
            }
            cJSON_AddItemToArray(tsn_port_man_cont_nwttsList, itemLocal);
        }
    }
    }

end:
    return item;
}

OpenAPI_app_session_context_update_data_t *OpenAPI_app_session_context_update_data_parseFromJSON(cJSON *app_session_context_update_dataJSON)
{
    OpenAPI_app_session_context_update_data_t *app_session_context_update_data_local_var = NULL;
    cJSON *af_app_id = cJSON_GetObjectItemCaseSensitive(app_session_context_update_dataJSON, "afAppId");

    if (af_app_id) {
    if (!cJSON_IsString(af_app_id)) {
        ogs_error("OpenAPI_app_session_context_update_data_parseFromJSON() failed [af_app_id]");
        goto end;
    }
    }

    cJSON *af_rout_req = cJSON_GetObjectItemCaseSensitive(app_session_context_update_dataJSON, "afRoutReq");

    OpenAPI_af_routing_requirement_rm_t *af_rout_req_local_nonprim = NULL;
    if (af_rout_req) {
    af_rout_req_local_nonprim = OpenAPI_af_routing_requirement_rm_parseFromJSON(af_rout_req);
    }

    cJSON *asp_id = cJSON_GetObjectItemCaseSensitive(app_session_context_update_dataJSON, "aspId");

    if (asp_id) {
    if (!cJSON_IsString(asp_id)) {
        ogs_error("OpenAPI_app_session_context_update_data_parseFromJSON() failed [asp_id]");
        goto end;
    }
    }

    cJSON *bdt_ref_id = cJSON_GetObjectItemCaseSensitive(app_session_context_update_dataJSON, "bdtRefId");

    if (bdt_ref_id) {
    if (!cJSON_IsString(bdt_ref_id)) {
        ogs_error("OpenAPI_app_session_context_update_data_parseFromJSON() failed [bdt_ref_id]");
        goto end;
    }
    }

    cJSON *ev_subsc = cJSON_GetObjectItemCaseSensitive(app_session_context_update_dataJSON, "evSubsc");

    OpenAPI_events_subsc_req_data_rm_t *ev_subsc_local_nonprim = NULL;
    if (ev_subsc) {
    ev_subsc_local_nonprim = OpenAPI_events_subsc_req_data_rm_parseFromJSON(ev_subsc);
    }

    cJSON *mcptt_id = cJSON_GetObjectItemCaseSensitive(app_session_context_update_dataJSON, "mcpttId");

    if (mcptt_id) {
    if (!cJSON_IsString(mcptt_id)) {
        ogs_error("OpenAPI_app_session_context_update_data_parseFromJSON() failed [mcptt_id]");
        goto end;
    }
    }

    cJSON *mc_video_id = cJSON_GetObjectItemCaseSensitive(app_session_context_update_dataJSON, "mcVideoId");

    if (mc_video_id) {
    if (!cJSON_IsString(mc_video_id)) {
        ogs_error("OpenAPI_app_session_context_update_data_parseFromJSON() failed [mc_video_id]");
        goto end;
    }
    }

    cJSON *med_components = cJSON_GetObjectItemCaseSensitive(app_session_context_update_dataJSON, "medComponents");

    OpenAPI_list_t *med_componentsList;
    if (med_components) {
    cJSON *med_components_local_map;
    if (!cJSON_IsObject(med_components)) {
        ogs_error("OpenAPI_app_session_context_update_data_parseFromJSON() failed [med_components]");
        goto end;
    }
    med_componentsList = OpenAPI_list_create();
    OpenAPI_map_t *localMapKeyPair = NULL;
    cJSON_ArrayForEach(med_components_local_map, med_components) {
        cJSON *localMapObject = med_components_local_map;
        if (cJSON_IsObject(med_components_local_map)) {
            localMapKeyPair = OpenAPI_map_create(
                ogs_strdup(localMapObject->string), OpenAPI_media_component_rm_parseFromJSON(localMapObject));
        } else if (cJSON_IsNull(med_components_local_map)) {
            localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
        } else {
            ogs_error("OpenAPI_app_session_context_update_data_parseFromJSON() failed [med_components]");
            goto end;
        }
        OpenAPI_list_add(med_componentsList , localMapKeyPair);
    }
    }

    cJSON *mps_id = cJSON_GetObjectItemCaseSensitive(app_session_context_update_dataJSON, "mpsId");

    if (mps_id) {
    if (!cJSON_IsString(mps_id)) {
        ogs_error("OpenAPI_app_session_context_update_data_parseFromJSON() failed [mps_id]");
        goto end;
    }
    }

    cJSON *mcs_id = cJSON_GetObjectItemCaseSensitive(app_session_context_update_dataJSON, "mcsId");

    if (mcs_id) {
    if (!cJSON_IsString(mcs_id)) {
        ogs_error("OpenAPI_app_session_context_update_data_parseFromJSON() failed [mcs_id]");
        goto end;
    }
    }

    cJSON *preempt_control_info = cJSON_GetObjectItemCaseSensitive(app_session_context_update_dataJSON, "preemptControlInfo");

    OpenAPI_preemption_control_information_rm_t *preempt_control_info_local_nonprim = NULL;
    if (preempt_control_info) {
    preempt_control_info_local_nonprim = OpenAPI_preemption_control_information_rm_parseFromJSON(preempt_control_info);
    }

    cJSON *res_prio = cJSON_GetObjectItemCaseSensitive(app_session_context_update_dataJSON, "resPrio");

    OpenAPI_reserv_priority_e res_prioVariable;
    if (res_prio) {
    if (!cJSON_IsString(res_prio)) {
        ogs_error("OpenAPI_app_session_context_update_data_parseFromJSON() failed [res_prio]");
        goto end;
    }
    res_prioVariable = OpenAPI_reserv_priority_FromString(res_prio->valuestring);
    }

    cJSON *serv_inf_status = cJSON_GetObjectItemCaseSensitive(app_session_context_update_dataJSON, "servInfStatus");

    OpenAPI_service_info_status_e serv_inf_statusVariable;
    if (serv_inf_status) {
    if (!cJSON_IsString(serv_inf_status)) {
        ogs_error("OpenAPI_app_session_context_update_data_parseFromJSON() failed [serv_inf_status]");
        goto end;
    }
    serv_inf_statusVariable = OpenAPI_service_info_status_FromString(serv_inf_status->valuestring);
    }

    cJSON *sip_fork_ind = cJSON_GetObjectItemCaseSensitive(app_session_context_update_dataJSON, "sipForkInd");

    OpenAPI_sip_forking_indication_e sip_fork_indVariable;
    if (sip_fork_ind) {
    if (!cJSON_IsString(sip_fork_ind)) {
        ogs_error("OpenAPI_app_session_context_update_data_parseFromJSON() failed [sip_fork_ind]");
        goto end;
    }
    sip_fork_indVariable = OpenAPI_sip_forking_indication_FromString(sip_fork_ind->valuestring);
    }

    cJSON *spon_id = cJSON_GetObjectItemCaseSensitive(app_session_context_update_dataJSON, "sponId");

    if (spon_id) {
    if (!cJSON_IsString(spon_id)) {
        ogs_error("OpenAPI_app_session_context_update_data_parseFromJSON() failed [spon_id]");
        goto end;
    }
    }

    cJSON *spon_status = cJSON_GetObjectItemCaseSensitive(app_session_context_update_dataJSON, "sponStatus");

    OpenAPI_sponsoring_status_e spon_statusVariable;
    if (spon_status) {
    if (!cJSON_IsString(spon_status)) {
        ogs_error("OpenAPI_app_session_context_update_data_parseFromJSON() failed [spon_status]");
        goto end;
    }
    spon_statusVariable = OpenAPI_sponsoring_status_FromString(spon_status->valuestring);
    }

    cJSON *tsn_bridge_man_cont = cJSON_GetObjectItemCaseSensitive(app_session_context_update_dataJSON, "tsnBridgeManCont");

    OpenAPI_bridge_management_container_t *tsn_bridge_man_cont_local_nonprim = NULL;
    if (tsn_bridge_man_cont) {
    tsn_bridge_man_cont_local_nonprim = OpenAPI_bridge_management_container_parseFromJSON(tsn_bridge_man_cont);
    }

    cJSON *tsn_port_man_cont_dstt = cJSON_GetObjectItemCaseSensitive(app_session_context_update_dataJSON, "tsnPortManContDstt");

    OpenAPI_port_management_container_t *tsn_port_man_cont_dstt_local_nonprim = NULL;
    if (tsn_port_man_cont_dstt) {
    tsn_port_man_cont_dstt_local_nonprim = OpenAPI_port_management_container_parseFromJSON(tsn_port_man_cont_dstt);
    }

    cJSON *tsn_port_man_cont_nwtts = cJSON_GetObjectItemCaseSensitive(app_session_context_update_dataJSON, "tsnPortManContNwtts");

    OpenAPI_list_t *tsn_port_man_cont_nwttsList;
    if (tsn_port_man_cont_nwtts) {
    cJSON *tsn_port_man_cont_nwtts_local_nonprimitive;
    if (!cJSON_IsArray(tsn_port_man_cont_nwtts)){
        ogs_error("OpenAPI_app_session_context_update_data_parseFromJSON() failed [tsn_port_man_cont_nwtts]");
        goto end;
    }

    tsn_port_man_cont_nwttsList = OpenAPI_list_create();

    cJSON_ArrayForEach(tsn_port_man_cont_nwtts_local_nonprimitive, tsn_port_man_cont_nwtts ) {
        if (!cJSON_IsObject(tsn_port_man_cont_nwtts_local_nonprimitive)) {
            ogs_error("OpenAPI_app_session_context_update_data_parseFromJSON() failed [tsn_port_man_cont_nwtts]");
            goto end;
        }
        OpenAPI_port_management_container_t *tsn_port_man_cont_nwttsItem = OpenAPI_port_management_container_parseFromJSON(tsn_port_man_cont_nwtts_local_nonprimitive);

        if (!tsn_port_man_cont_nwttsItem) {
            ogs_error("No tsn_port_man_cont_nwttsItem");
            OpenAPI_list_free(tsn_port_man_cont_nwttsList);
            goto end;
        }

        OpenAPI_list_add(tsn_port_man_cont_nwttsList, tsn_port_man_cont_nwttsItem);
    }
    }

    app_session_context_update_data_local_var = OpenAPI_app_session_context_update_data_create (
        af_app_id ? ogs_strdup(af_app_id->valuestring) : NULL,
        af_rout_req ? af_rout_req_local_nonprim : NULL,
        asp_id ? ogs_strdup(asp_id->valuestring) : NULL,
        bdt_ref_id ? ogs_strdup(bdt_ref_id->valuestring) : NULL,
        ev_subsc ? ev_subsc_local_nonprim : NULL,
        mcptt_id ? ogs_strdup(mcptt_id->valuestring) : NULL,
        mc_video_id ? ogs_strdup(mc_video_id->valuestring) : NULL,
        med_components ? med_componentsList : NULL,
        mps_id ? ogs_strdup(mps_id->valuestring) : NULL,
        mcs_id ? ogs_strdup(mcs_id->valuestring) : NULL,
        preempt_control_info ? preempt_control_info_local_nonprim : NULL,
        res_prio ? res_prioVariable : 0,
        serv_inf_status ? serv_inf_statusVariable : 0,
        sip_fork_ind ? sip_fork_indVariable : 0,
        spon_id ? ogs_strdup(spon_id->valuestring) : NULL,
        spon_status ? spon_statusVariable : 0,
        tsn_bridge_man_cont ? tsn_bridge_man_cont_local_nonprim : NULL,
        tsn_port_man_cont_dstt ? tsn_port_man_cont_dstt_local_nonprim : NULL,
        tsn_port_man_cont_nwtts ? tsn_port_man_cont_nwttsList : NULL
    );

    return app_session_context_update_data_local_var;
end:
    return NULL;
}

OpenAPI_app_session_context_update_data_t *OpenAPI_app_session_context_update_data_copy(OpenAPI_app_session_context_update_data_t *dst, OpenAPI_app_session_context_update_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_app_session_context_update_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_app_session_context_update_data_convertToJSON() failed");
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

    OpenAPI_app_session_context_update_data_free(dst);
    dst = OpenAPI_app_session_context_update_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

