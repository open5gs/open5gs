/*
 * session_rule.h
 *
 * 
 */

#ifndef _OpenAPI_session_rule_H_
#define _OpenAPI_session_rule_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ambr.h"
#include "authorized_default_qos.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_session_rule_s OpenAPI_session_rule_t;
typedef struct OpenAPI_session_rule_s {
    struct OpenAPI_ambr_s *auth_sess_ambr;
    struct OpenAPI_authorized_default_qos_s *auth_def_qos;
    char *sess_rule_id;
    char *ref_um_data;
    char *ref_um_n3g_data;
    char *ref_cond_data;
} OpenAPI_session_rule_t;

OpenAPI_session_rule_t *OpenAPI_session_rule_create(
    OpenAPI_ambr_t *auth_sess_ambr,
    OpenAPI_authorized_default_qos_t *auth_def_qos,
    char *sess_rule_id,
    char *ref_um_data,
    char *ref_um_n3g_data,
    char *ref_cond_data
);
void OpenAPI_session_rule_free(OpenAPI_session_rule_t *session_rule);
OpenAPI_session_rule_t *OpenAPI_session_rule_parseFromJSON(cJSON *session_ruleJSON);
cJSON *OpenAPI_session_rule_convertToJSON(OpenAPI_session_rule_t *session_rule);
OpenAPI_session_rule_t *OpenAPI_session_rule_copy(OpenAPI_session_rule_t *dst, OpenAPI_session_rule_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_session_rule_H_ */

