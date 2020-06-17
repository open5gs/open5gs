
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nwdaf_info.h"

OpenAPI_nwdaf_info_t *OpenAPI_nwdaf_info_create(
    OpenAPI_list_t *event_ids,
    OpenAPI_list_t *nwdaf_events,
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *tai_range_list
    )
{
    OpenAPI_nwdaf_info_t *nwdaf_info_local_var = OpenAPI_malloc(sizeof(OpenAPI_nwdaf_info_t));
    if (!nwdaf_info_local_var) {
        return NULL;
    }
    nwdaf_info_local_var->event_ids = event_ids;
    nwdaf_info_local_var->nwdaf_events = nwdaf_events;
    nwdaf_info_local_var->tai_list = tai_list;
    nwdaf_info_local_var->tai_range_list = tai_range_list;

    return nwdaf_info_local_var;
}

void OpenAPI_nwdaf_info_free(OpenAPI_nwdaf_info_t *nwdaf_info)
{
    if (NULL == nwdaf_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(nwdaf_info->event_ids, node) {
        OpenAPI_event_id_free(node->data);
    }
    OpenAPI_list_free(nwdaf_info->event_ids);
    OpenAPI_list_for_each(nwdaf_info->nwdaf_events, node) {
        OpenAPI_nwdaf_event_free(node->data);
    }
    OpenAPI_list_free(nwdaf_info->nwdaf_events);
    OpenAPI_list_for_each(nwdaf_info->tai_list, node) {
        OpenAPI_tai_free(node->data);
    }
    OpenAPI_list_free(nwdaf_info->tai_list);
    OpenAPI_list_for_each(nwdaf_info->tai_range_list, node) {
        OpenAPI_tai_range_free(node->data);
    }
    OpenAPI_list_free(nwdaf_info->tai_range_list);
    ogs_free(nwdaf_info);
}

cJSON *OpenAPI_nwdaf_info_convertToJSON(OpenAPI_nwdaf_info_t *nwdaf_info)
{
    cJSON *item = NULL;

    if (nwdaf_info == NULL) {
        ogs_error("OpenAPI_nwdaf_info_convertToJSON() failed [NwdafInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (nwdaf_info->event_ids) {
        cJSON *event_idsList = cJSON_AddArrayToObject(item, "eventIds");
        if (event_idsList == NULL) {
            ogs_error("OpenAPI_nwdaf_info_convertToJSON() failed [event_ids]");
            goto end;
        }

        OpenAPI_lnode_t *event_ids_node;
        if (nwdaf_info->event_ids) {
            OpenAPI_list_for_each(nwdaf_info->event_ids, event_ids_node) {
                cJSON *itemLocal = OpenAPI_event_id_convertToJSON(event_ids_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_nwdaf_info_convertToJSON() failed [event_ids]");
                    goto end;
                }
                cJSON_AddItemToArray(event_idsList, itemLocal);
            }
        }
    }

    if (nwdaf_info->nwdaf_events) {
        cJSON *nwdaf_eventsList = cJSON_AddArrayToObject(item, "nwdafEvents");
        if (nwdaf_eventsList == NULL) {
            ogs_error("OpenAPI_nwdaf_info_convertToJSON() failed [nwdaf_events]");
            goto end;
        }

        OpenAPI_lnode_t *nwdaf_events_node;
        if (nwdaf_info->nwdaf_events) {
            OpenAPI_list_for_each(nwdaf_info->nwdaf_events, nwdaf_events_node) {
                cJSON *itemLocal = OpenAPI_nwdaf_event_convertToJSON(nwdaf_events_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_nwdaf_info_convertToJSON() failed [nwdaf_events]");
                    goto end;
                }
                cJSON_AddItemToArray(nwdaf_eventsList, itemLocal);
            }
        }
    }

    if (nwdaf_info->tai_list) {
        cJSON *tai_listList = cJSON_AddArrayToObject(item, "taiList");
        if (tai_listList == NULL) {
            ogs_error("OpenAPI_nwdaf_info_convertToJSON() failed [tai_list]");
            goto end;
        }

        OpenAPI_lnode_t *tai_list_node;
        if (nwdaf_info->tai_list) {
            OpenAPI_list_for_each(nwdaf_info->tai_list, tai_list_node) {
                cJSON *itemLocal = OpenAPI_tai_convertToJSON(tai_list_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_nwdaf_info_convertToJSON() failed [tai_list]");
                    goto end;
                }
                cJSON_AddItemToArray(tai_listList, itemLocal);
            }
        }
    }

    if (nwdaf_info->tai_range_list) {
        cJSON *tai_range_listList = cJSON_AddArrayToObject(item, "taiRangeList");
        if (tai_range_listList == NULL) {
            ogs_error("OpenAPI_nwdaf_info_convertToJSON() failed [tai_range_list]");
            goto end;
        }

        OpenAPI_lnode_t *tai_range_list_node;
        if (nwdaf_info->tai_range_list) {
            OpenAPI_list_for_each(nwdaf_info->tai_range_list, tai_range_list_node) {
                cJSON *itemLocal = OpenAPI_tai_range_convertToJSON(tai_range_list_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_nwdaf_info_convertToJSON() failed [tai_range_list]");
                    goto end;
                }
                cJSON_AddItemToArray(tai_range_listList, itemLocal);
            }
        }
    }

end:
    return item;
}

OpenAPI_nwdaf_info_t *OpenAPI_nwdaf_info_parseFromJSON(cJSON *nwdaf_infoJSON)
{
    OpenAPI_nwdaf_info_t *nwdaf_info_local_var = NULL;
    cJSON *event_ids = cJSON_GetObjectItemCaseSensitive(nwdaf_infoJSON, "eventIds");

    OpenAPI_list_t *event_idsList;
    if (event_ids) {
        cJSON *event_ids_local_nonprimitive;
        if (!cJSON_IsArray(event_ids)) {
            ogs_error("OpenAPI_nwdaf_info_parseFromJSON() failed [event_ids]");
            goto end;
        }

        event_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(event_ids_local_nonprimitive, event_ids ) {
            if (!cJSON_IsObject(event_ids_local_nonprimitive)) {
                ogs_error("OpenAPI_nwdaf_info_parseFromJSON() failed [event_ids]");
                goto end;
            }
            OpenAPI_event_id_t *event_idsItem = OpenAPI_event_id_parseFromJSON(event_ids_local_nonprimitive);

            OpenAPI_list_add(event_idsList, event_idsItem);
        }
    }

    cJSON *nwdaf_events = cJSON_GetObjectItemCaseSensitive(nwdaf_infoJSON, "nwdafEvents");

    OpenAPI_list_t *nwdaf_eventsList;
    if (nwdaf_events) {
        cJSON *nwdaf_events_local_nonprimitive;
        if (!cJSON_IsArray(nwdaf_events)) {
            ogs_error("OpenAPI_nwdaf_info_parseFromJSON() failed [nwdaf_events]");
            goto end;
        }

        nwdaf_eventsList = OpenAPI_list_create();

        cJSON_ArrayForEach(nwdaf_events_local_nonprimitive, nwdaf_events ) {
            if (!cJSON_IsObject(nwdaf_events_local_nonprimitive)) {
                ogs_error("OpenAPI_nwdaf_info_parseFromJSON() failed [nwdaf_events]");
                goto end;
            }
            OpenAPI_nwdaf_event_t *nwdaf_eventsItem = OpenAPI_nwdaf_event_parseFromJSON(nwdaf_events_local_nonprimitive);

            OpenAPI_list_add(nwdaf_eventsList, nwdaf_eventsItem);
        }
    }

    cJSON *tai_list = cJSON_GetObjectItemCaseSensitive(nwdaf_infoJSON, "taiList");

    OpenAPI_list_t *tai_listList;
    if (tai_list) {
        cJSON *tai_list_local_nonprimitive;
        if (!cJSON_IsArray(tai_list)) {
            ogs_error("OpenAPI_nwdaf_info_parseFromJSON() failed [tai_list]");
            goto end;
        }

        tai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_list_local_nonprimitive, tai_list ) {
            if (!cJSON_IsObject(tai_list_local_nonprimitive)) {
                ogs_error("OpenAPI_nwdaf_info_parseFromJSON() failed [tai_list]");
                goto end;
            }
            OpenAPI_tai_t *tai_listItem = OpenAPI_tai_parseFromJSON(tai_list_local_nonprimitive);

            OpenAPI_list_add(tai_listList, tai_listItem);
        }
    }

    cJSON *tai_range_list = cJSON_GetObjectItemCaseSensitive(nwdaf_infoJSON, "taiRangeList");

    OpenAPI_list_t *tai_range_listList;
    if (tai_range_list) {
        cJSON *tai_range_list_local_nonprimitive;
        if (!cJSON_IsArray(tai_range_list)) {
            ogs_error("OpenAPI_nwdaf_info_parseFromJSON() failed [tai_range_list]");
            goto end;
        }

        tai_range_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_range_list_local_nonprimitive, tai_range_list ) {
            if (!cJSON_IsObject(tai_range_list_local_nonprimitive)) {
                ogs_error("OpenAPI_nwdaf_info_parseFromJSON() failed [tai_range_list]");
                goto end;
            }
            OpenAPI_tai_range_t *tai_range_listItem = OpenAPI_tai_range_parseFromJSON(tai_range_list_local_nonprimitive);

            OpenAPI_list_add(tai_range_listList, tai_range_listItem);
        }
    }

    nwdaf_info_local_var = OpenAPI_nwdaf_info_create (
        event_ids ? event_idsList : NULL,
        nwdaf_events ? nwdaf_eventsList : NULL,
        tai_list ? tai_listList : NULL,
        tai_range_list ? tai_range_listList : NULL
        );

    return nwdaf_info_local_var;
end:
    return NULL;
}

OpenAPI_nwdaf_info_t *OpenAPI_nwdaf_info_copy(OpenAPI_nwdaf_info_t *dst, OpenAPI_nwdaf_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nwdaf_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nwdaf_info_convertToJSON() failed");
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

    OpenAPI_nwdaf_info_free(dst);
    dst = OpenAPI_nwdaf_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

