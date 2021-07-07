/*
 * notification_control.h
 *
 * 
 */

#ifndef _OpenAPI_notification_control_H_
#define _OpenAPI_notification_control_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_notification_control_NULL = 0, OpenAPI_notification_control_REQUESTED, OpenAPI_notification_control_NOT_REQUESTED } OpenAPI_notification_control_e;

char* OpenAPI_notification_control_ToString(OpenAPI_notification_control_e notification_control);

OpenAPI_notification_control_e OpenAPI_notification_control_FromString(char* notification_control);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_notification_control_H_ */

