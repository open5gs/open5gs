/*
 * pro_se_eap_session.h
 *
 * Contains information related to the EAP session. If present the 5gPrukId IE shall carry the CP-PRUK ID.
 */

#ifndef _OpenAPI_pro_se_eap_session_H_
#define _OpenAPI_pro_se_eap_session_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "auth_result.h"
#include "links_value_schema.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pro_se_eap_session_s OpenAPI_pro_se_eap_session_t;
typedef struct OpenAPI_pro_se_eap_session_s {
    bool is_eap_payload_null;
    char *eap_payload;
    char *knr_pro_se;
    OpenAPI_list_t* _links;
    OpenAPI_auth_result_e auth_result;
    char *supported_features;
    bool is_nonce2_null;
    char *nonce2;
    char *_5g_pruk_id;
} OpenAPI_pro_se_eap_session_t;

OpenAPI_pro_se_eap_session_t *OpenAPI_pro_se_eap_session_create(
    bool is_eap_payload_null,
    char *eap_payload,
    char *knr_pro_se,
    OpenAPI_list_t* _links,
    OpenAPI_auth_result_e auth_result,
    char *supported_features,
    bool is_nonce2_null,
    char *nonce2,
    char *_5g_pruk_id
);
void OpenAPI_pro_se_eap_session_free(OpenAPI_pro_se_eap_session_t *pro_se_eap_session);
OpenAPI_pro_se_eap_session_t *OpenAPI_pro_se_eap_session_parseFromJSON(cJSON *pro_se_eap_sessionJSON);
cJSON *OpenAPI_pro_se_eap_session_convertToJSON(OpenAPI_pro_se_eap_session_t *pro_se_eap_session);
OpenAPI_pro_se_eap_session_t *OpenAPI_pro_se_eap_session_copy(OpenAPI_pro_se_eap_session_t *dst, OpenAPI_pro_se_eap_session_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pro_se_eap_session_H_ */

