/*
 * pdu_session_types.h
 *
 *
 */

#ifndef _OpenAPI_pdu_session_types_H_
#define _OpenAPI_pdu_session_types_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "pdu_session_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pdu_session_types_s OpenAPI_pdu_session_types_t;
typedef struct OpenAPI_pdu_session_types_s {
    struct OpenAPI_pdu_session_type_s *default_session_type;
    OpenAPI_list_t *allowed_session_types;
} OpenAPI_pdu_session_types_t;

OpenAPI_pdu_session_types_t *OpenAPI_pdu_session_types_create(
    OpenAPI_pdu_session_type_t *default_session_type,
    OpenAPI_list_t *allowed_session_types
    );
void OpenAPI_pdu_session_types_free(OpenAPI_pdu_session_types_t *pdu_session_types);
OpenAPI_pdu_session_types_t *OpenAPI_pdu_session_types_parseFromJSON(cJSON *pdu_session_typesJSON);
cJSON *OpenAPI_pdu_session_types_convertToJSON(OpenAPI_pdu_session_types_t *pdu_session_types);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pdu_session_types_H_ */

