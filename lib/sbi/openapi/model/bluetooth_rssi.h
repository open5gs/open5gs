/*
 * bluetooth_rssi.h
 *
 * 
 */

#ifndef _OpenAPI_bluetooth_rssi_H_
#define _OpenAPI_bluetooth_rssi_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_bluetooth_rssi_NULL = 0, OpenAPI_bluetooth_rssi__true } OpenAPI_bluetooth_rssi_e;

char* OpenAPI_bluetooth_rssi_ToString(OpenAPI_bluetooth_rssi_e bluetooth_rssi);

OpenAPI_bluetooth_rssi_e OpenAPI_bluetooth_rssi_FromString(char* bluetooth_rssi);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_bluetooth_rssi_H_ */

