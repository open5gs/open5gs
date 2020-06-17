/*
 * af_event.h
 *
 *
 */

#ifndef _OpenAPI_af_event_H_
#define _OpenAPI_af_event_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_af_event_s OpenAPI_af_event_t;
typedef struct OpenAPI_af_event_s {
} OpenAPI_af_event_t;

OpenAPI_af_event_t *OpenAPI_af_event_create(
    );
void OpenAPI_af_event_free(OpenAPI_af_event_t *af_event);
OpenAPI_af_event_t *OpenAPI_af_event_parseFromJSON(cJSON *af_eventJSON);
cJSON *OpenAPI_af_event_convertToJSON(OpenAPI_af_event_t *af_event);
OpenAPI_af_event_t *OpenAPI_af_event_copy(OpenAPI_af_event_t *dst, OpenAPI_af_event_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_af_event_H_ */

