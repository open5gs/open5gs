/*
 * network_perf_requirement.h
 *
 * Represents a network performance requirement.
 */

#ifndef _OpenAPI_network_perf_requirement_H_
#define _OpenAPI_network_perf_requirement_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_network_perf_requirement_s OpenAPI_network_perf_requirement_t;
#include "network_perf_order_criterion.h"
#include "network_perf_type.h"
#include "resource_usage_requirement.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_network_perf_requirement_s {
    OpenAPI_network_perf_type_e nw_perf_type;
    bool is_relative_ratio;
    int relative_ratio;
    bool is_absolute_num;
    int absolute_num;
    OpenAPI_network_perf_order_criterion_e order_criterion;
    struct OpenAPI_resource_usage_requirement_s *rsc_usg_req;
};

OpenAPI_network_perf_requirement_t *OpenAPI_network_perf_requirement_create(
    OpenAPI_network_perf_type_e nw_perf_type,
    bool is_relative_ratio,
    int relative_ratio,
    bool is_absolute_num,
    int absolute_num,
    OpenAPI_network_perf_order_criterion_e order_criterion,
    OpenAPI_resource_usage_requirement_t *rsc_usg_req
);
void OpenAPI_network_perf_requirement_free(OpenAPI_network_perf_requirement_t *network_perf_requirement);
OpenAPI_network_perf_requirement_t *OpenAPI_network_perf_requirement_parseFromJSON(cJSON *network_perf_requirementJSON);
cJSON *OpenAPI_network_perf_requirement_convertToJSON(OpenAPI_network_perf_requirement_t *network_perf_requirement);
OpenAPI_network_perf_requirement_t *OpenAPI_network_perf_requirement_copy(OpenAPI_network_perf_requirement_t *dst, OpenAPI_network_perf_requirement_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_network_perf_requirement_H_ */

