/*
 * muting_notifications_settings_1.h
 *
 * Indicates the Event producer NF settings to the Event consumer NF 
 */

#ifndef _OpenAPI_muting_notifications_settings_1_H_
#define _OpenAPI_muting_notifications_settings_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_muting_notifications_settings_1_s OpenAPI_muting_notifications_settings_1_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_muting_notifications_settings_1_s {
    bool is_max_no_of_notif;
    int max_no_of_notif;
    bool is_duration_buffered_notif;
    int duration_buffered_notif;
};

OpenAPI_muting_notifications_settings_1_t *OpenAPI_muting_notifications_settings_1_create(
    bool is_max_no_of_notif,
    int max_no_of_notif,
    bool is_duration_buffered_notif,
    int duration_buffered_notif
);
void OpenAPI_muting_notifications_settings_1_free(OpenAPI_muting_notifications_settings_1_t *muting_notifications_settings_1);
OpenAPI_muting_notifications_settings_1_t *OpenAPI_muting_notifications_settings_1_parseFromJSON(cJSON *muting_notifications_settings_1JSON);
cJSON *OpenAPI_muting_notifications_settings_1_convertToJSON(OpenAPI_muting_notifications_settings_1_t *muting_notifications_settings_1);
OpenAPI_muting_notifications_settings_1_t *OpenAPI_muting_notifications_settings_1_copy(OpenAPI_muting_notifications_settings_1_t *dst, OpenAPI_muting_notifications_settings_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_muting_notifications_settings_1_H_ */

