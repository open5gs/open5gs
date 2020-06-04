/*
 * notification_type.h
 *
 *
 */

#ifndef _OpenAPI_notification_type_H_
#define _OpenAPI_notification_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_notification_type_NULL = 0, OpenAPI_notification_type_N1_MESSAGES, OpenAPI_notification_type_N2_INFORMATION, OpenAPI_notification_type_LOCATION_NOTIFICATION, OpenAPI_notification_type_DATA_REMOVAL_NOTIFICATION, OpenAPI_notification_type_DATA_CHANGE_NOTIFICATION, OpenAPI_notification_type_LOCATION_UPDATE_NOTIFICATION } OpenAPI_notification_type_e;

char* OpenAPI_notification_type_ToString(OpenAPI_notification_type_e notification_type);

OpenAPI_notification_type_e OpenAPI_notification_type_FromString(char* notification_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_notification_type_H_ */

