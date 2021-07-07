/*
 * traffic_profile.h
 *
 * 
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

typedef enum { OpenAPI_traffic_profile_NULL = 0, OpenAPI_traffic_profile_SINGLE_TRANS_UL, OpenAPI_traffic_profile_SINGLE_TRANS_DL, OpenAPI_traffic_profile_DUAL_TRANS_UL_FIRST, OpenAPI_traffic_profile_DUAL_TRANS_DL_FIRST, OpenAPI_traffic_profile_MULTI_TRANS } OpenAPI_traffic_profile_e;

char* OpenAPI_traffic_profile_ToString(OpenAPI_traffic_profile_e traffic_profile);

OpenAPI_traffic_profile_e OpenAPI_traffic_profile_FromString(char* traffic_profile);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_traffic_profile_H_ */

