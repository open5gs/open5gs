/*
 * ee_profile_data.h
 *
 *
 */

#ifndef _OpenAPI_ee_profile_data_H_
#define _OpenAPI_ee_profile_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "event_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ee_profile_data_s OpenAPI_ee_profile_data_t;
typedef struct OpenAPI_ee_profile_data_s {
    OpenAPI_list_t *restricted_event_types;
    char *supported_features;
} OpenAPI_ee_profile_data_t;

OpenAPI_ee_profile_data_t *OpenAPI_ee_profile_data_create(
    OpenAPI_list_t *restricted_event_types,
    char *supported_features
    );
void OpenAPI_ee_profile_data_free(OpenAPI_ee_profile_data_t *ee_profile_data);
OpenAPI_ee_profile_data_t *OpenAPI_ee_profile_data_parseFromJSON(cJSON *ee_profile_dataJSON);
cJSON *OpenAPI_ee_profile_data_convertToJSON(OpenAPI_ee_profile_data_t *ee_profile_data);
OpenAPI_ee_profile_data_t *OpenAPI_ee_profile_data_copy(OpenAPI_ee_profile_data_t *dst, OpenAPI_ee_profile_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ee_profile_data_H_ */

