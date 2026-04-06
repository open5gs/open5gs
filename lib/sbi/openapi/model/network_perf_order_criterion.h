/*
 * network_perf_order_criterion.h
 *
 * 
 */

#ifndef _OpenAPI_network_perf_order_criterion_H_
#define _OpenAPI_network_perf_order_criterion_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_network_perf_order_criterion_NULL = 0, OpenAPI_network_perf_order_criterion_NUMBER_OF_UES, OpenAPI_network_perf_order_criterion_COMMUNICATION_PERF, OpenAPI_network_perf_order_criterion_MOBILITY_PERF } OpenAPI_network_perf_order_criterion_e;

char* OpenAPI_network_perf_order_criterion_ToString(OpenAPI_network_perf_order_criterion_e network_perf_order_criterion);

OpenAPI_network_perf_order_criterion_e OpenAPI_network_perf_order_criterion_FromString(char* network_perf_order_criterion);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_network_perf_order_criterion_H_ */

