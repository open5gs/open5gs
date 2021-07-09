/*
 * pdu_session_types_1.h
 *
 * 
 */

#ifndef _OpenAPI_pdu_session_types_1_H_
#define _OpenAPI_pdu_session_types_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "pdu_session_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pdu_session_types_1_s OpenAPI_pdu_session_types_1_t;
typedef struct OpenAPI_pdu_session_types_1_s {
    OpenAPI_pdu_session_type_e default_session_type;
    OpenAPI_list_t *allowed_session_types;
} OpenAPI_pdu_session_types_1_t;

OpenAPI_pdu_session_types_1_t *OpenAPI_pdu_session_types_1_create(
    OpenAPI_pdu_session_type_e default_session_type,
    OpenAPI_list_t *allowed_session_types
);
void OpenAPI_pdu_session_types_1_free(OpenAPI_pdu_session_types_1_t *pdu_session_types_1);
OpenAPI_pdu_session_types_1_t *OpenAPI_pdu_session_types_1_parseFromJSON(cJSON *pdu_session_types_1JSON);
cJSON *OpenAPI_pdu_session_types_1_convertToJSON(OpenAPI_pdu_session_types_1_t *pdu_session_types_1);
OpenAPI_pdu_session_types_1_t *OpenAPI_pdu_session_types_1_copy(OpenAPI_pdu_session_types_1_t *dst, OpenAPI_pdu_session_types_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pdu_session_types_1_H_ */

