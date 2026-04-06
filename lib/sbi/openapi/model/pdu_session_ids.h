/*
 * pdu_session_ids.h
 *
 * This data type contains a list of PDU Session IDs that identify suspected stale SMF registrations in UDM/UDR. 
 */

#ifndef _OpenAPI_pdu_session_ids_H_
#define _OpenAPI_pdu_session_ids_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_pdu_session_ids_s OpenAPI_pdu_session_ids_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_pdu_session_ids_s {
    OpenAPI_list_t *pdu_session_id_list;
};

OpenAPI_pdu_session_ids_t *OpenAPI_pdu_session_ids_create(
    OpenAPI_list_t *pdu_session_id_list
);
void OpenAPI_pdu_session_ids_free(OpenAPI_pdu_session_ids_t *pdu_session_ids);
OpenAPI_pdu_session_ids_t *OpenAPI_pdu_session_ids_parseFromJSON(cJSON *pdu_session_idsJSON);
cJSON *OpenAPI_pdu_session_ids_convertToJSON(OpenAPI_pdu_session_ids_t *pdu_session_ids);
OpenAPI_pdu_session_ids_t *OpenAPI_pdu_session_ids_copy(OpenAPI_pdu_session_ids_t *dst, OpenAPI_pdu_session_ids_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pdu_session_ids_H_ */

