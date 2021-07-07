/*
 * required_access_info.h
 *
 * 
 */

#ifndef _OpenAPI_required_access_info_H_
#define _OpenAPI_required_access_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_required_access_info_NULL = 0, OpenAPI_required_access_info_USER_LOCATION, OpenAPI_required_access_info_UE_TIME_ZONE } OpenAPI_required_access_info_e;

char* OpenAPI_required_access_info_ToString(OpenAPI_required_access_info_e required_access_info);

OpenAPI_required_access_info_e OpenAPI_required_access_info_FromString(char* required_access_info);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_required_access_info_H_ */

