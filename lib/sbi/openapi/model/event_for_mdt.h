/*
 * event_for_mdt.h
 *
 * 
 */

#ifndef _OpenAPI_event_for_mdt_H_
#define _OpenAPI_event_for_mdt_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_event_for_mdt_NULL = 0, OpenAPI_event_for_mdt_OUT_OF_COVERAG, OpenAPI_event_for_mdt_A2_EVENT } OpenAPI_event_for_mdt_e;

char* OpenAPI_event_for_mdt_ToString(OpenAPI_event_for_mdt_e event_for_mdt);

OpenAPI_event_for_mdt_e OpenAPI_event_for_mdt_FromString(char* event_for_mdt);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_event_for_mdt_H_ */

