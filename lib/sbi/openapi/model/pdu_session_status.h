/*
 * pdu_session_status.h
 *
 * Indicates whether the PDU session is established or terminated.
 */

#ifndef _OpenAPI_pdu_session_status_H_
#define _OpenAPI_pdu_session_status_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_pdu_session_status_NULL = 0, OpenAPI_pdu_session_status_ESTABLISHED, OpenAPI_pdu_session_status_TERMINATED } OpenAPI_pdu_session_status_e;

char* OpenAPI_pdu_session_status_ToString(OpenAPI_pdu_session_status_e pdu_session_status);

OpenAPI_pdu_session_status_e OpenAPI_pdu_session_status_FromString(char* pdu_session_status);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pdu_session_status_H_ */

