/*
 * af_request_authorization.h
 *
 * AF Request Authorization
 */

#ifndef _OpenAPI_af_request_authorization_H_
#define _OpenAPI_af_request_authorization_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_af_request_authorization_s OpenAPI_af_request_authorization_t;
#include "asti_allowed_info.h"
#include "gptp_allowed_info.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_af_request_authorization_s {
    OpenAPI_list_t *gptp_allowed_info_list;
    struct OpenAPI_asti_allowed_info_s *asti_allowed_info;
};

OpenAPI_af_request_authorization_t *OpenAPI_af_request_authorization_create(
    OpenAPI_list_t *gptp_allowed_info_list,
    OpenAPI_asti_allowed_info_t *asti_allowed_info
);
void OpenAPI_af_request_authorization_free(OpenAPI_af_request_authorization_t *af_request_authorization);
OpenAPI_af_request_authorization_t *OpenAPI_af_request_authorization_parseFromJSON(cJSON *af_request_authorizationJSON);
cJSON *OpenAPI_af_request_authorization_convertToJSON(OpenAPI_af_request_authorization_t *af_request_authorization);
OpenAPI_af_request_authorization_t *OpenAPI_af_request_authorization_copy(OpenAPI_af_request_authorization_t *dst, OpenAPI_af_request_authorization_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_af_request_authorization_H_ */

