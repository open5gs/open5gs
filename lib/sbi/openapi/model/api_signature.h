/*
 * api_signature.h
 *
 * API URI of the service operation
 */

#ifndef _OpenAPI_api_signature_H_
#define _OpenAPI_api_signature_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "callback_name.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_api_signature_s OpenAPI_api_signature_t;
typedef struct OpenAPI_api_signature_s {
    char *callback_type;
} OpenAPI_api_signature_t;

OpenAPI_api_signature_t *OpenAPI_api_signature_create(
    char *callback_type
);
void OpenAPI_api_signature_free(OpenAPI_api_signature_t *api_signature);
OpenAPI_api_signature_t *OpenAPI_api_signature_parseFromJSON(cJSON *api_signatureJSON);
cJSON *OpenAPI_api_signature_convertToJSON(OpenAPI_api_signature_t *api_signature);
OpenAPI_api_signature_t *OpenAPI_api_signature_copy(OpenAPI_api_signature_t *dst, OpenAPI_api_signature_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_api_signature_H_ */

