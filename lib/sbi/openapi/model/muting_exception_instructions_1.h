/*
 * muting_exception_instructions_1.h
 *
 * Indicates to an Event producer NF instructions for the subscription and stored events when an exception (e.g. full buffer) occurs at the Event producer NF while the event is muted. 
 */

#ifndef _OpenAPI_muting_exception_instructions_1_H_
#define _OpenAPI_muting_exception_instructions_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_muting_exception_instructions_1_s OpenAPI_muting_exception_instructions_1_t;
#include "buffered_notifications_action.h"
#include "subscription_action.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_muting_exception_instructions_1_s {
    OpenAPI_buffered_notifications_action_e buffered_notifs;
    OpenAPI_subscription_action_e subscription;
};

OpenAPI_muting_exception_instructions_1_t *OpenAPI_muting_exception_instructions_1_create(
    OpenAPI_buffered_notifications_action_e buffered_notifs,
    OpenAPI_subscription_action_e subscription
);
void OpenAPI_muting_exception_instructions_1_free(OpenAPI_muting_exception_instructions_1_t *muting_exception_instructions_1);
OpenAPI_muting_exception_instructions_1_t *OpenAPI_muting_exception_instructions_1_parseFromJSON(cJSON *muting_exception_instructions_1JSON);
cJSON *OpenAPI_muting_exception_instructions_1_convertToJSON(OpenAPI_muting_exception_instructions_1_t *muting_exception_instructions_1);
OpenAPI_muting_exception_instructions_1_t *OpenAPI_muting_exception_instructions_1_copy(OpenAPI_muting_exception_instructions_1_t *dst, OpenAPI_muting_exception_instructions_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_muting_exception_instructions_1_H_ */

