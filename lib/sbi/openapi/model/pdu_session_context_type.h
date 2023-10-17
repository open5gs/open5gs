/*
 * pdu_session_context_type.h
 *
 * 
 */

#ifndef _OpenAPI_pdu_session_context_type_H_
#define _OpenAPI_pdu_session_context_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_pdu_session_context_type_NULL = 0, OpenAPI_pdu_session_context_type_AF_COORDINATION_INFO } OpenAPI_pdu_session_context_type_e;

char* OpenAPI_pdu_session_context_type_ToString(OpenAPI_pdu_session_context_type_e pdu_session_context_type);

OpenAPI_pdu_session_context_type_e OpenAPI_pdu_session_context_type_FromString(char* pdu_session_context_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pdu_session_context_type_H_ */

