/*
 * quota_management_indicator.h
 *
 * 
 */

#ifndef _OpenAPI_quota_management_indicator_H_
#define _OpenAPI_quota_management_indicator_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_quota_management_indicator_NULL = 0, OpenAPI_quota_management_indicator_ONLINE_CHARGING, OpenAPI_quota_management_indicator_OFFLINE_CHARGING, OpenAPI_quota_management_indicator_QUOTA_MANAGEMENT_SUSPENDED } OpenAPI_quota_management_indicator_e;

char* OpenAPI_quota_management_indicator_ToString(OpenAPI_quota_management_indicator_e quota_management_indicator);

OpenAPI_quota_management_indicator_e OpenAPI_quota_management_indicator_FromString(char* quota_management_indicator);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_quota_management_indicator_H_ */

