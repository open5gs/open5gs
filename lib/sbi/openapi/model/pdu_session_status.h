/*
 * pdu_session_status.h
 *
 * Possible values are - \&quot;ACTIVE\&quot; - \&quot;RELEASED\&quot; 
 */

#ifndef _OpenAPI_pdu_session_status_H_
#define _OpenAPI_pdu_session_status_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "pdu_session_status_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pdu_session_status_s OpenAPI_pdu_session_status_t;
typedef struct OpenAPI_pdu_session_status_s {
} OpenAPI_pdu_session_status_t;

OpenAPI_pdu_session_status_t *OpenAPI_pdu_session_status_create(
);
void OpenAPI_pdu_session_status_free(OpenAPI_pdu_session_status_t *pdu_session_status);
OpenAPI_pdu_session_status_t *OpenAPI_pdu_session_status_parseFromJSON(cJSON *pdu_session_statusJSON);
cJSON *OpenAPI_pdu_session_status_convertToJSON(OpenAPI_pdu_session_status_t *pdu_session_status);
OpenAPI_pdu_session_status_t *OpenAPI_pdu_session_status_copy(OpenAPI_pdu_session_status_t *dst, OpenAPI_pdu_session_status_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pdu_session_status_H_ */

