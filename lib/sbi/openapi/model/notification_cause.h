/*
 * notification_cause.h
 *
 * Possible values are - QOS_FULFILLED - QOS_NOT_FULFILLED - UP_SEC_FULFILLED - UP_SEC_NOT_FULFILLED
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

typedef struct OpenAPI_notification_cause_s OpenAPI_notification_cause_t;
typedef struct OpenAPI_notification_cause_s {
} OpenAPI_notification_cause_t;

OpenAPI_notification_cause_t *OpenAPI_notification_cause_create(
    );
void OpenAPI_notification_cause_free(OpenAPI_notification_cause_t *notification_cause);
OpenAPI_notification_cause_t *OpenAPI_notification_cause_parseFromJSON(cJSON *notification_causeJSON);
cJSON *OpenAPI_notification_cause_convertToJSON(OpenAPI_notification_cause_t *notification_cause);
OpenAPI_notification_cause_t *OpenAPI_notification_cause_copy(OpenAPI_notification_cause_t *dst, OpenAPI_notification_cause_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_notification_cause_H_ */

