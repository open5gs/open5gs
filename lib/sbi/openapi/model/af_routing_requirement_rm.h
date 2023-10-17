/*
 * af_routing_requirement_rm.h
 *
 * This data type is defined in the same way as the AfRoutingRequirement data type, but with the OpenAPI nullable property set to true and the spVal and tempVals attributes defined as removable. 
 */

#ifndef _OpenAPI_af_routing_requirement_rm_H_
#define _OpenAPI_af_routing_requirement_rm_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "eas_ip_replacement_info.h"
#include "route_to_location.h"
#include "spatial_validity_rm.h"
#include "temporal_validity.h"
#include "up_path_chg_event.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_af_routing_requirement_rm_s OpenAPI_af_routing_requirement_rm_t;
typedef struct OpenAPI_af_routing_requirement_rm_s {
    bool is_app_reloc;
    int app_reloc;
    bool is_route_to_locs_null;
    OpenAPI_list_t *route_to_locs;
    bool is_sp_val_null;
    struct OpenAPI_spatial_validity_rm_s *sp_val;
    bool is_temp_vals_null;
    OpenAPI_list_t *temp_vals;
    bool is_up_path_chg_sub_null;
    struct OpenAPI_up_path_chg_event_s *up_path_chg_sub;
    bool is_addr_preser_ind_null;
    bool is_addr_preser_ind;
    int addr_preser_ind;
    bool is_sim_conn_ind_null;
    bool is_sim_conn_ind;
    int sim_conn_ind;
    bool is_sim_conn_term_null;
    bool is_sim_conn_term;
    int sim_conn_term;
    bool is_eas_ip_replace_infos_null;
    OpenAPI_list_t *eas_ip_replace_infos;
    bool is_eas_redis_ind;
    int eas_redis_ind;
    bool is_max_allowed_up_lat_null;
    bool is_max_allowed_up_lat;
    int max_allowed_up_lat;
} OpenAPI_af_routing_requirement_rm_t;

OpenAPI_af_routing_requirement_rm_t *OpenAPI_af_routing_requirement_rm_create(
    bool is_app_reloc,
    int app_reloc,
    bool is_route_to_locs_null,
    OpenAPI_list_t *route_to_locs,
    bool is_sp_val_null,
    OpenAPI_spatial_validity_rm_t *sp_val,
    bool is_temp_vals_null,
    OpenAPI_list_t *temp_vals,
    bool is_up_path_chg_sub_null,
    OpenAPI_up_path_chg_event_t *up_path_chg_sub,
    bool is_addr_preser_ind_null,
    bool is_addr_preser_ind,
    int addr_preser_ind,
    bool is_sim_conn_ind_null,
    bool is_sim_conn_ind,
    int sim_conn_ind,
    bool is_sim_conn_term_null,
    bool is_sim_conn_term,
    int sim_conn_term,
    bool is_eas_ip_replace_infos_null,
    OpenAPI_list_t *eas_ip_replace_infos,
    bool is_eas_redis_ind,
    int eas_redis_ind,
    bool is_max_allowed_up_lat_null,
    bool is_max_allowed_up_lat,
    int max_allowed_up_lat
);
void OpenAPI_af_routing_requirement_rm_free(OpenAPI_af_routing_requirement_rm_t *af_routing_requirement_rm);
OpenAPI_af_routing_requirement_rm_t *OpenAPI_af_routing_requirement_rm_parseFromJSON(cJSON *af_routing_requirement_rmJSON);
cJSON *OpenAPI_af_routing_requirement_rm_convertToJSON(OpenAPI_af_routing_requirement_rm_t *af_routing_requirement_rm);
OpenAPI_af_routing_requirement_rm_t *OpenAPI_af_routing_requirement_rm_copy(OpenAPI_af_routing_requirement_rm_t *dst, OpenAPI_af_routing_requirement_rm_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_af_routing_requirement_rm_H_ */

