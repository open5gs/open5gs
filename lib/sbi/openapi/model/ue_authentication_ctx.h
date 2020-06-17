/*
 * ue_authentication_ctx.h
 *
 *
 */

#ifndef _OpenAPI_ue_authentication_ctx_H_
#define _OpenAPI_ue_authentication_ctx_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "auth_type.h"
#include "av5g_aka.h"
#include "links_value_schema.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ue_authentication_ctx_s OpenAPI_ue_authentication_ctx_t;
typedef struct OpenAPI_ue_authentication_ctx_s {
    OpenAPI_auth_type_e auth_type;
    struct OpenAPI_av5g_aka_s *_5g_auth_data;
    OpenAPI_list_t* _links;
    char *serving_network_name;
} OpenAPI_ue_authentication_ctx_t;

OpenAPI_ue_authentication_ctx_t *OpenAPI_ue_authentication_ctx_create(
    OpenAPI_auth_type_e auth_type,
    OpenAPI_av5g_aka_t *_5g_auth_data,
    OpenAPI_list_t* _links,
    char *serving_network_name
    );
void OpenAPI_ue_authentication_ctx_free(OpenAPI_ue_authentication_ctx_t *ue_authentication_ctx);
OpenAPI_ue_authentication_ctx_t *OpenAPI_ue_authentication_ctx_parseFromJSON(cJSON *ue_authentication_ctxJSON);
cJSON *OpenAPI_ue_authentication_ctx_convertToJSON(OpenAPI_ue_authentication_ctx_t *ue_authentication_ctx);
OpenAPI_ue_authentication_ctx_t *OpenAPI_ue_authentication_ctx_copy(OpenAPI_ue_authentication_ctx_t *dst, OpenAPI_ue_authentication_ctx_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_authentication_ctx_H_ */

