/*
 * event_report_mode.h
 *
 * Event report mode.
 */

#ifndef _OpenAPI_event_report_mode_H_
#define _OpenAPI_event_report_mode_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_event_report_mode_NULL = 0, OpenAPI_event_report_mode_PERIODIC, OpenAPI_event_report_mode_ON_EVENT_DETECTION } OpenAPI_event_report_mode_e;

char* OpenAPI_event_report_mode_ToString(OpenAPI_event_report_mode_e event_report_mode);

OpenAPI_event_report_mode_e OpenAPI_event_report_mode_FromString(char* event_report_mode);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_event_report_mode_H_ */

