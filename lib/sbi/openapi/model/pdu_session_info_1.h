/*
 * pdu_session_info_1.h
 *
 * Represents combination of PDU Session parameter(s) information.
 */

#ifndef _OpenAPI_pdu_session_info_1_H_
#define _OpenAPI_pdu_session_info_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_pdu_session_info_1_s OpenAPI_pdu_session_info_1_t;
#include "access_type.h"
#include "pdu_session_type.h"
#include "ssc_mode.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_pdu_session_info_1_s {
    OpenAPI_pdu_session_type_e pdu_sess_type;
    OpenAPI_ssc_mode_e ssc_mode;
    OpenAPI_list_t *access_types;
};

OpenAPI_pdu_session_info_1_t *OpenAPI_pdu_session_info_1_create(
    OpenAPI_pdu_session_type_e pdu_sess_type,
    OpenAPI_ssc_mode_e ssc_mode,
    OpenAPI_list_t *access_types
);
void OpenAPI_pdu_session_info_1_free(OpenAPI_pdu_session_info_1_t *pdu_session_info_1);
OpenAPI_pdu_session_info_1_t *OpenAPI_pdu_session_info_1_parseFromJSON(cJSON *pdu_session_info_1JSON);
cJSON *OpenAPI_pdu_session_info_1_convertToJSON(OpenAPI_pdu_session_info_1_t *pdu_session_info_1);
OpenAPI_pdu_session_info_1_t *OpenAPI_pdu_session_info_1_copy(OpenAPI_pdu_session_info_1_t *dst, OpenAPI_pdu_session_info_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pdu_session_info_1_H_ */

