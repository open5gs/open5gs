/*
 * condition_event_type.h
 *
 * 
 */

#ifndef _OpenAPI_condition_event_type_H_
#define _OpenAPI_condition_event_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_condition_event_type_NULL = 0, OpenAPI_condition_event_type_NF_ADDED, OpenAPI_condition_event_type_NF_REMOVED } OpenAPI_condition_event_type_e;

char* OpenAPI_condition_event_type_ToString(OpenAPI_condition_event_type_e condition_event_type);

OpenAPI_condition_event_type_e OpenAPI_condition_event_type_FromString(char* condition_event_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_condition_event_type_H_ */

