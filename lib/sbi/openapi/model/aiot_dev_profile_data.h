/*
 * aiot_dev_profile_data.h
 *
 * Contains the AIoT Device Profile Data.
 */

#ifndef _OpenAPI_aiot_dev_profile_data_H_
#define _OpenAPI_aiot_dev_profile_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_aiot_dev_profile_data_s OpenAPI_aiot_dev_profile_data_t;
#include "last_known_aiotf_info.h"
#include "tid_handling_information.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_aiot_dev_profile_data_s {
    char *aiot_dev_perm_id;
    struct OpenAPI_last_known_aiotf_info_s *last_known_aiotf_info;
    char *tid_current;
    char *tid_previous;
    struct OpenAPI_tid_handling_information_s *tid_handling_information;
};

OpenAPI_aiot_dev_profile_data_t *OpenAPI_aiot_dev_profile_data_create(
    char *aiot_dev_perm_id,
    OpenAPI_last_known_aiotf_info_t *last_known_aiotf_info,
    char *tid_current,
    char *tid_previous,
    OpenAPI_tid_handling_information_t *tid_handling_information
);
void OpenAPI_aiot_dev_profile_data_free(OpenAPI_aiot_dev_profile_data_t *aiot_dev_profile_data);
OpenAPI_aiot_dev_profile_data_t *OpenAPI_aiot_dev_profile_data_parseFromJSON(cJSON *aiot_dev_profile_dataJSON);
cJSON *OpenAPI_aiot_dev_profile_data_convertToJSON(OpenAPI_aiot_dev_profile_data_t *aiot_dev_profile_data);
OpenAPI_aiot_dev_profile_data_t *OpenAPI_aiot_dev_profile_data_copy(OpenAPI_aiot_dev_profile_data_t *dst, OpenAPI_aiot_dev_profile_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_aiot_dev_profile_data_H_ */

