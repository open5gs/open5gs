/*
 * redirect_response_1.h
 *
 * The response shall include a Location header field containing a different URI  (pointing to a different URI of an other service instance), or the same URI if a request  is redirected to the same target resource via a different SCP. 
 */

#ifndef _OpenAPI_redirect_response_1_H_
#define _OpenAPI_redirect_response_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_redirect_response_1_s OpenAPI_redirect_response_1_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_redirect_response_1_s {
    char *cause;
    char *target_scp;
    char *target_sepp;
};

OpenAPI_redirect_response_1_t *OpenAPI_redirect_response_1_create(
    char *cause,
    char *target_scp,
    char *target_sepp
);
void OpenAPI_redirect_response_1_free(OpenAPI_redirect_response_1_t *redirect_response_1);
OpenAPI_redirect_response_1_t *OpenAPI_redirect_response_1_parseFromJSON(cJSON *redirect_response_1JSON);
cJSON *OpenAPI_redirect_response_1_convertToJSON(OpenAPI_redirect_response_1_t *redirect_response_1);
OpenAPI_redirect_response_1_t *OpenAPI_redirect_response_1_copy(OpenAPI_redirect_response_1_t *dst, OpenAPI_redirect_response_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_redirect_response_1_H_ */

