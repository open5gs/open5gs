/*
 * partial_success_report.h
 *
 * 
 */

#ifndef _OpenAPI_partial_success_report_H_
#define _OpenAPI_partial_success_report_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "failure_cause.h"
#include "policy_decision_failure_code.h"
#include "rule_report.h"
#include "session_rule_report.h"
#include "ue_camping_rep.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_partial_success_report_s OpenAPI_partial_success_report_t;
typedef struct OpenAPI_partial_success_report_s {
    OpenAPI_failure_cause_e failure_cause;
    OpenAPI_list_t *rule_reports;
    OpenAPI_list_t *sess_rule_reports;
    struct OpenAPI_ue_camping_rep_s *ue_camping_rep;
    OpenAPI_list_t *policy_dec_failure_reports;
} OpenAPI_partial_success_report_t;

OpenAPI_partial_success_report_t *OpenAPI_partial_success_report_create(
    OpenAPI_failure_cause_e failure_cause,
    OpenAPI_list_t *rule_reports,
    OpenAPI_list_t *sess_rule_reports,
    OpenAPI_ue_camping_rep_t *ue_camping_rep,
    OpenAPI_list_t *policy_dec_failure_reports
);
void OpenAPI_partial_success_report_free(OpenAPI_partial_success_report_t *partial_success_report);
OpenAPI_partial_success_report_t *OpenAPI_partial_success_report_parseFromJSON(cJSON *partial_success_reportJSON);
cJSON *OpenAPI_partial_success_report_convertToJSON(OpenAPI_partial_success_report_t *partial_success_report);
OpenAPI_partial_success_report_t *OpenAPI_partial_success_report_copy(OpenAPI_partial_success_report_t *dst, OpenAPI_partial_success_report_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_partial_success_report_H_ */

