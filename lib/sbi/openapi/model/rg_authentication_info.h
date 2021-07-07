/*
 * rg_authentication_info.h
 *
 * 
 */

#ifndef _OpenAPI_rg_authentication_info_H_
#define _OpenAPI_rg_authentication_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_rg_authentication_info_s OpenAPI_rg_authentication_info_t;
typedef struct OpenAPI_rg_authentication_info_s {
    char *suci;
    int authenticated_ind;
    char *supported_features;
} OpenAPI_rg_authentication_info_t;

OpenAPI_rg_authentication_info_t *OpenAPI_rg_authentication_info_create(
    char *suci,
    int authenticated_ind,
    char *supported_features
);
void OpenAPI_rg_authentication_info_free(OpenAPI_rg_authentication_info_t *rg_authentication_info);
OpenAPI_rg_authentication_info_t *OpenAPI_rg_authentication_info_parseFromJSON(cJSON *rg_authentication_infoJSON);
cJSON *OpenAPI_rg_authentication_info_convertToJSON(OpenAPI_rg_authentication_info_t *rg_authentication_info);
OpenAPI_rg_authentication_info_t *OpenAPI_rg_authentication_info_copy(OpenAPI_rg_authentication_info_t *dst, OpenAPI_rg_authentication_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_rg_authentication_info_H_ */

