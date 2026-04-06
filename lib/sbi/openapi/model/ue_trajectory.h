/*
 * ue_trajectory.h
 *
 * Represents timestamped UE positions.
 */

#ifndef _OpenAPI_ue_trajectory_H_
#define _OpenAPI_ue_trajectory_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_ue_trajectory_s OpenAPI_ue_trajectory_t;
#include "timestamped_location.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_ue_trajectory_s {
    char *supi;
    char *gpsi;
    OpenAPI_list_t *timestamped_locs;
};

OpenAPI_ue_trajectory_t *OpenAPI_ue_trajectory_create(
    char *supi,
    char *gpsi,
    OpenAPI_list_t *timestamped_locs
);
void OpenAPI_ue_trajectory_free(OpenAPI_ue_trajectory_t *ue_trajectory);
OpenAPI_ue_trajectory_t *OpenAPI_ue_trajectory_parseFromJSON(cJSON *ue_trajectoryJSON);
cJSON *OpenAPI_ue_trajectory_convertToJSON(OpenAPI_ue_trajectory_t *ue_trajectory);
OpenAPI_ue_trajectory_t *OpenAPI_ue_trajectory_copy(OpenAPI_ue_trajectory_t *dst, OpenAPI_ue_trajectory_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_trajectory_H_ */

