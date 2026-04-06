/*
 * allowed_target_aiot_device.h
 *
 * Contains the permanent AIoT Device ID or the filtering information, either aiotDevPermId, or filteringInfo shall be present. 
 */

#ifndef _OpenAPI_allowed_target_aiot_device_H_
#define _OpenAPI_allowed_target_aiot_device_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_allowed_target_aiot_device_s OpenAPI_allowed_target_aiot_device_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_allowed_target_aiot_device_s {
    char *aiot_dev_perm_id;
    char *filtering_info;
};

OpenAPI_allowed_target_aiot_device_t *OpenAPI_allowed_target_aiot_device_create(
    char *aiot_dev_perm_id,
    char *filtering_info
);
void OpenAPI_allowed_target_aiot_device_free(OpenAPI_allowed_target_aiot_device_t *allowed_target_aiot_device);
OpenAPI_allowed_target_aiot_device_t *OpenAPI_allowed_target_aiot_device_parseFromJSON(cJSON *allowed_target_aiot_deviceJSON);
cJSON *OpenAPI_allowed_target_aiot_device_convertToJSON(OpenAPI_allowed_target_aiot_device_t *allowed_target_aiot_device);
OpenAPI_allowed_target_aiot_device_t *OpenAPI_allowed_target_aiot_device_copy(OpenAPI_allowed_target_aiot_device_t *dst, OpenAPI_allowed_target_aiot_device_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_allowed_target_aiot_device_H_ */

