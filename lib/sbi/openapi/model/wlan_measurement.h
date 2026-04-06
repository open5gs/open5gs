/*
 * wlan_measurement.h
 *
 * contains the WLAN measurements to be collected for UE.
 */

#ifndef _OpenAPI_wlan_measurement_H_
#define _OpenAPI_wlan_measurement_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_wlan_measurement_s OpenAPI_wlan_measurement_t;
#include "measurement_name.h"
#include "wlan_rssi.h"
#include "wlan_rtt.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_wlan_measurement_s {
    OpenAPI_list_t *measurement_name_list;
    OpenAPI_wlan_rssi_e wlan_rssi;
    OpenAPI_wlan_rtt_e wlan_rtt;
};

OpenAPI_wlan_measurement_t *OpenAPI_wlan_measurement_create(
    OpenAPI_list_t *measurement_name_list,
    OpenAPI_wlan_rssi_e wlan_rssi,
    OpenAPI_wlan_rtt_e wlan_rtt
);
void OpenAPI_wlan_measurement_free(OpenAPI_wlan_measurement_t *wlan_measurement);
OpenAPI_wlan_measurement_t *OpenAPI_wlan_measurement_parseFromJSON(cJSON *wlan_measurementJSON);
cJSON *OpenAPI_wlan_measurement_convertToJSON(OpenAPI_wlan_measurement_t *wlan_measurement);
OpenAPI_wlan_measurement_t *OpenAPI_wlan_measurement_copy(OpenAPI_wlan_measurement_t *dst, OpenAPI_wlan_measurement_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_wlan_measurement_H_ */

