/*
 * hss_authentication_info_result.h
 *
 * 
 */

#ifndef _OpenAPI_hss_authentication_info_result_H_
#define _OpenAPI_hss_authentication_info_result_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "hss_authentication_vectors.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_hss_authentication_info_result_s OpenAPI_hss_authentication_info_result_t;
typedef struct OpenAPI_hss_authentication_info_result_s {
    char *supported_features;
    struct OpenAPI_hss_authentication_vectors_s *hss_authentication_vectors;
} OpenAPI_hss_authentication_info_result_t;

OpenAPI_hss_authentication_info_result_t *OpenAPI_hss_authentication_info_result_create(
    char *supported_features,
    OpenAPI_hss_authentication_vectors_t *hss_authentication_vectors
);
void OpenAPI_hss_authentication_info_result_free(OpenAPI_hss_authentication_info_result_t *hss_authentication_info_result);
OpenAPI_hss_authentication_info_result_t *OpenAPI_hss_authentication_info_result_parseFromJSON(cJSON *hss_authentication_info_resultJSON);
cJSON *OpenAPI_hss_authentication_info_result_convertToJSON(OpenAPI_hss_authentication_info_result_t *hss_authentication_info_result);
OpenAPI_hss_authentication_info_result_t *OpenAPI_hss_authentication_info_result_copy(OpenAPI_hss_authentication_info_result_t *dst, OpenAPI_hss_authentication_info_result_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_hss_authentication_info_result_H_ */

