/*
 * pdu_session_status_cfg.h
 *
 * 
 */

#ifndef _OpenAPI_pdu_session_status_cfg_H_
#define _OpenAPI_pdu_session_status_cfg_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pdu_session_status_cfg_s OpenAPI_pdu_session_status_cfg_t;
typedef struct OpenAPI_pdu_session_status_cfg_s {
    char *dnn;
} OpenAPI_pdu_session_status_cfg_t;

OpenAPI_pdu_session_status_cfg_t *OpenAPI_pdu_session_status_cfg_create(
    char *dnn
);
void OpenAPI_pdu_session_status_cfg_free(OpenAPI_pdu_session_status_cfg_t *pdu_session_status_cfg);
OpenAPI_pdu_session_status_cfg_t *OpenAPI_pdu_session_status_cfg_parseFromJSON(cJSON *pdu_session_status_cfgJSON);
cJSON *OpenAPI_pdu_session_status_cfg_convertToJSON(OpenAPI_pdu_session_status_cfg_t *pdu_session_status_cfg);
OpenAPI_pdu_session_status_cfg_t *OpenAPI_pdu_session_status_cfg_copy(OpenAPI_pdu_session_status_cfg_t *dst, OpenAPI_pdu_session_status_cfg_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pdu_session_status_cfg_H_ */

