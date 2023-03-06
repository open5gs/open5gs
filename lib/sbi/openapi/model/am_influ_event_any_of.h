/*
 * am_influ_event_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_am_influ_event_any_of_H_
#define _OpenAPI_am_influ_event_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_am_influ_event_any_of_NULL = 0, OpenAPI_am_influ_event_any_of_SERVICE_AREA_COVRG_OUTCOME } OpenAPI_am_influ_event_any_of_e;

char* OpenAPI_am_influ_event_any_of_ToString(OpenAPI_am_influ_event_any_of_e am_influ_event_any_of);

OpenAPI_am_influ_event_any_of_e OpenAPI_am_influ_event_any_of_FromString(char* am_influ_event_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_am_influ_event_any_of_H_ */

