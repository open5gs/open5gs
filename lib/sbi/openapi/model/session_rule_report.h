/*
 * session_rule_report.h
 *
 *
 */

#ifndef _OpenAPI_session_rule_report_H_
#define _OpenAPI_session_rule_report_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "rule_status.h"
#include "session_rule_failure_code.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_session_rule_report_s OpenAPI_session_rule_report_t;
typedef struct OpenAPI_session_rule_report_s {
    OpenAPI_list_t *rule_ids;
    struct OpenAPI_rule_status_s *rule_status;
    struct OpenAPI_session_rule_failure_code_s *sess_rule_failure_code;
} OpenAPI_session_rule_report_t;

OpenAPI_session_rule_report_t *OpenAPI_session_rule_report_create(
    OpenAPI_list_t *rule_ids,
    OpenAPI_rule_status_t *rule_status,
    OpenAPI_session_rule_failure_code_t *sess_rule_failure_code
    );
void OpenAPI_session_rule_report_free(OpenAPI_session_rule_report_t *session_rule_report);
OpenAPI_session_rule_report_t *OpenAPI_session_rule_report_parseFromJSON(cJSON *session_rule_reportJSON);
cJSON *OpenAPI_session_rule_report_convertToJSON(OpenAPI_session_rule_report_t *session_rule_report);
OpenAPI_session_rule_report_t *OpenAPI_session_rule_report_copy(OpenAPI_session_rule_report_t *dst, OpenAPI_session_rule_report_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_session_rule_report_H_ */

