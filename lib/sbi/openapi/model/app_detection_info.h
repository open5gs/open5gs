/*
 * app_detection_info.h
 *
 * 
 */

#ifndef _OpenAPI_app_detection_info_H_
#define _OpenAPI_app_detection_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "flow_information.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_app_detection_info_s OpenAPI_app_detection_info_t;
typedef struct OpenAPI_app_detection_info_s {
    char *app_id;
    char *instance_id;
    OpenAPI_list_t *sdf_descriptions;
} OpenAPI_app_detection_info_t;

OpenAPI_app_detection_info_t *OpenAPI_app_detection_info_create(
    char *app_id,
    char *instance_id,
    OpenAPI_list_t *sdf_descriptions
);
void OpenAPI_app_detection_info_free(OpenAPI_app_detection_info_t *app_detection_info);
OpenAPI_app_detection_info_t *OpenAPI_app_detection_info_parseFromJSON(cJSON *app_detection_infoJSON);
cJSON *OpenAPI_app_detection_info_convertToJSON(OpenAPI_app_detection_info_t *app_detection_info);
OpenAPI_app_detection_info_t *OpenAPI_app_detection_info_copy(OpenAPI_app_detection_info_t *dst, OpenAPI_app_detection_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_app_detection_info_H_ */

