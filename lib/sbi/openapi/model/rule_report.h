/*
 * rule_report.h
 *
 *
 */

#ifndef _OpenAPI_rule_report_H_
#define _OpenAPI_rule_report_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "failure_code.h"
#include "final_unit_action.h"
#include "ran_nas_rel_cause.h"
#include "rule_status.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_rule_report_s OpenAPI_rule_report_t;
typedef struct OpenAPI_rule_report_s {
    OpenAPI_list_t *pcc_rule_ids;
    struct OpenAPI_rule_status_s *rule_status;
    OpenAPI_list_t *cont_vers;
    struct OpenAPI_failure_code_s *failure_code;
    struct OpenAPI_final_unit_action_s *fin_unit_act;
    OpenAPI_list_t *ran_nas_rel_causes;
} OpenAPI_rule_report_t;

OpenAPI_rule_report_t *OpenAPI_rule_report_create(
    OpenAPI_list_t *pcc_rule_ids,
    OpenAPI_rule_status_t *rule_status,
    OpenAPI_list_t *cont_vers,
    OpenAPI_failure_code_t *failure_code,
    OpenAPI_final_unit_action_t *fin_unit_act,
    OpenAPI_list_t *ran_nas_rel_causes
    );
void OpenAPI_rule_report_free(OpenAPI_rule_report_t *rule_report);
OpenAPI_rule_report_t *OpenAPI_rule_report_parseFromJSON(cJSON *rule_reportJSON);
cJSON *OpenAPI_rule_report_convertToJSON(OpenAPI_rule_report_t *rule_report);
OpenAPI_rule_report_t *OpenAPI_rule_report_copy(OpenAPI_rule_report_t *dst, OpenAPI_rule_report_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_rule_report_H_ */

