/*
 * pdu_session_type.h
 *
 *
 */

#ifndef _OpenAPI_pdu_session_type_H_
#define _OpenAPI_pdu_session_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pdu_session_type_s OpenAPI_pdu_session_type_t;
typedef struct OpenAPI_pdu_session_type_s {
} OpenAPI_pdu_session_type_t;

OpenAPI_pdu_session_type_t *OpenAPI_pdu_session_type_create(
    );
void OpenAPI_pdu_session_type_free(OpenAPI_pdu_session_type_t *pdu_session_type);
OpenAPI_pdu_session_type_t *OpenAPI_pdu_session_type_parseFromJSON(cJSON *pdu_session_typeJSON);
cJSON *OpenAPI_pdu_session_type_convertToJSON(OpenAPI_pdu_session_type_t *pdu_session_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pdu_session_type_H_ */

