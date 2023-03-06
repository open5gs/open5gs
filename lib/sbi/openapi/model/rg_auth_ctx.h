/*
 * rg_auth_ctx.h
 *
 * Contains the UE id (i.e. SUPI) and the authentication indication.
 */

#ifndef _OpenAPI_rg_auth_ctx_H_
#define _OpenAPI_rg_auth_ctx_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "auth_result.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_rg_auth_ctx_s OpenAPI_rg_auth_ctx_t;
typedef struct OpenAPI_rg_auth_ctx_s {
    OpenAPI_auth_result_e auth_result;
    char *supi;
    bool is_auth_ind;
    int auth_ind;
} OpenAPI_rg_auth_ctx_t;

OpenAPI_rg_auth_ctx_t *OpenAPI_rg_auth_ctx_create(
    OpenAPI_auth_result_e auth_result,
    char *supi,
    bool is_auth_ind,
    int auth_ind
);
void OpenAPI_rg_auth_ctx_free(OpenAPI_rg_auth_ctx_t *rg_auth_ctx);
OpenAPI_rg_auth_ctx_t *OpenAPI_rg_auth_ctx_parseFromJSON(cJSON *rg_auth_ctxJSON);
cJSON *OpenAPI_rg_auth_ctx_convertToJSON(OpenAPI_rg_auth_ctx_t *rg_auth_ctx);
OpenAPI_rg_auth_ctx_t *OpenAPI_rg_auth_ctx_copy(OpenAPI_rg_auth_ctx_t *dst, OpenAPI_rg_auth_ctx_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_rg_auth_ctx_H_ */

