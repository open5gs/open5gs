/*
 * pdu_session.h
 *
 * 
 */

#ifndef _OpenAPI_pdu_session_H_
#define _OpenAPI_pdu_session_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "plmn_id.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pdu_session_s OpenAPI_pdu_session_t;
typedef struct OpenAPI_pdu_session_s {
    char *dnn;
    char *smf_instance_id;
    struct OpenAPI_plmn_id_s *plmn_id;
    struct OpenAPI_snssai_s *single_nssai;
} OpenAPI_pdu_session_t;

OpenAPI_pdu_session_t *OpenAPI_pdu_session_create(
    char *dnn,
    char *smf_instance_id,
    OpenAPI_plmn_id_t *plmn_id,
    OpenAPI_snssai_t *single_nssai
);
void OpenAPI_pdu_session_free(OpenAPI_pdu_session_t *pdu_session);
OpenAPI_pdu_session_t *OpenAPI_pdu_session_parseFromJSON(cJSON *pdu_sessionJSON);
cJSON *OpenAPI_pdu_session_convertToJSON(OpenAPI_pdu_session_t *pdu_session);
OpenAPI_pdu_session_t *OpenAPI_pdu_session_copy(OpenAPI_pdu_session_t *dst, OpenAPI_pdu_session_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pdu_session_H_ */

