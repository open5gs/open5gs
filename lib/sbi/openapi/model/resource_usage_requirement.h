/*
 * resource_usage_requirement.h
 *
 * resource usage requirement.
 */

#ifndef _OpenAPI_resource_usage_requirement_H_
#define _OpenAPI_resource_usage_requirement_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_resource_usage_requirement_s OpenAPI_resource_usage_requirement_t;
#include "traffic_direction.h"
#include "value_expression.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_resource_usage_requirement_s {
    OpenAPI_traffic_direction_e tfc_dirc;
    OpenAPI_value_expression_e val_exp;
};

OpenAPI_resource_usage_requirement_t *OpenAPI_resource_usage_requirement_create(
    OpenAPI_traffic_direction_e tfc_dirc,
    OpenAPI_value_expression_e val_exp
);
void OpenAPI_resource_usage_requirement_free(OpenAPI_resource_usage_requirement_t *resource_usage_requirement);
OpenAPI_resource_usage_requirement_t *OpenAPI_resource_usage_requirement_parseFromJSON(cJSON *resource_usage_requirementJSON);
cJSON *OpenAPI_resource_usage_requirement_convertToJSON(OpenAPI_resource_usage_requirement_t *resource_usage_requirement);
OpenAPI_resource_usage_requirement_t *OpenAPI_resource_usage_requirement_copy(OpenAPI_resource_usage_requirement_t *dst, OpenAPI_resource_usage_requirement_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_resource_usage_requirement_H_ */

