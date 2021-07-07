/*
 * service_parameter_data_patch.h
 *
 * 
 */

#ifndef _OpenAPI_service_parameter_data_patch_H_
#define _OpenAPI_service_parameter_data_patch_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_service_parameter_data_patch_s OpenAPI_service_parameter_data_patch_t;
typedef struct OpenAPI_service_parameter_data_patch_s {
    char *param_over_pc5;
    char *param_over_uu;
} OpenAPI_service_parameter_data_patch_t;

OpenAPI_service_parameter_data_patch_t *OpenAPI_service_parameter_data_patch_create(
    char *param_over_pc5,
    char *param_over_uu
);
void OpenAPI_service_parameter_data_patch_free(OpenAPI_service_parameter_data_patch_t *service_parameter_data_patch);
OpenAPI_service_parameter_data_patch_t *OpenAPI_service_parameter_data_patch_parseFromJSON(cJSON *service_parameter_data_patchJSON);
cJSON *OpenAPI_service_parameter_data_patch_convertToJSON(OpenAPI_service_parameter_data_patch_t *service_parameter_data_patch);
OpenAPI_service_parameter_data_patch_t *OpenAPI_service_parameter_data_patch_copy(OpenAPI_service_parameter_data_patch_t *dst, OpenAPI_service_parameter_data_patch_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_service_parameter_data_patch_H_ */

