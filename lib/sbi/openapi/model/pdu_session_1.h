/*
 * pdu_session_1.h
 *
 * 
 */

#ifndef _OpenAPI_pdu_session_1_H_
#define _OpenAPI_pdu_session_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "plmn_id_1.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pdu_session_1_s OpenAPI_pdu_session_1_t;
typedef struct OpenAPI_pdu_session_1_s {
    char *dnn;
    char *smf_instance_id;
    struct OpenAPI_plmn_id_1_s *plmn_id;
    struct OpenAPI_snssai_s *single_nssai;
} OpenAPI_pdu_session_1_t;

OpenAPI_pdu_session_1_t *OpenAPI_pdu_session_1_create(
    char *dnn,
    char *smf_instance_id,
    OpenAPI_plmn_id_1_t *plmn_id,
    OpenAPI_snssai_t *single_nssai
);
void OpenAPI_pdu_session_1_free(OpenAPI_pdu_session_1_t *pdu_session_1);
OpenAPI_pdu_session_1_t *OpenAPI_pdu_session_1_parseFromJSON(cJSON *pdu_session_1JSON);
cJSON *OpenAPI_pdu_session_1_convertToJSON(OpenAPI_pdu_session_1_t *pdu_session_1);
OpenAPI_pdu_session_1_t *OpenAPI_pdu_session_1_copy(OpenAPI_pdu_session_1_t *dst, OpenAPI_pdu_session_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pdu_session_1_H_ */

