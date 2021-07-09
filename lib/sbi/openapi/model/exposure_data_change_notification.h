/*
 * exposure_data_change_notification.h
 *
 * 
 */

#ifndef _OpenAPI_exposure_data_change_notification_H_
#define _OpenAPI_exposure_data_change_notification_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "access_and_mobility_data.h"
#include "pdu_session_management_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_exposure_data_change_notification_s OpenAPI_exposure_data_change_notification_t;
typedef struct OpenAPI_exposure_data_change_notification_s {
    char *ue_id;
    struct OpenAPI_access_and_mobility_data_s *access_and_mobility_data;
    OpenAPI_list_t *pdu_session_management_data;
    OpenAPI_list_t *del_resources;
} OpenAPI_exposure_data_change_notification_t;

OpenAPI_exposure_data_change_notification_t *OpenAPI_exposure_data_change_notification_create(
    char *ue_id,
    OpenAPI_access_and_mobility_data_t *access_and_mobility_data,
    OpenAPI_list_t *pdu_session_management_data,
    OpenAPI_list_t *del_resources
);
void OpenAPI_exposure_data_change_notification_free(OpenAPI_exposure_data_change_notification_t *exposure_data_change_notification);
OpenAPI_exposure_data_change_notification_t *OpenAPI_exposure_data_change_notification_parseFromJSON(cJSON *exposure_data_change_notificationJSON);
cJSON *OpenAPI_exposure_data_change_notification_convertToJSON(OpenAPI_exposure_data_change_notification_t *exposure_data_change_notification);
OpenAPI_exposure_data_change_notification_t *OpenAPI_exposure_data_change_notification_copy(OpenAPI_exposure_data_change_notification_t *dst, OpenAPI_exposure_data_change_notification_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_exposure_data_change_notification_H_ */

