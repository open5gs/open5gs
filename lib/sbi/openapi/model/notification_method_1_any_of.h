/*
 * notification_method_1_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_notification_method_1_any_of_H_
#define _OpenAPI_notification_method_1_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_notification_method_1_any_of_NULL = 0, OpenAPI_notification_method_1_any_of_PERIODIC, OpenAPI_notification_method_1_any_of_ONE_TIME, OpenAPI_notification_method_1_any_of_ON_EVENT_DETECTION } OpenAPI_notification_method_1_any_of_e;

char* OpenAPI_notification_method_1_any_of_ToString(OpenAPI_notification_method_1_any_of_e notification_method_1_any_of);

OpenAPI_notification_method_1_any_of_e OpenAPI_notification_method_1_any_of_FromString(char* notification_method_1_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_notification_method_1_any_of_H_ */

