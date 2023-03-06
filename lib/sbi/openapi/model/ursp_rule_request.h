/*
 * ursp_rule_request.h
 *
 * Contains parameters that can be used to guide the URSP.
 */

#ifndef _OpenAPI_ursp_rule_request_H_
#define _OpenAPI_ursp_rule_request_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "route_selection_parameter_set.h"
#include "traffic_descriptor_components.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ursp_rule_request_s OpenAPI_ursp_rule_request_t;
typedef struct OpenAPI_ursp_rule_request_s {
    struct OpenAPI_traffic_descriptor_components_s *traffic_desc;
    bool is_relat_precedence;
    int relat_precedence;
    OpenAPI_list_t *route_sel_param_sets;
} OpenAPI_ursp_rule_request_t;

OpenAPI_ursp_rule_request_t *OpenAPI_ursp_rule_request_create(
    OpenAPI_traffic_descriptor_components_t *traffic_desc,
    bool is_relat_precedence,
    int relat_precedence,
    OpenAPI_list_t *route_sel_param_sets
);
void OpenAPI_ursp_rule_request_free(OpenAPI_ursp_rule_request_t *ursp_rule_request);
OpenAPI_ursp_rule_request_t *OpenAPI_ursp_rule_request_parseFromJSON(cJSON *ursp_rule_requestJSON);
cJSON *OpenAPI_ursp_rule_request_convertToJSON(OpenAPI_ursp_rule_request_t *ursp_rule_request);
OpenAPI_ursp_rule_request_t *OpenAPI_ursp_rule_request_copy(OpenAPI_ursp_rule_request_t *dst, OpenAPI_ursp_rule_request_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ursp_rule_request_H_ */

