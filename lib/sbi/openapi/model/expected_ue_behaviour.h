/*
 * expected_ue_behaviour.h
 *
 *
 */

#ifndef _OpenAPI_expected_ue_behaviour_H_
#define _OpenAPI_expected_ue_behaviour_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "battery_indication_rm.h"
#include "location_area.h"
#include "scheduled_communication_time_rm.h"
#include "scheduled_communication_type_rm.h"
#include "stationary_indication_rm.h"
#include "traffic_profile_rm.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_expected_ue_behaviour_s OpenAPI_expected_ue_behaviour_t;
typedef struct OpenAPI_expected_ue_behaviour_s {
    char *af_instance_id;
    int reference_id;
    struct OpenAPI_stationary_indication_rm_s *stationary_indication;
    int communication_duration_time;
    struct OpenAPI_scheduled_communication_type_rm_s *scheduled_communication_type;
    int periodic_time;
    struct OpenAPI_scheduled_communication_time_rm_s *scheduled_communication_time;
    OpenAPI_list_t *expected_umts;
    struct OpenAPI_traffic_profile_rm_s *traffic_profile;
    struct OpenAPI_battery_indication_rm_s *battery_indication;
    char *validity_time;
} OpenAPI_expected_ue_behaviour_t;

OpenAPI_expected_ue_behaviour_t *OpenAPI_expected_ue_behaviour_create(
    char *af_instance_id,
    int reference_id,
    OpenAPI_stationary_indication_rm_t *stationary_indication,
    int communication_duration_time,
    OpenAPI_scheduled_communication_type_rm_t *scheduled_communication_type,
    int periodic_time,
    OpenAPI_scheduled_communication_time_rm_t *scheduled_communication_time,
    OpenAPI_list_t *expected_umts,
    OpenAPI_traffic_profile_rm_t *traffic_profile,
    OpenAPI_battery_indication_rm_t *battery_indication,
    char *validity_time
    );
void OpenAPI_expected_ue_behaviour_free(OpenAPI_expected_ue_behaviour_t *expected_ue_behaviour);
OpenAPI_expected_ue_behaviour_t *OpenAPI_expected_ue_behaviour_parseFromJSON(cJSON *expected_ue_behaviourJSON);
cJSON *OpenAPI_expected_ue_behaviour_convertToJSON(OpenAPI_expected_ue_behaviour_t *expected_ue_behaviour);
OpenAPI_expected_ue_behaviour_t *OpenAPI_expected_ue_behaviour_copy(OpenAPI_expected_ue_behaviour_t *dst, OpenAPI_expected_ue_behaviour_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_expected_ue_behaviour_H_ */

