/*
 * dn_perf_ordering_criterion.h
 *
 * 
 */

#ifndef _OpenAPI_dn_perf_ordering_criterion_H_
#define _OpenAPI_dn_perf_ordering_criterion_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_dn_perf_ordering_criterion_NULL = 0, OpenAPI_dn_perf_ordering_criterion_AVERAGE_TRAFFIC_RATE, OpenAPI_dn_perf_ordering_criterion_MAXIMUM_TRAFFIC_RATE, OpenAPI_dn_perf_ordering_criterion_AVERAGE_PACKET_DELAY, OpenAPI_dn_perf_ordering_criterion_MAXIMUM_PACKET_DELAY, OpenAPI_dn_perf_ordering_criterion_AVERAGE_PACKET_LOSS_RATE } OpenAPI_dn_perf_ordering_criterion_e;

char* OpenAPI_dn_perf_ordering_criterion_ToString(OpenAPI_dn_perf_ordering_criterion_e dn_perf_ordering_criterion);

OpenAPI_dn_perf_ordering_criterion_e OpenAPI_dn_perf_ordering_criterion_FromString(char* dn_perf_ordering_criterion);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dn_perf_ordering_criterion_H_ */

