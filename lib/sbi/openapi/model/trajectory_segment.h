/*
 * trajectory_segment.h
 *
 * Trajectory segment
 */

#ifndef _OpenAPI_trajectory_segment_H_
#define _OpenAPI_trajectory_segment_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_trajectory_segment_s OpenAPI_trajectory_segment_t;
#include "scheduled_area.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_trajectory_segment_s {
    OpenAPI_list_t *scheduled_area_list;
};

OpenAPI_trajectory_segment_t *OpenAPI_trajectory_segment_create(
    OpenAPI_list_t *scheduled_area_list
);
void OpenAPI_trajectory_segment_free(OpenAPI_trajectory_segment_t *trajectory_segment);
OpenAPI_trajectory_segment_t *OpenAPI_trajectory_segment_parseFromJSON(cJSON *trajectory_segmentJSON);
cJSON *OpenAPI_trajectory_segment_convertToJSON(OpenAPI_trajectory_segment_t *trajectory_segment);
OpenAPI_trajectory_segment_t *OpenAPI_trajectory_segment_copy(OpenAPI_trajectory_segment_t *dst, OpenAPI_trajectory_segment_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_trajectory_segment_H_ */

