/*
 * def_sub_service_info.h
 *
 * Service Specific information for Default Notification Subscription.
 */

#ifndef _OpenAPI_def_sub_service_info_H_
#define _OpenAPI_def_sub_service_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_def_sub_service_info_s OpenAPI_def_sub_service_info_t;
typedef struct OpenAPI_def_sub_service_info_s {
    OpenAPI_list_t *versions;
    char *supported_features;
} OpenAPI_def_sub_service_info_t;

OpenAPI_def_sub_service_info_t *OpenAPI_def_sub_service_info_create(
    OpenAPI_list_t *versions,
    char *supported_features
);
void OpenAPI_def_sub_service_info_free(OpenAPI_def_sub_service_info_t *def_sub_service_info);
OpenAPI_def_sub_service_info_t *OpenAPI_def_sub_service_info_parseFromJSON(cJSON *def_sub_service_infoJSON);
cJSON *OpenAPI_def_sub_service_info_convertToJSON(OpenAPI_def_sub_service_info_t *def_sub_service_info);
OpenAPI_def_sub_service_info_t *OpenAPI_def_sub_service_info_copy(OpenAPI_def_sub_service_info_t *dst, OpenAPI_def_sub_service_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_def_sub_service_info_H_ */

