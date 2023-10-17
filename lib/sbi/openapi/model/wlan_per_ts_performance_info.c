
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "wlan_per_ts_performance_info.h"

OpenAPI_wlan_per_ts_performance_info_t *OpenAPI_wlan_per_ts_performance_info_create(
    char *ts_start,
    int ts_duration,
    bool is_rssi,
    int rssi,
    bool is_rtt,
    int rtt,
    OpenAPI_traffic_information_t *traffic_info,
    bool is_number_of_ues,
    int number_of_ues,
    bool is_confidence,
    int confidence
)
{
    OpenAPI_wlan_per_ts_performance_info_t *wlan_per_ts_performance_info_local_var = ogs_malloc(sizeof(OpenAPI_wlan_per_ts_performance_info_t));
    ogs_assert(wlan_per_ts_performance_info_local_var);

    wlan_per_ts_performance_info_local_var->ts_start = ts_start;
    wlan_per_ts_performance_info_local_var->ts_duration = ts_duration;
    wlan_per_ts_performance_info_local_var->is_rssi = is_rssi;
    wlan_per_ts_performance_info_local_var->rssi = rssi;
    wlan_per_ts_performance_info_local_var->is_rtt = is_rtt;
    wlan_per_ts_performance_info_local_var->rtt = rtt;
    wlan_per_ts_performance_info_local_var->traffic_info = traffic_info;
    wlan_per_ts_performance_info_local_var->is_number_of_ues = is_number_of_ues;
    wlan_per_ts_performance_info_local_var->number_of_ues = number_of_ues;
    wlan_per_ts_performance_info_local_var->is_confidence = is_confidence;
    wlan_per_ts_performance_info_local_var->confidence = confidence;

    return wlan_per_ts_performance_info_local_var;
}

void OpenAPI_wlan_per_ts_performance_info_free(OpenAPI_wlan_per_ts_performance_info_t *wlan_per_ts_performance_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == wlan_per_ts_performance_info) {
        return;
    }
    if (wlan_per_ts_performance_info->ts_start) {
        ogs_free(wlan_per_ts_performance_info->ts_start);
        wlan_per_ts_performance_info->ts_start = NULL;
    }
    if (wlan_per_ts_performance_info->traffic_info) {
        OpenAPI_traffic_information_free(wlan_per_ts_performance_info->traffic_info);
        wlan_per_ts_performance_info->traffic_info = NULL;
    }
    ogs_free(wlan_per_ts_performance_info);
}

cJSON *OpenAPI_wlan_per_ts_performance_info_convertToJSON(OpenAPI_wlan_per_ts_performance_info_t *wlan_per_ts_performance_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (wlan_per_ts_performance_info == NULL) {
        ogs_error("OpenAPI_wlan_per_ts_performance_info_convertToJSON() failed [WlanPerTsPerformanceInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!wlan_per_ts_performance_info->ts_start) {
        ogs_error("OpenAPI_wlan_per_ts_performance_info_convertToJSON() failed [ts_start]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "tsStart", wlan_per_ts_performance_info->ts_start) == NULL) {
        ogs_error("OpenAPI_wlan_per_ts_performance_info_convertToJSON() failed [ts_start]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "tsDuration", wlan_per_ts_performance_info->ts_duration) == NULL) {
        ogs_error("OpenAPI_wlan_per_ts_performance_info_convertToJSON() failed [ts_duration]");
        goto end;
    }

    if (wlan_per_ts_performance_info->is_rssi) {
    if (cJSON_AddNumberToObject(item, "rssi", wlan_per_ts_performance_info->rssi) == NULL) {
        ogs_error("OpenAPI_wlan_per_ts_performance_info_convertToJSON() failed [rssi]");
        goto end;
    }
    }

    if (wlan_per_ts_performance_info->is_rtt) {
    if (cJSON_AddNumberToObject(item, "rtt", wlan_per_ts_performance_info->rtt) == NULL) {
        ogs_error("OpenAPI_wlan_per_ts_performance_info_convertToJSON() failed [rtt]");
        goto end;
    }
    }

    if (wlan_per_ts_performance_info->traffic_info) {
    cJSON *traffic_info_local_JSON = OpenAPI_traffic_information_convertToJSON(wlan_per_ts_performance_info->traffic_info);
    if (traffic_info_local_JSON == NULL) {
        ogs_error("OpenAPI_wlan_per_ts_performance_info_convertToJSON() failed [traffic_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "trafficInfo", traffic_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_wlan_per_ts_performance_info_convertToJSON() failed [traffic_info]");
        goto end;
    }
    }

    if (wlan_per_ts_performance_info->is_number_of_ues) {
    if (cJSON_AddNumberToObject(item, "numberOfUes", wlan_per_ts_performance_info->number_of_ues) == NULL) {
        ogs_error("OpenAPI_wlan_per_ts_performance_info_convertToJSON() failed [number_of_ues]");
        goto end;
    }
    }

    if (wlan_per_ts_performance_info->is_confidence) {
    if (cJSON_AddNumberToObject(item, "confidence", wlan_per_ts_performance_info->confidence) == NULL) {
        ogs_error("OpenAPI_wlan_per_ts_performance_info_convertToJSON() failed [confidence]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_wlan_per_ts_performance_info_t *OpenAPI_wlan_per_ts_performance_info_parseFromJSON(cJSON *wlan_per_ts_performance_infoJSON)
{
    OpenAPI_wlan_per_ts_performance_info_t *wlan_per_ts_performance_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ts_start = NULL;
    cJSON *ts_duration = NULL;
    cJSON *rssi = NULL;
    cJSON *rtt = NULL;
    cJSON *traffic_info = NULL;
    OpenAPI_traffic_information_t *traffic_info_local_nonprim = NULL;
    cJSON *number_of_ues = NULL;
    cJSON *confidence = NULL;
    ts_start = cJSON_GetObjectItemCaseSensitive(wlan_per_ts_performance_infoJSON, "tsStart");
    if (!ts_start) {
        ogs_error("OpenAPI_wlan_per_ts_performance_info_parseFromJSON() failed [ts_start]");
        goto end;
    }
    if (!cJSON_IsString(ts_start) && !cJSON_IsNull(ts_start)) {
        ogs_error("OpenAPI_wlan_per_ts_performance_info_parseFromJSON() failed [ts_start]");
        goto end;
    }

    ts_duration = cJSON_GetObjectItemCaseSensitive(wlan_per_ts_performance_infoJSON, "tsDuration");
    if (!ts_duration) {
        ogs_error("OpenAPI_wlan_per_ts_performance_info_parseFromJSON() failed [ts_duration]");
        goto end;
    }
    if (!cJSON_IsNumber(ts_duration)) {
        ogs_error("OpenAPI_wlan_per_ts_performance_info_parseFromJSON() failed [ts_duration]");
        goto end;
    }

    rssi = cJSON_GetObjectItemCaseSensitive(wlan_per_ts_performance_infoJSON, "rssi");
    if (rssi) {
    if (!cJSON_IsNumber(rssi)) {
        ogs_error("OpenAPI_wlan_per_ts_performance_info_parseFromJSON() failed [rssi]");
        goto end;
    }
    }

    rtt = cJSON_GetObjectItemCaseSensitive(wlan_per_ts_performance_infoJSON, "rtt");
    if (rtt) {
    if (!cJSON_IsNumber(rtt)) {
        ogs_error("OpenAPI_wlan_per_ts_performance_info_parseFromJSON() failed [rtt]");
        goto end;
    }
    }

    traffic_info = cJSON_GetObjectItemCaseSensitive(wlan_per_ts_performance_infoJSON, "trafficInfo");
    if (traffic_info) {
    traffic_info_local_nonprim = OpenAPI_traffic_information_parseFromJSON(traffic_info);
    if (!traffic_info_local_nonprim) {
        ogs_error("OpenAPI_traffic_information_parseFromJSON failed [traffic_info]");
        goto end;
    }
    }

    number_of_ues = cJSON_GetObjectItemCaseSensitive(wlan_per_ts_performance_infoJSON, "numberOfUes");
    if (number_of_ues) {
    if (!cJSON_IsNumber(number_of_ues)) {
        ogs_error("OpenAPI_wlan_per_ts_performance_info_parseFromJSON() failed [number_of_ues]");
        goto end;
    }
    }

    confidence = cJSON_GetObjectItemCaseSensitive(wlan_per_ts_performance_infoJSON, "confidence");
    if (confidence) {
    if (!cJSON_IsNumber(confidence)) {
        ogs_error("OpenAPI_wlan_per_ts_performance_info_parseFromJSON() failed [confidence]");
        goto end;
    }
    }

    wlan_per_ts_performance_info_local_var = OpenAPI_wlan_per_ts_performance_info_create (
        ogs_strdup(ts_start->valuestring),
        
        ts_duration->valuedouble,
        rssi ? true : false,
        rssi ? rssi->valuedouble : 0,
        rtt ? true : false,
        rtt ? rtt->valuedouble : 0,
        traffic_info ? traffic_info_local_nonprim : NULL,
        number_of_ues ? true : false,
        number_of_ues ? number_of_ues->valuedouble : 0,
        confidence ? true : false,
        confidence ? confidence->valuedouble : 0
    );

    return wlan_per_ts_performance_info_local_var;
end:
    if (traffic_info_local_nonprim) {
        OpenAPI_traffic_information_free(traffic_info_local_nonprim);
        traffic_info_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_wlan_per_ts_performance_info_t *OpenAPI_wlan_per_ts_performance_info_copy(OpenAPI_wlan_per_ts_performance_info_t *dst, OpenAPI_wlan_per_ts_performance_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_wlan_per_ts_performance_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_wlan_per_ts_performance_info_convertToJSON() failed");
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

    OpenAPI_wlan_per_ts_performance_info_free(dst);
    dst = OpenAPI_wlan_per_ts_performance_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

