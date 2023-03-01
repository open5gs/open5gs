/*
 * ue_differentiation_info.h
 *
 * Represents the UE Differentiation Information and its validity time
 */

#ifndef _OpenAPI_ue_differentiation_info_H_
#define _OpenAPI_ue_differentiation_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "battery_indication.h"
#include "periodic_communication_indicator.h"
#include "scheduled_communication_time.h"
#include "stationary_indication.h"
#include "traffic_profile.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ue_differentiation_info_s OpenAPI_ue_differentiation_info_t;
typedef struct OpenAPI_ue_differentiation_info_s {
    OpenAPI_periodic_communication_indicator_e periodic_com_ind;
    bool is_periodic_time;
    int periodic_time;
    struct OpenAPI_scheduled_communication_time_s *scheduled_com_time;
    OpenAPI_stationary_indication_e stationary_ind;
    OpenAPI_traffic_profile_e traffic_profile;
    struct OpenAPI_battery_indication_s *battery_ind;
    char *validity_time;
} OpenAPI_ue_differentiation_info_t;

OpenAPI_ue_differentiation_info_t *OpenAPI_ue_differentiation_info_create(
    OpenAPI_periodic_communication_indicator_e periodic_com_ind,
    bool is_periodic_time,
    int periodic_time,
    OpenAPI_scheduled_communication_time_t *scheduled_com_time,
    OpenAPI_stationary_indication_e stationary_ind,
    OpenAPI_traffic_profile_e traffic_profile,
    OpenAPI_battery_indication_t *battery_ind,
    char *validity_time
);
void OpenAPI_ue_differentiation_info_free(OpenAPI_ue_differentiation_info_t *ue_differentiation_info);
OpenAPI_ue_differentiation_info_t *OpenAPI_ue_differentiation_info_parseFromJSON(cJSON *ue_differentiation_infoJSON);
cJSON *OpenAPI_ue_differentiation_info_convertToJSON(OpenAPI_ue_differentiation_info_t *ue_differentiation_info);
OpenAPI_ue_differentiation_info_t *OpenAPI_ue_differentiation_info_copy(OpenAPI_ue_differentiation_info_t *dst, OpenAPI_ue_differentiation_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_differentiation_info_H_ */

