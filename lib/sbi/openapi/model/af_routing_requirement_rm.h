/*
 * af_routing_requirement_rm.h
 *
 * this data type is defined in the same way as the AfRoutingRequirement data type, but with the OpenAPI nullable property set to true and the spVal and tempVals attributes defined as removable.
 */

#ifndef _OpenAPI_af_routing_requirement_rm_H_
#define _OpenAPI_af_routing_requirement_rm_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
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
    OpenAPI_list_t *route_to_locs;
    struct OpenAPI_spatial_validity_rm_s *sp_val;
    OpenAPI_list_t *temp_vals;
    struct OpenAPI_up_path_chg_event_s *up_path_chg_sub;
    bool is_addr_preser_ind;
    int addr_preser_ind;
} OpenAPI_af_routing_requirement_rm_t;

OpenAPI_af_routing_requirement_rm_t *OpenAPI_af_routing_requirement_rm_create(
    bool is_app_reloc,
    int app_reloc,
    OpenAPI_list_t *route_to_locs,
    OpenAPI_spatial_validity_rm_t *sp_val,
    OpenAPI_list_t *temp_vals,
    OpenAPI_up_path_chg_event_t *up_path_chg_sub,
    bool is_addr_preser_ind,
    int addr_preser_ind
);
void OpenAPI_af_routing_requirement_rm_free(OpenAPI_af_routing_requirement_rm_t *af_routing_requirement_rm);
OpenAPI_af_routing_requirement_rm_t *OpenAPI_af_routing_requirement_rm_parseFromJSON(cJSON *af_routing_requirement_rmJSON);
cJSON *OpenAPI_af_routing_requirement_rm_convertToJSON(OpenAPI_af_routing_requirement_rm_t *af_routing_requirement_rm);
OpenAPI_af_routing_requirement_rm_t *OpenAPI_af_routing_requirement_rm_copy(OpenAPI_af_routing_requirement_rm_t *dst, OpenAPI_af_routing_requirement_rm_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_af_routing_requirement_rm_H_ */

