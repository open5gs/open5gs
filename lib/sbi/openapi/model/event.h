/*
 * event.h
 *
 * Possible values are: - SUCCESS_UE_POL_DEL_SP: Successful UE Policy Delivery related to    the invocation of AF provisioned Service Parameters. - UNSUCCESS_UE_POL_DEL_SP: Unsuccessful UE Policy Delivery related to the invocation of AF    provisioned Service Parameters. 
 */

#ifndef _OpenAPI_event_H_
#define _OpenAPI_event_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "event_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_event_s OpenAPI_event_t;
typedef struct OpenAPI_event_s {
} OpenAPI_event_t;

OpenAPI_event_t *OpenAPI_event_create(
);
void OpenAPI_event_free(OpenAPI_event_t *event);
OpenAPI_event_t *OpenAPI_event_parseFromJSON(cJSON *eventJSON);
cJSON *OpenAPI_event_convertToJSON(OpenAPI_event_t *event);
OpenAPI_event_t *OpenAPI_event_copy(OpenAPI_event_t *dst, OpenAPI_event_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_event_H_ */

