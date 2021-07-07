/*
 * pdu_session_status_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_pdu_session_status_any_of_H_
#define _OpenAPI_pdu_session_status_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_pdu_session_status_any_of_NULL = 0, OpenAPI_pdu_session_status_any_of_ACTIVE, OpenAPI_pdu_session_status_any_of_RELEASED } OpenAPI_pdu_session_status_any_of_e;

char* OpenAPI_pdu_session_status_any_of_ToString(OpenAPI_pdu_session_status_any_of_e pdu_session_status_any_of);

OpenAPI_pdu_session_status_any_of_e OpenAPI_pdu_session_status_any_of_FromString(char* pdu_session_status_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pdu_session_status_any_of_H_ */

