/*
 * announcement_privacy_indicator.h
 *
 * 
 */

#ifndef _OpenAPI_announcement_privacy_indicator_H_
#define _OpenAPI_announcement_privacy_indicator_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_announcement_privacy_indicator_NULL = 0, OpenAPI_announcement_privacy_indicator_NOT_PRIVATE, OpenAPI_announcement_privacy_indicator__PRIVATE } OpenAPI_announcement_privacy_indicator_e;

char* OpenAPI_announcement_privacy_indicator_ToString(OpenAPI_announcement_privacy_indicator_e announcement_privacy_indicator);

OpenAPI_announcement_privacy_indicator_e OpenAPI_announcement_privacy_indicator_FromString(char* announcement_privacy_indicator);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_announcement_privacy_indicator_H_ */

