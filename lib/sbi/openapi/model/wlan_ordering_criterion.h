/*
 * wlan_ordering_criterion.h
 *
 * 
 */

#ifndef _OpenAPI_wlan_ordering_criterion_H_
#define _OpenAPI_wlan_ordering_criterion_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_wlan_ordering_criterion_NULL = 0, OpenAPI_wlan_ordering_criterion_TIME_SLOT_START, OpenAPI_wlan_ordering_criterion_NUMBER_OF_UES, OpenAPI_wlan_ordering_criterion_RSSI, OpenAPI_wlan_ordering_criterion_RTT, OpenAPI_wlan_ordering_criterion_TRAFFIC_INFO } OpenAPI_wlan_ordering_criterion_e;

char* OpenAPI_wlan_ordering_criterion_ToString(OpenAPI_wlan_ordering_criterion_e wlan_ordering_criterion);

OpenAPI_wlan_ordering_criterion_e OpenAPI_wlan_ordering_criterion_FromString(char* wlan_ordering_criterion);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_wlan_ordering_criterion_H_ */

