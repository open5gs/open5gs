/*
 * pp_profile_data.h
 *
 * 
 */

#ifndef _OpenAPI_pp_profile_data_H_
#define _OpenAPI_pp_profile_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "allowed_mtc_provider_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pp_profile_data_s OpenAPI_pp_profile_data_t;
typedef struct OpenAPI_pp_profile_data_s {
    OpenAPI_list_t* allowed_mtc_providers;
    char *supported_features;
} OpenAPI_pp_profile_data_t;

OpenAPI_pp_profile_data_t *OpenAPI_pp_profile_data_create(
    OpenAPI_list_t* allowed_mtc_providers,
    char *supported_features
);
void OpenAPI_pp_profile_data_free(OpenAPI_pp_profile_data_t *pp_profile_data);
OpenAPI_pp_profile_data_t *OpenAPI_pp_profile_data_parseFromJSON(cJSON *pp_profile_dataJSON);
cJSON *OpenAPI_pp_profile_data_convertToJSON(OpenAPI_pp_profile_data_t *pp_profile_data);
OpenAPI_pp_profile_data_t *OpenAPI_pp_profile_data_copy(OpenAPI_pp_profile_data_t *dst, OpenAPI_pp_profile_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pp_profile_data_H_ */

