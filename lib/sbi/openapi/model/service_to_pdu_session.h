/*
 * service_to_pdu_session.h
 *
 *
 */

#ifndef _OpenAPI_service_to_pdu_session_H_
#define _OpenAPI_service_to_pdu_session_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "pdu_session_type.h"
#include "snssai.h"
#include "ssc_mode.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_service_to_pdu_session_s OpenAPI_service_to_pdu_session_t;
typedef struct OpenAPI_service_to_pdu_session_s {
    OpenAPI_list_t *ser_ids;
    OpenAPI_pdu_session_type_e pdu_session_type;
    OpenAPI_ssc_mode_e ssc_mode;
    OpenAPI_list_t *slice_info;
    OpenAPI_list_t *dnns;
} OpenAPI_service_to_pdu_session_t;

OpenAPI_service_to_pdu_session_t *OpenAPI_service_to_pdu_session_create(
    OpenAPI_list_t *ser_ids,
    OpenAPI_pdu_session_type_e pdu_session_type,
    OpenAPI_ssc_mode_e ssc_mode,
    OpenAPI_list_t *slice_info,
    OpenAPI_list_t *dnns
    );
void OpenAPI_service_to_pdu_session_free(OpenAPI_service_to_pdu_session_t *service_to_pdu_session);
OpenAPI_service_to_pdu_session_t *OpenAPI_service_to_pdu_session_parseFromJSON(cJSON *service_to_pdu_sessionJSON);
cJSON *OpenAPI_service_to_pdu_session_convertToJSON(OpenAPI_service_to_pdu_session_t *service_to_pdu_session);
OpenAPI_service_to_pdu_session_t *OpenAPI_service_to_pdu_session_copy(OpenAPI_service_to_pdu_session_t *dst, OpenAPI_service_to_pdu_session_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_service_to_pdu_session_H_ */

