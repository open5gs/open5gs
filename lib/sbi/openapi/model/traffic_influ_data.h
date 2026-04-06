/*
 * traffic_influ_data.h
 *
 * 
 */

#ifndef _OpenAPI_traffic_influ_data_H_
#define _OpenAPI_traffic_influ_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_traffic_influ_data_s OpenAPI_traffic_influ_data_t;
#include "af_header_handling_control_info_1.h"
#include "dnai_change_type.h"
#include "eth_flow_description_1.h"
#include "flow_info_1.h"
#include "network_area_info_2.h"
#include "plmn_id.h"
#include "route_to_location_1.h"
#include "snssai.h"
#include "subscribed_event.h"
#include "temporal_validity_1.h"
#include "traffic_correlation_info.h"
#include "traffic_data_set_1.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_traffic_influ_data_s {
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
    struct OpenAPI_plmn_id_s *plmn_id;
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
    struct OpenAPI_network_area_info_2_s *nw_area_info;
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
    struct OpenAPI_af_header_handling_control_info_1_s *af_hdr_req;
};

OpenAPI_traffic_influ_data_t *OpenAPI_traffic_influ_data_create(
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
    OpenAPI_plmn_id_t *plmn_id,
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
    OpenAPI_network_area_info_2_t *nw_area_info,
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
    OpenAPI_af_header_handling_control_info_1_t *af_hdr_req
);
void OpenAPI_traffic_influ_data_free(OpenAPI_traffic_influ_data_t *traffic_influ_data);
OpenAPI_traffic_influ_data_t *OpenAPI_traffic_influ_data_parseFromJSON(cJSON *traffic_influ_dataJSON);
cJSON *OpenAPI_traffic_influ_data_convertToJSON(OpenAPI_traffic_influ_data_t *traffic_influ_data);
OpenAPI_traffic_influ_data_t *OpenAPI_traffic_influ_data_copy(OpenAPI_traffic_influ_data_t *dst, OpenAPI_traffic_influ_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_traffic_influ_data_H_ */

