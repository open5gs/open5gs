/*
 * traffic_profile.h
 *
 * Possible values are - SINGLE_TRANS_UL: Uplink single packet transmission. - SINGLE_TRANS_DL: Downlink single packet transmission. - DUAL_TRANS_UL_FIRST: Dual packet transmission, firstly uplink packet transmission with subsequent downlink packet transmission. - DUAL_TRANS_DL_FIRST: Dual packet transmission, firstly downlink packet transmission with subsequent uplink packet transmission.
 */

#ifndef _OpenAPI_traffic_profile_H_
#define _OpenAPI_traffic_profile_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_traffic_profile_s OpenAPI_traffic_profile_t;
typedef struct OpenAPI_traffic_profile_s {
} OpenAPI_traffic_profile_t;

OpenAPI_traffic_profile_t *OpenAPI_traffic_profile_create(
    );
void OpenAPI_traffic_profile_free(OpenAPI_traffic_profile_t *traffic_profile);
OpenAPI_traffic_profile_t *OpenAPI_traffic_profile_parseFromJSON(cJSON *traffic_profileJSON);
cJSON *OpenAPI_traffic_profile_convertToJSON(OpenAPI_traffic_profile_t *traffic_profile);
OpenAPI_traffic_profile_t *OpenAPI_traffic_profile_copy(OpenAPI_traffic_profile_t *dst, OpenAPI_traffic_profile_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_traffic_profile_H_ */

