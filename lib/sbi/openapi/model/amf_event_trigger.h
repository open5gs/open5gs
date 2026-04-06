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

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_amf_event_trigger_NULL = 0, OpenAPI_amf_event_trigger_ONE_TIME, OpenAPI_amf_event_trigger_CONTINUOUS, OpenAPI_amf_event_trigger_PERIODIC } OpenAPI_amf_event_trigger_e;

char* OpenAPI_amf_event_trigger_ToString(OpenAPI_amf_event_trigger_e amf_event_trigger);

OpenAPI_amf_event_trigger_e OpenAPI_amf_event_trigger_FromString(char* amf_event_trigger);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_amf_event_trigger_H_ */

