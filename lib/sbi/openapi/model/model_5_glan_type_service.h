/*
 * model_5_glan_type_service.h
 *
 * 
 */

#ifndef _OpenAPI_model_5_glan_type_service_H_
#define _OpenAPI_model_5_glan_type_service_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_model_5_glan_type_service_s OpenAPI_model_5_glan_type_service_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_model_5_glan_type_service_s {
    char *internal_group_identifier;
};

OpenAPI_model_5_glan_type_service_t *OpenAPI_model_5_glan_type_service_create(
    char *internal_group_identifier
);
void OpenAPI_model_5_glan_type_service_free(OpenAPI_model_5_glan_type_service_t *model_5_glan_type_service);
OpenAPI_model_5_glan_type_service_t *OpenAPI_model_5_glan_type_service_parseFromJSON(cJSON *model_5_glan_type_serviceJSON);
cJSON *OpenAPI_model_5_glan_type_service_convertToJSON(OpenAPI_model_5_glan_type_service_t *model_5_glan_type_service);
OpenAPI_model_5_glan_type_service_t *OpenAPI_model_5_glan_type_service_copy(OpenAPI_model_5_glan_type_service_t *dst, OpenAPI_model_5_glan_type_service_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_model_5_glan_type_service_H_ */

