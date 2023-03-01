/*
 * service_specific_authorization_info.h
 *
 * Information related to active Service Specific Authorizations
 */

#ifndef _OpenAPI_service_specific_authorization_info_H_
#define _OpenAPI_service_specific_authorization_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "authorization_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_service_specific_authorization_info_s OpenAPI_service_specific_authorization_info_t;
typedef struct OpenAPI_service_specific_authorization_info_s {
    OpenAPI_list_t *service_specific_authorization_list;
} OpenAPI_service_specific_authorization_info_t;

OpenAPI_service_specific_authorization_info_t *OpenAPI_service_specific_authorization_info_create(
    OpenAPI_list_t *service_specific_authorization_list
);
void OpenAPI_service_specific_authorization_info_free(OpenAPI_service_specific_authorization_info_t *service_specific_authorization_info);
OpenAPI_service_specific_authorization_info_t *OpenAPI_service_specific_authorization_info_parseFromJSON(cJSON *service_specific_authorization_infoJSON);
cJSON *OpenAPI_service_specific_authorization_info_convertToJSON(OpenAPI_service_specific_authorization_info_t *service_specific_authorization_info);
OpenAPI_service_specific_authorization_info_t *OpenAPI_service_specific_authorization_info_copy(OpenAPI_service_specific_authorization_info_t *dst, OpenAPI_service_specific_authorization_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_service_specific_authorization_info_H_ */

