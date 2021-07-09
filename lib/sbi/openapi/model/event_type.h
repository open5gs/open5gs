/*
 * event_type.h
 *
 * 
 */

#ifndef _OpenAPI_event_type_H_
#define _OpenAPI_event_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "event_type_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_event_type_s OpenAPI_event_type_t;
typedef struct OpenAPI_event_type_s {
} OpenAPI_event_type_t;

OpenAPI_event_type_t *OpenAPI_event_type_create(
);
void OpenAPI_event_type_free(OpenAPI_event_type_t *event_type);
OpenAPI_event_type_t *OpenAPI_event_type_parseFromJSON(cJSON *event_typeJSON);
cJSON *OpenAPI_event_type_convertToJSON(OpenAPI_event_type_t *event_type);
OpenAPI_event_type_t *OpenAPI_event_type_copy(OpenAPI_event_type_t *dst, OpenAPI_event_type_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_event_type_H_ */

