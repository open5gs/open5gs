/*
 * wlan_rssi.h
 *
 * 
 */

#ifndef _OpenAPI_wlan_rssi_H_
#define _OpenAPI_wlan_rssi_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_wlan_rssi_NULL = 0, OpenAPI_wlan_rssi__true } OpenAPI_wlan_rssi_e;

char* OpenAPI_wlan_rssi_ToString(OpenAPI_wlan_rssi_e wlan_rssi);

OpenAPI_wlan_rssi_e OpenAPI_wlan_rssi_FromString(char* wlan_rssi);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_wlan_rssi_H_ */

