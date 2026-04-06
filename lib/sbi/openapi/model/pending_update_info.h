/*
 * pending_update_info.h
 *
 * 
 */

#ifndef _OpenAPI_pending_update_info_H_
#define _OpenAPI_pending_update_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_pending_update_info_NULL = 0, OpenAPI_pending_update_info_UE_LOCATION, OpenAPI_pending_update_info_TIMEZONE, OpenAPI_pending_update_info_ACCESS_TYPE, OpenAPI_pending_update_info_RAT_TYPE, OpenAPI_pending_update_info_AMF_ID } OpenAPI_pending_update_info_e;

char* OpenAPI_pending_update_info_ToString(OpenAPI_pending_update_info_e pending_update_info);

OpenAPI_pending_update_info_e OpenAPI_pending_update_info_FromString(char* pending_update_info);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pending_update_info_H_ */

