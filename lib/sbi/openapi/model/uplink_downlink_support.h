/*
 * uplink_downlink_support.h
 *
 * Represents whether an indication or capability is supported for the UL, the DL or both, UL and DL. 
 */

#ifndef _OpenAPI_uplink_downlink_support_H_
#define _OpenAPI_uplink_downlink_support_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_uplink_downlink_support_NULL = 0, OpenAPI_uplink_downlink_support_UL, OpenAPI_uplink_downlink_support_DL, OpenAPI_uplink_downlink_support_UL_DL } OpenAPI_uplink_downlink_support_e;

char* OpenAPI_uplink_downlink_support_ToString(OpenAPI_uplink_downlink_support_e uplink_downlink_support);

OpenAPI_uplink_downlink_support_e OpenAPI_uplink_downlink_support_FromString(char* uplink_downlink_support);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_uplink_downlink_support_H_ */

