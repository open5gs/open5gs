/*
 * event_type.h
 *
 * Event Type
 */

#ifndef _OpenAPI_event_type_H_
#define _OpenAPI_event_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_event_type_NULL = 0, OpenAPI_event_type_QOS_MONITORING, OpenAPI_event_type_USER_DATA_USAGE_MEASURES, OpenAPI_event_type_USER_DATA_USAGE_TRENDS, OpenAPI_event_type_TSC_MNGT_INFO, OpenAPI_event_type_UE_NAT_MAPPING_INFO, OpenAPI_event_type_HANDLING_OF_PAYLOAD_HEADERS_INFO, OpenAPI_event_type_SUBSCRIPTION_TERMINATION } OpenAPI_event_type_e;

char* OpenAPI_event_type_ToString(OpenAPI_event_type_e event_type);

OpenAPI_event_type_e OpenAPI_event_type_FromString(char* event_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_event_type_H_ */

