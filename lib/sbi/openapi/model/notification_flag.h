/*
 * notification_flag.h
 *
 * 
 */

#ifndef _OpenAPI_notification_flag_H_
#define _OpenAPI_notification_flag_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_notification_flag_NULL = 0, OpenAPI_notification_flag_ACTIVATE, OpenAPI_notification_flag_DEACTIVATE, OpenAPI_notification_flag_RETRIEVAL } OpenAPI_notification_flag_e;

char* OpenAPI_notification_flag_ToString(OpenAPI_notification_flag_e notification_flag);

OpenAPI_notification_flag_e OpenAPI_notification_flag_FromString(char* notification_flag);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_notification_flag_H_ */

