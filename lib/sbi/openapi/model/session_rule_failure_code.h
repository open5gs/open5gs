/*
 * session_rule_failure_code.h
 *
 * Possible values are   - NF_MAL: Indicate that the PCC rule could not be successfully installed (for those provisioned from the PCF) or activated (for those pre-defined in SMF) or enforced (for those already successfully installed) due to SMF/UPF malfunction.   - RES_LIM: Indicate that the PCC rule could not be successfully installed (for those provisioned from PCF) or activated (for those pre-defined in SMF) or enforced (for those already successfully installed) due to a limitation of resources at the SMF/UPF.   - UNSUCC_QOS_VAL: indicate that the QoS validation has failed.   - UE_STA_SUSP: Indicates that the UE is in suspend state.
 */

#ifndef _OpenAPI_session_rule_failure_code_H_
#define _OpenAPI_session_rule_failure_code_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_session_rule_failure_code_s OpenAPI_session_rule_failure_code_t;
typedef struct OpenAPI_session_rule_failure_code_s {
} OpenAPI_session_rule_failure_code_t;

OpenAPI_session_rule_failure_code_t *OpenAPI_session_rule_failure_code_create(
    );
void OpenAPI_session_rule_failure_code_free(OpenAPI_session_rule_failure_code_t *session_rule_failure_code);
OpenAPI_session_rule_failure_code_t *OpenAPI_session_rule_failure_code_parseFromJSON(cJSON *session_rule_failure_codeJSON);
cJSON *OpenAPI_session_rule_failure_code_convertToJSON(OpenAPI_session_rule_failure_code_t *session_rule_failure_code);
OpenAPI_session_rule_failure_code_t *OpenAPI_session_rule_failure_code_copy(OpenAPI_session_rule_failure_code_t *dst, OpenAPI_session_rule_failure_code_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_session_rule_failure_code_H_ */

