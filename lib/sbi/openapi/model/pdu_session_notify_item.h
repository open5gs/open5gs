/*
 * pdu_session_notify_item.h
 *
 *
 */

#ifndef _OpenAPI_pdu_session_notify_item_H_
#define _OpenAPI_pdu_session_notify_item_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "notification_cause.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pdu_session_notify_item_s OpenAPI_pdu_session_notify_item_t;
typedef struct OpenAPI_pdu_session_notify_item_s {
    OpenAPI_notification_cause_e notification_cause;
} OpenAPI_pdu_session_notify_item_t;

OpenAPI_pdu_session_notify_item_t *OpenAPI_pdu_session_notify_item_create(
    OpenAPI_notification_cause_e notification_cause
    );
void OpenAPI_pdu_session_notify_item_free(OpenAPI_pdu_session_notify_item_t *pdu_session_notify_item);
OpenAPI_pdu_session_notify_item_t *OpenAPI_pdu_session_notify_item_parseFromJSON(cJSON *pdu_session_notify_itemJSON);
cJSON *OpenAPI_pdu_session_notify_item_convertToJSON(OpenAPI_pdu_session_notify_item_t *pdu_session_notify_item);
OpenAPI_pdu_session_notify_item_t *OpenAPI_pdu_session_notify_item_copy(OpenAPI_pdu_session_notify_item_t *dst, OpenAPI_pdu_session_notify_item_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pdu_session_notify_item_H_ */

