/*
 * positioning_method_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_positioning_method_any_of_H_
#define _OpenAPI_positioning_method_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_positioning_method_any_of_NULL = 0, OpenAPI_positioning_method_any_of_CELLID, OpenAPI_positioning_method_any_of_ECID, OpenAPI_positioning_method_any_of_OTDOA, OpenAPI_positioning_method_any_of_BAROMETRIC_PRESSURE, OpenAPI_positioning_method_any_of_WLAN, OpenAPI_positioning_method_any_of_BLUETOOTH, OpenAPI_positioning_method_any_of_MBS, OpenAPI_positioning_method_any_of_MOTION_SENSOR, OpenAPI_positioning_method_any_of_DL_TDOA, OpenAPI_positioning_method_any_of_DL_AOD, OpenAPI_positioning_method_any_of_MULTI_RTT, OpenAPI_positioning_method_any_of_NR_ECID, OpenAPI_positioning_method_any_of_UL_TDOA, OpenAPI_positioning_method_any_of_UL_AOA, OpenAPI_positioning_method_any_of_NETWORK_SPECIFIC } OpenAPI_positioning_method_any_of_e;

char* OpenAPI_positioning_method_any_of_ToString(OpenAPI_positioning_method_any_of_e positioning_method_any_of);

OpenAPI_positioning_method_any_of_e OpenAPI_positioning_method_any_of_FromString(char* positioning_method_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_positioning_method_any_of_H_ */

