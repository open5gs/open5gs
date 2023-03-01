/*
 * npcf_pdu_session_status.h
 *
 * 
 */

#ifndef _OpenAPI_npcf_pdu_session_status_H_
#define _OpenAPI_npcf_pdu_session_status_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_npcf_pdu_session_status_NULL = 0, OpenAPI_npcf_pdu_session_status_ESTABLISHED, OpenAPI_npcf_pdu_session_status_TERMINATED } OpenAPI_npcf_pdu_session_status_e;

char* OpenAPI_npcf_pdu_session_status_ToString(OpenAPI_npcf_pdu_session_status_e npcf_pdu_session_status);

OpenAPI_npcf_pdu_session_status_e OpenAPI_npcf_pdu_session_status_FromString(char* npcf_pdu_session_status);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_npcf_pdu_session_status_H_ */

