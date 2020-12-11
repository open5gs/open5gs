/*
 * pdu_session_rel_cause.h
 *
 *
 */

#ifndef _OpenAPI_pdu_session_rel_cause_H_
#define _OpenAPI_pdu_session_rel_cause_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pdu_session_rel_cause_s OpenAPI_pdu_session_rel_cause_t;
typedef struct OpenAPI_pdu_session_rel_cause_s {
} OpenAPI_pdu_session_rel_cause_t;

OpenAPI_pdu_session_rel_cause_t *OpenAPI_pdu_session_rel_cause_create(
    );
void OpenAPI_pdu_session_rel_cause_free(OpenAPI_pdu_session_rel_cause_t *pdu_session_rel_cause);
OpenAPI_pdu_session_rel_cause_t *OpenAPI_pdu_session_rel_cause_parseFromJSON(cJSON *pdu_session_rel_causeJSON);
cJSON *OpenAPI_pdu_session_rel_cause_convertToJSON(OpenAPI_pdu_session_rel_cause_t *pdu_session_rel_cause);
OpenAPI_pdu_session_rel_cause_t *OpenAPI_pdu_session_rel_cause_copy(OpenAPI_pdu_session_rel_cause_t *dst, OpenAPI_pdu_session_rel_cause_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pdu_session_rel_cause_H_ */

