/*
 * notification_method_1.h
 *
 * Possible values are - PERIODIC - ONE_TIME - ON_EVENT_DETECTION 
 */

#ifndef _OpenAPI_notification_method_1_H_
#define _OpenAPI_notification_method_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "notification_method_1_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_notification_method_1_s OpenAPI_notification_method_1_t;
typedef struct OpenAPI_notification_method_1_s {
} OpenAPI_notification_method_1_t;

OpenAPI_notification_method_1_t *OpenAPI_notification_method_1_create(
);
void OpenAPI_notification_method_1_free(OpenAPI_notification_method_1_t *notification_method_1);
OpenAPI_notification_method_1_t *OpenAPI_notification_method_1_parseFromJSON(cJSON *notification_method_1JSON);
cJSON *OpenAPI_notification_method_1_convertToJSON(OpenAPI_notification_method_1_t *notification_method_1);
OpenAPI_notification_method_1_t *OpenAPI_notification_method_1_copy(OpenAPI_notification_method_1_t *dst, OpenAPI_notification_method_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_notification_method_1_H_ */

