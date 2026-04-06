/*
 * ranging_sl_privacy_check_related_action.h
 *
 * Indicates actions related to privacy check for ranging and sidelink positioning.
 */

#ifndef _OpenAPI_ranging_sl_privacy_check_related_action_H_
#define _OpenAPI_ranging_sl_privacy_check_related_action_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_ranging_sl_privacy_check_related_action_NULL = 0, OpenAPI_ranging_sl_privacy_check_related_action_RANGINGSL_NOT_ALLOWED, OpenAPI_ranging_sl_privacy_check_related_action_RANGINGSL_ALLOWED_WITH_NOTIFICATION, OpenAPI_ranging_sl_privacy_check_related_action_RANGINGSL_ALLOWED_WITHOUT_NOTIFICATION, OpenAPI_ranging_sl_privacy_check_related_action_RANGINGSL_ALLOWED_WITHOUT_RESPONSE, OpenAPI_ranging_sl_privacy_check_related_action_RANGINGSL_RESTRICTED_WITHOUT_RESPONSE } OpenAPI_ranging_sl_privacy_check_related_action_e;

char* OpenAPI_ranging_sl_privacy_check_related_action_ToString(OpenAPI_ranging_sl_privacy_check_related_action_e ranging_sl_privacy_check_related_action);

OpenAPI_ranging_sl_privacy_check_related_action_e OpenAPI_ranging_sl_privacy_check_related_action_FromString(char* ranging_sl_privacy_check_related_action);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ranging_sl_privacy_check_related_action_H_ */

