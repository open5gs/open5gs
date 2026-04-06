/*
 * redirect_response_add_info.h
 *
 * Additional information in the redirection response
 */

#ifndef _OpenAPI_redirect_response_add_info_H_
#define _OpenAPI_redirect_response_add_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_redirect_response_add_info_s OpenAPI_redirect_response_add_info_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_redirect_response_add_info_s {
    char *sepp_fqdn_for_discovery;
};

OpenAPI_redirect_response_add_info_t *OpenAPI_redirect_response_add_info_create(
    char *sepp_fqdn_for_discovery
);
void OpenAPI_redirect_response_add_info_free(OpenAPI_redirect_response_add_info_t *redirect_response_add_info);
OpenAPI_redirect_response_add_info_t *OpenAPI_redirect_response_add_info_parseFromJSON(cJSON *redirect_response_add_infoJSON);
cJSON *OpenAPI_redirect_response_add_info_convertToJSON(OpenAPI_redirect_response_add_info_t *redirect_response_add_info);
OpenAPI_redirect_response_add_info_t *OpenAPI_redirect_response_add_info_copy(OpenAPI_redirect_response_add_info_t *dst, OpenAPI_redirect_response_add_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_redirect_response_add_info_H_ */

