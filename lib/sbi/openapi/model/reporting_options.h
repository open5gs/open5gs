/*
 * reporting_options.h
 *
 *
 */

#ifndef _OpenAPI_reporting_options_H_
#define _OpenAPI_reporting_options_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "event_report_mode.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_reporting_options_s OpenAPI_reporting_options_t;
typedef struct OpenAPI_reporting_options_s {
    struct OpenAPI_event_report_mode_s *report_mode;
    int max_num_of_reports;
    char *expiry;
    int sampling_ratio;
    int guard_time;
    int report_period;
} OpenAPI_reporting_options_t;

OpenAPI_reporting_options_t *OpenAPI_reporting_options_create(
    OpenAPI_event_report_mode_t *report_mode,
    int max_num_of_reports,
    char *expiry,
    int sampling_ratio,
    int guard_time,
    int report_period
    );
void OpenAPI_reporting_options_free(OpenAPI_reporting_options_t *reporting_options);
OpenAPI_reporting_options_t *OpenAPI_reporting_options_parseFromJSON(cJSON *reporting_optionsJSON);
cJSON *OpenAPI_reporting_options_convertToJSON(OpenAPI_reporting_options_t *reporting_options);
OpenAPI_reporting_options_t *OpenAPI_reporting_options_copy(OpenAPI_reporting_options_t *dst, OpenAPI_reporting_options_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_reporting_options_H_ */

