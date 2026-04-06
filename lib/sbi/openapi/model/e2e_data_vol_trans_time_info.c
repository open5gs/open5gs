
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "e2e_data_vol_trans_time_info.h"

OpenAPI_e2e_data_vol_trans_time_info_t *OpenAPI_e2e_data_vol_trans_time_info_create(
    OpenAPI_list_t *e2e_data_vol_trans_times,
    OpenAPI_list_t *e2e_data_vol_trans_time_ue_lists,
    OpenAPI_list_t *geo_distr_infos,
    bool is_confidence,
    int confidence
)
{
    OpenAPI_e2e_data_vol_trans_time_info_t *e2e_data_vol_trans_time_info_local_var = ogs_malloc(sizeof(OpenAPI_e2e_data_vol_trans_time_info_t));
    ogs_assert(e2e_data_vol_trans_time_info_local_var);

    e2e_data_vol_trans_time_info_local_var->e2e_data_vol_trans_times = e2e_data_vol_trans_times;
    e2e_data_vol_trans_time_info_local_var->e2e_data_vol_trans_time_ue_lists = e2e_data_vol_trans_time_ue_lists;
    e2e_data_vol_trans_time_info_local_var->geo_distr_infos = geo_distr_infos;
    e2e_data_vol_trans_time_info_local_var->is_confidence = is_confidence;
    e2e_data_vol_trans_time_info_local_var->confidence = confidence;

    return e2e_data_vol_trans_time_info_local_var;
}

void OpenAPI_e2e_data_vol_trans_time_info_free(OpenAPI_e2e_data_vol_trans_time_info_t *e2e_data_vol_trans_time_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == e2e_data_vol_trans_time_info) {
        return;
    }
    if (e2e_data_vol_trans_time_info->e2e_data_vol_trans_times) {
        OpenAPI_list_for_each(e2e_data_vol_trans_time_info->e2e_data_vol_trans_times, node) {
            OpenAPI_e2e_data_vol_trans_time_per_ts_free(node->data);
        }
        OpenAPI_list_free(e2e_data_vol_trans_time_info->e2e_data_vol_trans_times);
        e2e_data_vol_trans_time_info->e2e_data_vol_trans_times = NULL;
    }
    if (e2e_data_vol_trans_time_info->e2e_data_vol_trans_time_ue_lists) {
        OpenAPI_list_for_each(e2e_data_vol_trans_time_info->e2e_data_vol_trans_time_ue_lists, node) {
            OpenAPI_e2e_data_vol_trans_time_ue_list_free(node->data);
        }
        OpenAPI_list_free(e2e_data_vol_trans_time_info->e2e_data_vol_trans_time_ue_lists);
        e2e_data_vol_trans_time_info->e2e_data_vol_trans_time_ue_lists = NULL;
    }
    if (e2e_data_vol_trans_time_info->geo_distr_infos) {
        OpenAPI_list_for_each(e2e_data_vol_trans_time_info->geo_distr_infos, node) {
            OpenAPI_geo_distribution_info_free(node->data);
        }
        OpenAPI_list_free(e2e_data_vol_trans_time_info->geo_distr_infos);
        e2e_data_vol_trans_time_info->geo_distr_infos = NULL;
    }
    ogs_free(e2e_data_vol_trans_time_info);
}

cJSON *OpenAPI_e2e_data_vol_trans_time_info_convertToJSON(OpenAPI_e2e_data_vol_trans_time_info_t *e2e_data_vol_trans_time_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (e2e_data_vol_trans_time_info == NULL) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_info_convertToJSON() failed [E2eDataVolTransTimeInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!e2e_data_vol_trans_time_info->e2e_data_vol_trans_times) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_info_convertToJSON() failed [e2e_data_vol_trans_times]");
        return NULL;
    }
    cJSON *e2e_data_vol_trans_timesList = cJSON_AddArrayToObject(item, "e2eDataVolTransTimes");
    if (e2e_data_vol_trans_timesList == NULL) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_info_convertToJSON() failed [e2e_data_vol_trans_times]");
        goto end;
    }
    OpenAPI_list_for_each(e2e_data_vol_trans_time_info->e2e_data_vol_trans_times, node) {
        cJSON *itemLocal = OpenAPI_e2e_data_vol_trans_time_per_ts_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_e2e_data_vol_trans_time_info_convertToJSON() failed [e2e_data_vol_trans_times]");
            goto end;
        }
        cJSON_AddItemToArray(e2e_data_vol_trans_timesList, itemLocal);
    }

    if (e2e_data_vol_trans_time_info->e2e_data_vol_trans_time_ue_lists) {
    cJSON *e2e_data_vol_trans_time_ue_listsList = cJSON_AddArrayToObject(item, "e2eDataVolTransTimeUeLists");
    if (e2e_data_vol_trans_time_ue_listsList == NULL) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_info_convertToJSON() failed [e2e_data_vol_trans_time_ue_lists]");
        goto end;
    }
    OpenAPI_list_for_each(e2e_data_vol_trans_time_info->e2e_data_vol_trans_time_ue_lists, node) {
        cJSON *itemLocal = OpenAPI_e2e_data_vol_trans_time_ue_list_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_e2e_data_vol_trans_time_info_convertToJSON() failed [e2e_data_vol_trans_time_ue_lists]");
            goto end;
        }
        cJSON_AddItemToArray(e2e_data_vol_trans_time_ue_listsList, itemLocal);
    }
    }

    if (e2e_data_vol_trans_time_info->geo_distr_infos) {
    cJSON *geo_distr_infosList = cJSON_AddArrayToObject(item, "geoDistrInfos");
    if (geo_distr_infosList == NULL) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_info_convertToJSON() failed [geo_distr_infos]");
        goto end;
    }
    OpenAPI_list_for_each(e2e_data_vol_trans_time_info->geo_distr_infos, node) {
        cJSON *itemLocal = OpenAPI_geo_distribution_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_e2e_data_vol_trans_time_info_convertToJSON() failed [geo_distr_infos]");
            goto end;
        }
        cJSON_AddItemToArray(geo_distr_infosList, itemLocal);
    }
    }

    if (e2e_data_vol_trans_time_info->is_confidence) {
    if (cJSON_AddNumberToObject(item, "confidence", e2e_data_vol_trans_time_info->confidence) == NULL) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_info_convertToJSON() failed [confidence]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_e2e_data_vol_trans_time_info_t *OpenAPI_e2e_data_vol_trans_time_info_parseFromJSON(cJSON *e2e_data_vol_trans_time_infoJSON)
{
    OpenAPI_e2e_data_vol_trans_time_info_t *e2e_data_vol_trans_time_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *e2e_data_vol_trans_times = NULL;
    OpenAPI_list_t *e2e_data_vol_trans_timesList = NULL;
    cJSON *e2e_data_vol_trans_time_ue_lists = NULL;
    OpenAPI_list_t *e2e_data_vol_trans_time_ue_listsList = NULL;
    cJSON *geo_distr_infos = NULL;
    OpenAPI_list_t *geo_distr_infosList = NULL;
    cJSON *confidence = NULL;
    e2e_data_vol_trans_times = cJSON_GetObjectItemCaseSensitive(e2e_data_vol_trans_time_infoJSON, "e2eDataVolTransTimes");
    if (!e2e_data_vol_trans_times) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_info_parseFromJSON() failed [e2e_data_vol_trans_times]");
        goto end;
    }
        cJSON *e2e_data_vol_trans_times_local = NULL;
        if (!cJSON_IsArray(e2e_data_vol_trans_times)) {
            ogs_error("OpenAPI_e2e_data_vol_trans_time_info_parseFromJSON() failed [e2e_data_vol_trans_times]");
            goto end;
        }

        e2e_data_vol_trans_timesList = OpenAPI_list_create();

        cJSON_ArrayForEach(e2e_data_vol_trans_times_local, e2e_data_vol_trans_times) {
            if (!cJSON_IsObject(e2e_data_vol_trans_times_local)) {
                ogs_error("OpenAPI_e2e_data_vol_trans_time_info_parseFromJSON() failed [e2e_data_vol_trans_times]");
                goto end;
            }
            OpenAPI_e2e_data_vol_trans_time_per_ts_t *e2e_data_vol_trans_timesItem = OpenAPI_e2e_data_vol_trans_time_per_ts_parseFromJSON(e2e_data_vol_trans_times_local);
            if (!e2e_data_vol_trans_timesItem) {
                ogs_error("No e2e_data_vol_trans_timesItem");
                goto end;
            }
            OpenAPI_list_add(e2e_data_vol_trans_timesList, e2e_data_vol_trans_timesItem);
        }

    e2e_data_vol_trans_time_ue_lists = cJSON_GetObjectItemCaseSensitive(e2e_data_vol_trans_time_infoJSON, "e2eDataVolTransTimeUeLists");
    if (e2e_data_vol_trans_time_ue_lists) {
        cJSON *e2e_data_vol_trans_time_ue_lists_local = NULL;
        if (!cJSON_IsArray(e2e_data_vol_trans_time_ue_lists)) {
            ogs_error("OpenAPI_e2e_data_vol_trans_time_info_parseFromJSON() failed [e2e_data_vol_trans_time_ue_lists]");
            goto end;
        }

        e2e_data_vol_trans_time_ue_listsList = OpenAPI_list_create();

        cJSON_ArrayForEach(e2e_data_vol_trans_time_ue_lists_local, e2e_data_vol_trans_time_ue_lists) {
            if (!cJSON_IsObject(e2e_data_vol_trans_time_ue_lists_local)) {
                ogs_error("OpenAPI_e2e_data_vol_trans_time_info_parseFromJSON() failed [e2e_data_vol_trans_time_ue_lists]");
                goto end;
            }
            OpenAPI_e2e_data_vol_trans_time_ue_list_t *e2e_data_vol_trans_time_ue_listsItem = OpenAPI_e2e_data_vol_trans_time_ue_list_parseFromJSON(e2e_data_vol_trans_time_ue_lists_local);
            if (!e2e_data_vol_trans_time_ue_listsItem) {
                ogs_error("No e2e_data_vol_trans_time_ue_listsItem");
                goto end;
            }
            OpenAPI_list_add(e2e_data_vol_trans_time_ue_listsList, e2e_data_vol_trans_time_ue_listsItem);
        }
    }

    geo_distr_infos = cJSON_GetObjectItemCaseSensitive(e2e_data_vol_trans_time_infoJSON, "geoDistrInfos");
    if (geo_distr_infos) {
        cJSON *geo_distr_infos_local = NULL;
        if (!cJSON_IsArray(geo_distr_infos)) {
            ogs_error("OpenAPI_e2e_data_vol_trans_time_info_parseFromJSON() failed [geo_distr_infos]");
            goto end;
        }

        geo_distr_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(geo_distr_infos_local, geo_distr_infos) {
            if (!cJSON_IsObject(geo_distr_infos_local)) {
                ogs_error("OpenAPI_e2e_data_vol_trans_time_info_parseFromJSON() failed [geo_distr_infos]");
                goto end;
            }
            OpenAPI_geo_distribution_info_t *geo_distr_infosItem = OpenAPI_geo_distribution_info_parseFromJSON(geo_distr_infos_local);
            if (!geo_distr_infosItem) {
                ogs_error("No geo_distr_infosItem");
                goto end;
            }
            OpenAPI_list_add(geo_distr_infosList, geo_distr_infosItem);
        }
    }

    confidence = cJSON_GetObjectItemCaseSensitive(e2e_data_vol_trans_time_infoJSON, "confidence");
    if (confidence) {
    if (!cJSON_IsNumber(confidence)) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_info_parseFromJSON() failed [confidence]");
        goto end;
    }
    }

    e2e_data_vol_trans_time_info_local_var = OpenAPI_e2e_data_vol_trans_time_info_create (
        e2e_data_vol_trans_timesList,
        e2e_data_vol_trans_time_ue_lists ? e2e_data_vol_trans_time_ue_listsList : NULL,
        geo_distr_infos ? geo_distr_infosList : NULL,
        confidence ? true : false,
        confidence ? confidence->valuedouble : 0
    );

    return e2e_data_vol_trans_time_info_local_var;
end:
    if (e2e_data_vol_trans_timesList) {
        OpenAPI_list_for_each(e2e_data_vol_trans_timesList, node) {
            OpenAPI_e2e_data_vol_trans_time_per_ts_free(node->data);
        }
        OpenAPI_list_free(e2e_data_vol_trans_timesList);
        e2e_data_vol_trans_timesList = NULL;
    }
    if (e2e_data_vol_trans_time_ue_listsList) {
        OpenAPI_list_for_each(e2e_data_vol_trans_time_ue_listsList, node) {
            OpenAPI_e2e_data_vol_trans_time_ue_list_free(node->data);
        }
        OpenAPI_list_free(e2e_data_vol_trans_time_ue_listsList);
        e2e_data_vol_trans_time_ue_listsList = NULL;
    }
    if (geo_distr_infosList) {
        OpenAPI_list_for_each(geo_distr_infosList, node) {
            OpenAPI_geo_distribution_info_free(node->data);
        }
        OpenAPI_list_free(geo_distr_infosList);
        geo_distr_infosList = NULL;
    }
    return NULL;
}

OpenAPI_e2e_data_vol_trans_time_info_t *OpenAPI_e2e_data_vol_trans_time_info_copy(OpenAPI_e2e_data_vol_trans_time_info_t *dst, OpenAPI_e2e_data_vol_trans_time_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_e2e_data_vol_trans_time_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_info_convertToJSON() failed");
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

    OpenAPI_e2e_data_vol_trans_time_info_free(dst);
    dst = OpenAPI_e2e_data_vol_trans_time_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

