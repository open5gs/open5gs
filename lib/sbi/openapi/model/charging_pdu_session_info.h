/*
 * charging_pdu_session_info.h
 *
 * Represents combination of PDU Session parameter(s) information.
 */

#ifndef _OpenAPI_charging_pdu_session_info_H_
#define _OpenAPI_charging_pdu_session_info_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_charging_pdu_session_info_s OpenAPI_charging_pdu_session_info_t;
#include "access_type.h"
#include "pdu_session_type.h"
#include "ssc_mode.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_charging_pdu_session_info_s {
    OpenAPI_pdu_session_type_e pdu_sess_type;
    OpenAPI_ssc_mode_e ssc_mode;
    OpenAPI_list_t *access_types;
};

OpenAPI_charging_pdu_session_info_t *OpenAPI_charging_pdu_session_info_create(
    OpenAPI_pdu_session_type_e pdu_sess_type,
    OpenAPI_ssc_mode_e ssc_mode,
    OpenAPI_list_t *access_types
);
void OpenAPI_charging_pdu_session_info_free(OpenAPI_charging_pdu_session_info_t *charging_pdu_session_info);
OpenAPI_charging_pdu_session_info_t *OpenAPI_charging_pdu_session_info_parseFromJSON(cJSON *charging_pdu_session_infoJSON);
cJSON *OpenAPI_charging_pdu_session_info_convertToJSON(OpenAPI_charging_pdu_session_info_t *charging_pdu_session_info);
OpenAPI_charging_pdu_session_info_t *OpenAPI_charging_pdu_session_info_copy(OpenAPI_charging_pdu_session_info_t *dst, OpenAPI_charging_pdu_session_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_charging_pdu_session_info_H_ */

