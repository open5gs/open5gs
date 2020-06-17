/*
 * av5_g_he_aka.h
 *
 *
 */

#ifndef _OpenAPI_av5_g_he_aka_H_
#define _OpenAPI_av5_g_he_aka_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "av_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_av5_g_he_aka_s OpenAPI_av5_g_he_aka_t;
typedef struct OpenAPI_av5_g_he_aka_s {
    OpenAPI_av_type_e av_type;
    char *rand;
    char *xres_star;
    char *autn;
    char *kausf;
} OpenAPI_av5_g_he_aka_t;

OpenAPI_av5_g_he_aka_t *OpenAPI_av5_g_he_aka_create(
    OpenAPI_av_type_e av_type,
    char *rand,
    char *xres_star,
    char *autn,
    char *kausf
    );
void OpenAPI_av5_g_he_aka_free(OpenAPI_av5_g_he_aka_t *av5_g_he_aka);
OpenAPI_av5_g_he_aka_t *OpenAPI_av5_g_he_aka_parseFromJSON(cJSON *av5_g_he_akaJSON);
cJSON *OpenAPI_av5_g_he_aka_convertToJSON(OpenAPI_av5_g_he_aka_t *av5_g_he_aka);
OpenAPI_av5_g_he_aka_t *OpenAPI_av5_g_he_aka_copy(OpenAPI_av5_g_he_aka_t *dst, OpenAPI_av5_g_he_aka_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_av5_g_he_aka_H_ */

