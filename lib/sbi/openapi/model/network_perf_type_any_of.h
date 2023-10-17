/*
 * network_perf_type_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_network_perf_type_any_of_H_
#define _OpenAPI_network_perf_type_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_network_perf_type_any_of_NULL = 0, OpenAPI_network_perf_type_any_of_GNB_ACTIVE_RATIO, OpenAPI_network_perf_type_any_of_GNB_COMPUTING_USAGE, OpenAPI_network_perf_type_any_of_GNB_MEMORY_USAGE, OpenAPI_network_perf_type_any_of_GNB_DISK_USAGE, OpenAPI_network_perf_type_any_of_NUM_OF_UE, OpenAPI_network_perf_type_any_of_SESS_SUCC_RATIO, OpenAPI_network_perf_type_any_of_HO_SUCC_RATIO } OpenAPI_network_perf_type_any_of_e;

char* OpenAPI_network_perf_type_any_of_ToString(OpenAPI_network_perf_type_any_of_e network_perf_type_any_of);

OpenAPI_network_perf_type_any_of_e OpenAPI_network_perf_type_any_of_FromString(char* network_perf_type_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_network_perf_type_any_of_H_ */

