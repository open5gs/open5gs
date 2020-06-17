/*
 * event_report_mode.h
 *
 *
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

typedef struct OpenAPI_event_report_mode_s OpenAPI_event_report_mode_t;
typedef struct OpenAPI_event_report_mode_s {
} OpenAPI_event_report_mode_t;

OpenAPI_event_report_mode_t *OpenAPI_event_report_mode_create(
    );
void OpenAPI_event_report_mode_free(OpenAPI_event_report_mode_t *event_report_mode);
OpenAPI_event_report_mode_t *OpenAPI_event_report_mode_parseFromJSON(cJSON *event_report_modeJSON);
cJSON *OpenAPI_event_report_mode_convertToJSON(OpenAPI_event_report_mode_t *event_report_mode);
OpenAPI_event_report_mode_t *OpenAPI_event_report_mode_copy(OpenAPI_event_report_mode_t *dst, OpenAPI_event_report_mode_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_event_report_mode_H_ */

