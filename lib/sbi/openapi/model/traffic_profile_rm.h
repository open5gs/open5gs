/*
 * traffic_profile_rm.h
 *
 *
 */

#ifndef _OpenAPI_traffic_profile_rm_H_
#define _OpenAPI_traffic_profile_rm_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "null_value.h"
#include "traffic_profile.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_traffic_profile_rm_s OpenAPI_traffic_profile_rm_t;
typedef struct OpenAPI_traffic_profile_rm_s {
} OpenAPI_traffic_profile_rm_t;

OpenAPI_traffic_profile_rm_t *OpenAPI_traffic_profile_rm_create(
    );
void OpenAPI_traffic_profile_rm_free(OpenAPI_traffic_profile_rm_t *traffic_profile_rm);
OpenAPI_traffic_profile_rm_t *OpenAPI_traffic_profile_rm_parseFromJSON(cJSON *traffic_profile_rmJSON);
cJSON *OpenAPI_traffic_profile_rm_convertToJSON(OpenAPI_traffic_profile_rm_t *traffic_profile_rm);
OpenAPI_traffic_profile_rm_t *OpenAPI_traffic_profile_rm_copy(OpenAPI_traffic_profile_rm_t *dst, OpenAPI_traffic_profile_rm_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_traffic_profile_rm_H_ */

