/*
 * amf_event_type.h
 *
 * Describes the supported event types of Namf_EventExposure Service
 */

#ifndef _OpenAPI_amf_event_type_H_
#define _OpenAPI_amf_event_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "amf_event_type_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_amf_event_type_s OpenAPI_amf_event_type_t;
typedef struct OpenAPI_amf_event_type_s {
} OpenAPI_amf_event_type_t;

OpenAPI_amf_event_type_t *OpenAPI_amf_event_type_create(
);
void OpenAPI_amf_event_type_free(OpenAPI_amf_event_type_t *amf_event_type);
OpenAPI_amf_event_type_t *OpenAPI_amf_event_type_parseFromJSON(cJSON *amf_event_typeJSON);
cJSON *OpenAPI_amf_event_type_convertToJSON(OpenAPI_amf_event_type_t *amf_event_type);
OpenAPI_amf_event_type_t *OpenAPI_amf_event_type_copy(OpenAPI_amf_event_type_t *dst, OpenAPI_amf_event_type_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_amf_event_type_H_ */

