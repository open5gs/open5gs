/*
 * event_reporting_status.h
 *
 * Indicates the status of event reporting.
 */

#ifndef _OpenAPI_event_reporting_status_H_
#define _OpenAPI_event_reporting_status_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_event_reporting_status_s OpenAPI_event_reporting_status_t;
typedef struct OpenAPI_event_reporting_status_s {
    bool is_event_report_counter;
    int event_report_counter;
    bool is_event_report_duration;
    int event_report_duration;
} OpenAPI_event_reporting_status_t;

OpenAPI_event_reporting_status_t *OpenAPI_event_reporting_status_create(
    bool is_event_report_counter,
    int event_report_counter,
    bool is_event_report_duration,
    int event_report_duration
);
void OpenAPI_event_reporting_status_free(OpenAPI_event_reporting_status_t *event_reporting_status);
OpenAPI_event_reporting_status_t *OpenAPI_event_reporting_status_parseFromJSON(cJSON *event_reporting_statusJSON);
cJSON *OpenAPI_event_reporting_status_convertToJSON(OpenAPI_event_reporting_status_t *event_reporting_status);
OpenAPI_event_reporting_status_t *OpenAPI_event_reporting_status_copy(OpenAPI_event_reporting_status_t *dst, OpenAPI_event_reporting_status_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_event_reporting_status_H_ */

