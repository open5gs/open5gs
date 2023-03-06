/*
 * wlan_ordering_criterion.h
 *
 * Possible values are: - TIME_SLOT_START: Indicates the order of time slot start. - NUMBER_OF_UES: Indicates the order of number of UEs. - RSSI: Indicates the order of RSSI. - RTT: Indicates the order of RTT. - TRAFFIC_INFO: Indicates the order of Traffic information. 
 */

#ifndef _OpenAPI_wlan_ordering_criterion_H_
#define _OpenAPI_wlan_ordering_criterion_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "wlan_ordering_criterion_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_wlan_ordering_criterion_s OpenAPI_wlan_ordering_criterion_t;
typedef struct OpenAPI_wlan_ordering_criterion_s {
} OpenAPI_wlan_ordering_criterion_t;

OpenAPI_wlan_ordering_criterion_t *OpenAPI_wlan_ordering_criterion_create(
);
void OpenAPI_wlan_ordering_criterion_free(OpenAPI_wlan_ordering_criterion_t *wlan_ordering_criterion);
OpenAPI_wlan_ordering_criterion_t *OpenAPI_wlan_ordering_criterion_parseFromJSON(cJSON *wlan_ordering_criterionJSON);
cJSON *OpenAPI_wlan_ordering_criterion_convertToJSON(OpenAPI_wlan_ordering_criterion_t *wlan_ordering_criterion);
OpenAPI_wlan_ordering_criterion_t *OpenAPI_wlan_ordering_criterion_copy(OpenAPI_wlan_ordering_criterion_t *dst, OpenAPI_wlan_ordering_criterion_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_wlan_ordering_criterion_H_ */

