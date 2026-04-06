/*
 * ue_proximity.h
 *
 * Represents the Observed or Predicted proximity information.
 */

#ifndef _OpenAPI_ue_proximity_H_
#define _OpenAPI_ue_proximity_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_ue_proximity_s OpenAPI_ue_proximity_t;
#include "location_orientation.h"
#include "ue_trajectory.h"
#include "velocity_estimate.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_ue_proximity_s {
    bool is_ue_distance;
    int ue_distance;
    struct OpenAPI_velocity_estimate_s *ue_velocity;
    bool is_avr_speed;
    float avr_speed;
    OpenAPI_location_orientation_e loc_orientation;
    OpenAPI_list_t *ue_trajectories;
    bool is_ratio;
    int ratio;
    bool is_confidence;
    int confidence;
};

OpenAPI_ue_proximity_t *OpenAPI_ue_proximity_create(
    bool is_ue_distance,
    int ue_distance,
    OpenAPI_velocity_estimate_t *ue_velocity,
    bool is_avr_speed,
    float avr_speed,
    OpenAPI_location_orientation_e loc_orientation,
    OpenAPI_list_t *ue_trajectories,
    bool is_ratio,
    int ratio,
    bool is_confidence,
    int confidence
);
void OpenAPI_ue_proximity_free(OpenAPI_ue_proximity_t *ue_proximity);
OpenAPI_ue_proximity_t *OpenAPI_ue_proximity_parseFromJSON(cJSON *ue_proximityJSON);
cJSON *OpenAPI_ue_proximity_convertToJSON(OpenAPI_ue_proximity_t *ue_proximity);
OpenAPI_ue_proximity_t *OpenAPI_ue_proximity_copy(OpenAPI_ue_proximity_t *dst, OpenAPI_ue_proximity_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_proximity_H_ */

