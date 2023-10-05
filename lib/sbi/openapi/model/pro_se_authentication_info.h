/*
 * pro_se_authentication_info.h
 *
 * Contains the UE id (i.e. SUCI) or CP-PRUK ID (in 5gPrukId IE), Relay Service Code and Nonce_1. 
 */

#ifndef _OpenAPI_pro_se_authentication_info_H_
#define _OpenAPI_pro_se_authentication_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pro_se_authentication_info_s OpenAPI_pro_se_authentication_info_t;
typedef struct OpenAPI_pro_se_authentication_info_s {
    char *supi_or_suci;
    char *_5g_pruk_id;
    int relay_service_code;
    bool is_nonce1_null;
    char *nonce1;
    char *supported_features;
} OpenAPI_pro_se_authentication_info_t;

OpenAPI_pro_se_authentication_info_t *OpenAPI_pro_se_authentication_info_create(
    char *supi_or_suci,
    char *_5g_pruk_id,
    int relay_service_code,
    bool is_nonce1_null,
    char *nonce1,
    char *supported_features
);
void OpenAPI_pro_se_authentication_info_free(OpenAPI_pro_se_authentication_info_t *pro_se_authentication_info);
OpenAPI_pro_se_authentication_info_t *OpenAPI_pro_se_authentication_info_parseFromJSON(cJSON *pro_se_authentication_infoJSON);
cJSON *OpenAPI_pro_se_authentication_info_convertToJSON(OpenAPI_pro_se_authentication_info_t *pro_se_authentication_info);
OpenAPI_pro_se_authentication_info_t *OpenAPI_pro_se_authentication_info_copy(OpenAPI_pro_se_authentication_info_t *dst, OpenAPI_pro_se_authentication_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pro_se_authentication_info_H_ */

