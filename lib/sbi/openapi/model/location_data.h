/*
 * location_data.h
 *
 * Information within Determine Location Response.
 */

#ifndef _OpenAPI_location_data_H_
#define _OpenAPI_location_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "accuracy_fulfilment_indicator.h"
#include "civic_address.h"
#include "ecgi.h"
#include "geographic_area.h"
#include "gnss_positioning_method_and_usage.h"
#include "local_area.h"
#include "minor_location_qo_s.h"
#include "ncgi.h"
#include "positioning_method_and_usage.h"
#include "ue_area_indication.h"
#include "velocity_estimate.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_location_data_s OpenAPI_location_data_t;
typedef struct OpenAPI_location_data_s {
    struct OpenAPI_geographic_area_s *location_estimate;
    struct OpenAPI_accuracy_fulfilment_indicator_s *accuracy_fulfilment_indicator;
    bool is_age_of_location_estimate;
    int age_of_location_estimate;
    char *timestamp_of_location_estimate;
    struct OpenAPI_velocity_estimate_s *velocity_estimate;
    struct OpenAPI_civic_address_s *civic_address;
    struct OpenAPI_local_area_s *local_location_estimate;
    OpenAPI_list_t *positioning_data_list;
    OpenAPI_list_t *gnss_positioning_data_list;
    struct OpenAPI_ecgi_s *ecgi;
    struct OpenAPI_ncgi_s *ncgi;
    bool is_altitude;
    double altitude;
    bool is_barometric_pressure;
    int barometric_pressure;
    char *serving_lmf_identification;
    char *ue_positioning_cap;
    struct OpenAPI_ue_area_indication_s *ue_area_ind;
    char *supported_features;
    struct OpenAPI_minor_location_qo_s_s *achieved_qos;
} OpenAPI_location_data_t;

OpenAPI_location_data_t *OpenAPI_location_data_create(
    OpenAPI_geographic_area_t *location_estimate,
    OpenAPI_accuracy_fulfilment_indicator_t *accuracy_fulfilment_indicator,
    bool is_age_of_location_estimate,
    int age_of_location_estimate,
    char *timestamp_of_location_estimate,
    OpenAPI_velocity_estimate_t *velocity_estimate,
    OpenAPI_civic_address_t *civic_address,
    OpenAPI_local_area_t *local_location_estimate,
    OpenAPI_list_t *positioning_data_list,
    OpenAPI_list_t *gnss_positioning_data_list,
    OpenAPI_ecgi_t *ecgi,
    OpenAPI_ncgi_t *ncgi,
    bool is_altitude,
    double altitude,
    bool is_barometric_pressure,
    int barometric_pressure,
    char *serving_lmf_identification,
    char *ue_positioning_cap,
    OpenAPI_ue_area_indication_t *ue_area_ind,
    char *supported_features,
    OpenAPI_minor_location_qo_s_t *achieved_qos
);
void OpenAPI_location_data_free(OpenAPI_location_data_t *location_data);
OpenAPI_location_data_t *OpenAPI_location_data_parseFromJSON(cJSON *location_dataJSON);
cJSON *OpenAPI_location_data_convertToJSON(OpenAPI_location_data_t *location_data);
OpenAPI_location_data_t *OpenAPI_location_data_copy(OpenAPI_location_data_t *dst, OpenAPI_location_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_location_data_H_ */

