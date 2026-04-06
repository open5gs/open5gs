/*
 * trajectory.h
 *
 * Trajectory
 */

#ifndef _OpenAPI_trajectory_H_
#define _OpenAPI_trajectory_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_trajectory_s OpenAPI_trajectory_t;
#include "scheduled_area.h"
#include "trajectory_segment.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_trajectory_s {
    OpenAPI_list_t *scheduled_area_list;
    OpenAPI_list_t *segment_list;
    bool is_acceptable_deviation_time;
    int acceptable_deviation_time;
};

OpenAPI_trajectory_t *OpenAPI_trajectory_create(
    OpenAPI_list_t *scheduled_area_list,
    OpenAPI_list_t *segment_list,
    bool is_acceptable_deviation_time,
    int acceptable_deviation_time
);
void OpenAPI_trajectory_free(OpenAPI_trajectory_t *trajectory);
OpenAPI_trajectory_t *OpenAPI_trajectory_parseFromJSON(cJSON *trajectoryJSON);
cJSON *OpenAPI_trajectory_convertToJSON(OpenAPI_trajectory_t *trajectory);
OpenAPI_trajectory_t *OpenAPI_trajectory_copy(OpenAPI_trajectory_t *dst, OpenAPI_trajectory_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_trajectory_H_ */

