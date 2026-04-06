/*
 * capability_report.h
 *
 * Contains capability support information.
 */

#ifndef _OpenAPI_capability_report_H_
#define _OpenAPI_capability_report_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_capability_report_s OpenAPI_capability_report_t;
#include "notif_cap.h"
#include "notif_cap_type.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_capability_report_s {
    OpenAPI_notif_cap_e cap_report;
    OpenAPI_notif_cap_type_e cap_type;
};

OpenAPI_capability_report_t *OpenAPI_capability_report_create(
    OpenAPI_notif_cap_e cap_report,
    OpenAPI_notif_cap_type_e cap_type
);
void OpenAPI_capability_report_free(OpenAPI_capability_report_t *capability_report);
OpenAPI_capability_report_t *OpenAPI_capability_report_parseFromJSON(cJSON *capability_reportJSON);
cJSON *OpenAPI_capability_report_convertToJSON(OpenAPI_capability_report_t *capability_report);
OpenAPI_capability_report_t *OpenAPI_capability_report_copy(OpenAPI_capability_report_t *dst, OpenAPI_capability_report_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_capability_report_H_ */

