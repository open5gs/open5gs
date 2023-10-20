
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nrf_info_served_nwdaf_info_value.h"

OpenAPI_nrf_info_served_nwdaf_info_value_t *OpenAPI_nrf_info_served_nwdaf_info_value_create(
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
    OpenAPI_nrf_info_served_nwdaf_info_value_t *nrf_info_served_nwdaf_info_value_local_var = ogs_malloc(sizeof(OpenAPI_nrf_info_served_nwdaf_info_value_t));
    ogs_assert(nrf_info_served_nwdaf_info_value_local_var);

    nrf_info_served_nwdaf_info_value_local_var->event_ids = event_ids;
    nrf_info_served_nwdaf_info_value_local_var->nwdaf_events = nwdaf_events;
    nrf_info_served_nwdaf_info_value_local_var->tai_list = tai_list;
    nrf_info_served_nwdaf_info_value_local_var->tai_range_list = tai_range_list;
    nrf_info_served_nwdaf_info_value_local_var->nwdaf_capability = nwdaf_capability;
    nrf_info_served_nwdaf_info_value_local_var->is_analytics_delay = is_analytics_delay;
    nrf_info_served_nwdaf_info_value_local_var->analytics_delay = analytics_delay;
    nrf_info_served_nwdaf_info_value_local_var->serving_nf_set_id_list = serving_nf_set_id_list;
    nrf_info_served_nwdaf_info_value_local_var->serving_nf_type_list = serving_nf_type_list;
    nrf_info_served_nwdaf_info_value_local_var->ml_analytics_list = ml_analytics_list;

    return nrf_info_served_nwdaf_info_value_local_var;
}

void OpenAPI_nrf_info_served_nwdaf_info_value_free(OpenAPI_nrf_info_served_nwdaf_info_value_t *nrf_info_served_nwdaf_info_value)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nrf_info_served_nwdaf_info_value) {
        return;
    }
    if (nrf_info_served_nwdaf_info_value->event_ids) {
        OpenAPI_list_for_each(nrf_info_served_nwdaf_info_value->event_ids, node) {
            OpenAPI_event_id_free(node->data);
        }
        OpenAPI_list_free(nrf_info_served_nwdaf_info_value->event_ids);
        nrf_info_served_nwdaf_info_value->event_ids = NULL;
    }
    if (nrf_info_served_nwdaf_info_value->nwdaf_events) {
        OpenAPI_list_for_each(nrf_info_served_nwdaf_info_value->nwdaf_events, node) {
            OpenAPI_nwdaf_event_free(node->data);
        }
        OpenAPI_list_free(nrf_info_served_nwdaf_info_value->nwdaf_events);
        nrf_info_served_nwdaf_info_value->nwdaf_events = NULL;
    }
    if (nrf_info_served_nwdaf_info_value->tai_list) {
        OpenAPI_list_for_each(nrf_info_served_nwdaf_info_value->tai_list, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(nrf_info_served_nwdaf_info_value->tai_list);
        nrf_info_served_nwdaf_info_value->tai_list = NULL;
    }
    if (nrf_info_served_nwdaf_info_value->tai_range_list) {
        OpenAPI_list_for_each(nrf_info_served_nwdaf_info_value->tai_range_list, node) {
            OpenAPI_tai_range_free(node->data);
        }
        OpenAPI_list_free(nrf_info_served_nwdaf_info_value->tai_range_list);
        nrf_info_served_nwdaf_info_value->tai_range_list = NULL;
    }
    if (nrf_info_served_nwdaf_info_value->nwdaf_capability) {
        OpenAPI_nwdaf_capability_free(nrf_info_served_nwdaf_info_value->nwdaf_capability);
        nrf_info_served_nwdaf_info_value->nwdaf_capability = NULL;
    }
    if (nrf_info_served_nwdaf_info_value->serving_nf_set_id_list) {
        OpenAPI_list_for_each(nrf_info_served_nwdaf_info_value->serving_nf_set_id_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(nrf_info_served_nwdaf_info_value->serving_nf_set_id_list);
        nrf_info_served_nwdaf_info_value->serving_nf_set_id_list = NULL;
    }
    if (nrf_info_served_nwdaf_info_value->serving_nf_type_list) {
        OpenAPI_list_free(nrf_info_served_nwdaf_info_value->serving_nf_type_list);
        nrf_info_served_nwdaf_info_value->serving_nf_type_list = NULL;
    }
    if (nrf_info_served_nwdaf_info_value->ml_analytics_list) {
        OpenAPI_list_for_each(nrf_info_served_nwdaf_info_value->ml_analytics_list, node) {
            OpenAPI_ml_analytics_info_free(node->data);
        }
        OpenAPI_list_free(nrf_info_served_nwdaf_info_value->ml_analytics_list);
        nrf_info_served_nwdaf_info_value->ml_analytics_list = NULL;
    }
    ogs_free(nrf_info_served_nwdaf_info_value);
}

cJSON *OpenAPI_nrf_info_served_nwdaf_info_value_convertToJSON(OpenAPI_nrf_info_served_nwdaf_info_value_t *nrf_info_served_nwdaf_info_value)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nrf_info_served_nwdaf_info_value == NULL) {
        ogs_error("OpenAPI_nrf_info_served_nwdaf_info_value_convertToJSON() failed [NrfInfo_servedNwdafInfo_value]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (nrf_info_served_nwdaf_info_value->event_ids) {
    cJSON *event_idsList = cJSON_AddArrayToObject(item, "eventIds");
    if (event_idsList == NULL) {
        ogs_error("OpenAPI_nrf_info_served_nwdaf_info_value_convertToJSON() failed [event_ids]");
        goto end;
    }
    OpenAPI_list_for_each(nrf_info_served_nwdaf_info_value->event_ids, node) {
        cJSON *itemLocal = OpenAPI_event_id_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nrf_info_served_nwdaf_info_value_convertToJSON() failed [event_ids]");
            goto end;
        }
        cJSON_AddItemToArray(event_idsList, itemLocal);
    }
    }

    if (nrf_info_served_nwdaf_info_value->nwdaf_events) {
    cJSON *nwdaf_eventsList = cJSON_AddArrayToObject(item, "nwdafEvents");
    if (nwdaf_eventsList == NULL) {
        ogs_error("OpenAPI_nrf_info_served_nwdaf_info_value_convertToJSON() failed [nwdaf_events]");
        goto end;
    }
    OpenAPI_list_for_each(nrf_info_served_nwdaf_info_value->nwdaf_events, node) {
        cJSON *itemLocal = OpenAPI_nwdaf_event_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nrf_info_served_nwdaf_info_value_convertToJSON() failed [nwdaf_events]");
            goto end;
        }
        cJSON_AddItemToArray(nwdaf_eventsList, itemLocal);
    }
    }

    if (nrf_info_served_nwdaf_info_value->tai_list) {
    cJSON *tai_listList = cJSON_AddArrayToObject(item, "taiList");
    if (tai_listList == NULL) {
        ogs_error("OpenAPI_nrf_info_served_nwdaf_info_value_convertToJSON() failed [tai_list]");
        goto end;
    }
    OpenAPI_list_for_each(nrf_info_served_nwdaf_info_value->tai_list, node) {
        cJSON *itemLocal = OpenAPI_tai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nrf_info_served_nwdaf_info_value_convertToJSON() failed [tai_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_listList, itemLocal);
    }
    }

    if (nrf_info_served_nwdaf_info_value->tai_range_list) {
    cJSON *tai_range_listList = cJSON_AddArrayToObject(item, "taiRangeList");
    if (tai_range_listList == NULL) {
        ogs_error("OpenAPI_nrf_info_served_nwdaf_info_value_convertToJSON() failed [tai_range_list]");
        goto end;
    }
    OpenAPI_list_for_each(nrf_info_served_nwdaf_info_value->tai_range_list, node) {
        cJSON *itemLocal = OpenAPI_tai_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nrf_info_served_nwdaf_info_value_convertToJSON() failed [tai_range_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_range_listList, itemLocal);
    }
    }

    if (nrf_info_served_nwdaf_info_value->nwdaf_capability) {
    cJSON *nwdaf_capability_local_JSON = OpenAPI_nwdaf_capability_convertToJSON(nrf_info_served_nwdaf_info_value->nwdaf_capability);
    if (nwdaf_capability_local_JSON == NULL) {
        ogs_error("OpenAPI_nrf_info_served_nwdaf_info_value_convertToJSON() failed [nwdaf_capability]");
        goto end;
    }
    cJSON_AddItemToObject(item, "nwdafCapability", nwdaf_capability_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_nrf_info_served_nwdaf_info_value_convertToJSON() failed [nwdaf_capability]");
        goto end;
    }
    }

    if (nrf_info_served_nwdaf_info_value->is_analytics_delay) {
    if (cJSON_AddNumberToObject(item, "analyticsDelay", nrf_info_served_nwdaf_info_value->analytics_delay) == NULL) {
        ogs_error("OpenAPI_nrf_info_served_nwdaf_info_value_convertToJSON() failed [analytics_delay]");
        goto end;
    }
    }

    if (nrf_info_served_nwdaf_info_value->serving_nf_set_id_list) {
    cJSON *serving_nf_set_id_listList = cJSON_AddArrayToObject(item, "servingNfSetIdList");
    if (serving_nf_set_id_listList == NULL) {
        ogs_error("OpenAPI_nrf_info_served_nwdaf_info_value_convertToJSON() failed [serving_nf_set_id_list]");
        goto end;
    }
    OpenAPI_list_for_each(nrf_info_served_nwdaf_info_value->serving_nf_set_id_list, node) {
        if (cJSON_AddStringToObject(serving_nf_set_id_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_nrf_info_served_nwdaf_info_value_convertToJSON() failed [serving_nf_set_id_list]");
            goto end;
        }
    }
    }

    if (nrf_info_served_nwdaf_info_value->serving_nf_type_list != OpenAPI_nf_type_NULL) {
    cJSON *serving_nf_type_listList = cJSON_AddArrayToObject(item, "servingNfTypeList");
    if (serving_nf_type_listList == NULL) {
        ogs_error("OpenAPI_nrf_info_served_nwdaf_info_value_convertToJSON() failed [serving_nf_type_list]");
        goto end;
    }
    OpenAPI_list_for_each(nrf_info_served_nwdaf_info_value->serving_nf_type_list, node) {
        if (cJSON_AddStringToObject(serving_nf_type_listList, "", OpenAPI_nf_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_nrf_info_served_nwdaf_info_value_convertToJSON() failed [serving_nf_type_list]");
            goto end;
        }
    }
    }

    if (nrf_info_served_nwdaf_info_value->ml_analytics_list) {
    cJSON *ml_analytics_listList = cJSON_AddArrayToObject(item, "mlAnalyticsList");
    if (ml_analytics_listList == NULL) {
        ogs_error("OpenAPI_nrf_info_served_nwdaf_info_value_convertToJSON() failed [ml_analytics_list]");
        goto end;
    }
    OpenAPI_list_for_each(nrf_info_served_nwdaf_info_value->ml_analytics_list, node) {
        cJSON *itemLocal = OpenAPI_ml_analytics_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nrf_info_served_nwdaf_info_value_convertToJSON() failed [ml_analytics_list]");
            goto end;
        }
        cJSON_AddItemToArray(ml_analytics_listList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_nrf_info_served_nwdaf_info_value_t *OpenAPI_nrf_info_served_nwdaf_info_value_parseFromJSON(cJSON *nrf_info_served_nwdaf_info_valueJSON)
{
    OpenAPI_nrf_info_served_nwdaf_info_value_t *nrf_info_served_nwdaf_info_value_local_var = NULL;
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
    event_ids = cJSON_GetObjectItemCaseSensitive(nrf_info_served_nwdaf_info_valueJSON, "eventIds");
    if (event_ids) {
        cJSON *event_ids_local = NULL;
        if (!cJSON_IsArray(event_ids)) {
            ogs_error("OpenAPI_nrf_info_served_nwdaf_info_value_parseFromJSON() failed [event_ids]");
            goto end;
        }

        event_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(event_ids_local, event_ids) {
            if (!cJSON_IsObject(event_ids_local)) {
                ogs_error("OpenAPI_nrf_info_served_nwdaf_info_value_parseFromJSON() failed [event_ids]");
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

    nwdaf_events = cJSON_GetObjectItemCaseSensitive(nrf_info_served_nwdaf_info_valueJSON, "nwdafEvents");
    if (nwdaf_events) {
        cJSON *nwdaf_events_local = NULL;
        if (!cJSON_IsArray(nwdaf_events)) {
            ogs_error("OpenAPI_nrf_info_served_nwdaf_info_value_parseFromJSON() failed [nwdaf_events]");
            goto end;
        }

        nwdaf_eventsList = OpenAPI_list_create();

        cJSON_ArrayForEach(nwdaf_events_local, nwdaf_events) {
            if (!cJSON_IsObject(nwdaf_events_local)) {
                ogs_error("OpenAPI_nrf_info_served_nwdaf_info_value_parseFromJSON() failed [nwdaf_events]");
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

    tai_list = cJSON_GetObjectItemCaseSensitive(nrf_info_served_nwdaf_info_valueJSON, "taiList");
    if (tai_list) {
        cJSON *tai_list_local = NULL;
        if (!cJSON_IsArray(tai_list)) {
            ogs_error("OpenAPI_nrf_info_served_nwdaf_info_value_parseFromJSON() failed [tai_list]");
            goto end;
        }

        tai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_list_local, tai_list) {
            if (!cJSON_IsObject(tai_list_local)) {
                ogs_error("OpenAPI_nrf_info_served_nwdaf_info_value_parseFromJSON() failed [tai_list]");
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

    tai_range_list = cJSON_GetObjectItemCaseSensitive(nrf_info_served_nwdaf_info_valueJSON, "taiRangeList");
    if (tai_range_list) {
        cJSON *tai_range_list_local = NULL;
        if (!cJSON_IsArray(tai_range_list)) {
            ogs_error("OpenAPI_nrf_info_served_nwdaf_info_value_parseFromJSON() failed [tai_range_list]");
            goto end;
        }

        tai_range_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_range_list_local, tai_range_list) {
            if (!cJSON_IsObject(tai_range_list_local)) {
                ogs_error("OpenAPI_nrf_info_served_nwdaf_info_value_parseFromJSON() failed [tai_range_list]");
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

    nwdaf_capability = cJSON_GetObjectItemCaseSensitive(nrf_info_served_nwdaf_info_valueJSON, "nwdafCapability");
    if (nwdaf_capability) {
    nwdaf_capability_local_nonprim = OpenAPI_nwdaf_capability_parseFromJSON(nwdaf_capability);
    if (!nwdaf_capability_local_nonprim) {
        ogs_error("OpenAPI_nwdaf_capability_parseFromJSON failed [nwdaf_capability]");
        goto end;
    }
    }

    analytics_delay = cJSON_GetObjectItemCaseSensitive(nrf_info_served_nwdaf_info_valueJSON, "analyticsDelay");
    if (analytics_delay) {
    if (!cJSON_IsNumber(analytics_delay)) {
        ogs_error("OpenAPI_nrf_info_served_nwdaf_info_value_parseFromJSON() failed [analytics_delay]");
        goto end;
    }
    }

    serving_nf_set_id_list = cJSON_GetObjectItemCaseSensitive(nrf_info_served_nwdaf_info_valueJSON, "servingNfSetIdList");
    if (serving_nf_set_id_list) {
        cJSON *serving_nf_set_id_list_local = NULL;
        if (!cJSON_IsArray(serving_nf_set_id_list)) {
            ogs_error("OpenAPI_nrf_info_served_nwdaf_info_value_parseFromJSON() failed [serving_nf_set_id_list]");
            goto end;
        }

        serving_nf_set_id_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(serving_nf_set_id_list_local, serving_nf_set_id_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(serving_nf_set_id_list_local)) {
                ogs_error("OpenAPI_nrf_info_served_nwdaf_info_value_parseFromJSON() failed [serving_nf_set_id_list]");
                goto end;
            }
            OpenAPI_list_add(serving_nf_set_id_listList, ogs_strdup(serving_nf_set_id_list_local->valuestring));
        }
    }

    serving_nf_type_list = cJSON_GetObjectItemCaseSensitive(nrf_info_served_nwdaf_info_valueJSON, "servingNfTypeList");
    if (serving_nf_type_list) {
        cJSON *serving_nf_type_list_local = NULL;
        if (!cJSON_IsArray(serving_nf_type_list)) {
            ogs_error("OpenAPI_nrf_info_served_nwdaf_info_value_parseFromJSON() failed [serving_nf_type_list]");
            goto end;
        }

        serving_nf_type_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(serving_nf_type_list_local, serving_nf_type_list) {
            OpenAPI_nf_type_e localEnum = OpenAPI_nf_type_NULL;
            if (!cJSON_IsString(serving_nf_type_list_local)) {
                ogs_error("OpenAPI_nrf_info_served_nwdaf_info_value_parseFromJSON() failed [serving_nf_type_list]");
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
            ogs_error("OpenAPI_nrf_info_served_nwdaf_info_value_parseFromJSON() failed: Expected serving_nf_type_listList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    ml_analytics_list = cJSON_GetObjectItemCaseSensitive(nrf_info_served_nwdaf_info_valueJSON, "mlAnalyticsList");
    if (ml_analytics_list) {
        cJSON *ml_analytics_list_local = NULL;
        if (!cJSON_IsArray(ml_analytics_list)) {
            ogs_error("OpenAPI_nrf_info_served_nwdaf_info_value_parseFromJSON() failed [ml_analytics_list]");
            goto end;
        }

        ml_analytics_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(ml_analytics_list_local, ml_analytics_list) {
            if (!cJSON_IsObject(ml_analytics_list_local)) {
                ogs_error("OpenAPI_nrf_info_served_nwdaf_info_value_parseFromJSON() failed [ml_analytics_list]");
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

    nrf_info_served_nwdaf_info_value_local_var = OpenAPI_nrf_info_served_nwdaf_info_value_create (
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

    return nrf_info_served_nwdaf_info_value_local_var;
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

OpenAPI_nrf_info_served_nwdaf_info_value_t *OpenAPI_nrf_info_served_nwdaf_info_value_copy(OpenAPI_nrf_info_served_nwdaf_info_value_t *dst, OpenAPI_nrf_info_served_nwdaf_info_value_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nrf_info_served_nwdaf_info_value_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nrf_info_served_nwdaf_info_value_convertToJSON() failed");
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

    OpenAPI_nrf_info_served_nwdaf_info_value_free(dst);
    dst = OpenAPI_nrf_info_served_nwdaf_info_value_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

