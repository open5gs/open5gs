/*
 * pdu_session_info.h
 *
 * indicates the DNN and S-NSSAI combination of a PDU session.
 */

#ifndef _OpenAPI_pdu_session_info_H_
#define _OpenAPI_pdu_session_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pdu_session_info_s OpenAPI_pdu_session_info_t;
typedef struct OpenAPI_pdu_session_info_s {
    struct OpenAPI_snssai_s *snssai;
    char *dnn;
} OpenAPI_pdu_session_info_t;

OpenAPI_pdu_session_info_t *OpenAPI_pdu_session_info_create(
    OpenAPI_snssai_t *snssai,
    char *dnn
);
void OpenAPI_pdu_session_info_free(OpenAPI_pdu_session_info_t *pdu_session_info);
OpenAPI_pdu_session_info_t *OpenAPI_pdu_session_info_parseFromJSON(cJSON *pdu_session_infoJSON);
cJSON *OpenAPI_pdu_session_info_convertToJSON(OpenAPI_pdu_session_info_t *pdu_session_info);
OpenAPI_pdu_session_info_t *OpenAPI_pdu_session_info_copy(OpenAPI_pdu_session_info_t *dst, OpenAPI_pdu_session_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pdu_session_info_H_ */

