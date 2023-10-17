/*
 * notification_method.h
 *
 * Possible values are: - PERIODIC: The subscribe of NWDAF Event is periodically. The periodic of the notification is identified by repetitionPeriod defined in clause 5.1.6.2.3.   - THRESHOLD: The subscribe of NWDAF Event is upon threshold exceeded. 
 */

#ifndef _OpenAPI_notification_method_H_
#define _OpenAPI_notification_method_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "notification_method_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_notification_method_s OpenAPI_notification_method_t;
typedef struct OpenAPI_notification_method_s {
} OpenAPI_notification_method_t;

OpenAPI_notification_method_t *OpenAPI_notification_method_create(
);
void OpenAPI_notification_method_free(OpenAPI_notification_method_t *notification_method);
OpenAPI_notification_method_t *OpenAPI_notification_method_parseFromJSON(cJSON *notification_methodJSON);
cJSON *OpenAPI_notification_method_convertToJSON(OpenAPI_notification_method_t *notification_method);
OpenAPI_notification_method_t *OpenAPI_notification_method_copy(OpenAPI_notification_method_t *dst, OpenAPI_notification_method_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_notification_method_H_ */

