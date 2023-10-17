/*
 * pro_se_authentication_vectors.h
 *
 * 
 */

#ifndef _OpenAPI_pro_se_authentication_vectors_H_
#define _OpenAPI_pro_se_authentication_vectors_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "av_eap_aka_prime.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pro_se_authentication_vectors_s OpenAPI_pro_se_authentication_vectors_t;
typedef struct OpenAPI_pro_se_authentication_vectors_s {
} OpenAPI_pro_se_authentication_vectors_t;

OpenAPI_pro_se_authentication_vectors_t *OpenAPI_pro_se_authentication_vectors_create(
);
void OpenAPI_pro_se_authentication_vectors_free(OpenAPI_pro_se_authentication_vectors_t *pro_se_authentication_vectors);
OpenAPI_pro_se_authentication_vectors_t *OpenAPI_pro_se_authentication_vectors_parseFromJSON(cJSON *pro_se_authentication_vectorsJSON);
cJSON *OpenAPI_pro_se_authentication_vectors_convertToJSON(OpenAPI_pro_se_authentication_vectors_t *pro_se_authentication_vectors);
OpenAPI_pro_se_authentication_vectors_t *OpenAPI_pro_se_authentication_vectors_copy(OpenAPI_pro_se_authentication_vectors_t *dst, OpenAPI_pro_se_authentication_vectors_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pro_se_authentication_vectors_H_ */

