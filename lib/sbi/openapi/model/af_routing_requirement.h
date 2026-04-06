/*
 * af_routing_requirement.h
 *
 * Describes AF requirements on routing traffic.
 */

#ifndef _OpenAPI_af_routing_requirement_H_
#define _OpenAPI_af_routing_requirement_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_af_routing_requirement_s OpenAPI_af_routing_requirement_t;
#include "eas_ip_replacement_info.h"
#include "route_to_location.h"
#include "sim_conn_fail_event.h"
#include "spatial_validity.h"
#include "temporal_validity.h"
#include "traff_route_req_outcome_event.h"
#include "traffic_correlation_info.h"
#include "up_path_chg_event.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_af_routing_requirement_s {
    bool is_app_reloc;
    int app_reloc;
    OpenAPI_list_t *route_to_locs;
    struct OpenAPI_spatial_validity_s *sp_val;
    OpenAPI_list_t *temp_vals;
    bool is_up_path_chg_sub_null;
    struct OpenAPI_up_path_chg_event_s *up_path_chg_sub;
    bool is_outcome_sub_null;
    struct OpenAPI_traff_route_req_outcome_event_s *outcome_sub;
    bool is_sim_conn_fail_sub_null;
    struct OpenAPI_sim_conn_fail_event_s *sim_conn_fail_sub;
    bool is_addr_preser_ind;
    int addr_preser_ind;
    bool is_sim_conn_ind;
    int sim_conn_ind;
    bool is_sim_conn_term;
    int sim_conn_term;
    OpenAPI_list_t *eas_ip_replace_infos;
    bool is_eas_redis_ind;
    int eas_redis_ind;
    bool is_max_allowed_up_lat;
    int max_allowed_up_lat;
    bool is_tfc_corre_info_null;
    struct OpenAPI_traffic_correlation_info_s *tfc_corre_info;
    bool is_cand_dnai_ind;
    int cand_dnai_ind;
    bool is_n6_delay_ind;
    int n6_delay_ind;
};

OpenAPI_af_routing_requirement_t *OpenAPI_af_routing_requirement_create(
    bool is_app_reloc,
    int app_reloc,
    OpenAPI_list_t *route_to_locs,
    OpenAPI_spatial_validity_t *sp_val,
    OpenAPI_list_t *temp_vals,
    bool is_up_path_chg_sub_null,
    OpenAPI_up_path_chg_event_t *up_path_chg_sub,
    bool is_outcome_sub_null,
    OpenAPI_traff_route_req_outcome_event_t *outcome_sub,
    bool is_sim_conn_fail_sub_null,
    OpenAPI_sim_conn_fail_event_t *sim_conn_fail_sub,
    bool is_addr_preser_ind,
    int addr_preser_ind,
    bool is_sim_conn_ind,
    int sim_conn_ind,
    bool is_sim_conn_term,
    int sim_conn_term,
    OpenAPI_list_t *eas_ip_replace_infos,
    bool is_eas_redis_ind,
    int eas_redis_ind,
    bool is_max_allowed_up_lat,
    int max_allowed_up_lat,
    bool is_tfc_corre_info_null,
    OpenAPI_traffic_correlation_info_t *tfc_corre_info,
    bool is_cand_dnai_ind,
    int cand_dnai_ind,
    bool is_n6_delay_ind,
    int n6_delay_ind
);
void OpenAPI_af_routing_requirement_free(OpenAPI_af_routing_requirement_t *af_routing_requirement);
OpenAPI_af_routing_requirement_t *OpenAPI_af_routing_requirement_parseFromJSON(cJSON *af_routing_requirementJSON);
cJSON *OpenAPI_af_routing_requirement_convertToJSON(OpenAPI_af_routing_requirement_t *af_routing_requirement);
OpenAPI_af_routing_requirement_t *OpenAPI_af_routing_requirement_copy(OpenAPI_af_routing_requirement_t *dst, OpenAPI_af_routing_requirement_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_af_routing_requirement_H_ */

