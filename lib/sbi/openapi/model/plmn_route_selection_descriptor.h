/*
 * plmn_route_selection_descriptor.h
 *
 * Contains the route selection descriptors (combinations of SNSSAI, DNNs, PDU session types, SSC modes and ATSSS information) allowed by subscription to the UE for a serving PLMN
 */

#ifndef _OpenAPI_plmn_route_selection_descriptor_H_
#define _OpenAPI_plmn_route_selection_descriptor_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "plmn_id_1.h"
#include "snssai_route_selection_descriptor.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_plmn_route_selection_descriptor_s OpenAPI_plmn_route_selection_descriptor_t;
typedef struct OpenAPI_plmn_route_selection_descriptor_s {
    struct OpenAPI_plmn_id_1_s *serving_plmn;
    OpenAPI_list_t *snssai_route_sel_descs;
} OpenAPI_plmn_route_selection_descriptor_t;

OpenAPI_plmn_route_selection_descriptor_t *OpenAPI_plmn_route_selection_descriptor_create(
    OpenAPI_plmn_id_1_t *serving_plmn,
    OpenAPI_list_t *snssai_route_sel_descs
);
void OpenAPI_plmn_route_selection_descriptor_free(OpenAPI_plmn_route_selection_descriptor_t *plmn_route_selection_descriptor);
OpenAPI_plmn_route_selection_descriptor_t *OpenAPI_plmn_route_selection_descriptor_parseFromJSON(cJSON *plmn_route_selection_descriptorJSON);
cJSON *OpenAPI_plmn_route_selection_descriptor_convertToJSON(OpenAPI_plmn_route_selection_descriptor_t *plmn_route_selection_descriptor);
OpenAPI_plmn_route_selection_descriptor_t *OpenAPI_plmn_route_selection_descriptor_copy(OpenAPI_plmn_route_selection_descriptor_t *dst, OpenAPI_plmn_route_selection_descriptor_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_plmn_route_selection_descriptor_H_ */

