/*
 * am_influ_event.h
 *
 * 
 */

#ifndef _OpenAPI_am_influ_event_H_
#define _OpenAPI_am_influ_event_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_am_influ_event_NULL = 0, OpenAPI_am_influ_event_SERVICE_AREA_COVRG_OUTCOME, OpenAPI_am_influ_event_SLICE_REPLACE_OUTCOME } OpenAPI_am_influ_event_e;

char* OpenAPI_am_influ_event_ToString(OpenAPI_am_influ_event_e am_influ_event);

OpenAPI_am_influ_event_e OpenAPI_am_influ_event_FromString(char* am_influ_event);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_am_influ_event_H_ */

