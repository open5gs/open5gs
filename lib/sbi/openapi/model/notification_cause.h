/*
 * notification_cause.h
 *
 *
 */

#ifndef _OpenAPI_notification_cause_H_
#define _OpenAPI_notification_cause_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_notification_cause_NULL = 0, OpenAPI_notification_cause_QOS_FULFILLED, OpenAPI_notification_cause_QOS_NOT_FULFILLED, OpenAPI_notification_cause_UP_SEC_FULFILLED, OpenAPI_notification_cause_UP_SEC_NOT_FULFILLED } OpenAPI_notification_cause_e;

char* OpenAPI_notification_cause_ToString(OpenAPI_notification_cause_e notification_cause);

OpenAPI_notification_cause_e OpenAPI_notification_cause_FromString(char* notification_cause);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_notification_cause_H_ */

