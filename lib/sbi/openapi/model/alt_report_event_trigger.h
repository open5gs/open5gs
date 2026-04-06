/*
 * alt_report_event_trigger.h
 *
 * the event-based altitude reporting requirement reason.
 */

#ifndef _OpenAPI_alt_report_event_trigger_H_
#define _OpenAPI_alt_report_event_trigger_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_alt_report_event_trigger_NULL = 0, OpenAPI_alt_report_event_trigger_TA_CHANGE, OpenAPI_alt_report_event_trigger_RAN_NODE } OpenAPI_alt_report_event_trigger_e;

char* OpenAPI_alt_report_event_trigger_ToString(OpenAPI_alt_report_event_trigger_e alt_report_event_trigger);

OpenAPI_alt_report_event_trigger_e OpenAPI_alt_report_event_trigger_FromString(char* alt_report_event_trigger);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_alt_report_event_trigger_H_ */

