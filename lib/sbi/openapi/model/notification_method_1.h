/*
 * notification_method_1.h
 *
 * 
 */

#ifndef _OpenAPI_notification_method_1_H_
#define _OpenAPI_notification_method_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_notification_method_1_NULL = 0, OpenAPI_notification_method_1_PERIODIC, OpenAPI_notification_method_1_ONE_TIME, OpenAPI_notification_method_1_ON_EVENT_DETECTION } OpenAPI_notification_method_1_e;

char* OpenAPI_notification_method_1_ToString(OpenAPI_notification_method_1_e notification_method_1);

OpenAPI_notification_method_1_e OpenAPI_notification_method_1_FromString(char* notification_method_1);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_notification_method_1_H_ */

