/*
 * aiot_dev_profile_data_list.h
 *
 * AIoT Device Profile Data of a list of AIoT devices.
 */

#ifndef _OpenAPI_aiot_dev_profile_data_list_H_
#define _OpenAPI_aiot_dev_profile_data_list_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_aiot_dev_profile_data_list_s OpenAPI_aiot_dev_profile_data_list_t;
#include "aiot_dev_profile_data.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_aiot_dev_profile_data_list_s {
    OpenAPI_list_t *aiot_dev_profile_data_list;
};

OpenAPI_aiot_dev_profile_data_list_t *OpenAPI_aiot_dev_profile_data_list_create(
    OpenAPI_list_t *aiot_dev_profile_data_list
);
void OpenAPI_aiot_dev_profile_data_list_free(OpenAPI_aiot_dev_profile_data_list_t *aiot_dev_profile_data_list);
OpenAPI_aiot_dev_profile_data_list_t *OpenAPI_aiot_dev_profile_data_list_parseFromJSON(cJSON *aiot_dev_profile_data_listJSON);
cJSON *OpenAPI_aiot_dev_profile_data_list_convertToJSON(OpenAPI_aiot_dev_profile_data_list_t *aiot_dev_profile_data_list);
OpenAPI_aiot_dev_profile_data_list_t *OpenAPI_aiot_dev_profile_data_list_copy(OpenAPI_aiot_dev_profile_data_list_t *dst, OpenAPI_aiot_dev_profile_data_list_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_aiot_dev_profile_data_list_H_ */

