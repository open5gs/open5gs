
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sim_conn_fail_event.h"

OpenAPI_sim_conn_fail_event_t *OpenAPI_sim_conn_fail_event_create(
    char *notification_uri,
    char *notif_corre_id
)
{
    OpenAPI_sim_conn_fail_event_t *sim_conn_fail_event_local_var = ogs_malloc(sizeof(OpenAPI_sim_conn_fail_event_t));
    ogs_assert(sim_conn_fail_event_local_var);

    sim_conn_fail_event_local_var->notification_uri = notification_uri;
    sim_conn_fail_event_local_var->notif_corre_id = notif_corre_id;

    return sim_conn_fail_event_local_var;
}

void OpenAPI_sim_conn_fail_event_free(OpenAPI_sim_conn_fail_event_t *sim_conn_fail_event)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == sim_conn_fail_event) {
        return;
    }
    if (sim_conn_fail_event->notification_uri) {
        ogs_free(sim_conn_fail_event->notification_uri);
        sim_conn_fail_event->notification_uri = NULL;
    }
    if (sim_conn_fail_event->notif_corre_id) {
        ogs_free(sim_conn_fail_event->notif_corre_id);
        sim_conn_fail_event->notif_corre_id = NULL;
    }
    ogs_free(sim_conn_fail_event);
}

cJSON *OpenAPI_sim_conn_fail_event_convertToJSON(OpenAPI_sim_conn_fail_event_t *sim_conn_fail_event)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (sim_conn_fail_event == NULL) {
        ogs_error("OpenAPI_sim_conn_fail_event_convertToJSON() failed [SimConnFailEvent]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!sim_conn_fail_event->notification_uri) {
        ogs_error("OpenAPI_sim_conn_fail_event_convertToJSON() failed [notification_uri]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "notificationUri", sim_conn_fail_event->notification_uri) == NULL) {
        ogs_error("OpenAPI_sim_conn_fail_event_convertToJSON() failed [notification_uri]");
        goto end;
    }

    if (!sim_conn_fail_event->notif_corre_id) {
        ogs_error("OpenAPI_sim_conn_fail_event_convertToJSON() failed [notif_corre_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "notifCorreId", sim_conn_fail_event->notif_corre_id) == NULL) {
        ogs_error("OpenAPI_sim_conn_fail_event_convertToJSON() failed [notif_corre_id]");
        goto end;
    }

end:
    return item;
}

OpenAPI_sim_conn_fail_event_t *OpenAPI_sim_conn_fail_event_parseFromJSON(cJSON *sim_conn_fail_eventJSON)
{
    OpenAPI_sim_conn_fail_event_t *sim_conn_fail_event_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *notification_uri = NULL;
    cJSON *notif_corre_id = NULL;
    notification_uri = cJSON_GetObjectItemCaseSensitive(sim_conn_fail_eventJSON, "notificationUri");
    if (!notification_uri) {
        ogs_error("OpenAPI_sim_conn_fail_event_parseFromJSON() failed [notification_uri]");
        goto end;
    }
    if (!cJSON_IsString(notification_uri)) {
        ogs_error("OpenAPI_sim_conn_fail_event_parseFromJSON() failed [notification_uri]");
        goto end;
    }

    notif_corre_id = cJSON_GetObjectItemCaseSensitive(sim_conn_fail_eventJSON, "notifCorreId");
    if (!notif_corre_id) {
        ogs_error("OpenAPI_sim_conn_fail_event_parseFromJSON() failed [notif_corre_id]");
        goto end;
    }
    if (!cJSON_IsString(notif_corre_id)) {
        ogs_error("OpenAPI_sim_conn_fail_event_parseFromJSON() failed [notif_corre_id]");
        goto end;
    }

    sim_conn_fail_event_local_var = OpenAPI_sim_conn_fail_event_create (
        ogs_strdup(notification_uri->valuestring),
        ogs_strdup(notif_corre_id->valuestring)
    );

    return sim_conn_fail_event_local_var;
end:
    return NULL;
}

OpenAPI_sim_conn_fail_event_t *OpenAPI_sim_conn_fail_event_copy(OpenAPI_sim_conn_fail_event_t *dst, OpenAPI_sim_conn_fail_event_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sim_conn_fail_event_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sim_conn_fail_event_convertToJSON() failed");
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

    OpenAPI_sim_conn_fail_event_free(dst);
    dst = OpenAPI_sim_conn_fail_event_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

