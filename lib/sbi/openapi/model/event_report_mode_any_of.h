/*
 * event_report_mode_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_event_report_mode_any_of_H_
#define _OpenAPI_event_report_mode_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_event_report_mode_any_of_NULL = 0, OpenAPI_event_report_mode_any_of_PERIODIC, OpenAPI_event_report_mode_any_of_ON_EVENT_DETECTION } OpenAPI_event_report_mode_any_of_e;

char* OpenAPI_event_report_mode_any_of_ToString(OpenAPI_event_report_mode_any_of_e event_report_mode_any_of);

OpenAPI_event_report_mode_any_of_e OpenAPI_event_report_mode_any_of_FromString(char* event_report_mode_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_event_report_mode_any_of_H_ */

