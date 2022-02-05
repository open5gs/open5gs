
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "app_session_context_req_data.h"

OpenAPI_app_session_context_req_data_t *OpenAPI_app_session_context_req_data_create(
    char *af_app_id,
    char *af_charg_id,
    OpenAPI_af_requested_data_e af_req_data,
    OpenAPI_af_routing_requirement_t *af_rout_req,
    char *asp_id,
    char *bdt_ref_id,
    char *dnn,
    OpenAPI_events_subsc_req_data_t *ev_subsc,
    char *mcptt_id,
    char *mc_video_id,
    OpenAPI_list_t* med_components,
    char *ip_domain,
    char *mps_id,
    char *mcs_id,
    OpenAPI_preemption_control_information_e preempt_control_info,
    OpenAPI_reserv_priority_e res_prio,
    OpenAPI_service_info_status_e serv_inf_status,
    char *notif_uri,
    char *serv_urn,
    OpenAPI_snssai_t *slice_info,
    char *spon_id,
    OpenAPI_sponsoring_status_e spon_status,
    char *supi,
    char *gpsi,
    char *supp_feat,
    char *ue_ipv4,
    char *ue_ipv6,
    char *ue_mac,
    OpenAPI_bridge_management_container_t *tsn_bridge_man_cont,
    OpenAPI_port_management_container_t *tsn_port_man_cont_dstt,
    OpenAPI_list_t *tsn_port_man_cont_nwtts
)
{
    OpenAPI_app_session_context_req_data_t *app_session_context_req_data_local_var = ogs_malloc(sizeof(OpenAPI_app_session_context_req_data_t));
    ogs_assert(app_session_context_req_data_local_var);

    app_session_context_req_data_local_var->af_app_id = af_app_id;
    app_session_context_req_data_local_var->af_charg_id = af_charg_id;
    app_session_context_req_data_local_var->af_req_data = af_req_data;
    app_session_context_req_data_local_var->af_rout_req = af_rout_req;
    app_session_context_req_data_local_var->asp_id = asp_id;
    app_session_context_req_data_local_var->bdt_ref_id = bdt_ref_id;
    app_session_context_req_data_local_var->dnn = dnn;
    app_session_context_req_data_local_var->ev_subsc = ev_subsc;
    app_session_context_req_data_local_var->mcptt_id = mcptt_id;
    app_session_context_req_data_local_var->mc_video_id = mc_video_id;
    app_session_context_req_data_local_var->med_components = med_components;
    app_session_context_req_data_local_var->ip_domain = ip_domain;
    app_session_context_req_data_local_var->mps_id = mps_id;
    app_session_context_req_data_local_var->mcs_id = mcs_id;
    app_session_context_req_data_local_var->preempt_control_info = preempt_control_info;
    app_session_context_req_data_local_var->res_prio = res_prio;
    app_session_context_req_data_local_var->serv_inf_status = serv_inf_status;
    app_session_context_req_data_local_var->notif_uri = notif_uri;
    app_session_context_req_data_local_var->serv_urn = serv_urn;
    app_session_context_req_data_local_var->slice_info = slice_info;
    app_session_context_req_data_local_var->spon_id = spon_id;
    app_session_context_req_data_local_var->spon_status = spon_status;
    app_session_context_req_data_local_var->supi = supi;
    app_session_context_req_data_local_var->gpsi = gpsi;
    app_session_context_req_data_local_var->supp_feat = supp_feat;
    app_session_context_req_data_local_var->ue_ipv4 = ue_ipv4;
    app_session_context_req_data_local_var->ue_ipv6 = ue_ipv6;
    app_session_context_req_data_local_var->ue_mac = ue_mac;
    app_session_context_req_data_local_var->tsn_bridge_man_cont = tsn_bridge_man_cont;
    app_session_context_req_data_local_var->tsn_port_man_cont_dstt = tsn_port_man_cont_dstt;
    app_session_context_req_data_local_var->tsn_port_man_cont_nwtts = tsn_port_man_cont_nwtts;

    return app_session_context_req_data_local_var;
}

void OpenAPI_app_session_context_req_data_free(OpenAPI_app_session_context_req_data_t *app_session_context_req_data)
{
    if (NULL == app_session_context_req_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(app_session_context_req_data->af_app_id);
    ogs_free(app_session_context_req_data->af_charg_id);
    OpenAPI_af_routing_requirement_free(app_session_context_req_data->af_rout_req);
    ogs_free(app_session_context_req_data->asp_id);
    ogs_free(app_session_context_req_data->bdt_ref_id);
    ogs_free(app_session_context_req_data->dnn);
    OpenAPI_events_subsc_req_data_free(app_session_context_req_data->ev_subsc);
    ogs_free(app_session_context_req_data->mcptt_id);
    ogs_free(app_session_context_req_data->mc_video_id);
    OpenAPI_list_for_each(app_session_context_req_data->med_components, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        ogs_free(localKeyValue->key);
        OpenAPI_media_component_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(app_session_context_req_data->med_components);
    ogs_free(app_session_context_req_data->ip_domain);
    ogs_free(app_session_context_req_data->mps_id);
    ogs_free(app_session_context_req_data->mcs_id);
    ogs_free(app_session_context_req_data->notif_uri);
    ogs_free(app_session_context_req_data->serv_urn);
    OpenAPI_snssai_free(app_session_context_req_data->slice_info);
    ogs_free(app_session_context_req_data->spon_id);
    ogs_free(app_session_context_req_data->supi);
    ogs_free(app_session_context_req_data->gpsi);
    ogs_free(app_session_context_req_data->supp_feat);
    ogs_free(app_session_context_req_data->ue_ipv4);
    ogs_free(app_session_context_req_data->ue_ipv6);
    ogs_free(app_session_context_req_data->ue_mac);
    OpenAPI_bridge_management_container_free(app_session_context_req_data->tsn_bridge_man_cont);
    OpenAPI_port_management_container_free(app_session_context_req_data->tsn_port_man_cont_dstt);
    OpenAPI_list_for_each(app_session_context_req_data->tsn_port_man_cont_nwtts, node) {
        OpenAPI_port_management_container_free(node->data);
    }
    OpenAPI_list_free(app_session_context_req_data->tsn_port_man_cont_nwtts);
    ogs_free(app_session_context_req_data);
}

cJSON *OpenAPI_app_session_context_req_data_convertToJSON(OpenAPI_app_session_context_req_data_t *app_session_context_req_data)
{
    cJSON *item = NULL;

    if (app_session_context_req_data == NULL) {
        ogs_error("OpenAPI_app_session_context_req_data_convertToJSON() failed [AppSessionContextReqData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (app_session_context_req_data->af_app_id) {
    if (cJSON_AddStringToObject(item, "afAppId", app_session_context_req_data->af_app_id) == NULL) {
        ogs_error("OpenAPI_app_session_context_req_data_convertToJSON() failed [af_app_id]");
        goto end;
    }
    }

    if (app_session_context_req_data->af_charg_id) {
    if (cJSON_AddStringToObject(item, "afChargId", app_session_context_req_data->af_charg_id) == NULL) {
        ogs_error("OpenAPI_app_session_context_req_data_convertToJSON() failed [af_charg_id]");
        goto end;
    }
    }

    if (app_session_context_req_data->af_req_data) {
    if (cJSON_AddStringToObject(item, "afReqData", OpenAPI_af_requested_data_ToString(app_session_context_req_data->af_req_data)) == NULL) {
        ogs_error("OpenAPI_app_session_context_req_data_convertToJSON() failed [af_req_data]");
        goto end;
    }
    }

    if (app_session_context_req_data->af_rout_req) {
    cJSON *af_rout_req_local_JSON = OpenAPI_af_routing_requirement_convertToJSON(app_session_context_req_data->af_rout_req);
    if (af_rout_req_local_JSON == NULL) {
        ogs_error("OpenAPI_app_session_context_req_data_convertToJSON() failed [af_rout_req]");
        goto end;
    }
    cJSON_AddItemToObject(item, "afRoutReq", af_rout_req_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_app_session_context_req_data_convertToJSON() failed [af_rout_req]");
        goto end;
    }
    }

    if (app_session_context_req_data->asp_id) {
    if (cJSON_AddStringToObject(item, "aspId", app_session_context_req_data->asp_id) == NULL) {
        ogs_error("OpenAPI_app_session_context_req_data_convertToJSON() failed [asp_id]");
        goto end;
    }
    }

    if (app_session_context_req_data->bdt_ref_id) {
    if (cJSON_AddStringToObject(item, "bdtRefId", app_session_context_req_data->bdt_ref_id) == NULL) {
        ogs_error("OpenAPI_app_session_context_req_data_convertToJSON() failed [bdt_ref_id]");
        goto end;
    }
    }

    if (app_session_context_req_data->dnn) {
    if (cJSON_AddStringToObject(item, "dnn", app_session_context_req_data->dnn) == NULL) {
        ogs_error("OpenAPI_app_session_context_req_data_convertToJSON() failed [dnn]");
        goto end;
    }
    }

    if (app_session_context_req_data->ev_subsc) {
    cJSON *ev_subsc_local_JSON = OpenAPI_events_subsc_req_data_convertToJSON(app_session_context_req_data->ev_subsc);
    if (ev_subsc_local_JSON == NULL) {
        ogs_error("OpenAPI_app_session_context_req_data_convertToJSON() failed [ev_subsc]");
        goto end;
    }
    cJSON_AddItemToObject(item, "evSubsc", ev_subsc_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_app_session_context_req_data_convertToJSON() failed [ev_subsc]");
        goto end;
    }
    }

    if (app_session_context_req_data->mcptt_id) {
    if (cJSON_AddStringToObject(item, "mcpttId", app_session_context_req_data->mcptt_id) == NULL) {
        ogs_error("OpenAPI_app_session_context_req_data_convertToJSON() failed [mcptt_id]");
        goto end;
    }
    }

    if (app_session_context_req_data->mc_video_id) {
    if (cJSON_AddStringToObject(item, "mcVideoId", app_session_context_req_data->mc_video_id) == NULL) {
        ogs_error("OpenAPI_app_session_context_req_data_convertToJSON() failed [mc_video_id]");
        goto end;
    }
    }

    if (app_session_context_req_data->med_components) {
    cJSON *med_components = cJSON_AddObjectToObject(item, "medComponents");
    if (med_components == NULL) {
        ogs_error("OpenAPI_app_session_context_req_data_convertToJSON() failed [med_components]");
        goto end;
    }
    cJSON *localMapObject = med_components;
    OpenAPI_lnode_t *med_components_node;
    if (app_session_context_req_data->med_components) {
        OpenAPI_list_for_each(app_session_context_req_data->med_components, med_components_node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)med_components_node->data;
        cJSON *itemLocal = localKeyValue->value ?
            OpenAPI_media_component_convertToJSON(localKeyValue->value) :
            cJSON_CreateNull();
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_app_session_context_req_data_convertToJSON() failed [med_components]");
            goto end;
        }
        cJSON_AddItemToObject(med_components, localKeyValue->key, itemLocal);
            }
        }
    }

    if (app_session_context_req_data->ip_domain) {
    if (cJSON_AddStringToObject(item, "ipDomain", app_session_context_req_data->ip_domain) == NULL) {
        ogs_error("OpenAPI_app_session_context_req_data_convertToJSON() failed [ip_domain]");
        goto end;
    }
    }

    if (app_session_context_req_data->mps_id) {
    if (cJSON_AddStringToObject(item, "mpsId", app_session_context_req_data->mps_id) == NULL) {
        ogs_error("OpenAPI_app_session_context_req_data_convertToJSON() failed [mps_id]");
        goto end;
    }
    }

    if (app_session_context_req_data->mcs_id) {
    if (cJSON_AddStringToObject(item, "mcsId", app_session_context_req_data->mcs_id) == NULL) {
        ogs_error("OpenAPI_app_session_context_req_data_convertToJSON() failed [mcs_id]");
        goto end;
    }
    }

    if (app_session_context_req_data->preempt_control_info) {
    if (cJSON_AddStringToObject(item, "preemptControlInfo", OpenAPI_preemption_control_information_ToString(app_session_context_req_data->preempt_control_info)) == NULL) {
        ogs_error("OpenAPI_app_session_context_req_data_convertToJSON() failed [preempt_control_info]");
        goto end;
    }
    }

    if (app_session_context_req_data->res_prio) {
    if (cJSON_AddStringToObject(item, "resPrio", OpenAPI_reserv_priority_ToString(app_session_context_req_data->res_prio)) == NULL) {
        ogs_error("OpenAPI_app_session_context_req_data_convertToJSON() failed [res_prio]");
        goto end;
    }
    }

    if (app_session_context_req_data->serv_inf_status) {
    if (cJSON_AddStringToObject(item, "servInfStatus", OpenAPI_service_info_status_ToString(app_session_context_req_data->serv_inf_status)) == NULL) {
        ogs_error("OpenAPI_app_session_context_req_data_convertToJSON() failed [serv_inf_status]");
        goto end;
    }
    }

    if (cJSON_AddStringToObject(item, "notifUri", app_session_context_req_data->notif_uri) == NULL) {
        ogs_error("OpenAPI_app_session_context_req_data_convertToJSON() failed [notif_uri]");
        goto end;
    }

    if (app_session_context_req_data->serv_urn) {
    if (cJSON_AddStringToObject(item, "servUrn", app_session_context_req_data->serv_urn) == NULL) {
        ogs_error("OpenAPI_app_session_context_req_data_convertToJSON() failed [serv_urn]");
        goto end;
    }
    }

    if (app_session_context_req_data->slice_info) {
    cJSON *slice_info_local_JSON = OpenAPI_snssai_convertToJSON(app_session_context_req_data->slice_info);
    if (slice_info_local_JSON == NULL) {
        ogs_error("OpenAPI_app_session_context_req_data_convertToJSON() failed [slice_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sliceInfo", slice_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_app_session_context_req_data_convertToJSON() failed [slice_info]");
        goto end;
    }
    }

    if (app_session_context_req_data->spon_id) {
    if (cJSON_AddStringToObject(item, "sponId", app_session_context_req_data->spon_id) == NULL) {
        ogs_error("OpenAPI_app_session_context_req_data_convertToJSON() failed [spon_id]");
        goto end;
    }
    }

    if (app_session_context_req_data->spon_status) {
    if (cJSON_AddStringToObject(item, "sponStatus", OpenAPI_sponsoring_status_ToString(app_session_context_req_data->spon_status)) == NULL) {
        ogs_error("OpenAPI_app_session_context_req_data_convertToJSON() failed [spon_status]");
        goto end;
    }
    }

    if (app_session_context_req_data->supi) {
    if (cJSON_AddStringToObject(item, "supi", app_session_context_req_data->supi) == NULL) {
        ogs_error("OpenAPI_app_session_context_req_data_convertToJSON() failed [supi]");
        goto end;
    }
    }

    if (app_session_context_req_data->gpsi) {
    if (cJSON_AddStringToObject(item, "gpsi", app_session_context_req_data->gpsi) == NULL) {
        ogs_error("OpenAPI_app_session_context_req_data_convertToJSON() failed [gpsi]");
        goto end;
    }
    }

    if (cJSON_AddStringToObject(item, "suppFeat", app_session_context_req_data->supp_feat) == NULL) {
        ogs_error("OpenAPI_app_session_context_req_data_convertToJSON() failed [supp_feat]");
        goto end;
    }

    if (app_session_context_req_data->ue_ipv4) {
    if (cJSON_AddStringToObject(item, "ueIpv4", app_session_context_req_data->ue_ipv4) == NULL) {
        ogs_error("OpenAPI_app_session_context_req_data_convertToJSON() failed [ue_ipv4]");
        goto end;
    }
    }

    if (app_session_context_req_data->ue_ipv6) {
    if (cJSON_AddStringToObject(item, "ueIpv6", app_session_context_req_data->ue_ipv6) == NULL) {
        ogs_error("OpenAPI_app_session_context_req_data_convertToJSON() failed [ue_ipv6]");
        goto end;
    }
    }

    if (app_session_context_req_data->ue_mac) {
    if (cJSON_AddStringToObject(item, "ueMac", app_session_context_req_data->ue_mac) == NULL) {
        ogs_error("OpenAPI_app_session_context_req_data_convertToJSON() failed [ue_mac]");
        goto end;
    }
    }

    if (app_session_context_req_data->tsn_bridge_man_cont) {
    cJSON *tsn_bridge_man_cont_local_JSON = OpenAPI_bridge_management_container_convertToJSON(app_session_context_req_data->tsn_bridge_man_cont);
    if (tsn_bridge_man_cont_local_JSON == NULL) {
        ogs_error("OpenAPI_app_session_context_req_data_convertToJSON() failed [tsn_bridge_man_cont]");
        goto end;
    }
    cJSON_AddItemToObject(item, "tsnBridgeManCont", tsn_bridge_man_cont_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_app_session_context_req_data_convertToJSON() failed [tsn_bridge_man_cont]");
        goto end;
    }
    }

    if (app_session_context_req_data->tsn_port_man_cont_dstt) {
    cJSON *tsn_port_man_cont_dstt_local_JSON = OpenAPI_port_management_container_convertToJSON(app_session_context_req_data->tsn_port_man_cont_dstt);
    if (tsn_port_man_cont_dstt_local_JSON == NULL) {
        ogs_error("OpenAPI_app_session_context_req_data_convertToJSON() failed [tsn_port_man_cont_dstt]");
        goto end;
    }
    cJSON_AddItemToObject(item, "tsnPortManContDstt", tsn_port_man_cont_dstt_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_app_session_context_req_data_convertToJSON() failed [tsn_port_man_cont_dstt]");
        goto end;
    }
    }

    if (app_session_context_req_data->tsn_port_man_cont_nwtts) {
    cJSON *tsn_port_man_cont_nwttsList = cJSON_AddArrayToObject(item, "tsnPortManContNwtts");
    if (tsn_port_man_cont_nwttsList == NULL) {
        ogs_error("OpenAPI_app_session_context_req_data_convertToJSON() failed [tsn_port_man_cont_nwtts]");
        goto end;
    }

    OpenAPI_lnode_t *tsn_port_man_cont_nwtts_node;
    if (app_session_context_req_data->tsn_port_man_cont_nwtts) {
        OpenAPI_list_for_each(app_session_context_req_data->tsn_port_man_cont_nwtts, tsn_port_man_cont_nwtts_node) {
            cJSON *itemLocal = OpenAPI_port_management_container_convertToJSON(tsn_port_man_cont_nwtts_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_app_session_context_req_data_convertToJSON() failed [tsn_port_man_cont_nwtts]");
                goto end;
            }
            cJSON_AddItemToArray(tsn_port_man_cont_nwttsList, itemLocal);
        }
    }
    }

end:
    return item;
}

OpenAPI_app_session_context_req_data_t *OpenAPI_app_session_context_req_data_parseFromJSON(cJSON *app_session_context_req_dataJSON)
{
    OpenAPI_app_session_context_req_data_t *app_session_context_req_data_local_var = NULL;
    cJSON *af_app_id = cJSON_GetObjectItemCaseSensitive(app_session_context_req_dataJSON, "afAppId");

    if (af_app_id) {
    if (!cJSON_IsString(af_app_id)) {
        ogs_error("OpenAPI_app_session_context_req_data_parseFromJSON() failed [af_app_id]");
        goto end;
    }
    }

    cJSON *af_charg_id = cJSON_GetObjectItemCaseSensitive(app_session_context_req_dataJSON, "afChargId");

    if (af_charg_id) {
    if (!cJSON_IsString(af_charg_id)) {
        ogs_error("OpenAPI_app_session_context_req_data_parseFromJSON() failed [af_charg_id]");
        goto end;
    }
    }

    cJSON *af_req_data = cJSON_GetObjectItemCaseSensitive(app_session_context_req_dataJSON, "afReqData");

    OpenAPI_af_requested_data_e af_req_dataVariable;
    if (af_req_data) {
    if (!cJSON_IsString(af_req_data)) {
        ogs_error("OpenAPI_app_session_context_req_data_parseFromJSON() failed [af_req_data]");
        goto end;
    }
    af_req_dataVariable = OpenAPI_af_requested_data_FromString(af_req_data->valuestring);
    }

    cJSON *af_rout_req = cJSON_GetObjectItemCaseSensitive(app_session_context_req_dataJSON, "afRoutReq");

    OpenAPI_af_routing_requirement_t *af_rout_req_local_nonprim = NULL;
    if (af_rout_req) {
    af_rout_req_local_nonprim = OpenAPI_af_routing_requirement_parseFromJSON(af_rout_req);
    }

    cJSON *asp_id = cJSON_GetObjectItemCaseSensitive(app_session_context_req_dataJSON, "aspId");

    if (asp_id) {
    if (!cJSON_IsString(asp_id)) {
        ogs_error("OpenAPI_app_session_context_req_data_parseFromJSON() failed [asp_id]");
        goto end;
    }
    }

    cJSON *bdt_ref_id = cJSON_GetObjectItemCaseSensitive(app_session_context_req_dataJSON, "bdtRefId");

    if (bdt_ref_id) {
    if (!cJSON_IsString(bdt_ref_id)) {
        ogs_error("OpenAPI_app_session_context_req_data_parseFromJSON() failed [bdt_ref_id]");
        goto end;
    }
    }

    cJSON *dnn = cJSON_GetObjectItemCaseSensitive(app_session_context_req_dataJSON, "dnn");

    if (dnn) {
    if (!cJSON_IsString(dnn)) {
        ogs_error("OpenAPI_app_session_context_req_data_parseFromJSON() failed [dnn]");
        goto end;
    }
    }

    cJSON *ev_subsc = cJSON_GetObjectItemCaseSensitive(app_session_context_req_dataJSON, "evSubsc");

    OpenAPI_events_subsc_req_data_t *ev_subsc_local_nonprim = NULL;
    if (ev_subsc) {
    ev_subsc_local_nonprim = OpenAPI_events_subsc_req_data_parseFromJSON(ev_subsc);
    }

    cJSON *mcptt_id = cJSON_GetObjectItemCaseSensitive(app_session_context_req_dataJSON, "mcpttId");

    if (mcptt_id) {
    if (!cJSON_IsString(mcptt_id)) {
        ogs_error("OpenAPI_app_session_context_req_data_parseFromJSON() failed [mcptt_id]");
        goto end;
    }
    }

    cJSON *mc_video_id = cJSON_GetObjectItemCaseSensitive(app_session_context_req_dataJSON, "mcVideoId");

    if (mc_video_id) {
    if (!cJSON_IsString(mc_video_id)) {
        ogs_error("OpenAPI_app_session_context_req_data_parseFromJSON() failed [mc_video_id]");
        goto end;
    }
    }

    cJSON *med_components = cJSON_GetObjectItemCaseSensitive(app_session_context_req_dataJSON, "medComponents");

    OpenAPI_list_t *med_componentsList;
    if (med_components) {
    cJSON *med_components_local_map;
    if (!cJSON_IsObject(med_components)) {
        ogs_error("OpenAPI_app_session_context_req_data_parseFromJSON() failed [med_components]");
        goto end;
    }
    med_componentsList = OpenAPI_list_create();
    OpenAPI_map_t *localMapKeyPair = NULL;
    cJSON_ArrayForEach(med_components_local_map, med_components) {
        cJSON *localMapObject = med_components_local_map;
        if (cJSON_IsObject(med_components_local_map)) {
            localMapKeyPair = OpenAPI_map_create(
                ogs_strdup(localMapObject->string), OpenAPI_media_component_parseFromJSON(localMapObject));
        } else if (cJSON_IsNull(med_components_local_map)) {
            localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
        } else {
            ogs_error("OpenAPI_app_session_context_req_data_parseFromJSON() failed [med_components]");
            goto end;
        }
        OpenAPI_list_add(med_componentsList , localMapKeyPair);
    }
    }

    cJSON *ip_domain = cJSON_GetObjectItemCaseSensitive(app_session_context_req_dataJSON, "ipDomain");

    if (ip_domain) {
    if (!cJSON_IsString(ip_domain)) {
        ogs_error("OpenAPI_app_session_context_req_data_parseFromJSON() failed [ip_domain]");
        goto end;
    }
    }

    cJSON *mps_id = cJSON_GetObjectItemCaseSensitive(app_session_context_req_dataJSON, "mpsId");

    if (mps_id) {
    if (!cJSON_IsString(mps_id)) {
        ogs_error("OpenAPI_app_session_context_req_data_parseFromJSON() failed [mps_id]");
        goto end;
    }
    }

    cJSON *mcs_id = cJSON_GetObjectItemCaseSensitive(app_session_context_req_dataJSON, "mcsId");

    if (mcs_id) {
    if (!cJSON_IsString(mcs_id)) {
        ogs_error("OpenAPI_app_session_context_req_data_parseFromJSON() failed [mcs_id]");
        goto end;
    }
    }

    cJSON *preempt_control_info = cJSON_GetObjectItemCaseSensitive(app_session_context_req_dataJSON, "preemptControlInfo");

    OpenAPI_preemption_control_information_e preempt_control_infoVariable;
    if (preempt_control_info) {
    if (!cJSON_IsString(preempt_control_info)) {
        ogs_error("OpenAPI_app_session_context_req_data_parseFromJSON() failed [preempt_control_info]");
        goto end;
    }
    preempt_control_infoVariable = OpenAPI_preemption_control_information_FromString(preempt_control_info->valuestring);
    }

    cJSON *res_prio = cJSON_GetObjectItemCaseSensitive(app_session_context_req_dataJSON, "resPrio");

    OpenAPI_reserv_priority_e res_prioVariable;
    if (res_prio) {
    if (!cJSON_IsString(res_prio)) {
        ogs_error("OpenAPI_app_session_context_req_data_parseFromJSON() failed [res_prio]");
        goto end;
    }
    res_prioVariable = OpenAPI_reserv_priority_FromString(res_prio->valuestring);
    }

    cJSON *serv_inf_status = cJSON_GetObjectItemCaseSensitive(app_session_context_req_dataJSON, "servInfStatus");

    OpenAPI_service_info_status_e serv_inf_statusVariable;
    if (serv_inf_status) {
    if (!cJSON_IsString(serv_inf_status)) {
        ogs_error("OpenAPI_app_session_context_req_data_parseFromJSON() failed [serv_inf_status]");
        goto end;
    }
    serv_inf_statusVariable = OpenAPI_service_info_status_FromString(serv_inf_status->valuestring);
    }

    cJSON *notif_uri = cJSON_GetObjectItemCaseSensitive(app_session_context_req_dataJSON, "notifUri");
    if (!notif_uri) {
        ogs_error("OpenAPI_app_session_context_req_data_parseFromJSON() failed [notif_uri]");
        goto end;
    }

    if (!cJSON_IsString(notif_uri)) {
        ogs_error("OpenAPI_app_session_context_req_data_parseFromJSON() failed [notif_uri]");
        goto end;
    }

    cJSON *serv_urn = cJSON_GetObjectItemCaseSensitive(app_session_context_req_dataJSON, "servUrn");

    if (serv_urn) {
    if (!cJSON_IsString(serv_urn)) {
        ogs_error("OpenAPI_app_session_context_req_data_parseFromJSON() failed [serv_urn]");
        goto end;
    }
    }

    cJSON *slice_info = cJSON_GetObjectItemCaseSensitive(app_session_context_req_dataJSON, "sliceInfo");

    OpenAPI_snssai_t *slice_info_local_nonprim = NULL;
    if (slice_info) {
    slice_info_local_nonprim = OpenAPI_snssai_parseFromJSON(slice_info);
    }

    cJSON *spon_id = cJSON_GetObjectItemCaseSensitive(app_session_context_req_dataJSON, "sponId");

    if (spon_id) {
    if (!cJSON_IsString(spon_id)) {
        ogs_error("OpenAPI_app_session_context_req_data_parseFromJSON() failed [spon_id]");
        goto end;
    }
    }

    cJSON *spon_status = cJSON_GetObjectItemCaseSensitive(app_session_context_req_dataJSON, "sponStatus");

    OpenAPI_sponsoring_status_e spon_statusVariable;
    if (spon_status) {
    if (!cJSON_IsString(spon_status)) {
        ogs_error("OpenAPI_app_session_context_req_data_parseFromJSON() failed [spon_status]");
        goto end;
    }
    spon_statusVariable = OpenAPI_sponsoring_status_FromString(spon_status->valuestring);
    }

    cJSON *supi = cJSON_GetObjectItemCaseSensitive(app_session_context_req_dataJSON, "supi");

    if (supi) {
    if (!cJSON_IsString(supi)) {
        ogs_error("OpenAPI_app_session_context_req_data_parseFromJSON() failed [supi]");
        goto end;
    }
    }

    cJSON *gpsi = cJSON_GetObjectItemCaseSensitive(app_session_context_req_dataJSON, "gpsi");

    if (gpsi) {
    if (!cJSON_IsString(gpsi)) {
        ogs_error("OpenAPI_app_session_context_req_data_parseFromJSON() failed [gpsi]");
        goto end;
    }
    }

    cJSON *supp_feat = cJSON_GetObjectItemCaseSensitive(app_session_context_req_dataJSON, "suppFeat");
    if (!supp_feat) {
        ogs_error("OpenAPI_app_session_context_req_data_parseFromJSON() failed [supp_feat]");
        goto end;
    }

    if (!cJSON_IsString(supp_feat)) {
        ogs_error("OpenAPI_app_session_context_req_data_parseFromJSON() failed [supp_feat]");
        goto end;
    }

    cJSON *ue_ipv4 = cJSON_GetObjectItemCaseSensitive(app_session_context_req_dataJSON, "ueIpv4");

    if (ue_ipv4) {
    if (!cJSON_IsString(ue_ipv4)) {
        ogs_error("OpenAPI_app_session_context_req_data_parseFromJSON() failed [ue_ipv4]");
        goto end;
    }
    }

    cJSON *ue_ipv6 = cJSON_GetObjectItemCaseSensitive(app_session_context_req_dataJSON, "ueIpv6");

    if (ue_ipv6) {
    if (!cJSON_IsString(ue_ipv6)) {
        ogs_error("OpenAPI_app_session_context_req_data_parseFromJSON() failed [ue_ipv6]");
        goto end;
    }
    }

    cJSON *ue_mac = cJSON_GetObjectItemCaseSensitive(app_session_context_req_dataJSON, "ueMac");

    if (ue_mac) {
    if (!cJSON_IsString(ue_mac)) {
        ogs_error("OpenAPI_app_session_context_req_data_parseFromJSON() failed [ue_mac]");
        goto end;
    }
    }

    cJSON *tsn_bridge_man_cont = cJSON_GetObjectItemCaseSensitive(app_session_context_req_dataJSON, "tsnBridgeManCont");

    OpenAPI_bridge_management_container_t *tsn_bridge_man_cont_local_nonprim = NULL;
    if (tsn_bridge_man_cont) {
    tsn_bridge_man_cont_local_nonprim = OpenAPI_bridge_management_container_parseFromJSON(tsn_bridge_man_cont);
    }

    cJSON *tsn_port_man_cont_dstt = cJSON_GetObjectItemCaseSensitive(app_session_context_req_dataJSON, "tsnPortManContDstt");

    OpenAPI_port_management_container_t *tsn_port_man_cont_dstt_local_nonprim = NULL;
    if (tsn_port_man_cont_dstt) {
    tsn_port_man_cont_dstt_local_nonprim = OpenAPI_port_management_container_parseFromJSON(tsn_port_man_cont_dstt);
    }

    cJSON *tsn_port_man_cont_nwtts = cJSON_GetObjectItemCaseSensitive(app_session_context_req_dataJSON, "tsnPortManContNwtts");

    OpenAPI_list_t *tsn_port_man_cont_nwttsList;
    if (tsn_port_man_cont_nwtts) {
    cJSON *tsn_port_man_cont_nwtts_local_nonprimitive;
    if (!cJSON_IsArray(tsn_port_man_cont_nwtts)){
        ogs_error("OpenAPI_app_session_context_req_data_parseFromJSON() failed [tsn_port_man_cont_nwtts]");
        goto end;
    }

    tsn_port_man_cont_nwttsList = OpenAPI_list_create();

    cJSON_ArrayForEach(tsn_port_man_cont_nwtts_local_nonprimitive, tsn_port_man_cont_nwtts ) {
        if (!cJSON_IsObject(tsn_port_man_cont_nwtts_local_nonprimitive)) {
            ogs_error("OpenAPI_app_session_context_req_data_parseFromJSON() failed [tsn_port_man_cont_nwtts]");
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

    app_session_context_req_data_local_var = OpenAPI_app_session_context_req_data_create (
        af_app_id ? ogs_strdup(af_app_id->valuestring) : NULL,
        af_charg_id ? ogs_strdup(af_charg_id->valuestring) : NULL,
        af_req_data ? af_req_dataVariable : 0,
        af_rout_req ? af_rout_req_local_nonprim : NULL,
        asp_id ? ogs_strdup(asp_id->valuestring) : NULL,
        bdt_ref_id ? ogs_strdup(bdt_ref_id->valuestring) : NULL,
        dnn ? ogs_strdup(dnn->valuestring) : NULL,
        ev_subsc ? ev_subsc_local_nonprim : NULL,
        mcptt_id ? ogs_strdup(mcptt_id->valuestring) : NULL,
        mc_video_id ? ogs_strdup(mc_video_id->valuestring) : NULL,
        med_components ? med_componentsList : NULL,
        ip_domain ? ogs_strdup(ip_domain->valuestring) : NULL,
        mps_id ? ogs_strdup(mps_id->valuestring) : NULL,
        mcs_id ? ogs_strdup(mcs_id->valuestring) : NULL,
        preempt_control_info ? preempt_control_infoVariable : 0,
        res_prio ? res_prioVariable : 0,
        serv_inf_status ? serv_inf_statusVariable : 0,
        ogs_strdup(notif_uri->valuestring),
        serv_urn ? ogs_strdup(serv_urn->valuestring) : NULL,
        slice_info ? slice_info_local_nonprim : NULL,
        spon_id ? ogs_strdup(spon_id->valuestring) : NULL,
        spon_status ? spon_statusVariable : 0,
        supi ? ogs_strdup(supi->valuestring) : NULL,
        gpsi ? ogs_strdup(gpsi->valuestring) : NULL,
        ogs_strdup(supp_feat->valuestring),
        ue_ipv4 ? ogs_strdup(ue_ipv4->valuestring) : NULL,
        ue_ipv6 ? ogs_strdup(ue_ipv6->valuestring) : NULL,
        ue_mac ? ogs_strdup(ue_mac->valuestring) : NULL,
        tsn_bridge_man_cont ? tsn_bridge_man_cont_local_nonprim : NULL,
        tsn_port_man_cont_dstt ? tsn_port_man_cont_dstt_local_nonprim : NULL,
        tsn_port_man_cont_nwtts ? tsn_port_man_cont_nwttsList : NULL
    );

    return app_session_context_req_data_local_var;
end:
    return NULL;
}

OpenAPI_app_session_context_req_data_t *OpenAPI_app_session_context_req_data_copy(OpenAPI_app_session_context_req_data_t *dst, OpenAPI_app_session_context_req_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_app_session_context_req_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_app_session_context_req_data_convertToJSON() failed");
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

    OpenAPI_app_session_context_req_data_free(dst);
    dst = OpenAPI_app_session_context_req_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

