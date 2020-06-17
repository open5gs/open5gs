/*
 * hss_authentication_vectors.h
 *
 *
 */

#ifndef _OpenAPI_hss_authentication_vectors_H_
#define _OpenAPI_hss_authentication_vectors_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_hss_authentication_vectors_s OpenAPI_hss_authentication_vectors_t;
typedef struct OpenAPI_hss_authentication_vectors_s {
} OpenAPI_hss_authentication_vectors_t;

OpenAPI_hss_authentication_vectors_t *OpenAPI_hss_authentication_vectors_create(
    );
void OpenAPI_hss_authentication_vectors_free(OpenAPI_hss_authentication_vectors_t *hss_authentication_vectors);
OpenAPI_hss_authentication_vectors_t *OpenAPI_hss_authentication_vectors_parseFromJSON(cJSON *hss_authentication_vectorsJSON);
cJSON *OpenAPI_hss_authentication_vectors_convertToJSON(OpenAPI_hss_authentication_vectors_t *hss_authentication_vectors);
OpenAPI_hss_authentication_vectors_t *OpenAPI_hss_authentication_vectors_copy(OpenAPI_hss_authentication_vectors_t *dst, OpenAPI_hss_authentication_vectors_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_hss_authentication_vectors_H_ */

