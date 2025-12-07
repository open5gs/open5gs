/*
 * area_event_info.h
 *
 * Indicates the information of area based event reporting.
 */

#ifndef _OpenAPI_area_event_info_H_
#define _OpenAPI_area_event_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "occurrence_info.h"
#include "reporting_area.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_area_event_info_s OpenAPI_area_event_info_t;
typedef struct OpenAPI_area_event_info_s {
    OpenAPI_list_t *area_definition;
    struct OpenAPI_occurrence_info_s *occurrence_info;
    bool is_minimum_interval;
    int minimum_interval;
    bool is_maximum_interval;
    int maximum_interval;
    bool is_sampling_interval;
    int sampling_interval;
    bool is_reporting_duration;
    int reporting_duration;
    bool is_reporting_location_req;
    int reporting_location_req;
} OpenAPI_area_event_info_t;

OpenAPI_area_event_info_t *OpenAPI_area_event_info_create(
    OpenAPI_list_t *area_definition,
    OpenAPI_occurrence_info_t *occurrence_info,
    bool is_minimum_interval,
    int minimum_interval,
    bool is_maximum_interval,
    int maximum_interval,
    bool is_sampling_interval,
    int sampling_interval,
    bool is_reporting_duration,
    int reporting_duration,
    bool is_reporting_location_req,
    int reporting_location_req
);
void OpenAPI_area_event_info_free(OpenAPI_area_event_info_t *area_event_info);
OpenAPI_area_event_info_t *OpenAPI_area_event_info_parseFromJSON(cJSON *area_event_infoJSON);
cJSON *OpenAPI_area_event_info_convertToJSON(OpenAPI_area_event_info_t *area_event_info);
OpenAPI_area_event_info_t *OpenAPI_area_event_info_copy(OpenAPI_area_event_info_t *dst, OpenAPI_area_event_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_area_event_info_H_ */

