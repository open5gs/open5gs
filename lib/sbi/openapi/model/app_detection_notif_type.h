/*
 * app_detection_notif_type.h
 *
 * 
 */

#ifndef _OpenAPI_app_detection_notif_type_H_
#define _OpenAPI_app_detection_notif_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_app_detection_notif_type_NULL = 0, OpenAPI_app_detection_notif_type_APP_START, OpenAPI_app_detection_notif_type_APP_STOP } OpenAPI_app_detection_notif_type_e;

char* OpenAPI_app_detection_notif_type_ToString(OpenAPI_app_detection_notif_type_e app_detection_notif_type);

OpenAPI_app_detection_notif_type_e OpenAPI_app_detection_notif_type_FromString(char* app_detection_notif_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_app_detection_notif_type_H_ */

