/*
 * sac_event_trigger.h
 *
 * Describes how NSACF should generate the report for the slice event
 */

#ifndef _OpenAPI_sac_event_trigger_H_
#define _OpenAPI_sac_event_trigger_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_sac_event_trigger_NULL = 0, OpenAPI_sac_event_trigger_THRESHOLD, OpenAPI_sac_event_trigger_PERIODIC } OpenAPI_sac_event_trigger_e;

char* OpenAPI_sac_event_trigger_ToString(OpenAPI_sac_event_trigger_e sac_event_trigger);

OpenAPI_sac_event_trigger_e OpenAPI_sac_event_trigger_FromString(char* sac_event_trigger);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sac_event_trigger_H_ */

