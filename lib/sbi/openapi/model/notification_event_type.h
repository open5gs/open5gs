/*
 * notification_event_type.h
 *
 *
 */

#ifndef _OpenAPI_notification_event_type_H_
#define _OpenAPI_notification_event_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_notification_event_type_NULL = 0, OpenAPI_notification_event_type_NF_REGISTERED, OpenAPI_notification_event_type_NF_DEREGISTERED, OpenAPI_notification_event_type_NF_PROFILE_CHANGED } OpenAPI_notification_event_type_e;

char* OpenAPI_notification_event_type_ToString(OpenAPI_notification_event_type_e notification_event_type);

OpenAPI_notification_event_type_e OpenAPI_notification_event_type_FromString(char* notification_event_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_notification_event_type_H_ */

