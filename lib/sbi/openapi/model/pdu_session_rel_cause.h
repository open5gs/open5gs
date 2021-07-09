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

typedef enum { OpenAPI_pdu_session_rel_cause_NULL = 0, OpenAPI_pdu_session_rel_cause_PS_TO_CS_HO } OpenAPI_pdu_session_rel_cause_e;

char* OpenAPI_pdu_session_rel_cause_ToString(OpenAPI_pdu_session_rel_cause_e pdu_session_rel_cause);

OpenAPI_pdu_session_rel_cause_e OpenAPI_pdu_session_rel_cause_FromString(char* pdu_session_rel_cause);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pdu_session_rel_cause_H_ */

