
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "muting_notifications_settings_1.h"

OpenAPI_muting_notifications_settings_1_t *OpenAPI_muting_notifications_settings_1_create(
    bool is_max_no_of_notif,
    int max_no_of_notif,
    bool is_duration_buffered_notif,
    int duration_buffered_notif
)
{
    OpenAPI_muting_notifications_settings_1_t *muting_notifications_settings_1_local_var = ogs_malloc(sizeof(OpenAPI_muting_notifications_settings_1_t));
    ogs_assert(muting_notifications_settings_1_local_var);

    muting_notifications_settings_1_local_var->is_max_no_of_notif = is_max_no_of_notif;
    muting_notifications_settings_1_local_var->max_no_of_notif = max_no_of_notif;
    muting_notifications_settings_1_local_var->is_duration_buffered_notif = is_duration_buffered_notif;
    muting_notifications_settings_1_local_var->duration_buffered_notif = duration_buffered_notif;

    return muting_notifications_settings_1_local_var;
}

void OpenAPI_muting_notifications_settings_1_free(OpenAPI_muting_notifications_settings_1_t *muting_notifications_settings_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == muting_notifications_settings_1) {
        return;
    }
    ogs_free(muting_notifications_settings_1);
}

cJSON *OpenAPI_muting_notifications_settings_1_convertToJSON(OpenAPI_muting_notifications_settings_1_t *muting_notifications_settings_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (muting_notifications_settings_1 == NULL) {
        ogs_error("OpenAPI_muting_notifications_settings_1_convertToJSON() failed [MutingNotificationsSettings_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (muting_notifications_settings_1->is_max_no_of_notif) {
    if (cJSON_AddNumberToObject(item, "maxNoOfNotif", muting_notifications_settings_1->max_no_of_notif) == NULL) {
        ogs_error("OpenAPI_muting_notifications_settings_1_convertToJSON() failed [max_no_of_notif]");
        goto end;
    }
    }

    if (muting_notifications_settings_1->is_duration_buffered_notif) {
    if (cJSON_AddNumberToObject(item, "durationBufferedNotif", muting_notifications_settings_1->duration_buffered_notif) == NULL) {
        ogs_error("OpenAPI_muting_notifications_settings_1_convertToJSON() failed [duration_buffered_notif]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_muting_notifications_settings_1_t *OpenAPI_muting_notifications_settings_1_parseFromJSON(cJSON *muting_notifications_settings_1JSON)
{
    OpenAPI_muting_notifications_settings_1_t *muting_notifications_settings_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *max_no_of_notif = NULL;
    cJSON *duration_buffered_notif = NULL;
    max_no_of_notif = cJSON_GetObjectItemCaseSensitive(muting_notifications_settings_1JSON, "maxNoOfNotif");
    if (max_no_of_notif) {
    if (!cJSON_IsNumber(max_no_of_notif)) {
        ogs_error("OpenAPI_muting_notifications_settings_1_parseFromJSON() failed [max_no_of_notif]");
        goto end;
    }
    }

    duration_buffered_notif = cJSON_GetObjectItemCaseSensitive(muting_notifications_settings_1JSON, "durationBufferedNotif");
    if (duration_buffered_notif) {
    if (!cJSON_IsNumber(duration_buffered_notif)) {
        ogs_error("OpenAPI_muting_notifications_settings_1_parseFromJSON() failed [duration_buffered_notif]");
        goto end;
    }
    }

    muting_notifications_settings_1_local_var = OpenAPI_muting_notifications_settings_1_create (
        max_no_of_notif ? true : false,
        max_no_of_notif ? max_no_of_notif->valuedouble : 0,
        duration_buffered_notif ? true : false,
        duration_buffered_notif ? duration_buffered_notif->valuedouble : 0
    );

    return muting_notifications_settings_1_local_var;
end:
    return NULL;
}

OpenAPI_muting_notifications_settings_1_t *OpenAPI_muting_notifications_settings_1_copy(OpenAPI_muting_notifications_settings_1_t *dst, OpenAPI_muting_notifications_settings_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_muting_notifications_settings_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_muting_notifications_settings_1_convertToJSON() failed");
        return NULL;
    }

    content = cJSON_Print(item);
    cJSON_Delete(item);

    if (!content) {
        ogs_error("cJSON_Print() failed");
        return NULL;
    }

    item = cJSON_Parse(content);
    ogs_free(content);
    if (!item) {
        ogs_error("cJSON_Parse() failed");
        return NULL;
    }

    OpenAPI_muting_notifications_settings_1_free(dst);
    dst = OpenAPI_muting_notifications_settings_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

