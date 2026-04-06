/*
 * buffered_notifications_action.h
 *
 * 
 */

#ifndef _OpenAPI_buffered_notifications_action_H_
#define _OpenAPI_buffered_notifications_action_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_buffered_notifications_action_NULL = 0, OpenAPI_buffered_notifications_action_SEND_ALL, OpenAPI_buffered_notifications_action_DISCARD_ALL, OpenAPI_buffered_notifications_action_DROP_OLD } OpenAPI_buffered_notifications_action_e;

char* OpenAPI_buffered_notifications_action_ToString(OpenAPI_buffered_notifications_action_e buffered_notifications_action);

OpenAPI_buffered_notifications_action_e OpenAPI_buffered_notifications_action_FromString(char* buffered_notifications_action);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_buffered_notifications_action_H_ */

