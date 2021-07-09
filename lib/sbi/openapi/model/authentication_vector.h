/*
 * authentication_vector.h
 *
 * 
 */

#ifndef _OpenAPI_authentication_vector_H_
#define _OpenAPI_authentication_vector_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "av5_ghe_aka.h"
#include "av_eap_aka_prime.h"
#include "av_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_authentication_vector_s OpenAPI_authentication_vector_t;
typedef struct OpenAPI_authentication_vector_s {
    OpenAPI_av_type_e av_type;
    char *rand;
    char *xres;
    char *autn;
    char *ck_prime;
    char *ik_prime;
    char *xres_star;
    char *kausf;
} OpenAPI_authentication_vector_t;

OpenAPI_authentication_vector_t *OpenAPI_authentication_vector_create(
    OpenAPI_av_type_e av_type,
    char *rand,
    char *xres,
    char *autn,
    char *ck_prime,
    char *ik_prime,
    char *xres_star,
    char *kausf
);
void OpenAPI_authentication_vector_free(OpenAPI_authentication_vector_t *authentication_vector);
OpenAPI_authentication_vector_t *OpenAPI_authentication_vector_parseFromJSON(cJSON *authentication_vectorJSON);
cJSON *OpenAPI_authentication_vector_convertToJSON(OpenAPI_authentication_vector_t *authentication_vector);
OpenAPI_authentication_vector_t *OpenAPI_authentication_vector_copy(OpenAPI_authentication_vector_t *dst, OpenAPI_authentication_vector_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_authentication_vector_H_ */

