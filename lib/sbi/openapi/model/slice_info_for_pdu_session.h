/*
 * slice_info_for_pdu_session.h
 *
 * Contains the slice information requested during PDU Session establishment procedure
 */

#ifndef _OpenAPI_slice_info_for_pdu_session_H_
#define _OpenAPI_slice_info_for_pdu_session_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "roaming_indication.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_slice_info_for_pdu_session_s OpenAPI_slice_info_for_pdu_session_t;
typedef struct OpenAPI_slice_info_for_pdu_session_s {
    struct OpenAPI_snssai_s *s_nssai;
    OpenAPI_roaming_indication_e roaming_indication;
    struct OpenAPI_snssai_s *home_snssai;
} OpenAPI_slice_info_for_pdu_session_t;

OpenAPI_slice_info_for_pdu_session_t *OpenAPI_slice_info_for_pdu_session_create(
    OpenAPI_snssai_t *s_nssai,
    OpenAPI_roaming_indication_e roaming_indication,
    OpenAPI_snssai_t *home_snssai
);
void OpenAPI_slice_info_for_pdu_session_free(OpenAPI_slice_info_for_pdu_session_t *slice_info_for_pdu_session);
OpenAPI_slice_info_for_pdu_session_t *OpenAPI_slice_info_for_pdu_session_parseFromJSON(cJSON *slice_info_for_pdu_sessionJSON);
cJSON *OpenAPI_slice_info_for_pdu_session_convertToJSON(OpenAPI_slice_info_for_pdu_session_t *slice_info_for_pdu_session);
OpenAPI_slice_info_for_pdu_session_t *OpenAPI_slice_info_for_pdu_session_copy(OpenAPI_slice_info_for_pdu_session_t *dst, OpenAPI_slice_info_for_pdu_session_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_slice_info_for_pdu_session_H_ */

