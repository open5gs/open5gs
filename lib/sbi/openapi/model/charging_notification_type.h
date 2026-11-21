/*
 * charging_notification_type.h
 *
 * 
 */

#ifndef _OpenAPI_charging_notification_type_H_
#define _OpenAPI_charging_notification_type_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_charging_notification_type_NULL = 0, OpenAPI_charging_notification_type_REAUTHORIZATION, OpenAPI_charging_notification_type_ABORT_CHARGING } OpenAPI_charging_notification_type_e;

char* OpenAPI_charging_notification_type_ToString(OpenAPI_charging_notification_type_e charging_notification_type);

OpenAPI_charging_notification_type_e OpenAPI_charging_notification_type_FromString(char* charging_notification_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_charging_notification_type_H_ */

