/*
 * subscribed_event.h
 *
 * 
 */

#ifndef _OpenAPI_subscribed_event_H_
#define _OpenAPI_subscribed_event_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_subscribed_event_NULL = 0, OpenAPI_subscribed_event_UP_PATH_CHANGE, OpenAPI_subscribed_event_TRAFF_ROUTE_REQ_OUTCOME, OpenAPI_subscribed_event_SIM_CONN_FAIL } OpenAPI_subscribed_event_e;

char* OpenAPI_subscribed_event_ToString(OpenAPI_subscribed_event_e subscribed_event);

OpenAPI_subscribed_event_e OpenAPI_subscribed_event_FromString(char* subscribed_event);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_subscribed_event_H_ */

