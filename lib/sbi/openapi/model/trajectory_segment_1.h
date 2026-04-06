/*
 * trajectory_segment_1.h
 *
 * A segment in assigned trajectory.
 */

#ifndef _OpenAPI_trajectory_segment_1_H_
#define _OpenAPI_trajectory_segment_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_trajectory_segment_1_s OpenAPI_trajectory_segment_1_t;
#include "location_area.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_trajectory_segment_1_s {
    OpenAPI_list_t *expected_umts;
};

OpenAPI_trajectory_segment_1_t *OpenAPI_trajectory_segment_1_create(
    OpenAPI_list_t *expected_umts
);
void OpenAPI_trajectory_segment_1_free(OpenAPI_trajectory_segment_1_t *trajectory_segment_1);
OpenAPI_trajectory_segment_1_t *OpenAPI_trajectory_segment_1_parseFromJSON(cJSON *trajectory_segment_1JSON);
cJSON *OpenAPI_trajectory_segment_1_convertToJSON(OpenAPI_trajectory_segment_1_t *trajectory_segment_1);
OpenAPI_trajectory_segment_1_t *OpenAPI_trajectory_segment_1_copy(OpenAPI_trajectory_segment_1_t *dst, OpenAPI_trajectory_segment_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_trajectory_segment_1_H_ */

