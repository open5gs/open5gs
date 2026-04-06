/*
 * mov_behav_info.h
 *
 * Represents the Movement Behaviour information.
 */

#ifndef _OpenAPI_mov_behav_info_H_
#define _OpenAPI_mov_behav_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_mov_behav_info_s OpenAPI_mov_behav_info_t;
#include "geographical_coordinates.h"
#include "mov_behav.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_mov_behav_info_s {
    struct OpenAPI_geographical_coordinates_s *geo_loc;
    bool is_altitude;
    double altitude;
    OpenAPI_list_t *supis;
    OpenAPI_list_t *gpsis;
    OpenAPI_list_t *mov_behavs;
    bool is_confidence;
    int confidence;
};

OpenAPI_mov_behav_info_t *OpenAPI_mov_behav_info_create(
    OpenAPI_geographical_coordinates_t *geo_loc,
    bool is_altitude,
    double altitude,
    OpenAPI_list_t *supis,
    OpenAPI_list_t *gpsis,
    OpenAPI_list_t *mov_behavs,
    bool is_confidence,
    int confidence
);
void OpenAPI_mov_behav_info_free(OpenAPI_mov_behav_info_t *mov_behav_info);
OpenAPI_mov_behav_info_t *OpenAPI_mov_behav_info_parseFromJSON(cJSON *mov_behav_infoJSON);
cJSON *OpenAPI_mov_behav_info_convertToJSON(OpenAPI_mov_behav_info_t *mov_behav_info);
OpenAPI_mov_behav_info_t *OpenAPI_mov_behav_info_copy(OpenAPI_mov_behav_info_t *dst, OpenAPI_mov_behav_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mov_behav_info_H_ */

