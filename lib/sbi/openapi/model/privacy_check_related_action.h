/*
 * privacy_check_related_action.h
 *
 *
 */

#ifndef _OpenAPI_privacy_check_related_action_H_
#define _OpenAPI_privacy_check_related_action_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_privacy_check_related_action_NULL = 0, OpenAPI_privacy_check_related_action_LOCATION_NOT_ALLOWED, OpenAPI_privacy_check_related_action_LOCATION_ALLOWED_WITH_NOTIFICATION, OpenAPI_privacy_check_related_action_LOCATION_ALLOWED_WITHOUT_NOTIFICATION, OpenAPI_privacy_check_related_action_LOCATION_ALLOWED_WITHOUT_RESPONSE, OpenAPI_privacy_check_related_action_LOCATION_RESTRICTED_WITHOUT_RESPONSE } OpenAPI_privacy_check_related_action_e;

char* OpenAPI_privacy_check_related_action_ToString(OpenAPI_privacy_check_related_action_e privacy_check_related_action);

OpenAPI_privacy_check_related_action_e OpenAPI_privacy_check_related_action_FromString(char* privacy_check_related_action);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_privacy_check_related_action_H_ */

