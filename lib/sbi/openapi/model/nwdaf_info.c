
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nwdaf_info.h"

OpenAPI_nwdaf_info_t *OpenAPI_nwdaf_info_create(
    OpenAPI_list_t *event_ids,
    OpenAPI_list_t *nwdaf_events,
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *tai_range_list,
    OpenAPI_nwdaf_capability_t *nwdaf_capability,
    bool is_analytics_delay,
    int analytics_delay,
    OpenAPI_list_t *serving_nf_set_id_list,
    OpenAPI_list_t *serving_nf_type_list,
    OpenAPI_list_t *ml_analytics_list
)
{
    OpenAPI_nwdaf_info_t *nwdaf_info_local_var = ogs_malloc(sizeof(OpenAPI_nwdaf_info_t));
    ogs_assert(nwdaf_info_local_var);

    nwdaf_info_local_var->event_ids = event_ids;
    nwdaf_info_local_var->nwdaf_events = nwdaf_events;
    nwdaf_info_local_var->tai_list = tai_list;
    nwdaf_info_local_var->tai_range_list = tai_range_list;
    nwdaf_info_local_var->nwdaf_capability = nwdaf_capability;
    nwdaf_info_local_var->is_analytics_delay = is_analytics_delay;
    nwdaf_info_local_var->analytics_delay = analytics_delay;
    nwdaf_info_local_var->serving_nf_set_id_list = serving_nf_set_id_list;
    nwdaf_info_local_var->serving_nf_type_list = serving_nf_type_list;
    nwdaf_info_local_var->ml_analytics_list = ml_analytics_list;

    return nwdaf_info_local_var;
}

void OpenAPI_nwdaf_info_free(OpenAPI_nwdaf_info_t *nwdaf_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nwdaf_info) {
        return;
    }
    if (nwdaf_info->event_ids) {
        OpenAPI_list_for_each(nwdaf_info->event_ids, node) {
            OpenAPI_event_id_free(node->data);
        }
        OpenAPI_list_free(nwdaf_info->event_ids);
        nwdaf_info->event_ids = NULL;
    }
    if (nwdaf_info->nwdaf_events) {
        OpenAPI_list_for_each(nwdaf_info->nwdaf_events, node) {
            OpenAPI_nwdaf_event_free(node->data);
        }
        OpenAPI_list_free(nwdaf_info->nwdaf_events);
        nwdaf_info->nwdaf_events = NULL;
    }
    if (nwdaf_info->tai_list) {
        OpenAPI_list_for_each(nwdaf_info->tai_list, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(nwdaf_info->tai_list);
        nwdaf_info->tai_list = NULL;
    }
    if (nwdaf_info->tai_range_list) {
        OpenAPI_list_for_each(nwdaf_info->tai_range_list, node) {
            OpenAPI_tai_range_free(node->data);
        }
        OpenAPI_list_free(nwdaf_info->tai_range_list);
        nwdaf_info->tai_range_list = NULL;
    }
    if (nwdaf_info->nwdaf_capability) {
        OpenAPI_nwdaf_capability_free(nwdaf_info->nwdaf_capability);
        nwdaf_info->nwdaf_capability = NULL;
    }
    if (nwdaf_info->serving_nf_set_id_list) {
        OpenAPI_list_for_each(nwdaf_info->serving_nf_set_id_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(nwdaf_info->serving_nf_set_id_list);
        nwdaf_info->serving_nf_set_id_list = NULL;
    }
    if (nwdaf_info->serving_nf_type_list) {
        OpenAPI_list_free(nwdaf_info->serving_nf_type_list);
        nwdaf_info->serving_nf_type_list = NULL;
    }
    if (nwdaf_info->ml_analytics_list) {
        OpenAPI_list_for_each(nwdaf_info->ml_analytics_list, node) {
            OpenAPI_ml_analytics_info_free(node->data);
        }
        OpenAPI_list_free(nwdaf_info->ml_analytics_list);
        nwdaf_info->ml_analytics_list = NULL;
    }
    ogs_free(nwdaf_info);
}

cJSON *OpenAPI_nwdaf_info_convertToJSON(OpenAPI_nwdaf_info_t *nwdaf_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

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
    OpenAPI_list_for_each(nwdaf_info->event_ids, node) {
        cJSON *itemLocal = OpenAPI_event_id_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nwdaf_info_convertToJSON() failed [event_ids]");
            goto end;
        }
        cJSON_AddItemToArray(event_idsList, itemLocal);
    }
    }

    if (nwdaf_info->nwdaf_events) {
    cJSON *nwdaf_eventsList = cJSON_AddArrayToObject(item, "nwdafEvents");
    if (nwdaf_eventsList == NULL) {
        ogs_error("OpenAPI_nwdaf_info_convertToJSON() failed [nwdaf_events]");
        goto end;
    }
    OpenAPI_list_for_each(nwdaf_info->nwdaf_events, node) {
        cJSON *itemLocal = OpenAPI_nwdaf_event_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nwdaf_info_convertToJSON() failed [nwdaf_events]");
            goto end;
        }
        cJSON_AddItemToArray(nwdaf_eventsList, itemLocal);
    }
    }

    if (nwdaf_info->tai_list) {
    cJSON *tai_listList = cJSON_AddArrayToObject(item, "taiList");
    if (tai_listList == NULL) {
        ogs_error("OpenAPI_nwdaf_info_convertToJSON() failed [tai_list]");
        goto end;
    }
    OpenAPI_list_for_each(nwdaf_info->tai_list, node) {
        cJSON *itemLocal = OpenAPI_tai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nwdaf_info_convertToJSON() failed [tai_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_listList, itemLocal);
    }
    }

    if (nwdaf_info->tai_range_list) {
    cJSON *tai_range_listList = cJSON_AddArrayToObject(item, "taiRangeList");
    if (tai_range_listList == NULL) {
        ogs_error("OpenAPI_nwdaf_info_convertToJSON() failed [tai_range_list]");
        goto end;
    }
    OpenAPI_list_for_each(nwdaf_info->tai_range_list, node) {
        cJSON *itemLocal = OpenAPI_tai_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nwdaf_info_convertToJSON() failed [tai_range_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_range_listList, itemLocal);
    }
    }

    if (nwdaf_info->nwdaf_capability) {
    cJSON *nwdaf_capability_local_JSON = OpenAPI_nwdaf_capability_convertToJSON(nwdaf_info->nwdaf_capability);
    if (nwdaf_capability_local_JSON == NULL) {
        ogs_error("OpenAPI_nwdaf_info_convertToJSON() failed [nwdaf_capability]");
        goto end;
    }
    cJSON_AddItemToObject(item, "nwdafCapability", nwdaf_capability_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_nwdaf_info_convertToJSON() failed [nwdaf_capability]");
        goto end;
    }
    }

    if (nwdaf_info->is_analytics_delay) {
    if (cJSON_AddNumberToObject(item, "analyticsDelay", nwdaf_info->analytics_delay) == NULL) {
        ogs_error("OpenAPI_nwdaf_info_convertToJSON() failed [analytics_delay]");
        goto end;
    }
    }

    if (nwdaf_info->serving_nf_set_id_list) {
    cJSON *serving_nf_set_id_listList = cJSON_AddArrayToObject(item, "servingNfSetIdList");
    if (serving_nf_set_id_listList == NULL) {
        ogs_error("OpenAPI_nwdaf_info_convertToJSON() failed [serving_nf_set_id_list]");
        goto end;
    }
    OpenAPI_list_for_each(nwdaf_info->serving_nf_set_id_list, node) {
        if (cJSON_AddStringToObject(serving_nf_set_id_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_nwdaf_info_convertToJSON() failed [serving_nf_set_id_list]");
            goto end;
        }
    }
    }

    if (nwdaf_info->serving_nf_type_list != OpenAPI_nf_type_NULL) {
    cJSON *serving_nf_type_listList = cJSON_AddArrayToObject(item, "servingNfTypeList");
    if (serving_nf_type_listList == NULL) {
        ogs_error("OpenAPI_nwdaf_info_convertToJSON() failed [serving_nf_type_list]");
        goto end;
    }
    OpenAPI_list_for_each(nwdaf_info->serving_nf_type_list, node) {
        if (cJSON_AddStringToObject(serving_nf_type_listList, "", OpenAPI_nf_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_nwdaf_info_convertToJSON() failed [serving_nf_type_list]");
            goto end;
        }
    }
    }

    if (nwdaf_info->ml_analytics_list) {
    cJSON *ml_analytics_listList = cJSON_AddArrayToObject(item, "mlAnalyticsList");
    if (ml_analytics_listList == NULL) {
        ogs_error("OpenAPI_nwdaf_info_convertToJSON() failed [ml_analytics_list]");
        goto end;
    }
    OpenAPI_list_for_each(nwdaf_info->ml_analytics_list, node) {
        cJSON *itemLocal = OpenAPI_ml_analytics_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nwdaf_info_convertToJSON() failed [ml_analytics_list]");
            goto end;
        }
        cJSON_AddItemToArray(ml_analytics_listList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_nwdaf_info_t *OpenAPI_nwdaf_info_parseFromJSON(cJSON *nwdaf_infoJSON)
{
    OpenAPI_nwdaf_info_t *nwdaf_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *event_ids = NULL;
    OpenAPI_list_t *event_idsList = NULL;
    cJSON *nwdaf_events = NULL;
    OpenAPI_list_t *nwdaf_eventsList = NULL;
    cJSON *tai_list = NULL;
    OpenAPI_list_t *tai_listList = NULL;
    cJSON *tai_range_list = NULL;
    OpenAPI_list_t *tai_range_listList = NULL;
    cJSON *nwdaf_capability = NULL;
    OpenAPI_nwdaf_capability_t *nwdaf_capability_local_nonprim = NULL;
    cJSON *analytics_delay = NULL;
    cJSON *serving_nf_set_id_list = NULL;
    OpenAPI_list_t *serving_nf_set_id_listList = NULL;
    cJSON *serving_nf_type_list = NULL;
    OpenAPI_list_t *serving_nf_type_listList = NULL;
    cJSON *ml_analytics_list = NULL;
    OpenAPI_list_t *ml_analytics_listList = NULL;
    event_ids = cJSON_GetObjectItemCaseSensitive(nwdaf_infoJSON, "eventIds");
    if (event_ids) {
        cJSON *event_ids_local = NULL;
        if (!cJSON_IsArray(event_ids)) {
            ogs_error("OpenAPI_nwdaf_info_parseFromJSON() failed [event_ids]");
            goto end;
        }

        event_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(event_ids_local, event_ids) {
            if (!cJSON_IsObject(event_ids_local)) {
                ogs_error("OpenAPI_nwdaf_info_parseFromJSON() failed [event_ids]");
                goto end;
            }
            OpenAPI_event_id_t *event_idsItem = OpenAPI_event_id_parseFromJSON(event_ids_local);
            if (!event_idsItem) {
                ogs_error("No event_idsItem");
                goto end;
            }
            OpenAPI_list_add(event_idsList, event_idsItem);
        }
    }

    nwdaf_events = cJSON_GetObjectItemCaseSensitive(nwdaf_infoJSON, "nwdafEvents");
    if (nwdaf_events) {
        cJSON *nwdaf_events_local = NULL;
        if (!cJSON_IsArray(nwdaf_events)) {
            ogs_error("OpenAPI_nwdaf_info_parseFromJSON() failed [nwdaf_events]");
            goto end;
        }

        nwdaf_eventsList = OpenAPI_list_create();

        cJSON_ArrayForEach(nwdaf_events_local, nwdaf_events) {
            if (!cJSON_IsObject(nwdaf_events_local)) {
                ogs_error("OpenAPI_nwdaf_info_parseFromJSON() failed [nwdaf_events]");
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

    tai_list = cJSON_GetObjectItemCaseSensitive(nwdaf_infoJSON, "taiList");
    if (tai_list) {
        cJSON *tai_list_local = NULL;
        if (!cJSON_IsArray(tai_list)) {
            ogs_error("OpenAPI_nwdaf_info_parseFromJSON() failed [tai_list]");
            goto end;
        }

        tai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_list_local, tai_list) {
            if (!cJSON_IsObject(tai_list_local)) {
                ogs_error("OpenAPI_nwdaf_info_parseFromJSON() failed [tai_list]");
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

    tai_range_list = cJSON_GetObjectItemCaseSensitive(nwdaf_infoJSON, "taiRangeList");
    if (tai_range_list) {
        cJSON *tai_range_list_local = NULL;
        if (!cJSON_IsArray(tai_range_list)) {
            ogs_error("OpenAPI_nwdaf_info_parseFromJSON() failed [tai_range_list]");
            goto end;
        }

        tai_range_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_range_list_local, tai_range_list) {
            if (!cJSON_IsObject(tai_range_list_local)) {
                ogs_error("OpenAPI_nwdaf_info_parseFromJSON() failed [tai_range_list]");
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

    nwdaf_capability = cJSON_GetObjectItemCaseSensitive(nwdaf_infoJSON, "nwdafCapability");
    if (nwdaf_capability) {
    nwdaf_capability_local_nonprim = OpenAPI_nwdaf_capability_parseFromJSON(nwdaf_capability);
    if (!nwdaf_capability_local_nonprim) {
        ogs_error("OpenAPI_nwdaf_capability_parseFromJSON failed [nwdaf_capability]");
        goto end;
    }
    }

    analytics_delay = cJSON_GetObjectItemCaseSensitive(nwdaf_infoJSON, "analyticsDelay");
    if (analytics_delay) {
    if (!cJSON_IsNumber(analytics_delay)) {
        ogs_error("OpenAPI_nwdaf_info_parseFromJSON() failed [analytics_delay]");
        goto end;
    }
    }

    serving_nf_set_id_list = cJSON_GetObjectItemCaseSensitive(nwdaf_infoJSON, "servingNfSetIdList");
    if (serving_nf_set_id_list) {
        cJSON *serving_nf_set_id_list_local = NULL;
        if (!cJSON_IsArray(serving_nf_set_id_list)) {
            ogs_error("OpenAPI_nwdaf_info_parseFromJSON() failed [serving_nf_set_id_list]");
            goto end;
        }

        serving_nf_set_id_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(serving_nf_set_id_list_local, serving_nf_set_id_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(serving_nf_set_id_list_local)) {
                ogs_error("OpenAPI_nwdaf_info_parseFromJSON() failed [serving_nf_set_id_list]");
                goto end;
            }
            OpenAPI_list_add(serving_nf_set_id_listList, ogs_strdup(serving_nf_set_id_list_local->valuestring));
        }
    }

    serving_nf_type_list = cJSON_GetObjectItemCaseSensitive(nwdaf_infoJSON, "servingNfTypeList");
    if (serving_nf_type_list) {
        cJSON *serving_nf_type_list_local = NULL;
        if (!cJSON_IsArray(serving_nf_type_list)) {
            ogs_error("OpenAPI_nwdaf_info_parseFromJSON() failed [serving_nf_type_list]");
            goto end;
        }

        serving_nf_type_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(serving_nf_type_list_local, serving_nf_type_list) {
            OpenAPI_nf_type_e localEnum = OpenAPI_nf_type_NULL;
            if (!cJSON_IsString(serving_nf_type_list_local)) {
                ogs_error("OpenAPI_nwdaf_info_parseFromJSON() failed [serving_nf_type_list]");
                goto end;
            }
            localEnum = OpenAPI_nf_type_FromString(serving_nf_type_list_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"serving_nf_type_list\" is not supported. Ignoring it ...",
                         serving_nf_type_list_local->valuestring);
            } else {
                OpenAPI_list_add(serving_nf_type_listList, (void *)localEnum);
            }
        }
        if (serving_nf_type_listList->count == 0) {
            ogs_error("OpenAPI_nwdaf_info_parseFromJSON() failed: Expected serving_nf_type_listList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    ml_analytics_list = cJSON_GetObjectItemCaseSensitive(nwdaf_infoJSON, "mlAnalyticsList");
    if (ml_analytics_list) {
        cJSON *ml_analytics_list_local = NULL;
        if (!cJSON_IsArray(ml_analytics_list)) {
            ogs_error("OpenAPI_nwdaf_info_parseFromJSON() failed [ml_analytics_list]");
            goto end;
        }

        ml_analytics_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(ml_analytics_list_local, ml_analytics_list) {
            if (!cJSON_IsObject(ml_analytics_list_local)) {
                ogs_error("OpenAPI_nwdaf_info_parseFromJSON() failed [ml_analytics_list]");
                goto end;
            }
            OpenAPI_ml_analytics_info_t *ml_analytics_listItem = OpenAPI_ml_analytics_info_parseFromJSON(ml_analytics_list_local);
            if (!ml_analytics_listItem) {
                ogs_error("No ml_analytics_listItem");
                goto end;
            }
            OpenAPI_list_add(ml_analytics_listList, ml_analytics_listItem);
        }
    }

    nwdaf_info_local_var = OpenAPI_nwdaf_info_create (
        event_ids ? event_idsList : NULL,
        nwdaf_events ? nwdaf_eventsList : NULL,
        tai_list ? tai_listList : NULL,
        tai_range_list ? tai_range_listList : NULL,
        nwdaf_capability ? nwdaf_capability_local_nonprim : NULL,
        analytics_delay ? true : false,
        analytics_delay ? analytics_delay->valuedouble : 0,
        serving_nf_set_id_list ? serving_nf_set_id_listList : NULL,
        serving_nf_type_list ? serving_nf_type_listList : NULL,
        ml_analytics_list ? ml_analytics_listList : NULL
    );

    return nwdaf_info_local_var;
end:
    if (event_idsList) {
        OpenAPI_list_for_each(event_idsList, node) {
            OpenAPI_event_id_free(node->data);
        }
        OpenAPI_list_free(event_idsList);
        event_idsList = NULL;
    }
    if (nwdaf_eventsList) {
        OpenAPI_list_for_each(nwdaf_eventsList, node) {
            OpenAPI_nwdaf_event_free(node->data);
        }
        OpenAPI_list_free(nwdaf_eventsList);
        nwdaf_eventsList = NULL;
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
    if (nwdaf_capability_local_nonprim) {
        OpenAPI_nwdaf_capability_free(nwdaf_capability_local_nonprim);
        nwdaf_capability_local_nonprim = NULL;
    }
    if (serving_nf_set_id_listList) {
        OpenAPI_list_for_each(serving_nf_set_id_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(serving_nf_set_id_listList);
        serving_nf_set_id_listList = NULL;
    }
    if (serving_nf_type_listList) {
        OpenAPI_list_free(serving_nf_type_listList);
        serving_nf_type_listList = NULL;
    }
    if (ml_analytics_listList) {
        OpenAPI_list_for_each(ml_analytics_listList, node) {
            OpenAPI_ml_analytics_info_free(node->data);
        }
        OpenAPI_list_free(ml_analytics_listList);
        ml_analytics_listList = NULL;
    }
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

