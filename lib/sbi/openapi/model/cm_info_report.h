/*
 * cm_info_report.h
 *
 *
 */

#ifndef _OpenAPI_cm_info_report_H_
#define _OpenAPI_cm_info_report_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "cm_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_cm_info_report_s OpenAPI_cm_info_report_t;
typedef struct OpenAPI_cm_info_report_s {
    OpenAPI_list_t *old_cm_info_list;
    OpenAPI_list_t *new_cm_info_list;
} OpenAPI_cm_info_report_t;

OpenAPI_cm_info_report_t *OpenAPI_cm_info_report_create(
    OpenAPI_list_t *old_cm_info_list,
    OpenAPI_list_t *new_cm_info_list
    );
void OpenAPI_cm_info_report_free(OpenAPI_cm_info_report_t *cm_info_report);
OpenAPI_cm_info_report_t *OpenAPI_cm_info_report_parseFromJSON(cJSON *cm_info_reportJSON);
cJSON *OpenAPI_cm_info_report_convertToJSON(OpenAPI_cm_info_report_t *cm_info_report);
OpenAPI_cm_info_report_t *OpenAPI_cm_info_report_copy(OpenAPI_cm_info_report_t *dst, OpenAPI_cm_info_report_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_cm_info_report_H_ */

