/*
 * pro_se_authentication_result.h
 *
 * Successful authentication for CP-PRUK ID.
 */

#ifndef _OpenAPI_pro_se_authentication_result_H_
#define _OpenAPI_pro_se_authentication_result_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pro_se_authentication_result_s OpenAPI_pro_se_authentication_result_t;
typedef struct OpenAPI_pro_se_authentication_result_s {
    char *knr_pro_se;
    bool is_nonce2_null;
    char *nonce2;
    char *supported_features;
} OpenAPI_pro_se_authentication_result_t;

OpenAPI_pro_se_authentication_result_t *OpenAPI_pro_se_authentication_result_create(
    char *knr_pro_se,
    bool is_nonce2_null,
    char *nonce2,
    char *supported_features
);
void OpenAPI_pro_se_authentication_result_free(OpenAPI_pro_se_authentication_result_t *pro_se_authentication_result);
OpenAPI_pro_se_authentication_result_t *OpenAPI_pro_se_authentication_result_parseFromJSON(cJSON *pro_se_authentication_resultJSON);
cJSON *OpenAPI_pro_se_authentication_result_convertToJSON(OpenAPI_pro_se_authentication_result_t *pro_se_authentication_result);
OpenAPI_pro_se_authentication_result_t *OpenAPI_pro_se_authentication_result_copy(OpenAPI_pro_se_authentication_result_t *dst, OpenAPI_pro_se_authentication_result_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pro_se_authentication_result_H_ */

