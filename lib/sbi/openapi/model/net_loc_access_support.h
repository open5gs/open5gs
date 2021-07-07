/*
 * net_loc_access_support.h
 *
 * 
 */

#ifndef _OpenAPI_net_loc_access_support_H_
#define _OpenAPI_net_loc_access_support_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_net_loc_access_support_NULL = 0, OpenAPI_net_loc_access_support_ANR_NOT_SUPPORTED, OpenAPI_net_loc_access_support_TZR_NOT_SUPPORTED, OpenAPI_net_loc_access_support_LOC_NOT_SUPPORTED } OpenAPI_net_loc_access_support_e;

char* OpenAPI_net_loc_access_support_ToString(OpenAPI_net_loc_access_support_e net_loc_access_support);

OpenAPI_net_loc_access_support_e OpenAPI_net_loc_access_support_FromString(char* net_loc_access_support);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_net_loc_access_support_H_ */

