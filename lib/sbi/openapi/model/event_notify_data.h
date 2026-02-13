/*
 * event_notify_data.h
 *
 * Information within Event Notify Request.
 */

#ifndef _OpenAPI_event_notify_data_H_
#define _OpenAPI_event_notify_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "civic_address.h"
#include "geographic_area.h"
#include "gnss_positioning_method_and_usage.h"
#include "local_area.h"
#include "minor_location_qo_s.h"
#include "positioning_method_and_usage.h"
#include "reported_event_type.h"
#include "termination_cause.h"
#include "velocity_estimate.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_event_notify_data_s OpenAPI_event_notify_data_t;
typedef struct OpenAPI_event_notify_data_s {
    struct OpenAPI_reported_event_type_s *reported_event_type;
    char *supi;
    char *gpsi;
    char *hgmlc_call_back_uri;
    char *ldr_reference;
    struct OpenAPI_geographic_area_s *location_estimate;
    bool is_age_of_location_estimate;
    int age_of_location_estimate;
    char *timestamp_of_location_estimate;
    struct OpenAPI_civic_address_s *civic_address;
    struct OpenAPI_local_area_s *local_location_estimate;
    OpenAPI_list_t *positioning_data_list;
    OpenAPI_list_t *gnss_positioning_data_list;
    char *serving_lm_fidentification;
    struct OpenAPI_termination_cause_s *termination_cause;
    struct OpenAPI_velocity_estimate_s *velocity_estimate;
    bool is_altitude;
    double altitude;
    struct OpenAPI_minor_location_qo_s_s *achieved_qos;
    char *supported_features;
} OpenAPI_event_notify_data_t;

OpenAPI_event_notify_data_t *OpenAPI_event_notify_data_create(
    OpenAPI_reported_event_type_t *reported_event_type,
    char *supi,
    char *gpsi,
    char *hgmlc_call_back_uri,
    char *ldr_reference,
    OpenAPI_geographic_area_t *location_estimate,
    bool is_age_of_location_estimate,
    int age_of_location_estimate,
    char *timestamp_of_location_estimate,
    OpenAPI_civic_address_t *civic_address,
    OpenAPI_local_area_t *local_location_estimate,
    OpenAPI_list_t *positioning_data_list,
    OpenAPI_list_t *gnss_positioning_data_list,
    char *serving_lm_fidentification,
    OpenAPI_termination_cause_t *termination_cause,
    OpenAPI_velocity_estimate_t *velocity_estimate,
    bool is_altitude,
    double altitude,
    OpenAPI_minor_location_qo_s_t *achieved_qos,
    char *supported_features
);
void OpenAPI_event_notify_data_free(OpenAPI_event_notify_data_t *event_notify_data);
OpenAPI_event_notify_data_t *OpenAPI_event_notify_data_parseFromJSON(cJSON *event_notify_dataJSON);
cJSON *OpenAPI_event_notify_data_convertToJSON(OpenAPI_event_notify_data_t *event_notify_data);
OpenAPI_event_notify_data_t *OpenAPI_event_notify_data_copy(OpenAPI_event_notify_data_t *dst, OpenAPI_event_notify_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_event_notify_data_H_ */

