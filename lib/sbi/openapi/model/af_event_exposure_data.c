
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "af_event_exposure_data.h"

OpenAPI_af_event_exposure_data_t *OpenAPI_af_event_exposure_data_create(
    OpenAPI_list_t *af_events,
    OpenAPI_list_t *af_ids,
    OpenAPI_list_t *app_ids,
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *tai_range_list
)
{
    OpenAPI_af_event_exposure_data_t *af_event_exposure_data_local_var = ogs_malloc(sizeof(OpenAPI_af_event_exposure_data_t));
    ogs_assert(af_event_exposure_data_local_var);

    af_event_exposure_data_local_var->af_events = af_events;
    af_event_exposure_data_local_var->af_ids = af_ids;
    af_event_exposure_data_local_var->app_ids = app_ids;
    af_event_exposure_data_local_var->tai_list = tai_list;
    af_event_exposure_data_local_var->tai_range_list = tai_range_list;

    return af_event_exposure_data_local_var;
}

void OpenAPI_af_event_exposure_data_free(OpenAPI_af_event_exposure_data_t *af_event_exposure_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == af_event_exposure_data) {
        return;
    }
    if (af_event_exposure_data->af_events) {
        OpenAPI_list_free(af_event_exposure_data->af_events);
        af_event_exposure_data->af_events = NULL;
    }
    if (af_event_exposure_data->af_ids) {
        OpenAPI_list_for_each(af_event_exposure_data->af_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(af_event_exposure_data->af_ids);
        af_event_exposure_data->af_ids = NULL;
    }
    if (af_event_exposure_data->app_ids) {
        OpenAPI_list_for_each(af_event_exposure_data->app_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(af_event_exposure_data->app_ids);
        af_event_exposure_data->app_ids = NULL;
    }
    if (af_event_exposure_data->tai_list) {
        OpenAPI_list_for_each(af_event_exposure_data->tai_list, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(af_event_exposure_data->tai_list);
        af_event_exposure_data->tai_list = NULL;
    }
    if (af_event_exposure_data->tai_range_list) {
        OpenAPI_list_for_each(af_event_exposure_data->tai_range_list, node) {
            OpenAPI_tai_range_free(node->data);
        }
        OpenAPI_list_free(af_event_exposure_data->tai_range_list);
        af_event_exposure_data->tai_range_list = NULL;
    }
    ogs_free(af_event_exposure_data);
}

cJSON *OpenAPI_af_event_exposure_data_convertToJSON(OpenAPI_af_event_exposure_data_t *af_event_exposure_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (af_event_exposure_data == NULL) {
        ogs_error("OpenAPI_af_event_exposure_data_convertToJSON() failed [AfEventExposureData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (af_event_exposure_data->af_events == OpenAPI_af_event_NULL) {
        ogs_error("OpenAPI_af_event_exposure_data_convertToJSON() failed [af_events]");
        return NULL;
    }
    cJSON *af_eventsList = cJSON_AddArrayToObject(item, "afEvents");
    if (af_eventsList == NULL) {
        ogs_error("OpenAPI_af_event_exposure_data_convertToJSON() failed [af_events]");
        goto end;
    }
    OpenAPI_list_for_each(af_event_exposure_data->af_events, node) {
        if (cJSON_AddStringToObject(af_eventsList, "", OpenAPI_af_event_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_af_event_exposure_data_convertToJSON() failed [af_events]");
            goto end;
        }
    }

    if (af_event_exposure_data->af_ids) {
    cJSON *af_idsList = cJSON_AddArrayToObject(item, "afIds");
    if (af_idsList == NULL) {
        ogs_error("OpenAPI_af_event_exposure_data_convertToJSON() failed [af_ids]");
        goto end;
    }
    OpenAPI_list_for_each(af_event_exposure_data->af_ids, node) {
        if (cJSON_AddStringToObject(af_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_af_event_exposure_data_convertToJSON() failed [af_ids]");
            goto end;
        }
    }
    }

    if (af_event_exposure_data->app_ids) {
    cJSON *app_idsList = cJSON_AddArrayToObject(item, "appIds");
    if (app_idsList == NULL) {
        ogs_error("OpenAPI_af_event_exposure_data_convertToJSON() failed [app_ids]");
        goto end;
    }
    OpenAPI_list_for_each(af_event_exposure_data->app_ids, node) {
        if (cJSON_AddStringToObject(app_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_af_event_exposure_data_convertToJSON() failed [app_ids]");
            goto end;
        }
    }
    }

    if (af_event_exposure_data->tai_list) {
    cJSON *tai_listList = cJSON_AddArrayToObject(item, "taiList");
    if (tai_listList == NULL) {
        ogs_error("OpenAPI_af_event_exposure_data_convertToJSON() failed [tai_list]");
        goto end;
    }
    OpenAPI_list_for_each(af_event_exposure_data->tai_list, node) {
        cJSON *itemLocal = OpenAPI_tai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_af_event_exposure_data_convertToJSON() failed [tai_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_listList, itemLocal);
    }
    }

    if (af_event_exposure_data->tai_range_list) {
    cJSON *tai_range_listList = cJSON_AddArrayToObject(item, "taiRangeList");
    if (tai_range_listList == NULL) {
        ogs_error("OpenAPI_af_event_exposure_data_convertToJSON() failed [tai_range_list]");
        goto end;
    }
    OpenAPI_list_for_each(af_event_exposure_data->tai_range_list, node) {
        cJSON *itemLocal = OpenAPI_tai_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_af_event_exposure_data_convertToJSON() failed [tai_range_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_range_listList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_af_event_exposure_data_t *OpenAPI_af_event_exposure_data_parseFromJSON(cJSON *af_event_exposure_dataJSON)
{
    OpenAPI_af_event_exposure_data_t *af_event_exposure_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *af_events = NULL;
    OpenAPI_list_t *af_eventsList = NULL;
    cJSON *af_ids = NULL;
    OpenAPI_list_t *af_idsList = NULL;
    cJSON *app_ids = NULL;
    OpenAPI_list_t *app_idsList = NULL;
    cJSON *tai_list = NULL;
    OpenAPI_list_t *tai_listList = NULL;
    cJSON *tai_range_list = NULL;
    OpenAPI_list_t *tai_range_listList = NULL;
    af_events = cJSON_GetObjectItemCaseSensitive(af_event_exposure_dataJSON, "afEvents");
    if (!af_events) {
        ogs_error("OpenAPI_af_event_exposure_data_parseFromJSON() failed [af_events]");
        goto end;
    }
        cJSON *af_events_local = NULL;
        if (!cJSON_IsArray(af_events)) {
            ogs_error("OpenAPI_af_event_exposure_data_parseFromJSON() failed [af_events]");
            goto end;
        }

        af_eventsList = OpenAPI_list_create();

        cJSON_ArrayForEach(af_events_local, af_events) {
            OpenAPI_af_event_e localEnum = OpenAPI_af_event_NULL;
            if (!cJSON_IsString(af_events_local)) {
                ogs_error("OpenAPI_af_event_exposure_data_parseFromJSON() failed [af_events]");
                goto end;
            }
            localEnum = OpenAPI_af_event_FromString(af_events_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"af_events\" is not supported. Ignoring it ...",
                         af_events_local->valuestring);
            } else {
                OpenAPI_list_add(af_eventsList, (void *)localEnum);
            }
        }
        if (af_eventsList->count == 0) {
            ogs_error("OpenAPI_af_event_exposure_data_parseFromJSON() failed: Expected af_eventsList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }

    af_ids = cJSON_GetObjectItemCaseSensitive(af_event_exposure_dataJSON, "afIds");
    if (af_ids) {
        cJSON *af_ids_local = NULL;
        if (!cJSON_IsArray(af_ids)) {
            ogs_error("OpenAPI_af_event_exposure_data_parseFromJSON() failed [af_ids]");
            goto end;
        }

        af_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(af_ids_local, af_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(af_ids_local)) {
                ogs_error("OpenAPI_af_event_exposure_data_parseFromJSON() failed [af_ids]");
                goto end;
            }
            OpenAPI_list_add(af_idsList, ogs_strdup(af_ids_local->valuestring));
        }
    }

    app_ids = cJSON_GetObjectItemCaseSensitive(af_event_exposure_dataJSON, "appIds");
    if (app_ids) {
        cJSON *app_ids_local = NULL;
        if (!cJSON_IsArray(app_ids)) {
            ogs_error("OpenAPI_af_event_exposure_data_parseFromJSON() failed [app_ids]");
            goto end;
        }

        app_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(app_ids_local, app_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(app_ids_local)) {
                ogs_error("OpenAPI_af_event_exposure_data_parseFromJSON() failed [app_ids]");
                goto end;
            }
            OpenAPI_list_add(app_idsList, ogs_strdup(app_ids_local->valuestring));
        }
    }

    tai_list = cJSON_GetObjectItemCaseSensitive(af_event_exposure_dataJSON, "taiList");
    if (tai_list) {
        cJSON *tai_list_local = NULL;
        if (!cJSON_IsArray(tai_list)) {
            ogs_error("OpenAPI_af_event_exposure_data_parseFromJSON() failed [tai_list]");
            goto end;
        }

        tai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_list_local, tai_list) {
            if (!cJSON_IsObject(tai_list_local)) {
                ogs_error("OpenAPI_af_event_exposure_data_parseFromJSON() failed [tai_list]");
                goto end;
            }
            OpenAPI_tai_t *tai_listItem = OpenAPI_tai_parseFromJSON(tai_list_local);
            if (!tai_listItem) {
                ogs_error("No tai_listItem");
                goto end;
            }
            OpenAPI_list_add(tai_listList, tai_listItem);
        }
    }

    tai_range_list = cJSON_GetObjectItemCaseSensitive(af_event_exposure_dataJSON, "taiRangeList");
    if (tai_range_list) {
        cJSON *tai_range_list_local = NULL;
        if (!cJSON_IsArray(tai_range_list)) {
            ogs_error("OpenAPI_af_event_exposure_data_parseFromJSON() failed [tai_range_list]");
            goto end;
        }

        tai_range_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_range_list_local, tai_range_list) {
            if (!cJSON_IsObject(tai_range_list_local)) {
                ogs_error("OpenAPI_af_event_exposure_data_parseFromJSON() failed [tai_range_list]");
                goto end;
            }
            OpenAPI_tai_range_t *tai_range_listItem = OpenAPI_tai_range_parseFromJSON(tai_range_list_local);
            if (!tai_range_listItem) {
                ogs_error("No tai_range_listItem");
                goto end;
            }
            OpenAPI_list_add(tai_range_listList, tai_range_listItem);
        }
    }

    af_event_exposure_data_local_var = OpenAPI_af_event_exposure_data_create (
        af_eventsList,
        af_ids ? af_idsList : NULL,
        app_ids ? app_idsList : NULL,
        tai_list ? tai_listList : NULL,
        tai_range_list ? tai_range_listList : NULL
    );

    return af_event_exposure_data_local_var;
end:
    if (af_eventsList) {
        OpenAPI_list_free(af_eventsList);
        af_eventsList = NULL;
    }
    if (af_idsList) {
        OpenAPI_list_for_each(af_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(af_idsList);
        af_idsList = NULL;
    }
    if (app_idsList) {
        OpenAPI_list_for_each(app_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(app_idsList);
        app_idsList = NULL;
    }
    if (tai_listList) {
        OpenAPI_list_for_each(tai_listList, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(tai_listList);
        tai_listList = NULL;
    }
    if (tai_range_listList) {
        OpenAPI_list_for_each(tai_range_listList, node) {
            OpenAPI_tai_range_free(node->data);
        }
        OpenAPI_list_free(tai_range_listList);
        tai_range_listList = NULL;
    }
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

