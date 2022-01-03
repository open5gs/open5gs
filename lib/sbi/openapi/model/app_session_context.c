
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "app_session_context.h"

OpenAPI_app_session_context_t *OpenAPI_app_session_context_create(
    OpenAPI_app_session_context_req_data_t *asc_req_data,
    OpenAPI_app_session_context_resp_data_t *asc_resp_data,
    OpenAPI_events_notification_t *evs_notif
)
{
    OpenAPI_app_session_context_t *app_session_context_local_var = ogs_malloc(sizeof(OpenAPI_app_session_context_t));
    ogs_assert(app_session_context_local_var);

    app_session_context_local_var->asc_req_data = asc_req_data;
    app_session_context_local_var->asc_resp_data = asc_resp_data;
    app_session_context_local_var->evs_notif = evs_notif;

    return app_session_context_local_var;
}

void OpenAPI_app_session_context_free(OpenAPI_app_session_context_t *app_session_context)
{
    if (NULL == app_session_context) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_app_session_context_req_data_free(app_session_context->asc_req_data);
    OpenAPI_app_session_context_resp_data_free(app_session_context->asc_resp_data);
    OpenAPI_events_notification_free(app_session_context->evs_notif);
    ogs_free(app_session_context);
}

cJSON *OpenAPI_app_session_context_convertToJSON(OpenAPI_app_session_context_t *app_session_context)
{
    cJSON *item = NULL;

    if (app_session_context == NULL) {
        ogs_error("OpenAPI_app_session_context_convertToJSON() failed [AppSessionContext]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (app_session_context->asc_req_data) {
    cJSON *asc_req_data_local_JSON = OpenAPI_app_session_context_req_data_convertToJSON(app_session_context->asc_req_data);
    if (asc_req_data_local_JSON == NULL) {
        ogs_error("OpenAPI_app_session_context_convertToJSON() failed [asc_req_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ascReqData", asc_req_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_app_session_context_convertToJSON() failed [asc_req_data]");
        goto end;
    }
    }

    if (app_session_context->asc_resp_data) {
    cJSON *asc_resp_data_local_JSON = OpenAPI_app_session_context_resp_data_convertToJSON(app_session_context->asc_resp_data);
    if (asc_resp_data_local_JSON == NULL) {
        ogs_error("OpenAPI_app_session_context_convertToJSON() failed [asc_resp_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ascRespData", asc_resp_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_app_session_context_convertToJSON() failed [asc_resp_data]");
        goto end;
    }
    }

    if (app_session_context->evs_notif) {
    cJSON *evs_notif_local_JSON = OpenAPI_events_notification_convertToJSON(app_session_context->evs_notif);
    if (evs_notif_local_JSON == NULL) {
        ogs_error("OpenAPI_app_session_context_convertToJSON() failed [evs_notif]");
        goto end;
    }
    cJSON_AddItemToObject(item, "evsNotif", evs_notif_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_app_session_context_convertToJSON() failed [evs_notif]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_app_session_context_t *OpenAPI_app_session_context_parseFromJSON(cJSON *app_session_contextJSON)
{
    OpenAPI_app_session_context_t *app_session_context_local_var = NULL;
    cJSON *asc_req_data = cJSON_GetObjectItemCaseSensitive(app_session_contextJSON, "ascReqData");

    OpenAPI_app_session_context_req_data_t *asc_req_data_local_nonprim = NULL;
    if (asc_req_data) {
    asc_req_data_local_nonprim = OpenAPI_app_session_context_req_data_parseFromJSON(asc_req_data);
    }

    cJSON *asc_resp_data = cJSON_GetObjectItemCaseSensitive(app_session_contextJSON, "ascRespData");

    OpenAPI_app_session_context_resp_data_t *asc_resp_data_local_nonprim = NULL;
    if (asc_resp_data) {
    asc_resp_data_local_nonprim = OpenAPI_app_session_context_resp_data_parseFromJSON(asc_resp_data);
    }

    cJSON *evs_notif = cJSON_GetObjectItemCaseSensitive(app_session_contextJSON, "evsNotif");

    OpenAPI_events_notification_t *evs_notif_local_nonprim = NULL;
    if (evs_notif) {
    evs_notif_local_nonprim = OpenAPI_events_notification_parseFromJSON(evs_notif);
    }

    app_session_context_local_var = OpenAPI_app_session_context_create (
        asc_req_data ? asc_req_data_local_nonprim : NULL,
        asc_resp_data ? asc_resp_data_local_nonprim : NULL,
        evs_notif ? evs_notif_local_nonprim : NULL
    );

    return app_session_context_local_var;
end:
    return NULL;
}

OpenAPI_app_session_context_t *OpenAPI_app_session_context_copy(OpenAPI_app_session_context_t *dst, OpenAPI_app_session_context_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_app_session_context_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_app_session_context_convertToJSON() failed");
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

    OpenAPI_app_session_context_free(dst);
    dst = OpenAPI_app_session_context_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

