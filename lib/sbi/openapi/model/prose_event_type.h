/*
 * prose_event_type.h
 *
 * 
 */

#ifndef _OpenAPI_prose_event_type_H_
#define _OpenAPI_prose_event_type_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_prose_event_type_NULL = 0, OpenAPI_prose_event_type_ANNOUNCING, OpenAPI_prose_event_type_MONITORING, OpenAPI_prose_event_type_MATCH_REPORT } OpenAPI_prose_event_type_e;

char* OpenAPI_prose_event_type_ToString(OpenAPI_prose_event_type_e prose_event_type);

OpenAPI_prose_event_type_e OpenAPI_prose_event_type_FromString(char* prose_event_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_prose_event_type_H_ */

