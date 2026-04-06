
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "_application_data_influence_data_subs_to_notify_post_request_inner.h"

OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_t *OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_create(
    char *up_path_chg_notif_corre_id,
    bool is_app_relo_ind,
    int app_relo_ind,
    char *af_app_id,
    char *dnn,
    OpenAPI_list_t *eth_traffic_filters,
    OpenAPI_snssai_t *snssai,
    char *inter_group_id,
    OpenAPI_list_t *inter_group_id_list,
    OpenAPI_list_t *subscriber_cat_list,
    OpenAPI_plmn_id_1_t *plmn_id,
    char *ipv4_addr,
    char *ipv6_addr,
    char *supi,
    OpenAPI_list_t* traffic_data_sets,
    OpenAPI_list_t *traffic_filters,
    OpenAPI_list_t *traffic_routes,
    char *sfc_id_dl,
    char *sfc_id_ul,
    bool is_metadata_null,
    char *metadata,
    bool is_traff_corre_ind,
    int traff_corre_ind,
    bool is_tfc_corre_info_null,
    OpenAPI_traffic_correlation_info_t *tfc_corre_info,
    bool is_cand_dnai_ind,
    int cand_dnai_ind,
    char *valid_start_time,
    char *valid_end_time,
    OpenAPI_list_t *temp_validities,
    OpenAPI_network_area_info_1_t *nw_area_info,
    char *up_path_chg_notif_uri,
    OpenAPI_list_t *headers,
    OpenAPI_list_t *subscribed_events,
    char *notif_uri,
    char *notif_corre_id,
    OpenAPI_dnai_change_type_e dnai_chg_type,
    bool is_af_ack_ind,
    int af_ack_ind,
    bool is_addr_preser_ind,
    int addr_preser_ind,
    bool is_max_allowed_up_lat,
    int max_allowed_up_lat,
    bool is_sim_conn_ind,
    int sim_conn_ind,
    bool is_sim_conn_term,
    int sim_conn_term,
    bool is_n6_delay_ind,
    int n6_delay_ind,
    char *supported_features,
    char *res_uri,
    OpenAPI_list_t *reset_ids,
    OpenAPI_list_t* nsc_supp_feats,
    bool is_af_hdr_req_null,
    OpenAPI_af_header_handling_control_info_t *af_hdr_req,
    OpenAPI_traffic_influ_data_t *traffic_influ_data,
    char *notif_id
)
{
    OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_t *_application_data_influence_data_subs_to_notify_post_request_inner_local_var = ogs_malloc(sizeof(OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_t));
    ogs_assert(_application_data_influence_data_subs_to_notify_post_request_inner_local_var);

    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->up_path_chg_notif_corre_id = up_path_chg_notif_corre_id;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->is_app_relo_ind = is_app_relo_ind;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->app_relo_ind = app_relo_ind;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->af_app_id = af_app_id;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->dnn = dnn;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->eth_traffic_filters = eth_traffic_filters;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->snssai = snssai;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->inter_group_id = inter_group_id;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->inter_group_id_list = inter_group_id_list;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->subscriber_cat_list = subscriber_cat_list;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->plmn_id = plmn_id;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->ipv4_addr = ipv4_addr;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->ipv6_addr = ipv6_addr;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->supi = supi;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->traffic_data_sets = traffic_data_sets;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->traffic_filters = traffic_filters;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->traffic_routes = traffic_routes;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->sfc_id_dl = sfc_id_dl;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->sfc_id_ul = sfc_id_ul;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->is_metadata_null = is_metadata_null;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->metadata = metadata;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->is_traff_corre_ind = is_traff_corre_ind;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->traff_corre_ind = traff_corre_ind;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->is_tfc_corre_info_null = is_tfc_corre_info_null;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->tfc_corre_info = tfc_corre_info;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->is_cand_dnai_ind = is_cand_dnai_ind;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->cand_dnai_ind = cand_dnai_ind;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->valid_start_time = valid_start_time;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->valid_end_time = valid_end_time;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->temp_validities = temp_validities;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->nw_area_info = nw_area_info;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->up_path_chg_notif_uri = up_path_chg_notif_uri;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->headers = headers;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->subscribed_events = subscribed_events;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->notif_uri = notif_uri;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->notif_corre_id = notif_corre_id;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->dnai_chg_type = dnai_chg_type;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->is_af_ack_ind = is_af_ack_ind;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->af_ack_ind = af_ack_ind;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->is_addr_preser_ind = is_addr_preser_ind;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->addr_preser_ind = addr_preser_ind;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->is_max_allowed_up_lat = is_max_allowed_up_lat;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->max_allowed_up_lat = max_allowed_up_lat;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->is_sim_conn_ind = is_sim_conn_ind;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->sim_conn_ind = sim_conn_ind;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->is_sim_conn_term = is_sim_conn_term;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->sim_conn_term = sim_conn_term;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->is_n6_delay_ind = is_n6_delay_ind;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->n6_delay_ind = n6_delay_ind;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->supported_features = supported_features;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->res_uri = res_uri;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->reset_ids = reset_ids;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->nsc_supp_feats = nsc_supp_feats;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->is_af_hdr_req_null = is_af_hdr_req_null;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->af_hdr_req = af_hdr_req;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->traffic_influ_data = traffic_influ_data;
    _application_data_influence_data_subs_to_notify_post_request_inner_local_var->notif_id = notif_id;

    return _application_data_influence_data_subs_to_notify_post_request_inner_local_var;
}

void OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_free(OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_t *_application_data_influence_data_subs_to_notify_post_request_inner)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == _application_data_influence_data_subs_to_notify_post_request_inner) {
        return;
    }
    if (_application_data_influence_data_subs_to_notify_post_request_inner->up_path_chg_notif_corre_id) {
        ogs_free(_application_data_influence_data_subs_to_notify_post_request_inner->up_path_chg_notif_corre_id);
        _application_data_influence_data_subs_to_notify_post_request_inner->up_path_chg_notif_corre_id = NULL;
    }
    if (_application_data_influence_data_subs_to_notify_post_request_inner->af_app_id) {
        ogs_free(_application_data_influence_data_subs_to_notify_post_request_inner->af_app_id);
        _application_data_influence_data_subs_to_notify_post_request_inner->af_app_id = NULL;
    }
    if (_application_data_influence_data_subs_to_notify_post_request_inner->dnn) {
        ogs_free(_application_data_influence_data_subs_to_notify_post_request_inner->dnn);
        _application_data_influence_data_subs_to_notify_post_request_inner->dnn = NULL;
    }
    if (_application_data_influence_data_subs_to_notify_post_request_inner->eth_traffic_filters) {
        OpenAPI_list_for_each(_application_data_influence_data_subs_to_notify_post_request_inner->eth_traffic_filters, node) {
            OpenAPI_eth_flow_description_free(node->data);
        }
        OpenAPI_list_free(_application_data_influence_data_subs_to_notify_post_request_inner->eth_traffic_filters);
        _application_data_influence_data_subs_to_notify_post_request_inner->eth_traffic_filters = NULL;
    }
    if (_application_data_influence_data_subs_to_notify_post_request_inner->snssai) {
        OpenAPI_snssai_free(_application_data_influence_data_subs_to_notify_post_request_inner->snssai);
        _application_data_influence_data_subs_to_notify_post_request_inner->snssai = NULL;
    }
    if (_application_data_influence_data_subs_to_notify_post_request_inner->inter_group_id) {
        ogs_free(_application_data_influence_data_subs_to_notify_post_request_inner->inter_group_id);
        _application_data_influence_data_subs_to_notify_post_request_inner->inter_group_id = NULL;
    }
    if (_application_data_influence_data_subs_to_notify_post_request_inner->inter_group_id_list) {
        OpenAPI_list_for_each(_application_data_influence_data_subs_to_notify_post_request_inner->inter_group_id_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(_application_data_influence_data_subs_to_notify_post_request_inner->inter_group_id_list);
        _application_data_influence_data_subs_to_notify_post_request_inner->inter_group_id_list = NULL;
    }
    if (_application_data_influence_data_subs_to_notify_post_request_inner->subscriber_cat_list) {
        OpenAPI_list_for_each(_application_data_influence_data_subs_to_notify_post_request_inner->subscriber_cat_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(_application_data_influence_data_subs_to_notify_post_request_inner->subscriber_cat_list);
        _application_data_influence_data_subs_to_notify_post_request_inner->subscriber_cat_list = NULL;
    }
    if (_application_data_influence_data_subs_to_notify_post_request_inner->plmn_id) {
        OpenAPI_plmn_id_1_free(_application_data_influence_data_subs_to_notify_post_request_inner->plmn_id);
        _application_data_influence_data_subs_to_notify_post_request_inner->plmn_id = NULL;
    }
    if (_application_data_influence_data_subs_to_notify_post_request_inner->ipv4_addr) {
        ogs_free(_application_data_influence_data_subs_to_notify_post_request_inner->ipv4_addr);
        _application_data_influence_data_subs_to_notify_post_request_inner->ipv4_addr = NULL;
    }
    if (_application_data_influence_data_subs_to_notify_post_request_inner->ipv6_addr) {
        ogs_free(_application_data_influence_data_subs_to_notify_post_request_inner->ipv6_addr);
        _application_data_influence_data_subs_to_notify_post_request_inner->ipv6_addr = NULL;
    }
    if (_application_data_influence_data_subs_to_notify_post_request_inner->supi) {
        ogs_free(_application_data_influence_data_subs_to_notify_post_request_inner->supi);
        _application_data_influence_data_subs_to_notify_post_request_inner->supi = NULL;
    }
    if (_application_data_influence_data_subs_to_notify_post_request_inner->traffic_data_sets) {
        OpenAPI_list_for_each(_application_data_influence_data_subs_to_notify_post_request_inner->traffic_data_sets, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_traffic_data_set_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(_application_data_influence_data_subs_to_notify_post_request_inner->traffic_data_sets);
        _application_data_influence_data_subs_to_notify_post_request_inner->traffic_data_sets = NULL;
    }
    if (_application_data_influence_data_subs_to_notify_post_request_inner->traffic_filters) {
        OpenAPI_list_for_each(_application_data_influence_data_subs_to_notify_post_request_inner->traffic_filters, node) {
            OpenAPI_flow_info_free(node->data);
        }
        OpenAPI_list_free(_application_data_influence_data_subs_to_notify_post_request_inner->traffic_filters);
        _application_data_influence_data_subs_to_notify_post_request_inner->traffic_filters = NULL;
    }
    if (_application_data_influence_data_subs_to_notify_post_request_inner->traffic_routes) {
        OpenAPI_list_for_each(_application_data_influence_data_subs_to_notify_post_request_inner->traffic_routes, node) {
            OpenAPI_route_to_location_free(node->data);
        }
        OpenAPI_list_free(_application_data_influence_data_subs_to_notify_post_request_inner->traffic_routes);
        _application_data_influence_data_subs_to_notify_post_request_inner->traffic_routes = NULL;
    }
    if (_application_data_influence_data_subs_to_notify_post_request_inner->sfc_id_dl) {
        ogs_free(_application_data_influence_data_subs_to_notify_post_request_inner->sfc_id_dl);
        _application_data_influence_data_subs_to_notify_post_request_inner->sfc_id_dl = NULL;
    }
    if (_application_data_influence_data_subs_to_notify_post_request_inner->sfc_id_ul) {
        ogs_free(_application_data_influence_data_subs_to_notify_post_request_inner->sfc_id_ul);
        _application_data_influence_data_subs_to_notify_post_request_inner->sfc_id_ul = NULL;
    }
    if (_application_data_influence_data_subs_to_notify_post_request_inner->metadata) {
        ogs_free(_application_data_influence_data_subs_to_notify_post_request_inner->metadata);
        _application_data_influence_data_subs_to_notify_post_request_inner->metadata = NULL;
    }
    if (_application_data_influence_data_subs_to_notify_post_request_inner->tfc_corre_info) {
        OpenAPI_traffic_correlation_info_free(_application_data_influence_data_subs_to_notify_post_request_inner->tfc_corre_info);
        _application_data_influence_data_subs_to_notify_post_request_inner->tfc_corre_info = NULL;
    }
    if (_application_data_influence_data_subs_to_notify_post_request_inner->valid_start_time) {
        ogs_free(_application_data_influence_data_subs_to_notify_post_request_inner->valid_start_time);
        _application_data_influence_data_subs_to_notify_post_request_inner->valid_start_time = NULL;
    }
    if (_application_data_influence_data_subs_to_notify_post_request_inner->valid_end_time) {
        ogs_free(_application_data_influence_data_subs_to_notify_post_request_inner->valid_end_time);
        _application_data_influence_data_subs_to_notify_post_request_inner->valid_end_time = NULL;
    }
    if (_application_data_influence_data_subs_to_notify_post_request_inner->temp_validities) {
        OpenAPI_list_for_each(_application_data_influence_data_subs_to_notify_post_request_inner->temp_validities, node) {
            OpenAPI_temporal_validity_free(node->data);
        }
        OpenAPI_list_free(_application_data_influence_data_subs_to_notify_post_request_inner->temp_validities);
        _application_data_influence_data_subs_to_notify_post_request_inner->temp_validities = NULL;
    }
    if (_application_data_influence_data_subs_to_notify_post_request_inner->nw_area_info) {
        OpenAPI_network_area_info_1_free(_application_data_influence_data_subs_to_notify_post_request_inner->nw_area_info);
        _application_data_influence_data_subs_to_notify_post_request_inner->nw_area_info = NULL;
    }
    if (_application_data_influence_data_subs_to_notify_post_request_inner->up_path_chg_notif_uri) {
        ogs_free(_application_data_influence_data_subs_to_notify_post_request_inner->up_path_chg_notif_uri);
        _application_data_influence_data_subs_to_notify_post_request_inner->up_path_chg_notif_uri = NULL;
    }
    if (_application_data_influence_data_subs_to_notify_post_request_inner->headers) {
        OpenAPI_list_for_each(_application_data_influence_data_subs_to_notify_post_request_inner->headers, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(_application_data_influence_data_subs_to_notify_post_request_inner->headers);
        _application_data_influence_data_subs_to_notify_post_request_inner->headers = NULL;
    }
    if (_application_data_influence_data_subs_to_notify_post_request_inner->subscribed_events) {
        OpenAPI_list_free(_application_data_influence_data_subs_to_notify_post_request_inner->subscribed_events);
        _application_data_influence_data_subs_to_notify_post_request_inner->subscribed_events = NULL;
    }
    if (_application_data_influence_data_subs_to_notify_post_request_inner->notif_uri) {
        ogs_free(_application_data_influence_data_subs_to_notify_post_request_inner->notif_uri);
        _application_data_influence_data_subs_to_notify_post_request_inner->notif_uri = NULL;
    }
    if (_application_data_influence_data_subs_to_notify_post_request_inner->notif_corre_id) {
        ogs_free(_application_data_influence_data_subs_to_notify_post_request_inner->notif_corre_id);
        _application_data_influence_data_subs_to_notify_post_request_inner->notif_corre_id = NULL;
    }
    if (_application_data_influence_data_subs_to_notify_post_request_inner->supported_features) {
        ogs_free(_application_data_influence_data_subs_to_notify_post_request_inner->supported_features);
        _application_data_influence_data_subs_to_notify_post_request_inner->supported_features = NULL;
    }
    if (_application_data_influence_data_subs_to_notify_post_request_inner->res_uri) {
        ogs_free(_application_data_influence_data_subs_to_notify_post_request_inner->res_uri);
        _application_data_influence_data_subs_to_notify_post_request_inner->res_uri = NULL;
    }
    if (_application_data_influence_data_subs_to_notify_post_request_inner->reset_ids) {
        OpenAPI_list_for_each(_application_data_influence_data_subs_to_notify_post_request_inner->reset_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(_application_data_influence_data_subs_to_notify_post_request_inner->reset_ids);
        _application_data_influence_data_subs_to_notify_post_request_inner->reset_ids = NULL;
    }
    if (_application_data_influence_data_subs_to_notify_post_request_inner->nsc_supp_feats) {
        OpenAPI_list_for_each(_application_data_influence_data_subs_to_notify_post_request_inner->nsc_supp_feats, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(_application_data_influence_data_subs_to_notify_post_request_inner->nsc_supp_feats);
        _application_data_influence_data_subs_to_notify_post_request_inner->nsc_supp_feats = NULL;
    }
    if (_application_data_influence_data_subs_to_notify_post_request_inner->af_hdr_req) {
        OpenAPI_af_header_handling_control_info_free(_application_data_influence_data_subs_to_notify_post_request_inner->af_hdr_req);
        _application_data_influence_data_subs_to_notify_post_request_inner->af_hdr_req = NULL;
    }
    if (_application_data_influence_data_subs_to_notify_post_request_inner->traffic_influ_data) {
        OpenAPI_traffic_influ_data_free(_application_data_influence_data_subs_to_notify_post_request_inner->traffic_influ_data);
        _application_data_influence_data_subs_to_notify_post_request_inner->traffic_influ_data = NULL;
    }
    if (_application_data_influence_data_subs_to_notify_post_request_inner->notif_id) {
        ogs_free(_application_data_influence_data_subs_to_notify_post_request_inner->notif_id);
        _application_data_influence_data_subs_to_notify_post_request_inner->notif_id = NULL;
    }
    ogs_free(_application_data_influence_data_subs_to_notify_post_request_inner);
}

cJSON *OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON(OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_t *_application_data_influence_data_subs_to_notify_post_request_inner)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (_application_data_influence_data_subs_to_notify_post_request_inner == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [_application_data_influenceData_subs_to_notify_post_request_inner]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (_application_data_influence_data_subs_to_notify_post_request_inner->up_path_chg_notif_corre_id) {
    if (cJSON_AddStringToObject(item, "upPathChgNotifCorreId", _application_data_influence_data_subs_to_notify_post_request_inner->up_path_chg_notif_corre_id) == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [up_path_chg_notif_corre_id]");
        goto end;
    }
    }

    if (_application_data_influence_data_subs_to_notify_post_request_inner->is_app_relo_ind) {
    if (cJSON_AddBoolToObject(item, "appReloInd", _application_data_influence_data_subs_to_notify_post_request_inner->app_relo_ind) == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [app_relo_ind]");
        goto end;
    }
    }

    if (_application_data_influence_data_subs_to_notify_post_request_inner->af_app_id) {
    if (cJSON_AddStringToObject(item, "afAppId", _application_data_influence_data_subs_to_notify_post_request_inner->af_app_id) == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [af_app_id]");
        goto end;
    }
    }

    if (_application_data_influence_data_subs_to_notify_post_request_inner->dnn) {
    if (cJSON_AddStringToObject(item, "dnn", _application_data_influence_data_subs_to_notify_post_request_inner->dnn) == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [dnn]");
        goto end;
    }
    }

    if (_application_data_influence_data_subs_to_notify_post_request_inner->eth_traffic_filters) {
    cJSON *eth_traffic_filtersList = cJSON_AddArrayToObject(item, "ethTrafficFilters");
    if (eth_traffic_filtersList == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [eth_traffic_filters]");
        goto end;
    }
    OpenAPI_list_for_each(_application_data_influence_data_subs_to_notify_post_request_inner->eth_traffic_filters, node) {
        cJSON *itemLocal = OpenAPI_eth_flow_description_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [eth_traffic_filters]");
            goto end;
        }
        cJSON_AddItemToArray(eth_traffic_filtersList, itemLocal);
    }
    }

    if (_application_data_influence_data_subs_to_notify_post_request_inner->snssai) {
    cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(_application_data_influence_data_subs_to_notify_post_request_inner->snssai);
    if (snssai_local_JSON == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [snssai]");
        goto end;
    }
    }

    if (_application_data_influence_data_subs_to_notify_post_request_inner->inter_group_id) {
    if (cJSON_AddStringToObject(item, "interGroupId", _application_data_influence_data_subs_to_notify_post_request_inner->inter_group_id) == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [inter_group_id]");
        goto end;
    }
    }

    if (_application_data_influence_data_subs_to_notify_post_request_inner->inter_group_id_list) {
    cJSON *inter_group_id_listList = cJSON_AddArrayToObject(item, "interGroupIdList");
    if (inter_group_id_listList == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [inter_group_id_list]");
        goto end;
    }
    OpenAPI_list_for_each(_application_data_influence_data_subs_to_notify_post_request_inner->inter_group_id_list, node) {
        if (cJSON_AddStringToObject(inter_group_id_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [inter_group_id_list]");
            goto end;
        }
    }
    }

    if (_application_data_influence_data_subs_to_notify_post_request_inner->subscriber_cat_list) {
    cJSON *subscriber_cat_listList = cJSON_AddArrayToObject(item, "subscriberCatList");
    if (subscriber_cat_listList == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [subscriber_cat_list]");
        goto end;
    }
    OpenAPI_list_for_each(_application_data_influence_data_subs_to_notify_post_request_inner->subscriber_cat_list, node) {
        if (cJSON_AddStringToObject(subscriber_cat_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [subscriber_cat_list]");
            goto end;
        }
    }
    }

    if (_application_data_influence_data_subs_to_notify_post_request_inner->plmn_id) {
    cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_1_convertToJSON(_application_data_influence_data_subs_to_notify_post_request_inner->plmn_id);
    if (plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [plmn_id]");
        goto end;
    }
    }

    if (_application_data_influence_data_subs_to_notify_post_request_inner->ipv4_addr) {
    if (cJSON_AddStringToObject(item, "ipv4Addr", _application_data_influence_data_subs_to_notify_post_request_inner->ipv4_addr) == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [ipv4_addr]");
        goto end;
    }
    }

    if (_application_data_influence_data_subs_to_notify_post_request_inner->ipv6_addr) {
    if (cJSON_AddStringToObject(item, "ipv6Addr", _application_data_influence_data_subs_to_notify_post_request_inner->ipv6_addr) == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [ipv6_addr]");
        goto end;
    }
    }

    if (_application_data_influence_data_subs_to_notify_post_request_inner->supi) {
    if (cJSON_AddStringToObject(item, "supi", _application_data_influence_data_subs_to_notify_post_request_inner->supi) == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [supi]");
        goto end;
    }
    }

    if (_application_data_influence_data_subs_to_notify_post_request_inner->traffic_data_sets) {
    cJSON *traffic_data_sets = cJSON_AddObjectToObject(item, "trafficDataSets");
    if (traffic_data_sets == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [traffic_data_sets]");
        goto end;
    }
    cJSON *localMapObject = traffic_data_sets;
    if (_application_data_influence_data_subs_to_notify_post_request_inner->traffic_data_sets) {
        OpenAPI_list_for_each(_application_data_influence_data_subs_to_notify_post_request_inner->traffic_data_sets, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [traffic_data_sets]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [traffic_data_sets]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_traffic_data_set_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (_application_data_influence_data_subs_to_notify_post_request_inner->traffic_filters) {
    cJSON *traffic_filtersList = cJSON_AddArrayToObject(item, "trafficFilters");
    if (traffic_filtersList == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [traffic_filters]");
        goto end;
    }
    OpenAPI_list_for_each(_application_data_influence_data_subs_to_notify_post_request_inner->traffic_filters, node) {
        cJSON *itemLocal = OpenAPI_flow_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [traffic_filters]");
            goto end;
        }
        cJSON_AddItemToArray(traffic_filtersList, itemLocal);
    }
    }

    if (_application_data_influence_data_subs_to_notify_post_request_inner->traffic_routes) {
    cJSON *traffic_routesList = cJSON_AddArrayToObject(item, "trafficRoutes");
    if (traffic_routesList == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [traffic_routes]");
        goto end;
    }
    OpenAPI_list_for_each(_application_data_influence_data_subs_to_notify_post_request_inner->traffic_routes, node) {
        cJSON *itemLocal = OpenAPI_route_to_location_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [traffic_routes]");
            goto end;
        }
        cJSON_AddItemToArray(traffic_routesList, itemLocal);
    }
    }

    if (_application_data_influence_data_subs_to_notify_post_request_inner->sfc_id_dl) {
    if (cJSON_AddStringToObject(item, "sfcIdDl", _application_data_influence_data_subs_to_notify_post_request_inner->sfc_id_dl) == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [sfc_id_dl]");
        goto end;
    }
    }

    if (_application_data_influence_data_subs_to_notify_post_request_inner->sfc_id_ul) {
    if (cJSON_AddStringToObject(item, "sfcIdUl", _application_data_influence_data_subs_to_notify_post_request_inner->sfc_id_ul) == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [sfc_id_ul]");
        goto end;
    }
    }

    if (_application_data_influence_data_subs_to_notify_post_request_inner->metadata) {
    if (cJSON_AddStringToObject(item, "metadata", _application_data_influence_data_subs_to_notify_post_request_inner->metadata) == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [metadata]");
        goto end;
    }
    } else if (_application_data_influence_data_subs_to_notify_post_request_inner->is_metadata_null) {
        if (cJSON_AddNullToObject(item, "metadata") == NULL) {
            ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [metadata]");
            goto end;
        }
    }

    if (_application_data_influence_data_subs_to_notify_post_request_inner->is_traff_corre_ind) {
    if (cJSON_AddBoolToObject(item, "traffCorreInd", _application_data_influence_data_subs_to_notify_post_request_inner->traff_corre_ind) == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [traff_corre_ind]");
        goto end;
    }
    }

    if (_application_data_influence_data_subs_to_notify_post_request_inner->tfc_corre_info) {
    cJSON *tfc_corre_info_local_JSON = OpenAPI_traffic_correlation_info_convertToJSON(_application_data_influence_data_subs_to_notify_post_request_inner->tfc_corre_info);
    if (tfc_corre_info_local_JSON == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [tfc_corre_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "tfcCorreInfo", tfc_corre_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [tfc_corre_info]");
        goto end;
    }
    } else if (_application_data_influence_data_subs_to_notify_post_request_inner->is_tfc_corre_info_null) {
        if (cJSON_AddNullToObject(item, "tfcCorreInfo") == NULL) {
            ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [tfc_corre_info]");
            goto end;
        }
    }

    if (_application_data_influence_data_subs_to_notify_post_request_inner->is_cand_dnai_ind) {
    if (cJSON_AddBoolToObject(item, "candDnaiInd", _application_data_influence_data_subs_to_notify_post_request_inner->cand_dnai_ind) == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [cand_dnai_ind]");
        goto end;
    }
    }

    if (_application_data_influence_data_subs_to_notify_post_request_inner->valid_start_time) {
    if (cJSON_AddStringToObject(item, "validStartTime", _application_data_influence_data_subs_to_notify_post_request_inner->valid_start_time) == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [valid_start_time]");
        goto end;
    }
    }

    if (_application_data_influence_data_subs_to_notify_post_request_inner->valid_end_time) {
    if (cJSON_AddStringToObject(item, "validEndTime", _application_data_influence_data_subs_to_notify_post_request_inner->valid_end_time) == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [valid_end_time]");
        goto end;
    }
    }

    if (_application_data_influence_data_subs_to_notify_post_request_inner->temp_validities) {
    cJSON *temp_validitiesList = cJSON_AddArrayToObject(item, "tempValidities");
    if (temp_validitiesList == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [temp_validities]");
        goto end;
    }
    OpenAPI_list_for_each(_application_data_influence_data_subs_to_notify_post_request_inner->temp_validities, node) {
        cJSON *itemLocal = OpenAPI_temporal_validity_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [temp_validities]");
            goto end;
        }
        cJSON_AddItemToArray(temp_validitiesList, itemLocal);
    }
    }

    if (_application_data_influence_data_subs_to_notify_post_request_inner->nw_area_info) {
    cJSON *nw_area_info_local_JSON = OpenAPI_network_area_info_1_convertToJSON(_application_data_influence_data_subs_to_notify_post_request_inner->nw_area_info);
    if (nw_area_info_local_JSON == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [nw_area_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "nwAreaInfo", nw_area_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [nw_area_info]");
        goto end;
    }
    }

    if (_application_data_influence_data_subs_to_notify_post_request_inner->up_path_chg_notif_uri) {
    if (cJSON_AddStringToObject(item, "upPathChgNotifUri", _application_data_influence_data_subs_to_notify_post_request_inner->up_path_chg_notif_uri) == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [up_path_chg_notif_uri]");
        goto end;
    }
    }

    if (_application_data_influence_data_subs_to_notify_post_request_inner->headers) {
    cJSON *headersList = cJSON_AddArrayToObject(item, "headers");
    if (headersList == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [headers]");
        goto end;
    }
    OpenAPI_list_for_each(_application_data_influence_data_subs_to_notify_post_request_inner->headers, node) {
        if (cJSON_AddStringToObject(headersList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [headers]");
            goto end;
        }
    }
    }

    if (_application_data_influence_data_subs_to_notify_post_request_inner->subscribed_events != OpenAPI_subscribed_event_NULL) {
    cJSON *subscribed_eventsList = cJSON_AddArrayToObject(item, "subscribedEvents");
    if (subscribed_eventsList == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [subscribed_events]");
        goto end;
    }
    OpenAPI_list_for_each(_application_data_influence_data_subs_to_notify_post_request_inner->subscribed_events, node) {
        if (cJSON_AddStringToObject(subscribed_eventsList, "", OpenAPI_subscribed_event_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [subscribed_events]");
            goto end;
        }
    }
    }

    if (_application_data_influence_data_subs_to_notify_post_request_inner->notif_uri) {
    if (cJSON_AddStringToObject(item, "notifUri", _application_data_influence_data_subs_to_notify_post_request_inner->notif_uri) == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [notif_uri]");
        goto end;
    }
    }

    if (_application_data_influence_data_subs_to_notify_post_request_inner->notif_corre_id) {
    if (cJSON_AddStringToObject(item, "notifCorreId", _application_data_influence_data_subs_to_notify_post_request_inner->notif_corre_id) == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [notif_corre_id]");
        goto end;
    }
    }

    if (_application_data_influence_data_subs_to_notify_post_request_inner->dnai_chg_type != OpenAPI_dnai_change_type_NULL) {
    if (cJSON_AddStringToObject(item, "dnaiChgType", OpenAPI_dnai_change_type_ToString(_application_data_influence_data_subs_to_notify_post_request_inner->dnai_chg_type)) == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [dnai_chg_type]");
        goto end;
    }
    }

    if (_application_data_influence_data_subs_to_notify_post_request_inner->is_af_ack_ind) {
    if (cJSON_AddBoolToObject(item, "afAckInd", _application_data_influence_data_subs_to_notify_post_request_inner->af_ack_ind) == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [af_ack_ind]");
        goto end;
    }
    }

    if (_application_data_influence_data_subs_to_notify_post_request_inner->is_addr_preser_ind) {
    if (cJSON_AddBoolToObject(item, "addrPreserInd", _application_data_influence_data_subs_to_notify_post_request_inner->addr_preser_ind) == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [addr_preser_ind]");
        goto end;
    }
    }

    if (_application_data_influence_data_subs_to_notify_post_request_inner->is_max_allowed_up_lat) {
    if (cJSON_AddNumberToObject(item, "maxAllowedUpLat", _application_data_influence_data_subs_to_notify_post_request_inner->max_allowed_up_lat) == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [max_allowed_up_lat]");
        goto end;
    }
    }

    if (_application_data_influence_data_subs_to_notify_post_request_inner->is_sim_conn_ind) {
    if (cJSON_AddBoolToObject(item, "simConnInd", _application_data_influence_data_subs_to_notify_post_request_inner->sim_conn_ind) == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [sim_conn_ind]");
        goto end;
    }
    }

    if (_application_data_influence_data_subs_to_notify_post_request_inner->is_sim_conn_term) {
    if (cJSON_AddNumberToObject(item, "simConnTerm", _application_data_influence_data_subs_to_notify_post_request_inner->sim_conn_term) == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [sim_conn_term]");
        goto end;
    }
    }

    if (_application_data_influence_data_subs_to_notify_post_request_inner->is_n6_delay_ind) {
    if (cJSON_AddBoolToObject(item, "n6DelayInd", _application_data_influence_data_subs_to_notify_post_request_inner->n6_delay_ind) == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [n6_delay_ind]");
        goto end;
    }
    }

    if (_application_data_influence_data_subs_to_notify_post_request_inner->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", _application_data_influence_data_subs_to_notify_post_request_inner->supported_features) == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (_application_data_influence_data_subs_to_notify_post_request_inner->res_uri) {
    if (cJSON_AddStringToObject(item, "resUri", _application_data_influence_data_subs_to_notify_post_request_inner->res_uri) == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [res_uri]");
        goto end;
    }
    }

    if (_application_data_influence_data_subs_to_notify_post_request_inner->reset_ids) {
    cJSON *reset_idsList = cJSON_AddArrayToObject(item, "resetIds");
    if (reset_idsList == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [reset_ids]");
        goto end;
    }
    OpenAPI_list_for_each(_application_data_influence_data_subs_to_notify_post_request_inner->reset_ids, node) {
        if (cJSON_AddStringToObject(reset_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [reset_ids]");
            goto end;
        }
    }
    }

    if (_application_data_influence_data_subs_to_notify_post_request_inner->nsc_supp_feats) {
    cJSON *nsc_supp_feats = cJSON_AddObjectToObject(item, "nscSuppFeats");
    if (nsc_supp_feats == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [nsc_supp_feats]");
        goto end;
    }
    cJSON *localMapObject = nsc_supp_feats;
    if (_application_data_influence_data_subs_to_notify_post_request_inner->nsc_supp_feats) {
        OpenAPI_list_for_each(_application_data_influence_data_subs_to_notify_post_request_inner->nsc_supp_feats, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [nsc_supp_feats]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [nsc_supp_feats]");
                goto end;
            }
            if (cJSON_AddStringToObject(localMapObject, localKeyValue->key, (char*)localKeyValue->value) == NULL) {
                ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [inner]");
                goto end;
            }
        }
    }
    }

    if (_application_data_influence_data_subs_to_notify_post_request_inner->af_hdr_req) {
    cJSON *af_hdr_req_local_JSON = OpenAPI_af_header_handling_control_info_convertToJSON(_application_data_influence_data_subs_to_notify_post_request_inner->af_hdr_req);
    if (af_hdr_req_local_JSON == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [af_hdr_req]");
        goto end;
    }
    cJSON_AddItemToObject(item, "afHdrReq", af_hdr_req_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [af_hdr_req]");
        goto end;
    }
    } else if (_application_data_influence_data_subs_to_notify_post_request_inner->is_af_hdr_req_null) {
        if (cJSON_AddNullToObject(item, "afHdrReq") == NULL) {
            ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [af_hdr_req]");
            goto end;
        }
    }

    if (_application_data_influence_data_subs_to_notify_post_request_inner->traffic_influ_data) {
    cJSON *traffic_influ_data_local_JSON = OpenAPI_traffic_influ_data_convertToJSON(_application_data_influence_data_subs_to_notify_post_request_inner->traffic_influ_data);
    if (traffic_influ_data_local_JSON == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [traffic_influ_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "trafficInfluData", traffic_influ_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [traffic_influ_data]");
        goto end;
    }
    }

    if (_application_data_influence_data_subs_to_notify_post_request_inner->notif_id) {
    if (cJSON_AddStringToObject(item, "notifId", _application_data_influence_data_subs_to_notify_post_request_inner->notif_id) == NULL) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed [notif_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_t *OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON(cJSON *_application_data_influence_data_subs_to_notify_post_request_innerJSON)
{
    OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_t *_application_data_influence_data_subs_to_notify_post_request_inner_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *up_path_chg_notif_corre_id = NULL;
    cJSON *app_relo_ind = NULL;
    cJSON *af_app_id = NULL;
    cJSON *dnn = NULL;
    cJSON *eth_traffic_filters = NULL;
    OpenAPI_list_t *eth_traffic_filtersList = NULL;
    cJSON *snssai = NULL;
    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    cJSON *inter_group_id = NULL;
    cJSON *inter_group_id_list = NULL;
    OpenAPI_list_t *inter_group_id_listList = NULL;
    cJSON *subscriber_cat_list = NULL;
    OpenAPI_list_t *subscriber_cat_listList = NULL;
    cJSON *plmn_id = NULL;
    OpenAPI_plmn_id_1_t *plmn_id_local_nonprim = NULL;
    cJSON *ipv4_addr = NULL;
    cJSON *ipv6_addr = NULL;
    cJSON *supi = NULL;
    cJSON *traffic_data_sets = NULL;
    OpenAPI_list_t *traffic_data_setsList = NULL;
    cJSON *traffic_filters = NULL;
    OpenAPI_list_t *traffic_filtersList = NULL;
    cJSON *traffic_routes = NULL;
    OpenAPI_list_t *traffic_routesList = NULL;
    cJSON *sfc_id_dl = NULL;
    cJSON *sfc_id_ul = NULL;
    cJSON *metadata = NULL;
    cJSON *traff_corre_ind = NULL;
    cJSON *tfc_corre_info = NULL;
    OpenAPI_traffic_correlation_info_t *tfc_corre_info_local_nonprim = NULL;
    cJSON *cand_dnai_ind = NULL;
    cJSON *valid_start_time = NULL;
    cJSON *valid_end_time = NULL;
    cJSON *temp_validities = NULL;
    OpenAPI_list_t *temp_validitiesList = NULL;
    cJSON *nw_area_info = NULL;
    OpenAPI_network_area_info_1_t *nw_area_info_local_nonprim = NULL;
    cJSON *up_path_chg_notif_uri = NULL;
    cJSON *headers = NULL;
    OpenAPI_list_t *headersList = NULL;
    cJSON *subscribed_events = NULL;
    OpenAPI_list_t *subscribed_eventsList = NULL;
    cJSON *notif_uri = NULL;
    cJSON *notif_corre_id = NULL;
    cJSON *dnai_chg_type = NULL;
    OpenAPI_dnai_change_type_e dnai_chg_typeVariable = 0;
    cJSON *af_ack_ind = NULL;
    cJSON *addr_preser_ind = NULL;
    cJSON *max_allowed_up_lat = NULL;
    cJSON *sim_conn_ind = NULL;
    cJSON *sim_conn_term = NULL;
    cJSON *n6_delay_ind = NULL;
    cJSON *supported_features = NULL;
    cJSON *res_uri = NULL;
    cJSON *reset_ids = NULL;
    OpenAPI_list_t *reset_idsList = NULL;
    cJSON *nsc_supp_feats = NULL;
    OpenAPI_list_t *nsc_supp_featsList = NULL;
    cJSON *af_hdr_req = NULL;
    OpenAPI_af_header_handling_control_info_t *af_hdr_req_local_nonprim = NULL;
    cJSON *traffic_influ_data = NULL;
    OpenAPI_traffic_influ_data_t *traffic_influ_data_local_nonprim = NULL;
    cJSON *notif_id = NULL;
    up_path_chg_notif_corre_id = cJSON_GetObjectItemCaseSensitive(_application_data_influence_data_subs_to_notify_post_request_innerJSON, "upPathChgNotifCorreId");
    if (up_path_chg_notif_corre_id) {
    if (!cJSON_IsString(up_path_chg_notif_corre_id) && !cJSON_IsNull(up_path_chg_notif_corre_id)) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [up_path_chg_notif_corre_id]");
        goto end;
    }
    }

    app_relo_ind = cJSON_GetObjectItemCaseSensitive(_application_data_influence_data_subs_to_notify_post_request_innerJSON, "appReloInd");
    if (app_relo_ind) {
    if (!cJSON_IsBool(app_relo_ind)) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [app_relo_ind]");
        goto end;
    }
    }

    af_app_id = cJSON_GetObjectItemCaseSensitive(_application_data_influence_data_subs_to_notify_post_request_innerJSON, "afAppId");
    if (af_app_id) {
    if (!cJSON_IsString(af_app_id) && !cJSON_IsNull(af_app_id)) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [af_app_id]");
        goto end;
    }
    }

    dnn = cJSON_GetObjectItemCaseSensitive(_application_data_influence_data_subs_to_notify_post_request_innerJSON, "dnn");
    if (dnn) {
    if (!cJSON_IsString(dnn) && !cJSON_IsNull(dnn)) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [dnn]");
        goto end;
    }
    }

    eth_traffic_filters = cJSON_GetObjectItemCaseSensitive(_application_data_influence_data_subs_to_notify_post_request_innerJSON, "ethTrafficFilters");
    if (eth_traffic_filters) {
        cJSON *eth_traffic_filters_local = NULL;
        if (!cJSON_IsArray(eth_traffic_filters)) {
            ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [eth_traffic_filters]");
            goto end;
        }

        eth_traffic_filtersList = OpenAPI_list_create();

        cJSON_ArrayForEach(eth_traffic_filters_local, eth_traffic_filters) {
            if (!cJSON_IsObject(eth_traffic_filters_local)) {
                ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [eth_traffic_filters]");
                goto end;
            }
            OpenAPI_eth_flow_description_t *eth_traffic_filtersItem = OpenAPI_eth_flow_description_parseFromJSON(eth_traffic_filters_local);
            if (!eth_traffic_filtersItem) {
                ogs_error("No eth_traffic_filtersItem");
                goto end;
            }
            OpenAPI_list_add(eth_traffic_filtersList, eth_traffic_filtersItem);
        }
    }

    snssai = cJSON_GetObjectItemCaseSensitive(_application_data_influence_data_subs_to_notify_post_request_innerJSON, "snssai");
    if (snssai) {
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    if (!snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [snssai]");
        goto end;
    }
    }

    inter_group_id = cJSON_GetObjectItemCaseSensitive(_application_data_influence_data_subs_to_notify_post_request_innerJSON, "interGroupId");
    if (inter_group_id) {
    if (!cJSON_IsString(inter_group_id) && !cJSON_IsNull(inter_group_id)) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [inter_group_id]");
        goto end;
    }
    }

    inter_group_id_list = cJSON_GetObjectItemCaseSensitive(_application_data_influence_data_subs_to_notify_post_request_innerJSON, "interGroupIdList");
    if (inter_group_id_list) {
        cJSON *inter_group_id_list_local = NULL;
        if (!cJSON_IsArray(inter_group_id_list)) {
            ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [inter_group_id_list]");
            goto end;
        }

        inter_group_id_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(inter_group_id_list_local, inter_group_id_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(inter_group_id_list_local)) {
                ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [inter_group_id_list]");
                goto end;
            }
            OpenAPI_list_add(inter_group_id_listList, ogs_strdup(inter_group_id_list_local->valuestring));
        }
    }

    subscriber_cat_list = cJSON_GetObjectItemCaseSensitive(_application_data_influence_data_subs_to_notify_post_request_innerJSON, "subscriberCatList");
    if (subscriber_cat_list) {
        cJSON *subscriber_cat_list_local = NULL;
        if (!cJSON_IsArray(subscriber_cat_list)) {
            ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [subscriber_cat_list]");
            goto end;
        }

        subscriber_cat_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(subscriber_cat_list_local, subscriber_cat_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(subscriber_cat_list_local)) {
                ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [subscriber_cat_list]");
                goto end;
            }
            OpenAPI_list_add(subscriber_cat_listList, ogs_strdup(subscriber_cat_list_local->valuestring));
        }
    }

    plmn_id = cJSON_GetObjectItemCaseSensitive(_application_data_influence_data_subs_to_notify_post_request_innerJSON, "plmnId");
    if (plmn_id) {
    plmn_id_local_nonprim = OpenAPI_plmn_id_1_parseFromJSON(plmn_id);
    if (!plmn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_1_parseFromJSON failed [plmn_id]");
        goto end;
    }
    }

    ipv4_addr = cJSON_GetObjectItemCaseSensitive(_application_data_influence_data_subs_to_notify_post_request_innerJSON, "ipv4Addr");
    if (ipv4_addr) {
    if (!cJSON_IsString(ipv4_addr) && !cJSON_IsNull(ipv4_addr)) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [ipv4_addr]");
        goto end;
    }
    }

    ipv6_addr = cJSON_GetObjectItemCaseSensitive(_application_data_influence_data_subs_to_notify_post_request_innerJSON, "ipv6Addr");
    if (ipv6_addr) {
    if (!cJSON_IsString(ipv6_addr) && !cJSON_IsNull(ipv6_addr)) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [ipv6_addr]");
        goto end;
    }
    }

    supi = cJSON_GetObjectItemCaseSensitive(_application_data_influence_data_subs_to_notify_post_request_innerJSON, "supi");
    if (supi) {
    if (!cJSON_IsString(supi) && !cJSON_IsNull(supi)) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [supi]");
        goto end;
    }
    }

    traffic_data_sets = cJSON_GetObjectItemCaseSensitive(_application_data_influence_data_subs_to_notify_post_request_innerJSON, "trafficDataSets");
    if (traffic_data_sets) {
        cJSON *traffic_data_sets_local_map = NULL;
        if (!cJSON_IsObject(traffic_data_sets) && !cJSON_IsNull(traffic_data_sets)) {
            ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [traffic_data_sets]");
            goto end;
        }
        if (cJSON_IsObject(traffic_data_sets)) {
            traffic_data_setsList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(traffic_data_sets_local_map, traffic_data_sets) {
                cJSON *localMapObject = traffic_data_sets_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_traffic_data_set_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(traffic_data_setsList, localMapKeyPair);
            }
        }
    }

    traffic_filters = cJSON_GetObjectItemCaseSensitive(_application_data_influence_data_subs_to_notify_post_request_innerJSON, "trafficFilters");
    if (traffic_filters) {
        cJSON *traffic_filters_local = NULL;
        if (!cJSON_IsArray(traffic_filters)) {
            ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [traffic_filters]");
            goto end;
        }

        traffic_filtersList = OpenAPI_list_create();

        cJSON_ArrayForEach(traffic_filters_local, traffic_filters) {
            if (!cJSON_IsObject(traffic_filters_local)) {
                ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [traffic_filters]");
                goto end;
            }
            OpenAPI_flow_info_t *traffic_filtersItem = OpenAPI_flow_info_parseFromJSON(traffic_filters_local);
            if (!traffic_filtersItem) {
                ogs_error("No traffic_filtersItem");
                goto end;
            }
            OpenAPI_list_add(traffic_filtersList, traffic_filtersItem);
        }
    }

    traffic_routes = cJSON_GetObjectItemCaseSensitive(_application_data_influence_data_subs_to_notify_post_request_innerJSON, "trafficRoutes");
    if (traffic_routes) {
        cJSON *traffic_routes_local = NULL;
        if (!cJSON_IsArray(traffic_routes)) {
            ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [traffic_routes]");
            goto end;
        }

        traffic_routesList = OpenAPI_list_create();

        cJSON_ArrayForEach(traffic_routes_local, traffic_routes) {
            if (!cJSON_IsObject(traffic_routes_local)) {
                ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [traffic_routes]");
                goto end;
            }
            OpenAPI_route_to_location_t *traffic_routesItem = OpenAPI_route_to_location_parseFromJSON(traffic_routes_local);
            if (!traffic_routesItem) {
                ogs_error("No traffic_routesItem");
                goto end;
            }
            OpenAPI_list_add(traffic_routesList, traffic_routesItem);
        }
    }

    sfc_id_dl = cJSON_GetObjectItemCaseSensitive(_application_data_influence_data_subs_to_notify_post_request_innerJSON, "sfcIdDl");
    if (sfc_id_dl) {
    if (!cJSON_IsString(sfc_id_dl) && !cJSON_IsNull(sfc_id_dl)) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [sfc_id_dl]");
        goto end;
    }
    }

    sfc_id_ul = cJSON_GetObjectItemCaseSensitive(_application_data_influence_data_subs_to_notify_post_request_innerJSON, "sfcIdUl");
    if (sfc_id_ul) {
    if (!cJSON_IsString(sfc_id_ul) && !cJSON_IsNull(sfc_id_ul)) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [sfc_id_ul]");
        goto end;
    }
    }

    metadata = cJSON_GetObjectItemCaseSensitive(_application_data_influence_data_subs_to_notify_post_request_innerJSON, "metadata");
    if (metadata) {
    if (!cJSON_IsNull(metadata)) {
    if (!cJSON_IsString(metadata) && !cJSON_IsNull(metadata)) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [metadata]");
        goto end;
    }
    }
    }

    traff_corre_ind = cJSON_GetObjectItemCaseSensitive(_application_data_influence_data_subs_to_notify_post_request_innerJSON, "traffCorreInd");
    if (traff_corre_ind) {
    if (!cJSON_IsBool(traff_corre_ind)) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [traff_corre_ind]");
        goto end;
    }
    }

    tfc_corre_info = cJSON_GetObjectItemCaseSensitive(_application_data_influence_data_subs_to_notify_post_request_innerJSON, "tfcCorreInfo");
    if (tfc_corre_info) {
    if (!cJSON_IsNull(tfc_corre_info)) {
    tfc_corre_info_local_nonprim = OpenAPI_traffic_correlation_info_parseFromJSON(tfc_corre_info);
    if (!tfc_corre_info_local_nonprim) {
        ogs_error("OpenAPI_traffic_correlation_info_parseFromJSON failed [tfc_corre_info]");
        goto end;
    }
    }
    }

    cand_dnai_ind = cJSON_GetObjectItemCaseSensitive(_application_data_influence_data_subs_to_notify_post_request_innerJSON, "candDnaiInd");
    if (cand_dnai_ind) {
    if (!cJSON_IsBool(cand_dnai_ind)) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [cand_dnai_ind]");
        goto end;
    }
    }

    valid_start_time = cJSON_GetObjectItemCaseSensitive(_application_data_influence_data_subs_to_notify_post_request_innerJSON, "validStartTime");
    if (valid_start_time) {
    if (!cJSON_IsString(valid_start_time) && !cJSON_IsNull(valid_start_time)) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [valid_start_time]");
        goto end;
    }
    }

    valid_end_time = cJSON_GetObjectItemCaseSensitive(_application_data_influence_data_subs_to_notify_post_request_innerJSON, "validEndTime");
    if (valid_end_time) {
    if (!cJSON_IsString(valid_end_time) && !cJSON_IsNull(valid_end_time)) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [valid_end_time]");
        goto end;
    }
    }

    temp_validities = cJSON_GetObjectItemCaseSensitive(_application_data_influence_data_subs_to_notify_post_request_innerJSON, "tempValidities");
    if (temp_validities) {
        cJSON *temp_validities_local = NULL;
        if (!cJSON_IsArray(temp_validities)) {
            ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [temp_validities]");
            goto end;
        }

        temp_validitiesList = OpenAPI_list_create();

        cJSON_ArrayForEach(temp_validities_local, temp_validities) {
            if (!cJSON_IsObject(temp_validities_local)) {
                ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [temp_validities]");
                goto end;
            }
            OpenAPI_temporal_validity_t *temp_validitiesItem = OpenAPI_temporal_validity_parseFromJSON(temp_validities_local);
            if (!temp_validitiesItem) {
                ogs_error("No temp_validitiesItem");
                goto end;
            }
            OpenAPI_list_add(temp_validitiesList, temp_validitiesItem);
        }
    }

    nw_area_info = cJSON_GetObjectItemCaseSensitive(_application_data_influence_data_subs_to_notify_post_request_innerJSON, "nwAreaInfo");
    if (nw_area_info) {
    nw_area_info_local_nonprim = OpenAPI_network_area_info_1_parseFromJSON(nw_area_info);
    if (!nw_area_info_local_nonprim) {
        ogs_error("OpenAPI_network_area_info_1_parseFromJSON failed [nw_area_info]");
        goto end;
    }
    }

    up_path_chg_notif_uri = cJSON_GetObjectItemCaseSensitive(_application_data_influence_data_subs_to_notify_post_request_innerJSON, "upPathChgNotifUri");
    if (up_path_chg_notif_uri) {
    if (!cJSON_IsString(up_path_chg_notif_uri) && !cJSON_IsNull(up_path_chg_notif_uri)) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [up_path_chg_notif_uri]");
        goto end;
    }
    }

    headers = cJSON_GetObjectItemCaseSensitive(_application_data_influence_data_subs_to_notify_post_request_innerJSON, "headers");
    if (headers) {
        cJSON *headers_local = NULL;
        if (!cJSON_IsArray(headers)) {
            ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [headers]");
            goto end;
        }

        headersList = OpenAPI_list_create();

        cJSON_ArrayForEach(headers_local, headers) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(headers_local)) {
                ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [headers]");
                goto end;
            }
            OpenAPI_list_add(headersList, ogs_strdup(headers_local->valuestring));
        }
    }

    subscribed_events = cJSON_GetObjectItemCaseSensitive(_application_data_influence_data_subs_to_notify_post_request_innerJSON, "subscribedEvents");
    if (subscribed_events) {
        cJSON *subscribed_events_local = NULL;
        if (!cJSON_IsArray(subscribed_events)) {
            ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [subscribed_events]");
            goto end;
        }

        subscribed_eventsList = OpenAPI_list_create();

        cJSON_ArrayForEach(subscribed_events_local, subscribed_events) {
            OpenAPI_subscribed_event_e localEnum = OpenAPI_subscribed_event_NULL;
            if (!cJSON_IsString(subscribed_events_local)) {
                ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [subscribed_events]");
                goto end;
            }
            localEnum = OpenAPI_subscribed_event_FromString(subscribed_events_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"subscribed_events\" is not supported. Ignoring it ...",
                         subscribed_events_local->valuestring);
            } else {
                OpenAPI_list_add(subscribed_eventsList, (void *)localEnum);
            }
        }
        if (subscribed_eventsList->count == 0) {
            ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed: Expected subscribed_eventsList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    notif_uri = cJSON_GetObjectItemCaseSensitive(_application_data_influence_data_subs_to_notify_post_request_innerJSON, "notifUri");
    if (notif_uri) {
    if (!cJSON_IsString(notif_uri) && !cJSON_IsNull(notif_uri)) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [notif_uri]");
        goto end;
    }
    }

    notif_corre_id = cJSON_GetObjectItemCaseSensitive(_application_data_influence_data_subs_to_notify_post_request_innerJSON, "notifCorreId");
    if (notif_corre_id) {
    if (!cJSON_IsString(notif_corre_id) && !cJSON_IsNull(notif_corre_id)) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [notif_corre_id]");
        goto end;
    }
    }

    dnai_chg_type = cJSON_GetObjectItemCaseSensitive(_application_data_influence_data_subs_to_notify_post_request_innerJSON, "dnaiChgType");
    if (dnai_chg_type) {
    if (!cJSON_IsString(dnai_chg_type)) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [dnai_chg_type]");
        goto end;
    }
    dnai_chg_typeVariable = OpenAPI_dnai_change_type_FromString(dnai_chg_type->valuestring);
    }

    af_ack_ind = cJSON_GetObjectItemCaseSensitive(_application_data_influence_data_subs_to_notify_post_request_innerJSON, "afAckInd");
    if (af_ack_ind) {
    if (!cJSON_IsBool(af_ack_ind)) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [af_ack_ind]");
        goto end;
    }
    }

    addr_preser_ind = cJSON_GetObjectItemCaseSensitive(_application_data_influence_data_subs_to_notify_post_request_innerJSON, "addrPreserInd");
    if (addr_preser_ind) {
    if (!cJSON_IsBool(addr_preser_ind)) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [addr_preser_ind]");
        goto end;
    }
    }

    max_allowed_up_lat = cJSON_GetObjectItemCaseSensitive(_application_data_influence_data_subs_to_notify_post_request_innerJSON, "maxAllowedUpLat");
    if (max_allowed_up_lat) {
    if (!cJSON_IsNumber(max_allowed_up_lat)) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [max_allowed_up_lat]");
        goto end;
    }
    }

    sim_conn_ind = cJSON_GetObjectItemCaseSensitive(_application_data_influence_data_subs_to_notify_post_request_innerJSON, "simConnInd");
    if (sim_conn_ind) {
    if (!cJSON_IsBool(sim_conn_ind)) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [sim_conn_ind]");
        goto end;
    }
    }

    sim_conn_term = cJSON_GetObjectItemCaseSensitive(_application_data_influence_data_subs_to_notify_post_request_innerJSON, "simConnTerm");
    if (sim_conn_term) {
    if (!cJSON_IsNumber(sim_conn_term)) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [sim_conn_term]");
        goto end;
    }
    }

    n6_delay_ind = cJSON_GetObjectItemCaseSensitive(_application_data_influence_data_subs_to_notify_post_request_innerJSON, "n6DelayInd");
    if (n6_delay_ind) {
    if (!cJSON_IsBool(n6_delay_ind)) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [n6_delay_ind]");
        goto end;
    }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(_application_data_influence_data_subs_to_notify_post_request_innerJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    res_uri = cJSON_GetObjectItemCaseSensitive(_application_data_influence_data_subs_to_notify_post_request_innerJSON, "resUri");
    if (res_uri) {
    if (!cJSON_IsString(res_uri) && !cJSON_IsNull(res_uri)) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [res_uri]");
        goto end;
    }
    }

    reset_ids = cJSON_GetObjectItemCaseSensitive(_application_data_influence_data_subs_to_notify_post_request_innerJSON, "resetIds");
    if (reset_ids) {
        cJSON *reset_ids_local = NULL;
        if (!cJSON_IsArray(reset_ids)) {
            ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [reset_ids]");
            goto end;
        }

        reset_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(reset_ids_local, reset_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(reset_ids_local)) {
                ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [reset_ids]");
                goto end;
            }
            OpenAPI_list_add(reset_idsList, ogs_strdup(reset_ids_local->valuestring));
        }
    }

    nsc_supp_feats = cJSON_GetObjectItemCaseSensitive(_application_data_influence_data_subs_to_notify_post_request_innerJSON, "nscSuppFeats");
    if (nsc_supp_feats) {
        cJSON *nsc_supp_feats_local_map = NULL;
        if (!cJSON_IsObject(nsc_supp_feats) && !cJSON_IsNull(nsc_supp_feats)) {
            ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [nsc_supp_feats]");
            goto end;
        }
        if (cJSON_IsObject(nsc_supp_feats)) {
            nsc_supp_featsList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(nsc_supp_feats_local_map, nsc_supp_feats) {
                cJSON *localMapObject = nsc_supp_feats_local_map;
                double *localDouble = NULL;
                int *localInt = NULL;
                if (!cJSON_IsString(localMapObject)) {
                    ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [inner]");
                    goto end;
                }
                localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), ogs_strdup(localMapObject->valuestring));
                OpenAPI_list_add(nsc_supp_featsList, localMapKeyPair);
            }
        }
    }

    af_hdr_req = cJSON_GetObjectItemCaseSensitive(_application_data_influence_data_subs_to_notify_post_request_innerJSON, "afHdrReq");
    if (af_hdr_req) {
    if (!cJSON_IsNull(af_hdr_req)) {
    af_hdr_req_local_nonprim = OpenAPI_af_header_handling_control_info_parseFromJSON(af_hdr_req);
    if (!af_hdr_req_local_nonprim) {
        ogs_error("OpenAPI_af_header_handling_control_info_parseFromJSON failed [af_hdr_req]");
        goto end;
    }
    }
    }

    traffic_influ_data = cJSON_GetObjectItemCaseSensitive(_application_data_influence_data_subs_to_notify_post_request_innerJSON, "trafficInfluData");
    if (traffic_influ_data) {
    traffic_influ_data_local_nonprim = OpenAPI_traffic_influ_data_parseFromJSON(traffic_influ_data);
    if (!traffic_influ_data_local_nonprim) {
        ogs_error("OpenAPI_traffic_influ_data_parseFromJSON failed [traffic_influ_data]");
        goto end;
    }
    }

    notif_id = cJSON_GetObjectItemCaseSensitive(_application_data_influence_data_subs_to_notify_post_request_innerJSON, "notifId");
    if (notif_id) {
    if (!cJSON_IsString(notif_id) && !cJSON_IsNull(notif_id)) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON() failed [notif_id]");
        goto end;
    }
    }

    _application_data_influence_data_subs_to_notify_post_request_inner_local_var = OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_create (
        up_path_chg_notif_corre_id && !cJSON_IsNull(up_path_chg_notif_corre_id) ? ogs_strdup(up_path_chg_notif_corre_id->valuestring) : NULL,
        app_relo_ind ? true : false,
        app_relo_ind ? app_relo_ind->valueint : 0,
        af_app_id && !cJSON_IsNull(af_app_id) ? ogs_strdup(af_app_id->valuestring) : NULL,
        dnn && !cJSON_IsNull(dnn) ? ogs_strdup(dnn->valuestring) : NULL,
        eth_traffic_filters ? eth_traffic_filtersList : NULL,
        snssai ? snssai_local_nonprim : NULL,
        inter_group_id && !cJSON_IsNull(inter_group_id) ? ogs_strdup(inter_group_id->valuestring) : NULL,
        inter_group_id_list ? inter_group_id_listList : NULL,
        subscriber_cat_list ? subscriber_cat_listList : NULL,
        plmn_id ? plmn_id_local_nonprim : NULL,
        ipv4_addr && !cJSON_IsNull(ipv4_addr) ? ogs_strdup(ipv4_addr->valuestring) : NULL,
        ipv6_addr && !cJSON_IsNull(ipv6_addr) ? ogs_strdup(ipv6_addr->valuestring) : NULL,
        supi && !cJSON_IsNull(supi) ? ogs_strdup(supi->valuestring) : NULL,
        traffic_data_sets ? traffic_data_setsList : NULL,
        traffic_filters ? traffic_filtersList : NULL,
        traffic_routes ? traffic_routesList : NULL,
        sfc_id_dl && !cJSON_IsNull(sfc_id_dl) ? ogs_strdup(sfc_id_dl->valuestring) : NULL,
        sfc_id_ul && !cJSON_IsNull(sfc_id_ul) ? ogs_strdup(sfc_id_ul->valuestring) : NULL,
        metadata && cJSON_IsNull(metadata) ? true : false,
        metadata && !cJSON_IsNull(metadata) ? ogs_strdup(metadata->valuestring) : NULL,
        traff_corre_ind ? true : false,
        traff_corre_ind ? traff_corre_ind->valueint : 0,
        tfc_corre_info && cJSON_IsNull(tfc_corre_info) ? true : false,
        tfc_corre_info ? tfc_corre_info_local_nonprim : NULL,
        cand_dnai_ind ? true : false,
        cand_dnai_ind ? cand_dnai_ind->valueint : 0,
        valid_start_time && !cJSON_IsNull(valid_start_time) ? ogs_strdup(valid_start_time->valuestring) : NULL,
        valid_end_time && !cJSON_IsNull(valid_end_time) ? ogs_strdup(valid_end_time->valuestring) : NULL,
        temp_validities ? temp_validitiesList : NULL,
        nw_area_info ? nw_area_info_local_nonprim : NULL,
        up_path_chg_notif_uri && !cJSON_IsNull(up_path_chg_notif_uri) ? ogs_strdup(up_path_chg_notif_uri->valuestring) : NULL,
        headers ? headersList : NULL,
        subscribed_events ? subscribed_eventsList : NULL,
        notif_uri && !cJSON_IsNull(notif_uri) ? ogs_strdup(notif_uri->valuestring) : NULL,
        notif_corre_id && !cJSON_IsNull(notif_corre_id) ? ogs_strdup(notif_corre_id->valuestring) : NULL,
        dnai_chg_type ? dnai_chg_typeVariable : 0,
        af_ack_ind ? true : false,
        af_ack_ind ? af_ack_ind->valueint : 0,
        addr_preser_ind ? true : false,
        addr_preser_ind ? addr_preser_ind->valueint : 0,
        max_allowed_up_lat ? true : false,
        max_allowed_up_lat ? max_allowed_up_lat->valuedouble : 0,
        sim_conn_ind ? true : false,
        sim_conn_ind ? sim_conn_ind->valueint : 0,
        sim_conn_term ? true : false,
        sim_conn_term ? sim_conn_term->valuedouble : 0,
        n6_delay_ind ? true : false,
        n6_delay_ind ? n6_delay_ind->valueint : 0,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL,
        res_uri && !cJSON_IsNull(res_uri) ? ogs_strdup(res_uri->valuestring) : NULL,
        reset_ids ? reset_idsList : NULL,
        nsc_supp_feats ? nsc_supp_featsList : NULL,
        af_hdr_req && cJSON_IsNull(af_hdr_req) ? true : false,
        af_hdr_req ? af_hdr_req_local_nonprim : NULL,
        traffic_influ_data ? traffic_influ_data_local_nonprim : NULL,
        notif_id && !cJSON_IsNull(notif_id) ? ogs_strdup(notif_id->valuestring) : NULL
    );

    return _application_data_influence_data_subs_to_notify_post_request_inner_local_var;
end:
    if (eth_traffic_filtersList) {
        OpenAPI_list_for_each(eth_traffic_filtersList, node) {
            OpenAPI_eth_flow_description_free(node->data);
        }
        OpenAPI_list_free(eth_traffic_filtersList);
        eth_traffic_filtersList = NULL;
    }
    if (snssai_local_nonprim) {
        OpenAPI_snssai_free(snssai_local_nonprim);
        snssai_local_nonprim = NULL;
    }
    if (inter_group_id_listList) {
        OpenAPI_list_for_each(inter_group_id_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(inter_group_id_listList);
        inter_group_id_listList = NULL;
    }
    if (subscriber_cat_listList) {
        OpenAPI_list_for_each(subscriber_cat_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(subscriber_cat_listList);
        subscriber_cat_listList = NULL;
    }
    if (plmn_id_local_nonprim) {
        OpenAPI_plmn_id_1_free(plmn_id_local_nonprim);
        plmn_id_local_nonprim = NULL;
    }
    if (traffic_data_setsList) {
        OpenAPI_list_for_each(traffic_data_setsList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_traffic_data_set_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(traffic_data_setsList);
        traffic_data_setsList = NULL;
    }
    if (traffic_filtersList) {
        OpenAPI_list_for_each(traffic_filtersList, node) {
            OpenAPI_flow_info_free(node->data);
        }
        OpenAPI_list_free(traffic_filtersList);
        traffic_filtersList = NULL;
    }
    if (traffic_routesList) {
        OpenAPI_list_for_each(traffic_routesList, node) {
            OpenAPI_route_to_location_free(node->data);
        }
        OpenAPI_list_free(traffic_routesList);
        traffic_routesList = NULL;
    }
    if (tfc_corre_info_local_nonprim) {
        OpenAPI_traffic_correlation_info_free(tfc_corre_info_local_nonprim);
        tfc_corre_info_local_nonprim = NULL;
    }
    if (temp_validitiesList) {
        OpenAPI_list_for_each(temp_validitiesList, node) {
            OpenAPI_temporal_validity_free(node->data);
        }
        OpenAPI_list_free(temp_validitiesList);
        temp_validitiesList = NULL;
    }
    if (nw_area_info_local_nonprim) {
        OpenAPI_network_area_info_1_free(nw_area_info_local_nonprim);
        nw_area_info_local_nonprim = NULL;
    }
    if (headersList) {
        OpenAPI_list_for_each(headersList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(headersList);
        headersList = NULL;
    }
    if (subscribed_eventsList) {
        OpenAPI_list_free(subscribed_eventsList);
        subscribed_eventsList = NULL;
    }
    if (reset_idsList) {
        OpenAPI_list_for_each(reset_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(reset_idsList);
        reset_idsList = NULL;
    }
    if (nsc_supp_featsList) {
        OpenAPI_list_for_each(nsc_supp_featsList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nsc_supp_featsList);
        nsc_supp_featsList = NULL;
    }
    if (af_hdr_req_local_nonprim) {
        OpenAPI_af_header_handling_control_info_free(af_hdr_req_local_nonprim);
        af_hdr_req_local_nonprim = NULL;
    }
    if (traffic_influ_data_local_nonprim) {
        OpenAPI_traffic_influ_data_free(traffic_influ_data_local_nonprim);
        traffic_influ_data_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_t *OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_copy(OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_t *dst, OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON() failed");
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

    OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_free(dst);
    dst = OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

