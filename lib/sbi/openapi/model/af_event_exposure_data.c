
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "af_event_exposure_data.h"

OpenAPI_af_event_exposure_data_t *OpenAPI_af_event_exposure_data_create(
    OpenAPI_list_t *af_events,
    OpenAPI_list_t *af_ids,
    OpenAPI_list_t *app_ids
)
{
    OpenAPI_af_event_exposure_data_t *af_event_exposure_data_local_var = ogs_malloc(sizeof(OpenAPI_af_event_exposure_data_t));
    ogs_assert(af_event_exposure_data_local_var);

    af_event_exposure_data_local_var->af_events = af_events;
    af_event_exposure_data_local_var->af_ids = af_ids;
    af_event_exposure_data_local_var->app_ids = app_ids;

    return af_event_exposure_data_local_var;
}

void OpenAPI_af_event_exposure_data_free(OpenAPI_af_event_exposure_data_t *af_event_exposure_data)
{
    if (NULL == af_event_exposure_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_free(af_event_exposure_data->af_events);
    OpenAPI_list_for_each(af_event_exposure_data->af_ids, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(af_event_exposure_data->af_ids);
    OpenAPI_list_for_each(af_event_exposure_data->app_ids, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(af_event_exposure_data->app_ids);
    ogs_free(af_event_exposure_data);
}

cJSON *OpenAPI_af_event_exposure_data_convertToJSON(OpenAPI_af_event_exposure_data_t *af_event_exposure_data)
{
    cJSON *item = NULL;

    if (af_event_exposure_data == NULL) {
        ogs_error("OpenAPI_af_event_exposure_data_convertToJSON() failed [AfEventExposureData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    cJSON *af_events = cJSON_AddArrayToObject(item, "afEvents");
    if (af_events == NULL) {
        ogs_error("OpenAPI_af_event_exposure_data_convertToJSON() failed [af_events]");
        goto end;
    }
    OpenAPI_lnode_t *af_events_node;
    OpenAPI_list_for_each(af_event_exposure_data->af_events, af_events_node) {
        if (cJSON_AddStringToObject(af_events, "", OpenAPI_af_event_ToString((intptr_t)af_events_node->data)) == NULL) {
            ogs_error("OpenAPI_af_event_exposure_data_convertToJSON() failed [af_events]");
            goto end;
        }
    }

    if (af_event_exposure_data->af_ids) {
    cJSON *af_ids = cJSON_AddArrayToObject(item, "afIds");
    if (af_ids == NULL) {
        ogs_error("OpenAPI_af_event_exposure_data_convertToJSON() failed [af_ids]");
        goto end;
    }

    OpenAPI_lnode_t *af_ids_node;
    OpenAPI_list_for_each(af_event_exposure_data->af_ids, af_ids_node)  {
    if (cJSON_AddStringToObject(af_ids, "", (char*)af_ids_node->data) == NULL) {
        ogs_error("OpenAPI_af_event_exposure_data_convertToJSON() failed [af_ids]");
        goto end;
    }
                    }
    }

    if (af_event_exposure_data->app_ids) {
    cJSON *app_ids = cJSON_AddArrayToObject(item, "appIds");
    if (app_ids == NULL) {
        ogs_error("OpenAPI_af_event_exposure_data_convertToJSON() failed [app_ids]");
        goto end;
    }

    OpenAPI_lnode_t *app_ids_node;
    OpenAPI_list_for_each(af_event_exposure_data->app_ids, app_ids_node)  {
    if (cJSON_AddStringToObject(app_ids, "", (char*)app_ids_node->data) == NULL) {
        ogs_error("OpenAPI_af_event_exposure_data_convertToJSON() failed [app_ids]");
        goto end;
    }
                    }
    }

end:
    return item;
}

OpenAPI_af_event_exposure_data_t *OpenAPI_af_event_exposure_data_parseFromJSON(cJSON *af_event_exposure_dataJSON)
{
    OpenAPI_af_event_exposure_data_t *af_event_exposure_data_local_var = NULL;
    cJSON *af_events = cJSON_GetObjectItemCaseSensitive(af_event_exposure_dataJSON, "afEvents");
    if (!af_events) {
        ogs_error("OpenAPI_af_event_exposure_data_parseFromJSON() failed [af_events]");
        goto end;
    }

    OpenAPI_list_t *af_eventsList;
    cJSON *af_events_local_nonprimitive;
    if (!cJSON_IsArray(af_events)) {
        ogs_error("OpenAPI_af_event_exposure_data_parseFromJSON() failed [af_events]");
        goto end;
    }

    af_eventsList = OpenAPI_list_create();

    cJSON_ArrayForEach(af_events_local_nonprimitive, af_events ) {
        if (!cJSON_IsString(af_events_local_nonprimitive)){
            ogs_error("OpenAPI_af_event_exposure_data_parseFromJSON() failed [af_events]");
            goto end;
        }

        OpenAPI_list_add(af_eventsList, (void *)OpenAPI_af_event_FromString(af_events_local_nonprimitive->valuestring));
    }

    cJSON *af_ids = cJSON_GetObjectItemCaseSensitive(af_event_exposure_dataJSON, "afIds");

    OpenAPI_list_t *af_idsList;
    if (af_ids) {
    cJSON *af_ids_local;
    if (!cJSON_IsArray(af_ids)) {
        ogs_error("OpenAPI_af_event_exposure_data_parseFromJSON() failed [af_ids]");
        goto end;
    }
    af_idsList = OpenAPI_list_create();

    cJSON_ArrayForEach(af_ids_local, af_ids) {
    if (!cJSON_IsString(af_ids_local)) {
        ogs_error("OpenAPI_af_event_exposure_data_parseFromJSON() failed [af_ids]");
        goto end;
    }
    OpenAPI_list_add(af_idsList , ogs_strdup(af_ids_local->valuestring));
    }
    }

    cJSON *app_ids = cJSON_GetObjectItemCaseSensitive(af_event_exposure_dataJSON, "appIds");

    OpenAPI_list_t *app_idsList;
    if (app_ids) {
    cJSON *app_ids_local;
    if (!cJSON_IsArray(app_ids)) {
        ogs_error("OpenAPI_af_event_exposure_data_parseFromJSON() failed [app_ids]");
        goto end;
    }
    app_idsList = OpenAPI_list_create();

    cJSON_ArrayForEach(app_ids_local, app_ids) {
    if (!cJSON_IsString(app_ids_local)) {
        ogs_error("OpenAPI_af_event_exposure_data_parseFromJSON() failed [app_ids]");
        goto end;
    }
    OpenAPI_list_add(app_idsList , ogs_strdup(app_ids_local->valuestring));
    }
    }

    af_event_exposure_data_local_var = OpenAPI_af_event_exposure_data_create (
        af_eventsList,
        af_ids ? af_idsList : NULL,
        app_ids ? app_idsList : NULL
    );

    return af_event_exposure_data_local_var;
end:
    return NULL;
}

OpenAPI_af_event_exposure_data_t *OpenAPI_af_event_exposure_data_copy(OpenAPI_af_event_exposure_data_t *dst, OpenAPI_af_event_exposure_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_af_event_exposure_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_af_event_exposure_data_convertToJSON() failed");
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

    OpenAPI_af_event_exposure_data_free(dst);
    dst = OpenAPI_af_event_exposure_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

