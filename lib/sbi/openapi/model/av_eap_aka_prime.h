/*
 * av_eap_aka_prime.h
 *
 * 
 */

#ifndef _OpenAPI_av_eap_aka_prime_H_
#define _OpenAPI_av_eap_aka_prime_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "av_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_av_eap_aka_prime_s OpenAPI_av_eap_aka_prime_t;
typedef struct OpenAPI_av_eap_aka_prime_s {
    OpenAPI_av_type_e av_type;
    char *rand;
    char *xres;
    char *autn;
    char *ck_prime;
    char *ik_prime;
} OpenAPI_av_eap_aka_prime_t;

OpenAPI_av_eap_aka_prime_t *OpenAPI_av_eap_aka_prime_create(
    OpenAPI_av_type_e av_type,
    char *rand,
    char *xres,
    char *autn,
    char *ck_prime,
    char *ik_prime
);
void OpenAPI_av_eap_aka_prime_free(OpenAPI_av_eap_aka_prime_t *av_eap_aka_prime);
OpenAPI_av_eap_aka_prime_t *OpenAPI_av_eap_aka_prime_parseFromJSON(cJSON *av_eap_aka_primeJSON);
cJSON *OpenAPI_av_eap_aka_prime_convertToJSON(OpenAPI_av_eap_aka_prime_t *av_eap_aka_prime);
OpenAPI_av_eap_aka_prime_t *OpenAPI_av_eap_aka_prime_copy(OpenAPI_av_eap_aka_prime_t *dst, OpenAPI_av_eap_aka_prime_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_av_eap_aka_prime_H_ */

