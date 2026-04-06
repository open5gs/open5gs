/*
 * _application_data_influence_data_subs_to_notify_post_request_inner.h
 *
 * 
 */

#ifndef _OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_H_
#define _OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_s OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_t;
#include "af_header_handling_control_info.h"
#include "dnai_change_type.h"
#include "eth_flow_description.h"
#include "flow_info.h"
#include "network_area_info_1.h"
#include "plmn_id_1.h"
#include "route_to_location.h"
#include "snssai.h"
#include "subscribed_event.h"
#include "temporal_validity.h"
#include "traffic_correlation_info.h"
#include "traffic_data_set.h"
#include "traffic_influ_data.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_s {
    char *up_path_chg_notif_corre_id;
    bool is_app_relo_ind;
    int app_relo_ind;
    char *af_app_id;
    char *dnn;
    OpenAPI_list_t *eth_traffic_filters;
    struct OpenAPI_snssai_s *snssai;
    char *inter_group_id;
    OpenAPI_list_t *inter_group_id_list;
    OpenAPI_list_t *subscriber_cat_list;
    struct OpenAPI_plmn_id_1_s *plmn_id;
    char *ipv4_addr;
    char *ipv6_addr;
    char *supi;
    OpenAPI_list_t* traffic_data_sets;
    OpenAPI_list_t *traffic_filters;
    OpenAPI_list_t *traffic_routes;
    char *sfc_id_dl;
    char *sfc_id_ul;
    bool is_metadata_null;
    char *metadata;
    bool is_traff_corre_ind;
    int traff_corre_ind;
    bool is_tfc_corre_info_null;
    struct OpenAPI_traffic_correlation_info_s *tfc_corre_info;
    bool is_cand_dnai_ind;
    int cand_dnai_ind;
    char *valid_start_time;
    char *valid_end_time;
    OpenAPI_list_t *temp_validities;
    struct OpenAPI_network_area_info_1_s *nw_area_info;
    char *up_path_chg_notif_uri;
    OpenAPI_list_t *headers;
    OpenAPI_list_t *subscribed_events;
    char *notif_uri;
    char *notif_corre_id;
    OpenAPI_dnai_change_type_e dnai_chg_type;
    bool is_af_ack_ind;
    int af_ack_ind;
    bool is_addr_preser_ind;
    int addr_preser_ind;
    bool is_max_allowed_up_lat;
    int max_allowed_up_lat;
    bool is_sim_conn_ind;
    int sim_conn_ind;
    bool is_sim_conn_term;
    int sim_conn_term;
    bool is_n6_delay_ind;
    int n6_delay_ind;
    char *supported_features;
    char *res_uri;
    OpenAPI_list_t *reset_ids;
    OpenAPI_list_t* nsc_supp_feats;
    bool is_af_hdr_req_null;
    struct OpenAPI_af_header_handling_control_info_s *af_hdr_req;
    struct OpenAPI_traffic_influ_data_s *traffic_influ_data;
    char *notif_id;
};

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
);
void OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_free(OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_t *_application_data_influence_data_subs_to_notify_post_request_inner);
OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_t *OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_parseFromJSON(cJSON *_application_data_influence_data_subs_to_notify_post_request_innerJSON);
cJSON *OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_convertToJSON(OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_t *_application_data_influence_data_subs_to_notify_post_request_inner);
OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_t *OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_copy(OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_t *dst, OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI__application_data_influence_data_subs_to_notify_post_request_inner_H_ */

