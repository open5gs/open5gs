/*
 * gba_authentication_info_result.h
 *
 * 
 */

#ifndef _OpenAPI_gba_authentication_info_result_H_
#define _OpenAPI_gba_authentication_info_result_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "model_3_g_aka_av.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_gba_authentication_info_result_s OpenAPI_gba_authentication_info_result_t;
typedef struct OpenAPI_gba_authentication_info_result_s {
    struct OpenAPI_model_3_g_aka_av_s *_3g_aka_av;
    char *supported_features;
} OpenAPI_gba_authentication_info_result_t;

OpenAPI_gba_authentication_info_result_t *OpenAPI_gba_authentication_info_result_create(
    OpenAPI_model_3_g_aka_av_t *_3g_aka_av,
    char *supported_features
);
void OpenAPI_gba_authentication_info_result_free(OpenAPI_gba_authentication_info_result_t *gba_authentication_info_result);
OpenAPI_gba_authentication_info_result_t *OpenAPI_gba_authentication_info_result_parseFromJSON(cJSON *gba_authentication_info_resultJSON);
cJSON *OpenAPI_gba_authentication_info_result_convertToJSON(OpenAPI_gba_authentication_info_result_t *gba_authentication_info_result);
OpenAPI_gba_authentication_info_result_t *OpenAPI_gba_authentication_info_result_copy(OpenAPI_gba_authentication_info_result_t *dst, OpenAPI_gba_authentication_info_result_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_gba_authentication_info_result_H_ */

