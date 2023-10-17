/*
 * event_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_event_any_of_H_
#define _OpenAPI_event_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_event_any_of_NULL = 0, OpenAPI_event_any_of_SUCCESS_UE_POL_DEL_SP, OpenAPI_event_any_of_UNSUCCESS_UE_POL_DEL_SP } OpenAPI_event_any_of_e;

char* OpenAPI_event_any_of_ToString(OpenAPI_event_any_of_e event_any_of);

OpenAPI_event_any_of_e OpenAPI_event_any_of_FromString(char* event_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_event_any_of_H_ */

