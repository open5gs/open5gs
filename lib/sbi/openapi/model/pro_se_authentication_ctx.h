/*
 * pro_se_authentication_ctx.h
 *
 * Contains the information related to the resource generated to handle the ProSe authentication.
 */

#ifndef _OpenAPI_pro_se_authentication_ctx_H_
#define _OpenAPI_pro_se_authentication_ctx_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "auth_type.h"
#include "links_value_schema.h"
#include "pro_se_auth_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pro_se_authentication_ctx_s OpenAPI_pro_se_authentication_ctx_t;
typedef struct OpenAPI_pro_se_authentication_ctx_s {
    OpenAPI_auth_type_e auth_type;
    OpenAPI_list_t* _links;
    struct OpenAPI_pro_se_auth_data_s *pro_se_auth_data;
    char *supported_features;
} OpenAPI_pro_se_authentication_ctx_t;

OpenAPI_pro_se_authentication_ctx_t *OpenAPI_pro_se_authentication_ctx_create(
    OpenAPI_auth_type_e auth_type,
    OpenAPI_list_t* _links,
    OpenAPI_pro_se_auth_data_t *pro_se_auth_data,
    char *supported_features
);
void OpenAPI_pro_se_authentication_ctx_free(OpenAPI_pro_se_authentication_ctx_t *pro_se_authentication_ctx);
OpenAPI_pro_se_authentication_ctx_t *OpenAPI_pro_se_authentication_ctx_parseFromJSON(cJSON *pro_se_authentication_ctxJSON);
cJSON *OpenAPI_pro_se_authentication_ctx_convertToJSON(OpenAPI_pro_se_authentication_ctx_t *pro_se_authentication_ctx);
OpenAPI_pro_se_authentication_ctx_t *OpenAPI_pro_se_authentication_ctx_copy(OpenAPI_pro_se_authentication_ctx_t *dst, OpenAPI_pro_se_authentication_ctx_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pro_se_authentication_ctx_H_ */

