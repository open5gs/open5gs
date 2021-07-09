/*
 * status_notification.h
 *
 * 
 */

#ifndef _OpenAPI_status_notification_H_
#define _OpenAPI_status_notification_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "apn_rate_status.h"
#include "small_data_rate_status.h"
#include "status_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_status_notification_s OpenAPI_status_notification_t;
typedef struct OpenAPI_status_notification_s {
    struct OpenAPI_status_info_s *status_info;
    struct OpenAPI_small_data_rate_status_s *small_data_rate_status;
    struct OpenAPI_apn_rate_status_s *apn_rate_status;
} OpenAPI_status_notification_t;

OpenAPI_status_notification_t *OpenAPI_status_notification_create(
    OpenAPI_status_info_t *status_info,
    OpenAPI_small_data_rate_status_t *small_data_rate_status,
    OpenAPI_apn_rate_status_t *apn_rate_status
);
void OpenAPI_status_notification_free(OpenAPI_status_notification_t *status_notification);
OpenAPI_status_notification_t *OpenAPI_status_notification_parseFromJSON(cJSON *status_notificationJSON);
cJSON *OpenAPI_status_notification_convertToJSON(OpenAPI_status_notification_t *status_notification);
OpenAPI_status_notification_t *OpenAPI_status_notification_copy(OpenAPI_status_notification_t *dst, OpenAPI_status_notification_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_status_notification_H_ */

