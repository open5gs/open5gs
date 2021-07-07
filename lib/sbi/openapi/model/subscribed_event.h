/*
 * subscribed_event.h
 *
 * Possible values are - UP_PATH_CHANGE: The AF requests to be notified when the UP path changes for the PDU session. 
 */

#ifndef _OpenAPI_subscribed_event_H_
#define _OpenAPI_subscribed_event_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "subscribed_event_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_subscribed_event_s OpenAPI_subscribed_event_t;
typedef struct OpenAPI_subscribed_event_s {
} OpenAPI_subscribed_event_t;

OpenAPI_subscribed_event_t *OpenAPI_subscribed_event_create(
);
void OpenAPI_subscribed_event_free(OpenAPI_subscribed_event_t *subscribed_event);
OpenAPI_subscribed_event_t *OpenAPI_subscribed_event_parseFromJSON(cJSON *subscribed_eventJSON);
cJSON *OpenAPI_subscribed_event_convertToJSON(OpenAPI_subscribed_event_t *subscribed_event);
OpenAPI_subscribed_event_t *OpenAPI_subscribed_event_copy(OpenAPI_subscribed_event_t *dst, OpenAPI_subscribed_event_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_subscribed_event_H_ */

