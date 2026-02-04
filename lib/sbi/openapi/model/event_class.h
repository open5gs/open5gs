/*
 * event_class.h
 *
 * Specifies event classes.
 */

#ifndef _OpenAPI_event_class_H_
#define _OpenAPI_event_class_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "event_class_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_event_class_s OpenAPI_event_class_t;
typedef struct OpenAPI_event_class_s {
} OpenAPI_event_class_t;

OpenAPI_event_class_t *OpenAPI_event_class_create(
);
void OpenAPI_event_class_free(OpenAPI_event_class_t *event_class);
OpenAPI_event_class_t *OpenAPI_event_class_parseFromJSON(cJSON *event_classJSON);
cJSON *OpenAPI_event_class_convertToJSON(OpenAPI_event_class_t *event_class);
OpenAPI_event_class_t *OpenAPI_event_class_copy(OpenAPI_event_class_t *dst, OpenAPI_event_class_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_event_class_H_ */

