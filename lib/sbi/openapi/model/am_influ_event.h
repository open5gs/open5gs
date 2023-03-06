/*
 * am_influ_event.h
 *
 * Represents the service area coverage outcome event.
 */

#ifndef _OpenAPI_am_influ_event_H_
#define _OpenAPI_am_influ_event_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "am_influ_event_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_am_influ_event_s OpenAPI_am_influ_event_t;
typedef struct OpenAPI_am_influ_event_s {
} OpenAPI_am_influ_event_t;

OpenAPI_am_influ_event_t *OpenAPI_am_influ_event_create(
);
void OpenAPI_am_influ_event_free(OpenAPI_am_influ_event_t *am_influ_event);
OpenAPI_am_influ_event_t *OpenAPI_am_influ_event_parseFromJSON(cJSON *am_influ_eventJSON);
cJSON *OpenAPI_am_influ_event_convertToJSON(OpenAPI_am_influ_event_t *am_influ_event);
OpenAPI_am_influ_event_t *OpenAPI_am_influ_event_copy(OpenAPI_am_influ_event_t *dst, OpenAPI_am_influ_event_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_am_influ_event_H_ */

