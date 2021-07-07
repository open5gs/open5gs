/*
 * redundant_pdu_session_information.h
 *
 * Redundant PDU Session Information
 */

#ifndef _OpenAPI_redundant_pdu_session_information_H_
#define _OpenAPI_redundant_pdu_session_information_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "rsn.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_redundant_pdu_session_information_s OpenAPI_redundant_pdu_session_information_t;
typedef struct OpenAPI_redundant_pdu_session_information_s {
    OpenAPI_rsn_e rsn;
} OpenAPI_redundant_pdu_session_information_t;

OpenAPI_redundant_pdu_session_information_t *OpenAPI_redundant_pdu_session_information_create(
    OpenAPI_rsn_e rsn
);
void OpenAPI_redundant_pdu_session_information_free(OpenAPI_redundant_pdu_session_information_t *redundant_pdu_session_information);
OpenAPI_redundant_pdu_session_information_t *OpenAPI_redundant_pdu_session_information_parseFromJSON(cJSON *redundant_pdu_session_informationJSON);
cJSON *OpenAPI_redundant_pdu_session_information_convertToJSON(OpenAPI_redundant_pdu_session_information_t *redundant_pdu_session_information);
OpenAPI_redundant_pdu_session_information_t *OpenAPI_redundant_pdu_session_information_copy(OpenAPI_redundant_pdu_session_information_t *dst, OpenAPI_redundant_pdu_session_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_redundant_pdu_session_information_H_ */

