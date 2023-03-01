/*
 * pro_se_authentication_info_result.h
 *
 * 
 */

#ifndef _OpenAPI_pro_se_authentication_info_result_H_
#define _OpenAPI_pro_se_authentication_info_result_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "auth_type.h"
#include "pro_se_authentication_vectors.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pro_se_authentication_info_result_s OpenAPI_pro_se_authentication_info_result_t;
typedef struct OpenAPI_pro_se_authentication_info_result_s {
    OpenAPI_auth_type_e auth_type;
    struct OpenAPI_pro_se_authentication_vectors_s *prose_authentication_vectors;
    char *supi;
    char *supported_features;
} OpenAPI_pro_se_authentication_info_result_t;

OpenAPI_pro_se_authentication_info_result_t *OpenAPI_pro_se_authentication_info_result_create(
    OpenAPI_auth_type_e auth_type,
    OpenAPI_pro_se_authentication_vectors_t *prose_authentication_vectors,
    char *supi,
    char *supported_features
);
void OpenAPI_pro_se_authentication_info_result_free(OpenAPI_pro_se_authentication_info_result_t *pro_se_authentication_info_result);
OpenAPI_pro_se_authentication_info_result_t *OpenAPI_pro_se_authentication_info_result_parseFromJSON(cJSON *pro_se_authentication_info_resultJSON);
cJSON *OpenAPI_pro_se_authentication_info_result_convertToJSON(OpenAPI_pro_se_authentication_info_result_t *pro_se_authentication_info_result);
OpenAPI_pro_se_authentication_info_result_t *OpenAPI_pro_se_authentication_info_result_copy(OpenAPI_pro_se_authentication_info_result_t *dst, OpenAPI_pro_se_authentication_info_result_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pro_se_authentication_info_result_H_ */

