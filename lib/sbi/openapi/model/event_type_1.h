/*
 * event_type_1.h
 *
 * Event Type
 */

#ifndef _OpenAPI_event_type_1_H_
#define _OpenAPI_event_type_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_event_type_1_NULL = 0, OpenAPI_event_type_1_QOS_MONITORING, OpenAPI_event_type_1_USER_DATA_USAGE_MEASURES, OpenAPI_event_type_1_USER_DATA_USAGE_TRENDS, OpenAPI_event_type_1_TSC_MNGT_INFO, OpenAPI_event_type_1_UE_NAT_MAPPING_INFO, OpenAPI_event_type_1_HANDLING_OF_PAYLOAD_HEADERS_INFO, OpenAPI_event_type_1_SUBSCRIPTION_TERMINATION } OpenAPI_event_type_1_e;

char* OpenAPI_event_type_1_ToString(OpenAPI_event_type_1_e event_type_1);

OpenAPI_event_type_1_e OpenAPI_event_type_1_FromString(char* event_type_1);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_event_type_1_H_ */

