/*
 * pro_se_authentication_info_request.h
 *
 * 
 */

#ifndef _OpenAPI_pro_se_authentication_info_request_H_
#define _OpenAPI_pro_se_authentication_info_request_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "resynchronization_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pro_se_authentication_info_request_s OpenAPI_pro_se_authentication_info_request_t;
typedef struct OpenAPI_pro_se_authentication_info_request_s {
    char *serving_network_name;
    int relay_service_code;
    struct OpenAPI_resynchronization_info_s *resynchronization_info;
    char *supported_features;
} OpenAPI_pro_se_authentication_info_request_t;

OpenAPI_pro_se_authentication_info_request_t *OpenAPI_pro_se_authentication_info_request_create(
    char *serving_network_name,
    int relay_service_code,
    OpenAPI_resynchronization_info_t *resynchronization_info,
    char *supported_features
);
void OpenAPI_pro_se_authentication_info_request_free(OpenAPI_pro_se_authentication_info_request_t *pro_se_authentication_info_request);
OpenAPI_pro_se_authentication_info_request_t *OpenAPI_pro_se_authentication_info_request_parseFromJSON(cJSON *pro_se_authentication_info_requestJSON);
cJSON *OpenAPI_pro_se_authentication_info_request_convertToJSON(OpenAPI_pro_se_authentication_info_request_t *pro_se_authentication_info_request);
OpenAPI_pro_se_authentication_info_request_t *OpenAPI_pro_se_authentication_info_request_copy(OpenAPI_pro_se_authentication_info_request_t *dst, OpenAPI_pro_se_authentication_info_request_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pro_se_authentication_info_request_H_ */

