/*
 * mov_behav_req.h
 *
 * Represents the Movement Behaviour analytics requirements.
 */

#ifndef _OpenAPI_mov_behav_req_H_
#define _OpenAPI_mov_behav_req_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_mov_behav_req_s OpenAPI_mov_behav_req_t;
#include "loc_info_granularity.h"
#include "threshold_level.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_mov_behav_req_s {
    OpenAPI_loc_info_granularity_e location_gran_req;
    struct OpenAPI_threshold_level_s *report_thresholds;
};

OpenAPI_mov_behav_req_t *OpenAPI_mov_behav_req_create(
    OpenAPI_loc_info_granularity_e location_gran_req,
    OpenAPI_threshold_level_t *report_thresholds
);
void OpenAPI_mov_behav_req_free(OpenAPI_mov_behav_req_t *mov_behav_req);
OpenAPI_mov_behav_req_t *OpenAPI_mov_behav_req_parseFromJSON(cJSON *mov_behav_reqJSON);
cJSON *OpenAPI_mov_behav_req_convertToJSON(OpenAPI_mov_behav_req_t *mov_behav_req);
OpenAPI_mov_behav_req_t *OpenAPI_mov_behav_req_copy(OpenAPI_mov_behav_req_t *dst, OpenAPI_mov_behav_req_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mov_behav_req_H_ */

