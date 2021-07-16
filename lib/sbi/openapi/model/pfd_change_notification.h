/*
 * pfd_change_notification.h
 *
 * 
 */

#ifndef _OpenAPI_pfd_change_notification_H_
#define _OpenAPI_pfd_change_notification_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "pfd_content.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pfd_change_notification_s OpenAPI_pfd_change_notification_t;
typedef struct OpenAPI_pfd_change_notification_s {
    char *application_id;
    bool is_removal_flag;
    int removal_flag;
    bool is_partial_flag;
    int partial_flag;
    OpenAPI_list_t *pfds;
} OpenAPI_pfd_change_notification_t;

OpenAPI_pfd_change_notification_t *OpenAPI_pfd_change_notification_create(
    char *application_id,
    bool is_removal_flag,
    int removal_flag,
    bool is_partial_flag,
    int partial_flag,
    OpenAPI_list_t *pfds
);
void OpenAPI_pfd_change_notification_free(OpenAPI_pfd_change_notification_t *pfd_change_notification);
OpenAPI_pfd_change_notification_t *OpenAPI_pfd_change_notification_parseFromJSON(cJSON *pfd_change_notificationJSON);
cJSON *OpenAPI_pfd_change_notification_convertToJSON(OpenAPI_pfd_change_notification_t *pfd_change_notification);
OpenAPI_pfd_change_notification_t *OpenAPI_pfd_change_notification_copy(OpenAPI_pfd_change_notification_t *dst, OpenAPI_pfd_change_notification_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pfd_change_notification_H_ */

