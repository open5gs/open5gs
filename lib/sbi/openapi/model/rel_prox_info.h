/*
 * rel_prox_info.h
 *
 * Represents the Relative Proximity information.
 */

#ifndef _OpenAPI_rel_prox_info_H_
#define _OpenAPI_rel_prox_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_rel_prox_info_s OpenAPI_rel_prox_info_t;
#include "time_to_collision_info.h"
#include "ue_proximity.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_rel_prox_info_s {
    char *ts_start;
    int ts_duration;
    OpenAPI_list_t *supis;
    OpenAPI_list_t *gpsis;
    OpenAPI_list_t *ue_proximities;
    struct OpenAPI_time_to_collision_info_s *ttc_info;
    OpenAPI_list_t *int_group_ids;
    OpenAPI_list_t *exter_group_ids;
};

OpenAPI_rel_prox_info_t *OpenAPI_rel_prox_info_create(
    char *ts_start,
    int ts_duration,
    OpenAPI_list_t *supis,
    OpenAPI_list_t *gpsis,
    OpenAPI_list_t *ue_proximities,
    OpenAPI_time_to_collision_info_t *ttc_info,
    OpenAPI_list_t *int_group_ids,
    OpenAPI_list_t *exter_group_ids
);
void OpenAPI_rel_prox_info_free(OpenAPI_rel_prox_info_t *rel_prox_info);
OpenAPI_rel_prox_info_t *OpenAPI_rel_prox_info_parseFromJSON(cJSON *rel_prox_infoJSON);
cJSON *OpenAPI_rel_prox_info_convertToJSON(OpenAPI_rel_prox_info_t *rel_prox_info);
OpenAPI_rel_prox_info_t *OpenAPI_rel_prox_info_copy(OpenAPI_rel_prox_info_t *dst, OpenAPI_rel_prox_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_rel_prox_info_H_ */

