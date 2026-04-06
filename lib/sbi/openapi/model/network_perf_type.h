/*
 * network_perf_type.h
 *
 * 
 */

#ifndef _OpenAPI_network_perf_type_H_
#define _OpenAPI_network_perf_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_network_perf_type_NULL = 0, OpenAPI_network_perf_type_GNB_ACTIVE_RATIO, OpenAPI_network_perf_type_GNB_COMPUTING_USAGE, OpenAPI_network_perf_type_GNB_MEMORY_USAGE, OpenAPI_network_perf_type_GNB_DISK_USAGE, OpenAPI_network_perf_type_GNB_RSC_USAGE_OVERALL_TRAFFIC, OpenAPI_network_perf_type_GNB_RSC_USAGE_GBR_TRAFFIC, OpenAPI_network_perf_type_GNB_RSC_USAGE_DELAY_CRIT_GBR_TRAFFIC, OpenAPI_network_perf_type_NUM_OF_UE, OpenAPI_network_perf_type_SESS_SUCC_RATIO, OpenAPI_network_perf_type_HO_SUCC_RATIO } OpenAPI_network_perf_type_e;

char* OpenAPI_network_perf_type_ToString(OpenAPI_network_perf_type_e network_perf_type);

OpenAPI_network_perf_type_e OpenAPI_network_perf_type_FromString(char* network_perf_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_network_perf_type_H_ */

