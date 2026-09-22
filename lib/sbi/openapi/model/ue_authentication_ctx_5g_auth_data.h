/*
 * ue_authentication_ctx_5g_auth_data.h
 *
 * oneOf Av5gAka | EapPayload (TS 29.509), expressed as an x-open5gs-union object: the member that is set is serialized as the JSON value itself, i.e. an Av5gAka object for 5G AKA or the base64 EAP packet string for EAP-AKA&#39;/EAP-TLS, matching the oneOf wire format. EapPayload is inlined non-nullable here because a selected oneOf alternative always carries a packet. 
 */

#ifndef _OpenAPI_ue_authentication_ctx_5g_auth_data_H_
#define _OpenAPI_ue_authentication_ctx_5g_auth_data_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_ue_authentication_ctx_5g_auth_data_s OpenAPI_ue_authentication_ctx_5g_auth_data_t;
#include "av5g_aka.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_ue_authentication_ctx_5g_auth_data_s {
    struct OpenAPI_av5g_aka_s *av5g_aka;
    char *eap_payload;
};

OpenAPI_ue_authentication_ctx_5g_auth_data_t *OpenAPI_ue_authentication_ctx_5g_auth_data_create(
    OpenAPI_av5g_aka_t *av5g_aka,
    char *eap_payload
);
void OpenAPI_ue_authentication_ctx_5g_auth_data_free(OpenAPI_ue_authentication_ctx_5g_auth_data_t *ue_authentication_ctx_5g_auth_data);
OpenAPI_ue_authentication_ctx_5g_auth_data_t *OpenAPI_ue_authentication_ctx_5g_auth_data_parseFromJSON(cJSON *ue_authentication_ctx_5g_auth_dataJSON);
cJSON *OpenAPI_ue_authentication_ctx_5g_auth_data_convertToJSON(OpenAPI_ue_authentication_ctx_5g_auth_data_t *ue_authentication_ctx_5g_auth_data);
OpenAPI_ue_authentication_ctx_5g_auth_data_t *OpenAPI_ue_authentication_ctx_5g_auth_data_copy(OpenAPI_ue_authentication_ctx_5g_auth_data_t *dst, OpenAPI_ue_authentication_ctx_5g_auth_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_authentication_ctx_5g_auth_data_H_ */

