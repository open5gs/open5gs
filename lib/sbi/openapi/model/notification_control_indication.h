/*
 * notification_control_indication.h
 *
 * 
 */

#ifndef _OpenAPI_notification_control_indication_H_
#define _OpenAPI_notification_control_indication_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_notification_control_indication_NULL = 0, OpenAPI_notification_control_indication_DDN_FAILURE, OpenAPI_notification_control_indication_DDD_STATUS } OpenAPI_notification_control_indication_e;

char* OpenAPI_notification_control_indication_ToString(OpenAPI_notification_control_indication_e notification_control_indication);

OpenAPI_notification_control_indication_e OpenAPI_notification_control_indication_FromString(char* notification_control_indication);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_notification_control_indication_H_ */

