/*
 * notification_control.h
 *
 *
 */

#ifndef _OpenAPI_notification_control_H_
#define _OpenAPI_notification_control_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_notification_control_s OpenAPI_notification_control_t;
typedef struct OpenAPI_notification_control_s {
} OpenAPI_notification_control_t;

OpenAPI_notification_control_t *OpenAPI_notification_control_create(
    );
void OpenAPI_notification_control_free(OpenAPI_notification_control_t *notification_control);
OpenAPI_notification_control_t *OpenAPI_notification_control_parseFromJSON(cJSON *notification_controlJSON);
cJSON *OpenAPI_notification_control_convertToJSON(OpenAPI_notification_control_t *notification_control);
OpenAPI_notification_control_t *OpenAPI_notification_control_copy(OpenAPI_notification_control_t *dst, OpenAPI_notification_control_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_notification_control_H_ */

