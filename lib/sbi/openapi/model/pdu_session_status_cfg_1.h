/*
 * pdu_session_status_cfg_1.h
 *
 * 
 */

#ifndef _OpenAPI_pdu_session_status_cfg_1_H_
#define _OpenAPI_pdu_session_status_cfg_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pdu_session_status_cfg_1_s OpenAPI_pdu_session_status_cfg_1_t;
typedef struct OpenAPI_pdu_session_status_cfg_1_s {
    char *dnn;
} OpenAPI_pdu_session_status_cfg_1_t;

OpenAPI_pdu_session_status_cfg_1_t *OpenAPI_pdu_session_status_cfg_1_create(
    char *dnn
);
void OpenAPI_pdu_session_status_cfg_1_free(OpenAPI_pdu_session_status_cfg_1_t *pdu_session_status_cfg_1);
OpenAPI_pdu_session_status_cfg_1_t *OpenAPI_pdu_session_status_cfg_1_parseFromJSON(cJSON *pdu_session_status_cfg_1JSON);
cJSON *OpenAPI_pdu_session_status_cfg_1_convertToJSON(OpenAPI_pdu_session_status_cfg_1_t *pdu_session_status_cfg_1);
OpenAPI_pdu_session_status_cfg_1_t *OpenAPI_pdu_session_status_cfg_1_copy(OpenAPI_pdu_session_status_cfg_1_t *dst, OpenAPI_pdu_session_status_cfg_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pdu_session_status_cfg_1_H_ */

