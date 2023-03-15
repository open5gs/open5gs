
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "wlan_performance_info.h"

OpenAPI_wlan_performance_info_t *OpenAPI_wlan_performance_info_create(
    OpenAPI_network_area_info_t *network_area,
    OpenAPI_list_t *wlan_per_ssid_infos
)
{
    OpenAPI_wlan_performance_info_t *wlan_performance_info_local_var = ogs_malloc(sizeof(OpenAPI_wlan_performance_info_t));
    ogs_assert(wlan_performance_info_local_var);

    wlan_performance_info_local_var->network_area = network_area;
    wlan_performance_info_local_var->wlan_per_ssid_infos = wlan_per_ssid_infos;

    return wlan_performance_info_local_var;
}

void OpenAPI_wlan_performance_info_free(OpenAPI_wlan_performance_info_t *wlan_performance_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == wlan_performance_info) {
        return;
    }
    if (wlan_performance_info->network_area) {
        OpenAPI_network_area_info_free(wlan_performance_info->network_area);
        wlan_performance_info->network_area = NULL;
    }
    if (wlan_performance_info->wlan_per_ssid_infos) {
        OpenAPI_list_for_each(wlan_performance_info->wlan_per_ssid_infos, node) {
            OpenAPI_wlan_per_ss_id_performance_info_free(node->data);
        }
        OpenAPI_list_free(wlan_performance_info->wlan_per_ssid_infos);
        wlan_performance_info->wlan_per_ssid_infos = NULL;
    }
    ogs_free(wlan_performance_info);
}

cJSON *OpenAPI_wlan_performance_info_convertToJSON(OpenAPI_wlan_performance_info_t *wlan_performance_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (wlan_performance_info == NULL) {
        ogs_error("OpenAPI_wlan_performance_info_convertToJSON() failed [WlanPerformanceInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (wlan_performance_info->network_area) {
    cJSON *network_area_local_JSON = OpenAPI_network_area_info_convertToJSON(wlan_performance_info->network_area);
    if (network_area_local_JSON == NULL) {
        ogs_error("OpenAPI_wlan_performance_info_convertToJSON() failed [network_area]");
        goto end;
    }
    cJSON_AddItemToObject(item, "networkArea", network_area_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_wlan_performance_info_convertToJSON() failed [network_area]");
        goto end;
    }
    }

    if (!wlan_performance_info->wlan_per_ssid_infos) {
        ogs_error("OpenAPI_wlan_performance_info_convertToJSON() failed [wlan_per_ssid_infos]");
        return NULL;
    }
    cJSON *wlan_per_ssid_infosList = cJSON_AddArrayToObject(item, "wlanPerSsidInfos");
    if (wlan_per_ssid_infosList == NULL) {
        ogs_error("OpenAPI_wlan_performance_info_convertToJSON() failed [wlan_per_ssid_infos]");
        goto end;
    }
    OpenAPI_list_for_each(wlan_performance_info->wlan_per_ssid_infos, node) {
        cJSON *itemLocal = OpenAPI_wlan_per_ss_id_performance_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_wlan_performance_info_convertToJSON() failed [wlan_per_ssid_infos]");
            goto end;
        }
        cJSON_AddItemToArray(wlan_per_ssid_infosList, itemLocal);
    }

end:
    return item;
}

OpenAPI_wlan_performance_info_t *OpenAPI_wlan_performance_info_parseFromJSON(cJSON *wlan_performance_infoJSON)
{
    OpenAPI_wlan_performance_info_t *wlan_performance_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *network_area = NULL;
    OpenAPI_network_area_info_t *network_area_local_nonprim = NULL;
    cJSON *wlan_per_ssid_infos = NULL;
    OpenAPI_list_t *wlan_per_ssid_infosList = NULL;
    network_area = cJSON_GetObjectItemCaseSensitive(wlan_performance_infoJSON, "networkArea");
    if (network_area) {
    network_area_local_nonprim = OpenAPI_network_area_info_parseFromJSON(network_area);
    if (!network_area_local_nonprim) {
        ogs_error("OpenAPI_network_area_info_parseFromJSON failed [network_area]");
        goto end;
    }
    }

    wlan_per_ssid_infos = cJSON_GetObjectItemCaseSensitive(wlan_performance_infoJSON, "wlanPerSsidInfos");
    if (!wlan_per_ssid_infos) {
        ogs_error("OpenAPI_wlan_performance_info_parseFromJSON() failed [wlan_per_ssid_infos]");
        goto end;
    }
        cJSON *wlan_per_ssid_infos_local = NULL;
        if (!cJSON_IsArray(wlan_per_ssid_infos)) {
            ogs_error("OpenAPI_wlan_performance_info_parseFromJSON() failed [wlan_per_ssid_infos]");
            goto end;
        }

        wlan_per_ssid_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(wlan_per_ssid_infos_local, wlan_per_ssid_infos) {
            if (!cJSON_IsObject(wlan_per_ssid_infos_local)) {
                ogs_error("OpenAPI_wlan_performance_info_parseFromJSON() failed [wlan_per_ssid_infos]");
                goto end;
            }
            OpenAPI_wlan_per_ss_id_performance_info_t *wlan_per_ssid_infosItem = OpenAPI_wlan_per_ss_id_performance_info_parseFromJSON(wlan_per_ssid_infos_local);
            if (!wlan_per_ssid_infosItem) {
                ogs_error("No wlan_per_ssid_infosItem");
                goto end;
            }
            OpenAPI_list_add(wlan_per_ssid_infosList, wlan_per_ssid_infosItem);
        }

    wlan_performance_info_local_var = OpenAPI_wlan_performance_info_create (
        network_area ? network_area_local_nonprim : NULL,
        wlan_per_ssid_infosList
    );

    return wlan_performance_info_local_var;
end:
    if (network_area_local_nonprim) {
        OpenAPI_network_area_info_free(network_area_local_nonprim);
        network_area_local_nonprim = NULL;
    }
    if (wlan_per_ssid_infosList) {
        OpenAPI_list_for_each(wlan_per_ssid_infosList, node) {
            OpenAPI_wlan_per_ss_id_performance_info_free(node->data);
        }
        OpenAPI_list_free(wlan_per_ssid_infosList);
        wlan_per_ssid_infosList = NULL;
    }
    return NULL;
}

OpenAPI_wlan_performance_info_t *OpenAPI_wlan_performance_info_copy(OpenAPI_wlan_performance_info_t *dst, OpenAPI_wlan_performance_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_wlan_performance_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_wlan_performance_info_convertToJSON() failed");
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

    OpenAPI_wlan_performance_info_free(dst);
    dst = OpenAPI_wlan_performance_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

