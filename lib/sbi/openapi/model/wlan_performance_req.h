/*
 * wlan_performance_req.h
 *
 * Represents other WLAN performance analytics requirements.
 */

#ifndef _OpenAPI_wlan_performance_req_H_
#define _OpenAPI_wlan_performance_req_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "matching_direction.h"
#include "wlan_ordering_criterion.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_wlan_performance_req_s OpenAPI_wlan_performance_req_t;
typedef struct OpenAPI_wlan_performance_req_s {
    OpenAPI_list_t *ss_ids;
    OpenAPI_list_t *bss_ids;
    struct OpenAPI_wlan_ordering_criterion_s *wlan_order_criter;
    struct OpenAPI_matching_direction_s *order;
} OpenAPI_wlan_performance_req_t;

OpenAPI_wlan_performance_req_t *OpenAPI_wlan_performance_req_create(
    OpenAPI_list_t *ss_ids,
    OpenAPI_list_t *bss_ids,
    OpenAPI_wlan_ordering_criterion_t *wlan_order_criter,
    OpenAPI_matching_direction_t *order
);
void OpenAPI_wlan_performance_req_free(OpenAPI_wlan_performance_req_t *wlan_performance_req);
OpenAPI_wlan_performance_req_t *OpenAPI_wlan_performance_req_parseFromJSON(cJSON *wlan_performance_reqJSON);
cJSON *OpenAPI_wlan_performance_req_convertToJSON(OpenAPI_wlan_performance_req_t *wlan_performance_req);
OpenAPI_wlan_performance_req_t *OpenAPI_wlan_performance_req_copy(OpenAPI_wlan_performance_req_t *dst, OpenAPI_wlan_performance_req_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_wlan_performance_req_H_ */

