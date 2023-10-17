/*
 * eap_auth_method_200_response.h
 *
 * 
 */

#ifndef _OpenAPI_eap_auth_method_200_response_H_
#define _OpenAPI_eap_auth_method_200_response_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "links_value_schema.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_eap_auth_method_200_response_s OpenAPI_eap_auth_method_200_response_t;
typedef struct OpenAPI_eap_auth_method_200_response_s {
    bool is_eap_payload_null;
    char *eap_payload;
    OpenAPI_list_t* _links;
} OpenAPI_eap_auth_method_200_response_t;

OpenAPI_eap_auth_method_200_response_t *OpenAPI_eap_auth_method_200_response_create(
    bool is_eap_payload_null,
    char *eap_payload,
    OpenAPI_list_t* _links
);
void OpenAPI_eap_auth_method_200_response_free(OpenAPI_eap_auth_method_200_response_t *eap_auth_method_200_response);
OpenAPI_eap_auth_method_200_response_t *OpenAPI_eap_auth_method_200_response_parseFromJSON(cJSON *eap_auth_method_200_responseJSON);
cJSON *OpenAPI_eap_auth_method_200_response_convertToJSON(OpenAPI_eap_auth_method_200_response_t *eap_auth_method_200_response);
OpenAPI_eap_auth_method_200_response_t *OpenAPI_eap_auth_method_200_response_copy(OpenAPI_eap_auth_method_200_response_t *dst, OpenAPI_eap_auth_method_200_response_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_eap_auth_method_200_response_H_ */

