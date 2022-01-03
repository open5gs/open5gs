
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "up_path_chg_event.h"

OpenAPI_up_path_chg_event_t *OpenAPI_up_path_chg_event_create(
    char *notification_uri,
    char *notif_corre_id,
    OpenAPI_dnai_change_type_e dnai_chg_type,
    bool is_af_ack_ind,
    int af_ack_ind
)
{
    OpenAPI_up_path_chg_event_t *up_path_chg_event_local_var = ogs_malloc(sizeof(OpenAPI_up_path_chg_event_t));
    ogs_assert(up_path_chg_event_local_var);

    up_path_chg_event_local_var->notification_uri = notification_uri;
    up_path_chg_event_local_var->notif_corre_id = notif_corre_id;
    up_path_chg_event_local_var->dnai_chg_type = dnai_chg_type;
    up_path_chg_event_local_var->is_af_ack_ind = is_af_ack_ind;
    up_path_chg_event_local_var->af_ack_ind = af_ack_ind;

    return up_path_chg_event_local_var;
}

void OpenAPI_up_path_chg_event_free(OpenAPI_up_path_chg_event_t *up_path_chg_event)
{
    if (NULL == up_path_chg_event) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(up_path_chg_event->notification_uri);
    ogs_free(up_path_chg_event->notif_corre_id);
    ogs_free(up_path_chg_event);
}

cJSON *OpenAPI_up_path_chg_event_convertToJSON(OpenAPI_up_path_chg_event_t *up_path_chg_event)
{
    cJSON *item = NULL;

    if (up_path_chg_event == NULL) {
        ogs_error("OpenAPI_up_path_chg_event_convertToJSON() failed [UpPathChgEvent]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddStringToObject(item, "notificationUri", up_path_chg_event->notification_uri) == NULL) {
        ogs_error("OpenAPI_up_path_chg_event_convertToJSON() failed [notification_uri]");
        goto end;
    }

    if (cJSON_AddStringToObject(item, "notifCorreId", up_path_chg_event->notif_corre_id) == NULL) {
        ogs_error("OpenAPI_up_path_chg_event_convertToJSON() failed [notif_corre_id]");
        goto end;
    }

    if (cJSON_AddStringToObject(item, "dnaiChgType", OpenAPI_dnai_change_type_ToString(up_path_chg_event->dnai_chg_type)) == NULL) {
        ogs_error("OpenAPI_up_path_chg_event_convertToJSON() failed [dnai_chg_type]");
        goto end;
    }

    if (up_path_chg_event->is_af_ack_ind) {
    if (cJSON_AddBoolToObject(item, "afAckInd", up_path_chg_event->af_ack_ind) == NULL) {
        ogs_error("OpenAPI_up_path_chg_event_convertToJSON() failed [af_ack_ind]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_up_path_chg_event_t *OpenAPI_up_path_chg_event_parseFromJSON(cJSON *up_path_chg_eventJSON)
{
    OpenAPI_up_path_chg_event_t *up_path_chg_event_local_var = NULL;
    cJSON *notification_uri = cJSON_GetObjectItemCaseSensitive(up_path_chg_eventJSON, "notificationUri");
    if (!notification_uri) {
        ogs_error("OpenAPI_up_path_chg_event_parseFromJSON() failed [notification_uri]");
        goto end;
    }

    if (!cJSON_IsString(notification_uri)) {
        ogs_error("OpenAPI_up_path_chg_event_parseFromJSON() failed [notification_uri]");
        goto end;
    }

    cJSON *notif_corre_id = cJSON_GetObjectItemCaseSensitive(up_path_chg_eventJSON, "notifCorreId");
    if (!notif_corre_id) {
        ogs_error("OpenAPI_up_path_chg_event_parseFromJSON() failed [notif_corre_id]");
        goto end;
    }

    if (!cJSON_IsString(notif_corre_id)) {
        ogs_error("OpenAPI_up_path_chg_event_parseFromJSON() failed [notif_corre_id]");
        goto end;
    }

    cJSON *dnai_chg_type = cJSON_GetObjectItemCaseSensitive(up_path_chg_eventJSON, "dnaiChgType");
    if (!dnai_chg_type) {
        ogs_error("OpenAPI_up_path_chg_event_parseFromJSON() failed [dnai_chg_type]");
        goto end;
    }

    OpenAPI_dnai_change_type_e dnai_chg_typeVariable;
    if (!cJSON_IsString(dnai_chg_type)) {
        ogs_error("OpenAPI_up_path_chg_event_parseFromJSON() failed [dnai_chg_type]");
        goto end;
    }
    dnai_chg_typeVariable = OpenAPI_dnai_change_type_FromString(dnai_chg_type->valuestring);

    cJSON *af_ack_ind = cJSON_GetObjectItemCaseSensitive(up_path_chg_eventJSON, "afAckInd");

    if (af_ack_ind) {
    if (!cJSON_IsBool(af_ack_ind)) {
        ogs_error("OpenAPI_up_path_chg_event_parseFromJSON() failed [af_ack_ind]");
        goto end;
    }
    }

    up_path_chg_event_local_var = OpenAPI_up_path_chg_event_create (
        ogs_strdup(notification_uri->valuestring),
        ogs_strdup(notif_corre_id->valuestring),
        dnai_chg_typeVariable,
        af_ack_ind ? true : false,
        af_ack_ind ? af_ack_ind->valueint : 0
    );

    return up_path_chg_event_local_var;
end:
    return NULL;
}

OpenAPI_up_path_chg_event_t *OpenAPI_up_path_chg_event_copy(OpenAPI_up_path_chg_event_t *dst, OpenAPI_up_path_chg_event_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_up_path_chg_event_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_up_path_chg_event_convertToJSON() failed");
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

    OpenAPI_up_path_chg_event_free(dst);
    dst = OpenAPI_up_path_chg_event_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

