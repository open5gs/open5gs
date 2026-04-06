/*
 * muting_notifications_settings.h
 *
 * Indicates the Event producer NF settings to the Event consumer NF 
 */

#ifndef _OpenAPI_muting_notifications_settings_H_
#define _OpenAPI_muting_notifications_settings_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_muting_notifications_settings_s OpenAPI_muting_notifications_settings_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_muting_notifications_settings_s {
    bool is_max_no_of_notif;
    int max_no_of_notif;
    bool is_duration_buffered_notif;
    int duration_buffered_notif;
};

OpenAPI_muting_notifications_settings_t *OpenAPI_muting_notifications_settings_create(
    bool is_max_no_of_notif,
    int max_no_of_notif,
    bool is_duration_buffered_notif,
    int duration_buffered_notif
);
void OpenAPI_muting_notifications_settings_free(OpenAPI_muting_notifications_settings_t *muting_notifications_settings);
OpenAPI_muting_notifications_settings_t *OpenAPI_muting_notifications_settings_parseFromJSON(cJSON *muting_notifications_settingsJSON);
cJSON *OpenAPI_muting_notifications_settings_convertToJSON(OpenAPI_muting_notifications_settings_t *muting_notifications_settings);
OpenAPI_muting_notifications_settings_t *OpenAPI_muting_notifications_settings_copy(OpenAPI_muting_notifications_settings_t *dst, OpenAPI_muting_notifications_settings_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_muting_notifications_settings_H_ */

