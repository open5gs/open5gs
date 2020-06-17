/*
 * notification_data.h
 *
 * Data sent in notifications from NRF to subscribed NF Instances
 */

#ifndef _OpenAPI_notification_data_H_
#define _OpenAPI_notification_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "change_item.h"
#include "nf_profile.h"
#include "notification_event_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_notification_data_s OpenAPI_notification_data_t;
typedef struct OpenAPI_notification_data_s {
    OpenAPI_notification_event_type_e event;
    char *nf_instance_uri;
    struct OpenAPI_nf_profile_s *nf_profile;
    OpenAPI_list_t *profile_changes;
} OpenAPI_notification_data_t;

OpenAPI_notification_data_t *OpenAPI_notification_data_create(
    OpenAPI_notification_event_type_e event,
    char *nf_instance_uri,
    OpenAPI_nf_profile_t *nf_profile,
    OpenAPI_list_t *profile_changes
    );
void OpenAPI_notification_data_free(OpenAPI_notification_data_t *notification_data);
OpenAPI_notification_data_t *OpenAPI_notification_data_parseFromJSON(cJSON *notification_dataJSON);
cJSON *OpenAPI_notification_data_convertToJSON(OpenAPI_notification_data_t *notification_data);
OpenAPI_notification_data_t *OpenAPI_notification_data_copy(OpenAPI_notification_data_t *dst, OpenAPI_notification_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_notification_data_H_ */

