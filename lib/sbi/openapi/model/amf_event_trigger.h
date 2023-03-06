/*
 * amf_event_trigger.h
 *
 * Describes how AMF should generate the report for the event
 */

#ifndef _OpenAPI_amf_event_trigger_H_
#define _OpenAPI_amf_event_trigger_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "amf_event_trigger_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_amf_event_trigger_s OpenAPI_amf_event_trigger_t;
typedef struct OpenAPI_amf_event_trigger_s {
} OpenAPI_amf_event_trigger_t;

OpenAPI_amf_event_trigger_t *OpenAPI_amf_event_trigger_create(
);
void OpenAPI_amf_event_trigger_free(OpenAPI_amf_event_trigger_t *amf_event_trigger);
OpenAPI_amf_event_trigger_t *OpenAPI_amf_event_trigger_parseFromJSON(cJSON *amf_event_triggerJSON);
cJSON *OpenAPI_amf_event_trigger_convertToJSON(OpenAPI_amf_event_trigger_t *amf_event_trigger);
OpenAPI_amf_event_trigger_t *OpenAPI_amf_event_trigger_copy(OpenAPI_amf_event_trigger_t *dst, OpenAPI_amf_event_trigger_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_amf_event_trigger_H_ */

