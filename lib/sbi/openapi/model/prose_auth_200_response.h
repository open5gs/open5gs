/*
 * prose_auth_200_response.h
 *
 * 
 */

#ifndef _OpenAPI_prose_auth_200_response_H_
#define _OpenAPI_prose_auth_200_response_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_prose_auth_200_response_s OpenAPI_prose_auth_200_response_t;
#include "links_value_schema.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_prose_auth_200_response_s {
    char *eap_payload;
    OpenAPI_list_t* _links;
};

OpenAPI_prose_auth_200_response_t *OpenAPI_prose_auth_200_response_create(
    char *eap_payload,
    OpenAPI_list_t* _links
);
void OpenAPI_prose_auth_200_response_free(OpenAPI_prose_auth_200_response_t *prose_auth_200_response);
OpenAPI_prose_auth_200_response_t *OpenAPI_prose_auth_200_response_parseFromJSON(cJSON *prose_auth_200_responseJSON);
cJSON *OpenAPI_prose_auth_200_response_convertToJSON(OpenAPI_prose_auth_200_response_t *prose_auth_200_response);
OpenAPI_prose_auth_200_response_t *OpenAPI_prose_auth_200_response_copy(OpenAPI_prose_auth_200_response_t *dst, OpenAPI_prose_auth_200_response_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_prose_auth_200_response_H_ */

