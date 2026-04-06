/*
 * wlan_rtt.h
 *
 * 
 */

#ifndef _OpenAPI_wlan_rtt_H_
#define _OpenAPI_wlan_rtt_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_wlan_rtt_NULL = 0, OpenAPI_wlan_rtt__true } OpenAPI_wlan_rtt_e;

char* OpenAPI_wlan_rtt_ToString(OpenAPI_wlan_rtt_e wlan_rtt);

OpenAPI_wlan_rtt_e OpenAPI_wlan_rtt_FromString(char* wlan_rtt);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_wlan_rtt_H_ */

