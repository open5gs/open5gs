/*
 * expected_ue_behaviour_data.h
 *
 * 
 */

#ifndef _OpenAPI_expected_ue_behaviour_data_H_
#define _OpenAPI_expected_ue_behaviour_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "battery_indication.h"
#include "location_area.h"
#include "scheduled_communication_time.h"
#include "scheduled_communication_type.h"
#include "stationary_indication.h"
#include "traffic_profile.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_expected_ue_behaviour_data_s OpenAPI_expected_ue_behaviour_data_t;
typedef struct OpenAPI_expected_ue_behaviour_data_s {
    OpenAPI_stationary_indication_e stationary_indication;
    bool is_communication_duration_time;
    int communication_duration_time;
    bool is_periodic_time;
    int periodic_time;
    struct OpenAPI_scheduled_communication_time_s *scheduled_communication_time;
    OpenAPI_scheduled_communication_type_e scheduled_communication_type;
    OpenAPI_list_t *expected_umts;
    OpenAPI_traffic_profile_e traffic_profile;
    struct OpenAPI_battery_indication_s *battery_indication;
    char *validity_time;
} OpenAPI_expected_ue_behaviour_data_t;

OpenAPI_expected_ue_behaviour_data_t *OpenAPI_expected_ue_behaviour_data_create(
    OpenAPI_stationary_indication_e stationary_indication,
    bool is_communication_duration_time,
    int communication_duration_time,
    bool is_periodic_time,
    int periodic_time,
    OpenAPI_scheduled_communication_time_t *scheduled_communication_time,
    OpenAPI_scheduled_communication_type_e scheduled_communication_type,
    OpenAPI_list_t *expected_umts,
    OpenAPI_traffic_profile_e traffic_profile,
    OpenAPI_battery_indication_t *battery_indication,
    char *validity_time
);
void OpenAPI_expected_ue_behaviour_data_free(OpenAPI_expected_ue_behaviour_data_t *expected_ue_behaviour_data);
OpenAPI_expected_ue_behaviour_data_t *OpenAPI_expected_ue_behaviour_data_parseFromJSON(cJSON *expected_ue_behaviour_dataJSON);
cJSON *OpenAPI_expected_ue_behaviour_data_convertToJSON(OpenAPI_expected_ue_behaviour_data_t *expected_ue_behaviour_data);
OpenAPI_expected_ue_behaviour_data_t *OpenAPI_expected_ue_behaviour_data_copy(OpenAPI_expected_ue_behaviour_data_t *dst, OpenAPI_expected_ue_behaviour_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_expected_ue_behaviour_data_H_ */

