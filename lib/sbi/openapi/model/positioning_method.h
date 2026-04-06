/*
 * positioning_method.h
 *
 * Indicates supported positioning methods.
 */

#ifndef _OpenAPI_positioning_method_H_
#define _OpenAPI_positioning_method_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_positioning_method_NULL = 0, OpenAPI_positioning_method_CELLID, OpenAPI_positioning_method_ECID, OpenAPI_positioning_method_OTDOA, OpenAPI_positioning_method_BAROMETRIC_PRESSURE, OpenAPI_positioning_method_WLAN, OpenAPI_positioning_method_BLUETOOTH, OpenAPI_positioning_method_MBS, OpenAPI_positioning_method_MOTION_SENSOR, OpenAPI_positioning_method_DL_TDOA, OpenAPI_positioning_method_DL_AOD, OpenAPI_positioning_method_MULTI_RTT, OpenAPI_positioning_method_NR_ECID, OpenAPI_positioning_method_UL_TDOA, OpenAPI_positioning_method_UL_AOA, OpenAPI_positioning_method_NETWORK_SPECIFIC, OpenAPI_positioning_method_SL_TDOA, OpenAPI_positioning_method_SL_TOA, OpenAPI_positioning_method_SL_AoA, OpenAPI_positioning_method_SL_RT, OpenAPI_positioning_method_AIML } OpenAPI_positioning_method_e;

char* OpenAPI_positioning_method_ToString(OpenAPI_positioning_method_e positioning_method);

OpenAPI_positioning_method_e OpenAPI_positioning_method_FromString(char* positioning_method);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_positioning_method_H_ */

