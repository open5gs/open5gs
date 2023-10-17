/*
 * wlan_per_ts_performance_info.h
 *
 * WLAN performance information per Time Slot during the analytics target period.
 */

#ifndef _OpenAPI_wlan_per_ts_performance_info_H_
#define _OpenAPI_wlan_per_ts_performance_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "traffic_information.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_wlan_per_ts_performance_info_s OpenAPI_wlan_per_ts_performance_info_t;
typedef struct OpenAPI_wlan_per_ts_performance_info_s {
    char *ts_start;
    int ts_duration;
    bool is_rssi;
    int rssi;
    bool is_rtt;
    int rtt;
    struct OpenAPI_traffic_information_s *traffic_info;
    bool is_number_of_ues;
    int number_of_ues;
    bool is_confidence;
    int confidence;
} OpenAPI_wlan_per_ts_performance_info_t;

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
);
void OpenAPI_wlan_per_ts_performance_info_free(OpenAPI_wlan_per_ts_performance_info_t *wlan_per_ts_performance_info);
OpenAPI_wlan_per_ts_performance_info_t *OpenAPI_wlan_per_ts_performance_info_parseFromJSON(cJSON *wlan_per_ts_performance_infoJSON);
cJSON *OpenAPI_wlan_per_ts_performance_info_convertToJSON(OpenAPI_wlan_per_ts_performance_info_t *wlan_per_ts_performance_info);
OpenAPI_wlan_per_ts_performance_info_t *OpenAPI_wlan_per_ts_performance_info_copy(OpenAPI_wlan_per_ts_performance_info_t *dst, OpenAPI_wlan_per_ts_performance_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_wlan_per_ts_performance_info_H_ */

