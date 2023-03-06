/*
 * dn_perf_ordering_criterion_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_dn_perf_ordering_criterion_any_of_H_
#define _OpenAPI_dn_perf_ordering_criterion_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_dn_perf_ordering_criterion_any_of_NULL = 0, OpenAPI_dn_perf_ordering_criterion_any_of_AVERAGE_TRAFFIC_RATE, OpenAPI_dn_perf_ordering_criterion_any_of_MAXIMUM_TRAFFIC_RATE, OpenAPI_dn_perf_ordering_criterion_any_of_AVERAGE_PACKET_DELAY, OpenAPI_dn_perf_ordering_criterion_any_of_MAXIMUM_PACKET_DELAY, OpenAPI_dn_perf_ordering_criterion_any_of_AVERAGE_PACKET_LOSS_RATE } OpenAPI_dn_perf_ordering_criterion_any_of_e;

char* OpenAPI_dn_perf_ordering_criterion_any_of_ToString(OpenAPI_dn_perf_ordering_criterion_any_of_e dn_perf_ordering_criterion_any_of);

OpenAPI_dn_perf_ordering_criterion_any_of_e OpenAPI_dn_perf_ordering_criterion_any_of_FromString(char* dn_perf_ordering_criterion_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dn_perf_ordering_criterion_any_of_H_ */

