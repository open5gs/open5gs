/*
 * access_token_err.h
 *
 * Error returned in the access token response message
 */

#ifndef _OpenAPI_access_token_err_H_
#define _OpenAPI_access_token_err_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_access_token_err_s OpenAPI_access_token_err_t;
typedef enum { OpenAPI_access_token_err_ERROR_NULL = 0, OpenAPI_access_token_err_ERROR_invalid_request, OpenAPI_access_token_err_ERROR_invalid_client, OpenAPI_access_token_err_ERROR_invalid_grant, OpenAPI_access_token_err_ERROR_unauthorized_client, OpenAPI_access_token_err_ERROR_unsupported_grant_type, OpenAPI_access_token_err_ERROR_invalid_scope } OpenAPI_access_token_err_error_e;

char* OpenAPI_access_token_err_error_ToString(OpenAPI_access_token_err_error_e error);

OpenAPI_access_token_err_error_e OpenAPI_access_token_err_error_FromString(char* error);
typedef struct OpenAPI_access_token_err_s {
    OpenAPI_access_token_err_error_e error;
    char *error_description;
    char *error_uri;
} OpenAPI_access_token_err_t;

OpenAPI_access_token_err_t *OpenAPI_access_token_err_create(
    OpenAPI_access_token_err_error_e error,
    char *error_description,
    char *error_uri
);
void OpenAPI_access_token_err_free(OpenAPI_access_token_err_t *access_token_err);
OpenAPI_access_token_err_t *OpenAPI_access_token_err_parseFromJSON(cJSON *access_token_errJSON);
cJSON *OpenAPI_access_token_err_convertToJSON(OpenAPI_access_token_err_t *access_token_err);
OpenAPI_access_token_err_t *OpenAPI_access_token_err_copy(OpenAPI_access_token_err_t *dst, OpenAPI_access_token_err_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_access_token_err_H_ */

