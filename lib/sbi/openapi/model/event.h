/*
 * event.h
 *
 * 
 */

#ifndef _OpenAPI_event_H_
#define _OpenAPI_event_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_event_NULL = 0, OpenAPI_event_SUCCESS_UE_POL_DEL_SP, OpenAPI_event_UNSUCCESS_UE_POL_DEL_SP, OpenAPI_event_PARTLY_UNSUCC_UE_POL_DEL_SP, OpenAPI_event_UNSUCCESS_PCF_SERVICE_AUTHORIZATION } OpenAPI_event_e;

char* OpenAPI_event_ToString(OpenAPI_event_e event);

OpenAPI_event_e OpenAPI_event_FromString(char* event);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_event_H_ */

