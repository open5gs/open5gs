/*
 * reported_event_type.h
 *
 * Specifies type of event reporting.
 */

#ifndef _OpenAPI_reported_event_type_H_
#define _OpenAPI_reported_event_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "reported_event_type_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_reported_event_type_s OpenAPI_reported_event_type_t;
typedef struct OpenAPI_reported_event_type_s {
} OpenAPI_reported_event_type_t;

OpenAPI_reported_event_type_t *OpenAPI_reported_event_type_create(
);
void OpenAPI_reported_event_type_free(OpenAPI_reported_event_type_t *reported_event_type);
OpenAPI_reported_event_type_t *OpenAPI_reported_event_type_parseFromJSON(cJSON *reported_event_typeJSON);
cJSON *OpenAPI_reported_event_type_convertToJSON(OpenAPI_reported_event_type_t *reported_event_type);
OpenAPI_reported_event_type_t *OpenAPI_reported_event_type_copy(OpenAPI_reported_event_type_t *dst, OpenAPI_reported_event_type_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_reported_event_type_H_ */

