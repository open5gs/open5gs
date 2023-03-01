/*
 * app_detection_report.h
 *
 * Indicates the start or stop of the detected application traffic and the application identifier of the detected application traffic.
 */

#ifndef _OpenAPI_app_detection_report_H_
#define _OpenAPI_app_detection_report_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "app_detection_notif_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_app_detection_report_s OpenAPI_app_detection_report_t;
typedef struct OpenAPI_app_detection_report_s {
    OpenAPI_app_detection_notif_type_e ad_notif_type;
    char *af_app_id;
} OpenAPI_app_detection_report_t;

OpenAPI_app_detection_report_t *OpenAPI_app_detection_report_create(
    OpenAPI_app_detection_notif_type_e ad_notif_type,
    char *af_app_id
);
void OpenAPI_app_detection_report_free(OpenAPI_app_detection_report_t *app_detection_report);
OpenAPI_app_detection_report_t *OpenAPI_app_detection_report_parseFromJSON(cJSON *app_detection_reportJSON);
cJSON *OpenAPI_app_detection_report_convertToJSON(OpenAPI_app_detection_report_t *app_detection_report);
OpenAPI_app_detection_report_t *OpenAPI_app_detection_report_copy(OpenAPI_app_detection_report_t *dst, OpenAPI_app_detection_report_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_app_detection_report_H_ */

