/*
 * error_report.h
 *
 *
 */

#ifndef _OpenAPI_error_report_H_
#define _OpenAPI_error_report_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "problem_details.h"
#include "rule_report.h"
#include "session_rule_report.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_error_report_s OpenAPI_error_report_t;
typedef struct OpenAPI_error_report_s {
    struct OpenAPI_problem_details_s *error;
    OpenAPI_list_t *rule_reports;
    OpenAPI_list_t *sess_rule_reports;
} OpenAPI_error_report_t;

OpenAPI_error_report_t *OpenAPI_error_report_create(
    OpenAPI_problem_details_t *error,
    OpenAPI_list_t *rule_reports,
    OpenAPI_list_t *sess_rule_reports
    );
void OpenAPI_error_report_free(OpenAPI_error_report_t *error_report);
OpenAPI_error_report_t *OpenAPI_error_report_parseFromJSON(cJSON *error_reportJSON);
cJSON *OpenAPI_error_report_convertToJSON(OpenAPI_error_report_t *error_report);
OpenAPI_error_report_t *OpenAPI_error_report_copy(OpenAPI_error_report_t *dst, OpenAPI_error_report_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_error_report_H_ */

