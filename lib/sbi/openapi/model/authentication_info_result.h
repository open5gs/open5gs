/*
 * authentication_info_result.h
 *
 *
 */

#ifndef _OpenAPI_authentication_info_result_H_
#define _OpenAPI_authentication_info_result_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "auth_type.h"
#include "authentication_vector.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_authentication_info_result_s OpenAPI_authentication_info_result_t;
typedef struct OpenAPI_authentication_info_result_s {
    OpenAPI_auth_type_e auth_type;
    char *supported_features;
    struct OpenAPI_authentication_vector_s *authentication_vector;
    char *supi;
} OpenAPI_authentication_info_result_t;

OpenAPI_authentication_info_result_t *OpenAPI_authentication_info_result_create(
    OpenAPI_auth_type_e auth_type,
    char *supported_features,
    OpenAPI_authentication_vector_t *authentication_vector,
    char *supi
    );
void OpenAPI_authentication_info_result_free(OpenAPI_authentication_info_result_t *authentication_info_result);
OpenAPI_authentication_info_result_t *OpenAPI_authentication_info_result_parseFromJSON(cJSON *authentication_info_resultJSON);
cJSON *OpenAPI_authentication_info_result_convertToJSON(OpenAPI_authentication_info_result_t *authentication_info_result);
OpenAPI_authentication_info_result_t *OpenAPI_authentication_info_result_copy(OpenAPI_authentication_info_result_t *dst, OpenAPI_authentication_info_result_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_authentication_info_result_H_ */

