
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "wlan_measurement_1.h"

OpenAPI_wlan_measurement_1_t *OpenAPI_wlan_measurement_1_create(
    OpenAPI_list_t *measurement_name_list,
    OpenAPI_wlan_rssi_e wlan_rssi,
    OpenAPI_wlan_rtt_e wlan_rtt
)
{
    OpenAPI_wlan_measurement_1_t *wlan_measurement_1_local_var = ogs_malloc(sizeof(OpenAPI_wlan_measurement_1_t));
    ogs_assert(wlan_measurement_1_local_var);

    wlan_measurement_1_local_var->measurement_name_list = measurement_name_list;
    wlan_measurement_1_local_var->wlan_rssi = wlan_rssi;
    wlan_measurement_1_local_var->wlan_rtt = wlan_rtt;

    return wlan_measurement_1_local_var;
}

void OpenAPI_wlan_measurement_1_free(OpenAPI_wlan_measurement_1_t *wlan_measurement_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == wlan_measurement_1) {
        return;
    }
    if (wlan_measurement_1->measurement_name_list) {
        OpenAPI_list_for_each(wlan_measurement_1->measurement_name_list, node) {
            OpenAPI_measurement_name_free(node->data);
        }
        OpenAPI_list_free(wlan_measurement_1->measurement_name_list);
        wlan_measurement_1->measurement_name_list = NULL;
    }
    ogs_free(wlan_measurement_1);
}

cJSON *OpenAPI_wlan_measurement_1_convertToJSON(OpenAPI_wlan_measurement_1_t *wlan_measurement_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (wlan_measurement_1 == NULL) {
        ogs_error("OpenAPI_wlan_measurement_1_convertToJSON() failed [WlanMeasurement_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (wlan_measurement_1->measurement_name_list) {
    cJSON *measurement_name_listList = cJSON_AddArrayToObject(item, "measurementNameList");
    if (measurement_name_listList == NULL) {
        ogs_error("OpenAPI_wlan_measurement_1_convertToJSON() failed [measurement_name_list]");
        goto end;
    }
    OpenAPI_list_for_each(wlan_measurement_1->measurement_name_list, node) {
        cJSON *itemLocal = OpenAPI_measurement_name_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_wlan_measurement_1_convertToJSON() failed [measurement_name_list]");
            goto end;
        }
        cJSON_AddItemToArray(measurement_name_listList, itemLocal);
    }
    }

    if (wlan_measurement_1->wlan_rssi != OpenAPI_wlan_rssi_NULL) {
    if (cJSON_AddStringToObject(item, "wlanRssi", OpenAPI_wlan_rssi_ToString(wlan_measurement_1->wlan_rssi)) == NULL) {
        ogs_error("OpenAPI_wlan_measurement_1_convertToJSON() failed [wlan_rssi]");
        goto end;
    }
    }

    if (wlan_measurement_1->wlan_rtt != OpenAPI_wlan_rtt_NULL) {
    if (cJSON_AddStringToObject(item, "wlanRtt", OpenAPI_wlan_rtt_ToString(wlan_measurement_1->wlan_rtt)) == NULL) {
        ogs_error("OpenAPI_wlan_measurement_1_convertToJSON() failed [wlan_rtt]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_wlan_measurement_1_t *OpenAPI_wlan_measurement_1_parseFromJSON(cJSON *wlan_measurement_1JSON)
{
    OpenAPI_wlan_measurement_1_t *wlan_measurement_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *measurement_name_list = NULL;
    OpenAPI_list_t *measurement_name_listList = NULL;
    cJSON *wlan_rssi = NULL;
    OpenAPI_wlan_rssi_e wlan_rssiVariable = 0;
    cJSON *wlan_rtt = NULL;
    OpenAPI_wlan_rtt_e wlan_rttVariable = 0;
    measurement_name_list = cJSON_GetObjectItemCaseSensitive(wlan_measurement_1JSON, "measurementNameList");
    if (measurement_name_list) {
        cJSON *measurement_name_list_local = NULL;
        if (!cJSON_IsArray(measurement_name_list)) {
            ogs_error("OpenAPI_wlan_measurement_1_parseFromJSON() failed [measurement_name_list]");
            goto end;
        }

        measurement_name_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(measurement_name_list_local, measurement_name_list) {
            if (!cJSON_IsObject(measurement_name_list_local)) {
                ogs_error("OpenAPI_wlan_measurement_1_parseFromJSON() failed [measurement_name_list]");
                goto end;
            }
            OpenAPI_measurement_name_t *measurement_name_listItem = OpenAPI_measurement_name_parseFromJSON(measurement_name_list_local);
            if (!measurement_name_listItem) {
                ogs_error("No measurement_name_listItem");
                goto end;
            }
            OpenAPI_list_add(measurement_name_listList, measurement_name_listItem);
        }
    }

    wlan_rssi = cJSON_GetObjectItemCaseSensitive(wlan_measurement_1JSON, "wlanRssi");
    if (wlan_rssi) {
    if (!cJSON_IsString(wlan_rssi)) {
        ogs_error("OpenAPI_wlan_measurement_1_parseFromJSON() failed [wlan_rssi]");
        goto end;
    }
    wlan_rssiVariable = OpenAPI_wlan_rssi_FromString(wlan_rssi->valuestring);
    }

    wlan_rtt = cJSON_GetObjectItemCaseSensitive(wlan_measurement_1JSON, "wlanRtt");
    if (wlan_rtt) {
    if (!cJSON_IsString(wlan_rtt)) {
        ogs_error("OpenAPI_wlan_measurement_1_parseFromJSON() failed [wlan_rtt]");
        goto end;
    }
    wlan_rttVariable = OpenAPI_wlan_rtt_FromString(wlan_rtt->valuestring);
    }

    wlan_measurement_1_local_var = OpenAPI_wlan_measurement_1_create (
        measurement_name_list ? measurement_name_listList : NULL,
        wlan_rssi ? wlan_rssiVariable : 0,
        wlan_rtt ? wlan_rttVariable : 0
    );

    return wlan_measurement_1_local_var;
end:
    if (measurement_name_listList) {
        OpenAPI_list_for_each(measurement_name_listList, node) {
            OpenAPI_measurement_name_free(node->data);
        }
        OpenAPI_list_free(measurement_name_listList);
        measurement_name_listList = NULL;
    }
    return NULL;
}

OpenAPI_wlan_measurement_1_t *OpenAPI_wlan_measurement_1_copy(OpenAPI_wlan_measurement_1_t *dst, OpenAPI_wlan_measurement_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_wlan_measurement_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_wlan_measurement_1_convertToJSON() failed");
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

    OpenAPI_wlan_measurement_1_free(dst);
    dst = OpenAPI_wlan_measurement_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

