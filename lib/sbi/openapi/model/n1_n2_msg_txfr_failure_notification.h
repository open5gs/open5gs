/*
 * n1_n2_msg_txfr_failure_notification.h
 *
 * 
 */

#ifndef _OpenAPI_n1_n2_msg_txfr_failure_notification_H_
#define _OpenAPI_n1_n2_msg_txfr_failure_notification_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "n1_n2_message_transfer_cause.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_n1_n2_msg_txfr_failure_notification_s OpenAPI_n1_n2_msg_txfr_failure_notification_t;
typedef struct OpenAPI_n1_n2_msg_txfr_failure_notification_s {
    OpenAPI_n1_n2_message_transfer_cause_e cause;
    char *n1n2_msg_data_uri;
} OpenAPI_n1_n2_msg_txfr_failure_notification_t;

OpenAPI_n1_n2_msg_txfr_failure_notification_t *OpenAPI_n1_n2_msg_txfr_failure_notification_create(
    OpenAPI_n1_n2_message_transfer_cause_e cause,
    char *n1n2_msg_data_uri
);
void OpenAPI_n1_n2_msg_txfr_failure_notification_free(OpenAPI_n1_n2_msg_txfr_failure_notification_t *n1_n2_msg_txfr_failure_notification);
OpenAPI_n1_n2_msg_txfr_failure_notification_t *OpenAPI_n1_n2_msg_txfr_failure_notification_parseFromJSON(cJSON *n1_n2_msg_txfr_failure_notificationJSON);
cJSON *OpenAPI_n1_n2_msg_txfr_failure_notification_convertToJSON(OpenAPI_n1_n2_msg_txfr_failure_notification_t *n1_n2_msg_txfr_failure_notification);
OpenAPI_n1_n2_msg_txfr_failure_notification_t *OpenAPI_n1_n2_msg_txfr_failure_notification_copy(OpenAPI_n1_n2_msg_txfr_failure_notification_t *dst, OpenAPI_n1_n2_msg_txfr_failure_notification_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_n1_n2_msg_txfr_failure_notification_H_ */

