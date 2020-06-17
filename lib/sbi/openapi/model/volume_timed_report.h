/*
 * volume_timed_report.h
 *
 *
 */

#ifndef _OpenAPI_volume_timed_report_H_
#define _OpenAPI_volume_timed_report_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_volume_timed_report_s OpenAPI_volume_timed_report_t;
typedef struct OpenAPI_volume_timed_report_s {
    char *start_time_stamp;
    char *end_time_stamp;
    long downlink_volume;
    long uplink_volume;
} OpenAPI_volume_timed_report_t;

OpenAPI_volume_timed_report_t *OpenAPI_volume_timed_report_create(
    char *start_time_stamp,
    char *end_time_stamp,
    long downlink_volume,
    long uplink_volume
    );
void OpenAPI_volume_timed_report_free(OpenAPI_volume_timed_report_t *volume_timed_report);
OpenAPI_volume_timed_report_t *OpenAPI_volume_timed_report_parseFromJSON(cJSON *volume_timed_reportJSON);
cJSON *OpenAPI_volume_timed_report_convertToJSON(OpenAPI_volume_timed_report_t *volume_timed_report);
OpenAPI_volume_timed_report_t *OpenAPI_volume_timed_report_copy(OpenAPI_volume_timed_report_t *dst, OpenAPI_volume_timed_report_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_volume_timed_report_H_ */

