/*
 * nrf_info_served_nwdaf_info_value.h
 *
 * 
 */

#ifndef _OpenAPI_nrf_info_served_nwdaf_info_value_H_
#define _OpenAPI_nrf_info_served_nwdaf_info_value_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "event_id.h"
#include "ml_analytics_info.h"
#include "nf_type.h"
#include "nwdaf_capability.h"
#include "nwdaf_event.h"
#include "nwdaf_info.h"
#include "tai.h"
#include "tai_range.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nrf_info_served_nwdaf_info_value_s OpenAPI_nrf_info_served_nwdaf_info_value_t;
typedef struct OpenAPI_nrf_info_served_nwdaf_info_value_s {
    OpenAPI_list_t *event_ids;
    OpenAPI_list_t *nwdaf_events;
    OpenAPI_list_t *tai_list;
    OpenAPI_list_t *tai_range_list;
    struct OpenAPI_nwdaf_capability_s *nwdaf_capability;
    bool is_analytics_delay;
    int analytics_delay;
    OpenAPI_list_t *serving_nf_set_id_list;
    OpenAPI_list_t *serving_nf_type_list;
    OpenAPI_list_t *ml_analytics_list;
} OpenAPI_nrf_info_served_nwdaf_info_value_t;

OpenAPI_nrf_info_served_nwdaf_info_value_t *OpenAPI_nrf_info_served_nwdaf_info_value_create(
    OpenAPI_list_t *event_ids,
    OpenAPI_list_t *nwdaf_events,
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *tai_range_list,
    OpenAPI_nwdaf_capability_t *nwdaf_capability,
    bool is_analytics_delay,
    int analytics_delay,
    OpenAPI_list_t *serving_nf_set_id_list,
    OpenAPI_list_t *serving_nf_type_list,
    OpenAPI_list_t *ml_analytics_list
);
void OpenAPI_nrf_info_served_nwdaf_info_value_free(OpenAPI_nrf_info_served_nwdaf_info_value_t *nrf_info_served_nwdaf_info_value);
OpenAPI_nrf_info_served_nwdaf_info_value_t *OpenAPI_nrf_info_served_nwdaf_info_value_parseFromJSON(cJSON *nrf_info_served_nwdaf_info_valueJSON);
cJSON *OpenAPI_nrf_info_served_nwdaf_info_value_convertToJSON(OpenAPI_nrf_info_served_nwdaf_info_value_t *nrf_info_served_nwdaf_info_value);
OpenAPI_nrf_info_served_nwdaf_info_value_t *OpenAPI_nrf_info_served_nwdaf_info_value_copy(OpenAPI_nrf_info_served_nwdaf_info_value_t *dst, OpenAPI_nrf_info_served_nwdaf_info_value_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nrf_info_served_nwdaf_info_value_H_ */

