/*
 * dn_perf_ordering_criterion.h
 *
 * Possible values are:   - AVERAGE_TRAFFIC_RATE: Indicates the average traffic rate.   - MAXIMUM_TRAFFIC_RATE: Indicates the maximum traffic rate.   - AVERAGE_PACKET_DELAY: Indicates the average packet delay.   - MAXIMUM_PACKET_DELAY: Indicates the maximum packet delay.   - AVERAGE_PACKET_LOSS_RATE: Indicates the average packet loss rate. 
 */

#ifndef _OpenAPI_dn_perf_ordering_criterion_H_
#define _OpenAPI_dn_perf_ordering_criterion_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "dn_perf_ordering_criterion_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_dn_perf_ordering_criterion_s OpenAPI_dn_perf_ordering_criterion_t;
typedef struct OpenAPI_dn_perf_ordering_criterion_s {
} OpenAPI_dn_perf_ordering_criterion_t;

OpenAPI_dn_perf_ordering_criterion_t *OpenAPI_dn_perf_ordering_criterion_create(
);
void OpenAPI_dn_perf_ordering_criterion_free(OpenAPI_dn_perf_ordering_criterion_t *dn_perf_ordering_criterion);
OpenAPI_dn_perf_ordering_criterion_t *OpenAPI_dn_perf_ordering_criterion_parseFromJSON(cJSON *dn_perf_ordering_criterionJSON);
cJSON *OpenAPI_dn_perf_ordering_criterion_convertToJSON(OpenAPI_dn_perf_ordering_criterion_t *dn_perf_ordering_criterion);
OpenAPI_dn_perf_ordering_criterion_t *OpenAPI_dn_perf_ordering_criterion_copy(OpenAPI_dn_perf_ordering_criterion_t *dst, OpenAPI_dn_perf_ordering_criterion_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dn_perf_ordering_criterion_H_ */

