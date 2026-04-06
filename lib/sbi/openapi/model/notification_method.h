/*
 * notification_method.h
 *
 * 
 */

#ifndef _OpenAPI_notification_method_H_
#define _OpenAPI_notification_method_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_notification_method_NULL = 0, OpenAPI_notification_method_PERIODIC, OpenAPI_notification_method_THRESHOLD } OpenAPI_notification_method_e;

char* OpenAPI_notification_method_ToString(OpenAPI_notification_method_e notification_method);

OpenAPI_notification_method_e OpenAPI_notification_method_FromString(char* notification_method);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_notification_method_H_ */

