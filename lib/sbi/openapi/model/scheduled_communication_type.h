/*
 * scheduled_communication_type.h
 *
 *
 */

#ifndef _OpenAPI_scheduled_communication_type_H_
#define _OpenAPI_scheduled_communication_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_scheduled_communication_type_NULL = 0, OpenAPI_scheduled_communication_type_DOWNLINK_ONLY, OpenAPI_scheduled_communication_type_UPLINK_ONLY, OpenAPI_scheduled_communication_type_BIDIRECTIONAL } OpenAPI_scheduled_communication_type_e;

char* OpenAPI_scheduled_communication_type_ToString(OpenAPI_scheduled_communication_type_e scheduled_communication_type);

OpenAPI_scheduled_communication_type_e OpenAPI_scheduled_communication_type_FromString(char* scheduled_communication_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_scheduled_communication_type_H_ */

