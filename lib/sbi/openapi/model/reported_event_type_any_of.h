/*
 * reported_event_type_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_reported_event_type_any_of_H_
#define _OpenAPI_reported_event_type_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_reported_event_type_any_of_NULL = 0, OpenAPI_reported_event_type_any_of_PERIODIC_EVENT, OpenAPI_reported_event_type_any_of_ENTERING_AREA_EVENT, OpenAPI_reported_event_type_any_of_LEAVING_AREA_EVENT, OpenAPI_reported_event_type_any_of_BEING_INSIDE_AREA_EVENT, OpenAPI_reported_event_type_any_of_MOTION_EVENT, OpenAPI_reported_event_type_any_of_MAXIMUM_INTERVAL_EXPIRATION_EVENT, OpenAPI_reported_event_type_any_of_LOCATION_CANCELLATION_EVENT } OpenAPI_reported_event_type_any_of_e;

char* OpenAPI_reported_event_type_any_of_ToString(OpenAPI_reported_event_type_any_of_e reported_event_type_any_of);

OpenAPI_reported_event_type_any_of_e OpenAPI_reported_event_type_any_of_FromString(char* reported_event_type_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_reported_event_type_any_of_H_ */

