/*
 * amf_event.h
 *
 * 
 */

#ifndef _OpenAPI_amf_event_H_
#define _OpenAPI_amf_event_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "amf_event_area.h"
#include "amf_event_type.h"
#include "location_filter.h"
#include "reachability_filter.h"
#include "traffic_descriptor.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_amf_event_s OpenAPI_amf_event_t;
typedef struct OpenAPI_amf_event_s {
    struct OpenAPI_amf_event_type_s *type;
    bool is_immediate_flag;
    int immediate_flag;
    OpenAPI_list_t *area_list;
    OpenAPI_list_t *location_filter_list;
    bool is_ref_id;
    int ref_id;
    OpenAPI_list_t *traffic_descriptor_list;
    bool is_report_ue_reachable;
    int report_ue_reachable;
    struct OpenAPI_reachability_filter_s *reachability_filter;
    bool is_max_reports;
    int max_reports;
    bool is_max_response_time;
    int max_response_time;
} OpenAPI_amf_event_t;

OpenAPI_amf_event_t *OpenAPI_amf_event_create(
    OpenAPI_amf_event_type_t *type,
    bool is_immediate_flag,
    int immediate_flag,
    OpenAPI_list_t *area_list,
    OpenAPI_list_t *location_filter_list,
    bool is_ref_id,
    int ref_id,
    OpenAPI_list_t *traffic_descriptor_list,
    bool is_report_ue_reachable,
    int report_ue_reachable,
    OpenAPI_reachability_filter_t *reachability_filter,
    bool is_max_reports,
    int max_reports,
    bool is_max_response_time,
    int max_response_time
);
void OpenAPI_amf_event_free(OpenAPI_amf_event_t *amf_event);
OpenAPI_amf_event_t *OpenAPI_amf_event_parseFromJSON(cJSON *amf_eventJSON);
cJSON *OpenAPI_amf_event_convertToJSON(OpenAPI_amf_event_t *amf_event);
OpenAPI_amf_event_t *OpenAPI_amf_event_copy(OpenAPI_amf_event_t *dst, OpenAPI_amf_event_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_amf_event_H_ */

