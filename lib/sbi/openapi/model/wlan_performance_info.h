/*
 * wlan_performance_info.h
 *
 * The WLAN performance related information.
 */

#ifndef _OpenAPI_wlan_performance_info_H_
#define _OpenAPI_wlan_performance_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "network_area_info.h"
#include "wlan_per_ss_id_performance_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_wlan_performance_info_s OpenAPI_wlan_performance_info_t;
typedef struct OpenAPI_wlan_performance_info_s {
    struct OpenAPI_network_area_info_s *network_area;
    OpenAPI_list_t *wlan_per_ssid_infos;
} OpenAPI_wlan_performance_info_t;

OpenAPI_wlan_performance_info_t *OpenAPI_wlan_performance_info_create(
    OpenAPI_network_area_info_t *network_area,
    OpenAPI_list_t *wlan_per_ssid_infos
);
void OpenAPI_wlan_performance_info_free(OpenAPI_wlan_performance_info_t *wlan_performance_info);
OpenAPI_wlan_performance_info_t *OpenAPI_wlan_performance_info_parseFromJSON(cJSON *wlan_performance_infoJSON);
cJSON *OpenAPI_wlan_performance_info_convertToJSON(OpenAPI_wlan_performance_info_t *wlan_performance_info);
OpenAPI_wlan_performance_info_t *OpenAPI_wlan_performance_info_copy(OpenAPI_wlan_performance_info_t *dst, OpenAPI_wlan_performance_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_wlan_performance_info_H_ */

