/*
 * ext_redirect_response.h
 *
 * Extension of the redirection response
 */

#ifndef _OpenAPI_ext_redirect_response_H_
#define _OpenAPI_ext_redirect_response_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_ext_redirect_response_s OpenAPI_ext_redirect_response_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_ext_redirect_response_s {
    char *cause;
    char *target_scp;
    char *target_sepp;
    char *sepp_fqdn_for_discovery;
};

OpenAPI_ext_redirect_response_t *OpenAPI_ext_redirect_response_create(
    char *cause,
    char *target_scp,
    char *target_sepp,
    char *sepp_fqdn_for_discovery
);
void OpenAPI_ext_redirect_response_free(OpenAPI_ext_redirect_response_t *ext_redirect_response);
OpenAPI_ext_redirect_response_t *OpenAPI_ext_redirect_response_parseFromJSON(cJSON *ext_redirect_responseJSON);
cJSON *OpenAPI_ext_redirect_response_convertToJSON(OpenAPI_ext_redirect_response_t *ext_redirect_response);
OpenAPI_ext_redirect_response_t *OpenAPI_ext_redirect_response_copy(OpenAPI_ext_redirect_response_t *dst, OpenAPI_ext_redirect_response_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ext_redirect_response_H_ */

