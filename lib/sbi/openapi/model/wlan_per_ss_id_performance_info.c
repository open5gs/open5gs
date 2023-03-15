
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "wlan_per_ss_id_performance_info.h"

OpenAPI_wlan_per_ss_id_performance_info_t *OpenAPI_wlan_per_ss_id_performance_info_create(
    char *ss_id,
    OpenAPI_list_t *wlan_per_ts_infos
)
{
    OpenAPI_wlan_per_ss_id_performance_info_t *wlan_per_ss_id_performance_info_local_var = ogs_malloc(sizeof(OpenAPI_wlan_per_ss_id_performance_info_t));
    ogs_assert(wlan_per_ss_id_performance_info_local_var);

    wlan_per_ss_id_performance_info_local_var->ss_id = ss_id;
    wlan_per_ss_id_performance_info_local_var->wlan_per_ts_infos = wlan_per_ts_infos;

    return wlan_per_ss_id_performance_info_local_var;
}

void OpenAPI_wlan_per_ss_id_performance_info_free(OpenAPI_wlan_per_ss_id_performance_info_t *wlan_per_ss_id_performance_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == wlan_per_ss_id_performance_info) {
        return;
    }
    if (wlan_per_ss_id_performance_info->ss_id) {
        ogs_free(wlan_per_ss_id_performance_info->ss_id);
        wlan_per_ss_id_performance_info->ss_id = NULL;
    }
    if (wlan_per_ss_id_performance_info->wlan_per_ts_infos) {
        OpenAPI_list_for_each(wlan_per_ss_id_performance_info->wlan_per_ts_infos, node) {
            OpenAPI_wlan_per_ts_performance_info_free(node->data);
        }
        OpenAPI_list_free(wlan_per_ss_id_performance_info->wlan_per_ts_infos);
        wlan_per_ss_id_performance_info->wlan_per_ts_infos = NULL;
    }
    ogs_free(wlan_per_ss_id_performance_info);
}

cJSON *OpenAPI_wlan_per_ss_id_performance_info_convertToJSON(OpenAPI_wlan_per_ss_id_performance_info_t *wlan_per_ss_id_performance_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (wlan_per_ss_id_performance_info == NULL) {
        ogs_error("OpenAPI_wlan_per_ss_id_performance_info_convertToJSON() failed [WlanPerSsIdPerformanceInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!wlan_per_ss_id_performance_info->ss_id) {
        ogs_error("OpenAPI_wlan_per_ss_id_performance_info_convertToJSON() failed [ss_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "ssId", wlan_per_ss_id_performance_info->ss_id) == NULL) {
        ogs_error("OpenAPI_wlan_per_ss_id_performance_info_convertToJSON() failed [ss_id]");
        goto end;
    }

    if (!wlan_per_ss_id_performance_info->wlan_per_ts_infos) {
        ogs_error("OpenAPI_wlan_per_ss_id_performance_info_convertToJSON() failed [wlan_per_ts_infos]");
        return NULL;
    }
    cJSON *wlan_per_ts_infosList = cJSON_AddArrayToObject(item, "wlanPerTsInfos");
    if (wlan_per_ts_infosList == NULL) {
        ogs_error("OpenAPI_wlan_per_ss_id_performance_info_convertToJSON() failed [wlan_per_ts_infos]");
        goto end;
    }
    OpenAPI_list_for_each(wlan_per_ss_id_performance_info->wlan_per_ts_infos, node) {
        cJSON *itemLocal = OpenAPI_wlan_per_ts_performance_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_wlan_per_ss_id_performance_info_convertToJSON() failed [wlan_per_ts_infos]");
            goto end;
        }
        cJSON_AddItemToArray(wlan_per_ts_infosList, itemLocal);
    }

end:
    return item;
}

OpenAPI_wlan_per_ss_id_performance_info_t *OpenAPI_wlan_per_ss_id_performance_info_parseFromJSON(cJSON *wlan_per_ss_id_performance_infoJSON)
{
    OpenAPI_wlan_per_ss_id_performance_info_t *wlan_per_ss_id_performance_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ss_id = NULL;
    cJSON *wlan_per_ts_infos = NULL;
    OpenAPI_list_t *wlan_per_ts_infosList = NULL;
    ss_id = cJSON_GetObjectItemCaseSensitive(wlan_per_ss_id_performance_infoJSON, "ssId");
    if (!ss_id) {
        ogs_error("OpenAPI_wlan_per_ss_id_performance_info_parseFromJSON() failed [ss_id]");
        goto end;
    }
    if (!cJSON_IsString(ss_id)) {
        ogs_error("OpenAPI_wlan_per_ss_id_performance_info_parseFromJSON() failed [ss_id]");
        goto end;
    }

    wlan_per_ts_infos = cJSON_GetObjectItemCaseSensitive(wlan_per_ss_id_performance_infoJSON, "wlanPerTsInfos");
    if (!wlan_per_ts_infos) {
        ogs_error("OpenAPI_wlan_per_ss_id_performance_info_parseFromJSON() failed [wlan_per_ts_infos]");
        goto end;
    }
        cJSON *wlan_per_ts_infos_local = NULL;
        if (!cJSON_IsArray(wlan_per_ts_infos)) {
            ogs_error("OpenAPI_wlan_per_ss_id_performance_info_parseFromJSON() failed [wlan_per_ts_infos]");
            goto end;
        }

        wlan_per_ts_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(wlan_per_ts_infos_local, wlan_per_ts_infos) {
            if (!cJSON_IsObject(wlan_per_ts_infos_local)) {
                ogs_error("OpenAPI_wlan_per_ss_id_performance_info_parseFromJSON() failed [wlan_per_ts_infos]");
                goto end;
            }
            OpenAPI_wlan_per_ts_performance_info_t *wlan_per_ts_infosItem = OpenAPI_wlan_per_ts_performance_info_parseFromJSON(wlan_per_ts_infos_local);
            if (!wlan_per_ts_infosItem) {
                ogs_error("No wlan_per_ts_infosItem");
                goto end;
            }
            OpenAPI_list_add(wlan_per_ts_infosList, wlan_per_ts_infosItem);
        }

    wlan_per_ss_id_performance_info_local_var = OpenAPI_wlan_per_ss_id_performance_info_create (
        ogs_strdup(ss_id->valuestring),
        wlan_per_ts_infosList
    );

    return wlan_per_ss_id_performance_info_local_var;
end:
    if (wlan_per_ts_infosList) {
        OpenAPI_list_for_each(wlan_per_ts_infosList, node) {
            OpenAPI_wlan_per_ts_performance_info_free(node->data);
        }
        OpenAPI_list_free(wlan_per_ts_infosList);
        wlan_per_ts_infosList = NULL;
    }
    return NULL;
}

OpenAPI_wlan_per_ss_id_performance_info_t *OpenAPI_wlan_per_ss_id_performance_info_copy(OpenAPI_wlan_per_ss_id_performance_info_t *dst, OpenAPI_wlan_per_ss_id_performance_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_wlan_per_ss_id_performance_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_wlan_per_ss_id_performance_info_convertToJSON() failed");
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

    OpenAPI_wlan_per_ss_id_performance_info_free(dst);
    dst = OpenAPI_wlan_per_ss_id_performance_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

