/*
 * wlan_ordering_criterion_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_wlan_ordering_criterion_any_of_H_
#define _OpenAPI_wlan_ordering_criterion_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_wlan_ordering_criterion_any_of_NULL = 0, OpenAPI_wlan_ordering_criterion_any_of_TIME_SLOT_START, OpenAPI_wlan_ordering_criterion_any_of_NUMBER_OF_UES, OpenAPI_wlan_ordering_criterion_any_of_RSSI, OpenAPI_wlan_ordering_criterion_any_of_RTT, OpenAPI_wlan_ordering_criterion_any_of_TRAFFIC_INFO } OpenAPI_wlan_ordering_criterion_any_of_e;

char* OpenAPI_wlan_ordering_criterion_any_of_ToString(OpenAPI_wlan_ordering_criterion_any_of_e wlan_ordering_criterion_any_of);

OpenAPI_wlan_ordering_criterion_any_of_e OpenAPI_wlan_ordering_criterion_any_of_FromString(char* wlan_ordering_criterion_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_wlan_ordering_criterion_any_of_H_ */

