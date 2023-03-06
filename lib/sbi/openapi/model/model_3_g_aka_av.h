/*
 * model_3_g_aka_av.h
 *
 * Contains RAND, XRES, AUTN, CK, and IK
 */

#ifndef _OpenAPI_model_3_g_aka_av_H_
#define _OpenAPI_model_3_g_aka_av_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_model_3_g_aka_av_s OpenAPI_model_3_g_aka_av_t;
typedef struct OpenAPI_model_3_g_aka_av_s {
    char *rand;
    char *xres;
    char *autn;
    char *ck;
    char *ik;
} OpenAPI_model_3_g_aka_av_t;

OpenAPI_model_3_g_aka_av_t *OpenAPI_model_3_g_aka_av_create(
    char *rand,
    char *xres,
    char *autn,
    char *ck,
    char *ik
);
void OpenAPI_model_3_g_aka_av_free(OpenAPI_model_3_g_aka_av_t *model_3_g_aka_av);
OpenAPI_model_3_g_aka_av_t *OpenAPI_model_3_g_aka_av_parseFromJSON(cJSON *model_3_g_aka_avJSON);
cJSON *OpenAPI_model_3_g_aka_av_convertToJSON(OpenAPI_model_3_g_aka_av_t *model_3_g_aka_av);
OpenAPI_model_3_g_aka_av_t *OpenAPI_model_3_g_aka_av_copy(OpenAPI_model_3_g_aka_av_t *dst, OpenAPI_model_3_g_aka_av_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_model_3_g_aka_av_H_ */

