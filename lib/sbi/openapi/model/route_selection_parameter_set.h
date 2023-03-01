/*
 * route_selection_parameter_set.h
 *
 * Contains parameters that can be used to guide the Route Selection Descriptors of the URSP. 
 */

#ifndef _OpenAPI_route_selection_parameter_set_H_
#define _OpenAPI_route_selection_parameter_set_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "geographical_area.h"
#include "snssai.h"
#include "tai_1.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_route_selection_parameter_set_s OpenAPI_route_selection_parameter_set_t;
typedef struct OpenAPI_route_selection_parameter_set_s {
    char *dnn;
    struct OpenAPI_snssai_s *snssai;
    bool is_precedence;
    int precedence;
    OpenAPI_list_t *spatial_validity_areas;
    OpenAPI_list_t *spatial_validity_tais;
} OpenAPI_route_selection_parameter_set_t;

OpenAPI_route_selection_parameter_set_t *OpenAPI_route_selection_parameter_set_create(
    char *dnn,
    OpenAPI_snssai_t *snssai,
    bool is_precedence,
    int precedence,
    OpenAPI_list_t *spatial_validity_areas,
    OpenAPI_list_t *spatial_validity_tais
);
void OpenAPI_route_selection_parameter_set_free(OpenAPI_route_selection_parameter_set_t *route_selection_parameter_set);
OpenAPI_route_selection_parameter_set_t *OpenAPI_route_selection_parameter_set_parseFromJSON(cJSON *route_selection_parameter_setJSON);
cJSON *OpenAPI_route_selection_parameter_set_convertToJSON(OpenAPI_route_selection_parameter_set_t *route_selection_parameter_set);
OpenAPI_route_selection_parameter_set_t *OpenAPI_route_selection_parameter_set_copy(OpenAPI_route_selection_parameter_set_t *dst, OpenAPI_route_selection_parameter_set_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_route_selection_parameter_set_H_ */

