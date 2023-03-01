/*
 * wlan_per_ss_id_performance_info.h
 *
 * The WLAN performance per SSID.
 */

#ifndef _OpenAPI_wlan_per_ss_id_performance_info_H_
#define _OpenAPI_wlan_per_ss_id_performance_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "wlan_per_ts_performance_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_wlan_per_ss_id_performance_info_s OpenAPI_wlan_per_ss_id_performance_info_t;
typedef struct OpenAPI_wlan_per_ss_id_performance_info_s {
    char *ss_id;
    OpenAPI_list_t *wlan_per_ts_infos;
} OpenAPI_wlan_per_ss_id_performance_info_t;

OpenAPI_wlan_per_ss_id_performance_info_t *OpenAPI_wlan_per_ss_id_performance_info_create(
    char *ss_id,
    OpenAPI_list_t *wlan_per_ts_infos
);
void OpenAPI_wlan_per_ss_id_performance_info_free(OpenAPI_wlan_per_ss_id_performance_info_t *wlan_per_ss_id_performance_info);
OpenAPI_wlan_per_ss_id_performance_info_t *OpenAPI_wlan_per_ss_id_performance_info_parseFromJSON(cJSON *wlan_per_ss_id_performance_infoJSON);
cJSON *OpenAPI_wlan_per_ss_id_performance_info_convertToJSON(OpenAPI_wlan_per_ss_id_performance_info_t *wlan_per_ss_id_performance_info);
OpenAPI_wlan_per_ss_id_performance_info_t *OpenAPI_wlan_per_ss_id_performance_info_copy(OpenAPI_wlan_per_ss_id_performance_info_t *dst, OpenAPI_wlan_per_ss_id_performance_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_wlan_per_ss_id_performance_info_H_ */

