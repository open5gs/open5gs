/*
 * charging_location_info.h
 *
 * Represents UE location information.
 */

#ifndef _OpenAPI_charging_location_info_H_
#define _OpenAPI_charging_location_info_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_charging_location_info_s OpenAPI_charging_location_info_t;
#include "charging_geo_distribution_info.h"
#include "charging_user_location.h"
#include "geographical_area.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_charging_location_info_s {
    struct OpenAPI_charging_user_location_s *loc;
    struct OpenAPI_geographical_area_s *geo_loc;
    bool is_ratio;
    int ratio;
    bool is_confidence;
    int confidence;
    OpenAPI_list_t *geo_distr_infos;
    bool is_dist_threshold;
    int dist_threshold;
};

OpenAPI_charging_location_info_t *OpenAPI_charging_location_info_create(
    OpenAPI_charging_user_location_t *loc,
    OpenAPI_geographical_area_t *geo_loc,
    bool is_ratio,
    int ratio,
    bool is_confidence,
    int confidence,
    OpenAPI_list_t *geo_distr_infos,
    bool is_dist_threshold,
    int dist_threshold
);
void OpenAPI_charging_location_info_free(OpenAPI_charging_location_info_t *charging_location_info);
OpenAPI_charging_location_info_t *OpenAPI_charging_location_info_parseFromJSON(cJSON *charging_location_infoJSON);
cJSON *OpenAPI_charging_location_info_convertToJSON(OpenAPI_charging_location_info_t *charging_location_info);
OpenAPI_charging_location_info_t *OpenAPI_charging_location_info_copy(OpenAPI_charging_location_info_t *dst, OpenAPI_charging_location_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_charging_location_info_H_ */

