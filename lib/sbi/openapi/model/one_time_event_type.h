/*
 * one_time_event_type.h
 *
 * 
 */

#ifndef _OpenAPI_one_time_event_type_H_
#define _OpenAPI_one_time_event_type_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_one_time_event_type_NULL = 0, OpenAPI_one_time_event_type_IEC, OpenAPI_one_time_event_type_PEC } OpenAPI_one_time_event_type_e;

char* OpenAPI_one_time_event_type_ToString(OpenAPI_one_time_event_type_e one_time_event_type);

OpenAPI_one_time_event_type_e OpenAPI_one_time_event_type_FromString(char* one_time_event_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_one_time_event_type_H_ */

