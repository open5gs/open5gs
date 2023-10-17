/*
 * ue_mobility.h
 *
 * Represents UE mobility information.
 */

#ifndef _OpenAPI_ue_mobility_H_
#define _OpenAPI_ue_mobility_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "location_info.h"
#include "scheduled_communication_time_1.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ue_mobility_s OpenAPI_ue_mobility_t;
typedef struct OpenAPI_ue_mobility_s {
    char *ts;
    struct OpenAPI_scheduled_communication_time_1_s *recurring_time;
    bool is_duration;
    int duration;
    bool is_duration_variance;
    float duration_variance;
    OpenAPI_list_t *loc_infos;
} OpenAPI_ue_mobility_t;

OpenAPI_ue_mobility_t *OpenAPI_ue_mobility_create(
    char *ts,
    OpenAPI_scheduled_communication_time_1_t *recurring_time,
    bool is_duration,
    int duration,
    bool is_duration_variance,
    float duration_variance,
    OpenAPI_list_t *loc_infos
);
void OpenAPI_ue_mobility_free(OpenAPI_ue_mobility_t *ue_mobility);
OpenAPI_ue_mobility_t *OpenAPI_ue_mobility_parseFromJSON(cJSON *ue_mobilityJSON);
cJSON *OpenAPI_ue_mobility_convertToJSON(OpenAPI_ue_mobility_t *ue_mobility);
OpenAPI_ue_mobility_t *OpenAPI_ue_mobility_copy(OpenAPI_ue_mobility_t *dst, OpenAPI_ue_mobility_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_mobility_H_ */

