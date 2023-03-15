
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nwdaf_data.h"

OpenAPI_nwdaf_data_t *OpenAPI_nwdaf_data_create(
    char *nwdaf_instance_id,
    OpenAPI_list_t *nwdaf_events
)
{
    OpenAPI_nwdaf_data_t *nwdaf_data_local_var = ogs_malloc(sizeof(OpenAPI_nwdaf_data_t));
    ogs_assert(nwdaf_data_local_var);

    nwdaf_data_local_var->nwdaf_instance_id = nwdaf_instance_id;
    nwdaf_data_local_var->nwdaf_events = nwdaf_events;

    return nwdaf_data_local_var;
}

void OpenAPI_nwdaf_data_free(OpenAPI_nwdaf_data_t *nwdaf_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nwdaf_data) {
        return;
    }
    if (nwdaf_data->nwdaf_instance_id) {
        ogs_free(nwdaf_data->nwdaf_instance_id);
        nwdaf_data->nwdaf_instance_id = NULL;
    }
    if (nwdaf_data->nwdaf_events) {
        OpenAPI_list_for_each(nwdaf_data->nwdaf_events, node) {
            OpenAPI_nwdaf_event_free(node->data);
        }
        OpenAPI_list_free(nwdaf_data->nwdaf_events);
        nwdaf_data->nwdaf_events = NULL;
    }
    ogs_free(nwdaf_data);
}

cJSON *OpenAPI_nwdaf_data_convertToJSON(OpenAPI_nwdaf_data_t *nwdaf_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nwdaf_data == NULL) {
        ogs_error("OpenAPI_nwdaf_data_convertToJSON() failed [NwdafData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!nwdaf_data->nwdaf_instance_id) {
        ogs_error("OpenAPI_nwdaf_data_convertToJSON() failed [nwdaf_instance_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "nwdafInstanceId", nwdaf_data->nwdaf_instance_id) == NULL) {
        ogs_error("OpenAPI_nwdaf_data_convertToJSON() failed [nwdaf_instance_id]");
        goto end;
    }

    if (nwdaf_data->nwdaf_events) {
    cJSON *nwdaf_eventsList = cJSON_AddArrayToObject(item, "nwdafEvents");
    if (nwdaf_eventsList == NULL) {
        ogs_error("OpenAPI_nwdaf_data_convertToJSON() failed [nwdaf_events]");
        goto end;
    }
    OpenAPI_list_for_each(nwdaf_data->nwdaf_events, node) {
        cJSON *itemLocal = OpenAPI_nwdaf_event_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nwdaf_data_convertToJSON() failed [nwdaf_events]");
            goto end;
        }
        cJSON_AddItemToArray(nwdaf_eventsList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_nwdaf_data_t *OpenAPI_nwdaf_data_parseFromJSON(cJSON *nwdaf_dataJSON)
{
    OpenAPI_nwdaf_data_t *nwdaf_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *nwdaf_instance_id = NULL;
    cJSON *nwdaf_events = NULL;
    OpenAPI_list_t *nwdaf_eventsList = NULL;
    nwdaf_instance_id = cJSON_GetObjectItemCaseSensitive(nwdaf_dataJSON, "nwdafInstanceId");
    if (!nwdaf_instance_id) {
        ogs_error("OpenAPI_nwdaf_data_parseFromJSON() failed [nwdaf_instance_id]");
        goto end;
    }
    if (!cJSON_IsString(nwdaf_instance_id)) {
        ogs_error("OpenAPI_nwdaf_data_parseFromJSON() failed [nwdaf_instance_id]");
        goto end;
    }

    nwdaf_events = cJSON_GetObjectItemCaseSensitive(nwdaf_dataJSON, "nwdafEvents");
    if (nwdaf_events) {
        cJSON *nwdaf_events_local = NULL;
        if (!cJSON_IsArray(nwdaf_events)) {
            ogs_error("OpenAPI_nwdaf_data_parseFromJSON() failed [nwdaf_events]");
            goto end;
        }

        nwdaf_eventsList = OpenAPI_list_create();

        cJSON_ArrayForEach(nwdaf_events_local, nwdaf_events) {
            if (!cJSON_IsObject(nwdaf_events_local)) {
                ogs_error("OpenAPI_nwdaf_data_parseFromJSON() failed [nwdaf_events]");
                goto end;
            }
            OpenAPI_nwdaf_event_t *nwdaf_eventsItem = OpenAPI_nwdaf_event_parseFromJSON(nwdaf_events_local);
            if (!nwdaf_eventsItem) {
                ogs_error("No nwdaf_eventsItem");
                goto end;
            }
            OpenAPI_list_add(nwdaf_eventsList, nwdaf_eventsItem);
        }
    }

    nwdaf_data_local_var = OpenAPI_nwdaf_data_create (
        ogs_strdup(nwdaf_instance_id->valuestring),
        nwdaf_events ? nwdaf_eventsList : NULL
    );

    return nwdaf_data_local_var;
end:
    if (nwdaf_eventsList) {
        OpenAPI_list_for_each(nwdaf_eventsList, node) {
            OpenAPI_nwdaf_event_free(node->data);
        }
        OpenAPI_list_free(nwdaf_eventsList);
        nwdaf_eventsList = NULL;
    }
    return NULL;
}

OpenAPI_nwdaf_data_t *OpenAPI_nwdaf_data_copy(OpenAPI_nwdaf_data_t *dst, OpenAPI_nwdaf_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nwdaf_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nwdaf_data_convertToJSON() failed");
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

    OpenAPI_nwdaf_data_free(dst);
    dst = OpenAPI_nwdaf_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

