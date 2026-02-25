/*
 * ue_location_info.h
 *
 * Indicates location information of a UE.
 */

#ifndef _OpenAPI_ue_location_info_H_
#define _OpenAPI_ue_location_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "geographic_area.h"
#include "velocity_estimate.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ue_location_info_s OpenAPI_ue_location_info_t;
typedef struct OpenAPI_ue_location_info_s {
    struct OpenAPI_geographic_area_s *location_estimate;
    bool is_age_of_location_estimate;
    int age_of_location_estimate;
    char *timestamp_of_location_estimate;
    struct OpenAPI_velocity_estimate_s *velocity_estimate;
    bool is_age_of_velocity_estimate;
    int age_of_velocity_estimate;
    char *timestamp_of_velocity_estimate;
} OpenAPI_ue_location_info_t;

OpenAPI_ue_location_info_t *OpenAPI_ue_location_info_create(
    OpenAPI_geographic_area_t *location_estimate,
    bool is_age_of_location_estimate,
    int age_of_location_estimate,
    char *timestamp_of_location_estimate,
    OpenAPI_velocity_estimate_t *velocity_estimate,
    bool is_age_of_velocity_estimate,
    int age_of_velocity_estimate,
    char *timestamp_of_velocity_estimate
);
void OpenAPI_ue_location_info_free(OpenAPI_ue_location_info_t *ue_location_info);
OpenAPI_ue_location_info_t *OpenAPI_ue_location_info_parseFromJSON(cJSON *ue_location_infoJSON);
cJSON *OpenAPI_ue_location_info_convertToJSON(OpenAPI_ue_location_info_t *ue_location_info);
OpenAPI_ue_location_info_t *OpenAPI_ue_location_info_copy(OpenAPI_ue_location_info_t *dst, OpenAPI_ue_location_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_location_info_H_ */

