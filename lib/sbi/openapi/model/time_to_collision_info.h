/*
 * time_to_collision_info.h
 *
 * Represents Time To Collision (TTC) information.
 */

#ifndef _OpenAPI_time_to_collision_info_H_
#define _OpenAPI_time_to_collision_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_time_to_collision_info_s OpenAPI_time_to_collision_info_t;
#include "point_altitude_uncertainty.h"
#include "range_direction.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_time_to_collision_info_s {
    char *ttc;
    bool is_accuracy;
    int accuracy;
    bool is_confidence;
    int confidence;
    struct OpenAPI_point_altitude_uncertainty_s *collision_space;
    bool is_col_spc_confidence;
    int col_spc_confidence;
    struct OpenAPI_range_direction_s *colli_direction;
    bool is_col_dir_confidence;
    int col_dir_confidence;
};

OpenAPI_time_to_collision_info_t *OpenAPI_time_to_collision_info_create(
    char *ttc,
    bool is_accuracy,
    int accuracy,
    bool is_confidence,
    int confidence,
    OpenAPI_point_altitude_uncertainty_t *collision_space,
    bool is_col_spc_confidence,
    int col_spc_confidence,
    OpenAPI_range_direction_t *colli_direction,
    bool is_col_dir_confidence,
    int col_dir_confidence
);
void OpenAPI_time_to_collision_info_free(OpenAPI_time_to_collision_info_t *time_to_collision_info);
OpenAPI_time_to_collision_info_t *OpenAPI_time_to_collision_info_parseFromJSON(cJSON *time_to_collision_infoJSON);
cJSON *OpenAPI_time_to_collision_info_convertToJSON(OpenAPI_time_to_collision_info_t *time_to_collision_info);
OpenAPI_time_to_collision_info_t *OpenAPI_time_to_collision_info_copy(OpenAPI_time_to_collision_info_t *dst, OpenAPI_time_to_collision_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_time_to_collision_info_H_ */

