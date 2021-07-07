/*
 * amf_event_trigger_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_amf_event_trigger_any_of_H_
#define _OpenAPI_amf_event_trigger_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_amf_event_trigger_any_of_NULL = 0, OpenAPI_amf_event_trigger_any_of_ONE_TIME, OpenAPI_amf_event_trigger_any_of_CONTINUOUS, OpenAPI_amf_event_trigger_any_of_PERIODIC } OpenAPI_amf_event_trigger_any_of_e;

char* OpenAPI_amf_event_trigger_any_of_ToString(OpenAPI_amf_event_trigger_any_of_e amf_event_trigger_any_of);

OpenAPI_amf_event_trigger_any_of_e OpenAPI_amf_event_trigger_any_of_FromString(char* amf_event_trigger_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_amf_event_trigger_any_of_H_ */

