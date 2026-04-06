/*
 * event_notify_data_type.h
 *
 * 
 */

#ifndef _OpenAPI_event_notify_data_type_H_
#define _OpenAPI_event_notify_data_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_event_notify_data_type_NULL = 0, OpenAPI_event_notify_data_type_UE_AVAILABLE, OpenAPI_event_notify_data_type_PERIODIC, OpenAPI_event_notify_data_type_ENTERING_INTO_AREA, OpenAPI_event_notify_data_type_LEAVING_FROM_AREA, OpenAPI_event_notify_data_type_BEING_INSIDE_AREA, OpenAPI_event_notify_data_type_MOTION, OpenAPI_event_notify_data_type_MAXIMUM_INTERVAL_EXPIRATION_EVENT, OpenAPI_event_notify_data_type_LOCATION_CANCELLATION_EVENT, OpenAPI_event_notify_data_type_ACTIVATION_OF_DEFERRED_LOCATION, OpenAPI_event_notify_data_type_UE_MOBILITY_FOR_DEFERRED_LOCATION, OpenAPI_event_notify_data_type__5GC_MT_LR, OpenAPI_event_notify_data_type_DIRECT_REPORT_EVENT, OpenAPI_event_notify_data_type_CUMULATIVE_EVENT_REPORT } OpenAPI_event_notify_data_type_e;

char* OpenAPI_event_notify_data_type_ToString(OpenAPI_event_notify_data_type_e event_notify_data_type);

OpenAPI_event_notify_data_type_e OpenAPI_event_notify_data_type_FromString(char* event_notify_data_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_event_notify_data_type_H_ */

