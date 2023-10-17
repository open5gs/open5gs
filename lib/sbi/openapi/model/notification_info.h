/*
 * notification_info.h
 *
 * Notification Correlation ID and Notification URI provided by the NF service consumer 
 */

#ifndef _OpenAPI_notification_info_H_
#define _OpenAPI_notification_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_notification_info_s OpenAPI_notification_info_t;
typedef struct OpenAPI_notification_info_s {
    char *notif_id;
    char *notif_uri;
    bool is_up_buffer_ind;
    int up_buffer_ind;
} OpenAPI_notification_info_t;

OpenAPI_notification_info_t *OpenAPI_notification_info_create(
    char *notif_id,
    char *notif_uri,
    bool is_up_buffer_ind,
    int up_buffer_ind
);
void OpenAPI_notification_info_free(OpenAPI_notification_info_t *notification_info);
OpenAPI_notification_info_t *OpenAPI_notification_info_parseFromJSON(cJSON *notification_infoJSON);
cJSON *OpenAPI_notification_info_convertToJSON(OpenAPI_notification_info_t *notification_info);
OpenAPI_notification_info_t *OpenAPI_notification_info_copy(OpenAPI_notification_info_t *dst, OpenAPI_notification_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_notification_info_H_ */

