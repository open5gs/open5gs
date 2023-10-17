/*
 * dn_performance_req.h
 *
 * Represents other DN performance analytics requirements.
 */

#ifndef _OpenAPI_dn_performance_req_H_
#define _OpenAPI_dn_performance_req_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "dn_perf_ordering_criterion.h"
#include "matching_direction.h"
#include "threshold_level.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_dn_performance_req_s OpenAPI_dn_performance_req_t;
typedef struct OpenAPI_dn_performance_req_s {
    struct OpenAPI_dn_perf_ordering_criterion_s *dn_perf_order_criter;
    struct OpenAPI_matching_direction_s *order;
    OpenAPI_list_t *report_thresholds;
} OpenAPI_dn_performance_req_t;

OpenAPI_dn_performance_req_t *OpenAPI_dn_performance_req_create(
    OpenAPI_dn_perf_ordering_criterion_t *dn_perf_order_criter,
    OpenAPI_matching_direction_t *order,
    OpenAPI_list_t *report_thresholds
);
void OpenAPI_dn_performance_req_free(OpenAPI_dn_performance_req_t *dn_performance_req);
OpenAPI_dn_performance_req_t *OpenAPI_dn_performance_req_parseFromJSON(cJSON *dn_performance_reqJSON);
cJSON *OpenAPI_dn_performance_req_convertToJSON(OpenAPI_dn_performance_req_t *dn_performance_req);
OpenAPI_dn_performance_req_t *OpenAPI_dn_performance_req_copy(OpenAPI_dn_performance_req_t *dst, OpenAPI_dn_performance_req_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dn_performance_req_H_ */

