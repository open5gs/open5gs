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
#include "dnai_change_type.h"
#include "eth_flow_description.h"
#include "flow_info.h"
#include "network_area_info_1.h"
#include "route_to_location.h"
#include "snssai.h"
#include "subscribed_event.h"
#include "temporal_validity.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_traffic_influ_data_s OpenAPI_traffic_influ_data_t;
typedef struct OpenAPI_traffic_influ_data_s {
    char *up_path_chg_notif_corre_id;
    bool is_app_relo_ind;
    int app_relo_ind;
    char *af_app_id;
    char *dnn;
    OpenAPI_list_t *eth_traffic_filters;
    struct OpenAPI_snssai_s *snssai;
    char *inter_group_id;
    char *supi;
    OpenAPI_list_t *traffic_filters;
    OpenAPI_list_t *traffic_routes;
    bool is_traff_corre_ind;
    int traff_corre_ind;
    char *valid_start_time;
    char *valid_end_time;
    OpenAPI_list_t *temp_validities;
    struct OpenAPI_network_area_info_1_s *nw_area_info;
    char *up_path_chg_notif_uri;
    OpenAPI_list_t *headers;
    OpenAPI_list_t *subscribed_events;
    OpenAPI_dnai_change_type_e dnai_chg_type;
    bool is_af_ack_ind;
    int af_ack_ind;
    bool is_addr_preser_ind;
    int addr_preser_ind;
    char *supported_features;
    char *res_uri;
} OpenAPI_traffic_influ_data_t;

OpenAPI_traffic_influ_data_t *OpenAPI_traffic_influ_data_create(
    char *up_path_chg_notif_corre_id,
    bool is_app_relo_ind,
    int app_relo_ind,
    char *af_app_id,
    char *dnn,
    OpenAPI_list_t *eth_traffic_filters,
    OpenAPI_snssai_t *snssai,
    char *inter_group_id,
    char *supi,
    OpenAPI_list_t *traffic_filters,
    OpenAPI_list_t *traffic_routes,
    bool is_traff_corre_ind,
    int traff_corre_ind,
    char *valid_start_time,
    char *valid_end_time,
    OpenAPI_list_t *temp_validities,
    OpenAPI_network_area_info_1_t *nw_area_info,
    char *up_path_chg_notif_uri,
    OpenAPI_list_t *headers,
    OpenAPI_list_t *subscribed_events,
    OpenAPI_dnai_change_type_e dnai_chg_type,
    bool is_af_ack_ind,
    int af_ack_ind,
    bool is_addr_preser_ind,
    int addr_preser_ind,
    char *supported_features,
    char *res_uri
);
void OpenAPI_traffic_influ_data_free(OpenAPI_traffic_influ_data_t *traffic_influ_data);
OpenAPI_traffic_influ_data_t *OpenAPI_traffic_influ_data_parseFromJSON(cJSON *traffic_influ_dataJSON);
cJSON *OpenAPI_traffic_influ_data_convertToJSON(OpenAPI_traffic_influ_data_t *traffic_influ_data);
OpenAPI_traffic_influ_data_t *OpenAPI_traffic_influ_data_copy(OpenAPI_traffic_influ_data_t *dst, OpenAPI_traffic_influ_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_traffic_influ_data_H_ */

