/*
 * ue_authentication_ctx_5g_auth_data.h
 *
 * 
 */

#ifndef _OpenAPI_ue_authentication_ctx_5g_auth_data_H_
#define _OpenAPI_ue_authentication_ctx_5g_auth_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "av5g_aka.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ue_authentication_ctx_5g_auth_data_s OpenAPI_ue_authentication_ctx_5g_auth_data_t;
typedef struct OpenAPI_ue_authentication_ctx_5g_auth_data_s {
    char *rand;
    char *hxres_star;
    char *autn;
} OpenAPI_ue_authentication_ctx_5g_auth_data_t;

OpenAPI_ue_authentication_ctx_5g_auth_data_t *OpenAPI_ue_authentication_ctx_5g_auth_data_create(
    char *rand,
    char *hxres_star,
    char *autn
);
void OpenAPI_ue_authentication_ctx_5g_auth_data_free(OpenAPI_ue_authentication_ctx_5g_auth_data_t *ue_authentication_ctx_5g_auth_data);
OpenAPI_ue_authentication_ctx_5g_auth_data_t *OpenAPI_ue_authentication_ctx_5g_auth_data_parseFromJSON(cJSON *ue_authentication_ctx_5g_auth_dataJSON);
cJSON *OpenAPI_ue_authentication_ctx_5g_auth_data_convertToJSON(OpenAPI_ue_authentication_ctx_5g_auth_data_t *ue_authentication_ctx_5g_auth_data);
OpenAPI_ue_authentication_ctx_5g_auth_data_t *OpenAPI_ue_authentication_ctx_5g_auth_data_copy(OpenAPI_ue_authentication_ctx_5g_auth_data_t *dst, OpenAPI_ue_authentication_ctx_5g_auth_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_authentication_ctx_5g_auth_data_H_ */

