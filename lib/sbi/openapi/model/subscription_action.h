/*
 * subscription_action.h
 *
 * 
 */

#ifndef _OpenAPI_subscription_action_H_
#define _OpenAPI_subscription_action_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_subscription_action_NULL = 0, OpenAPI_subscription_action_CLOSE, OpenAPI_subscription_action_CONTINUE_WITH_MUTING, OpenAPI_subscription_action_CONTINUE_WITHOUT_MUTING } OpenAPI_subscription_action_e;

char* OpenAPI_subscription_action_ToString(OpenAPI_subscription_action_e subscription_action);

OpenAPI_subscription_action_e OpenAPI_subscription_action_FromString(char* subscription_action);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_subscription_action_H_ */

