/*
 * mov_behav.h
 *
 * Represents the Movement Behaviour information per time slot.
 */

#ifndef _OpenAPI_mov_behav_H_
#define _OpenAPI_mov_behav_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_mov_behav_s OpenAPI_mov_behav_t;
#include "direction_info.h"
#include "speed_threshold_info.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_mov_behav_s {
    char *ts_start;
    int ts_duration;
    bool is_num_of_ue;
    int num_of_ue;
    bool is_ratio;
    int ratio;
    bool is_avr_speed;
    float avr_speed;
    OpenAPI_list_t *speed_thresd_infos;
    OpenAPI_list_t *direction_ue_infos;
};

OpenAPI_mov_behav_t *OpenAPI_mov_behav_create(
    char *ts_start,
    int ts_duration,
    bool is_num_of_ue,
    int num_of_ue,
    bool is_ratio,
    int ratio,
    bool is_avr_speed,
    float avr_speed,
    OpenAPI_list_t *speed_thresd_infos,
    OpenAPI_list_t *direction_ue_infos
);
void OpenAPI_mov_behav_free(OpenAPI_mov_behav_t *mov_behav);
OpenAPI_mov_behav_t *OpenAPI_mov_behav_parseFromJSON(cJSON *mov_behavJSON);
cJSON *OpenAPI_mov_behav_convertToJSON(OpenAPI_mov_behav_t *mov_behav);
OpenAPI_mov_behav_t *OpenAPI_mov_behav_copy(OpenAPI_mov_behav_t *dst, OpenAPI_mov_behav_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mov_behav_H_ */

