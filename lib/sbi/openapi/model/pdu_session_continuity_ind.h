/*
 * pdu_session_continuity_ind.h
 *
 *
 */

#ifndef _OpenAPI_pdu_session_continuity_ind_H_
#define _OpenAPI_pdu_session_continuity_ind_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_pdu_session_continuity_ind_NULL = 0, OpenAPI_pdu_session_continuity_ind_MAINTAIN_PDUSESSION, OpenAPI_pdu_session_continuity_ind_RECONNECT_PDUSESSION, OpenAPI_pdu_session_continuity_ind_RELEASE_PDUSESSION } OpenAPI_pdu_session_continuity_ind_e;

char* OpenAPI_pdu_session_continuity_ind_ToString(OpenAPI_pdu_session_continuity_ind_e pdu_session_continuity_ind);

OpenAPI_pdu_session_continuity_ind_e OpenAPI_pdu_session_continuity_ind_FromString(char* pdu_session_continuity_ind);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pdu_session_continuity_ind_H_ */

