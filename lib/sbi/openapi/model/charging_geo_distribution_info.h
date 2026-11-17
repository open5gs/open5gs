/*
 * charging_geo_distribution_info.h
 *
 * Represents the geographical distribution of the UEs.
 */

#ifndef _OpenAPI_charging_geo_distribution_info_H_
#define _OpenAPI_charging_geo_distribution_info_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_charging_geo_distribution_info_s OpenAPI_charging_geo_distribution_info_t;
#include "charging_user_location.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_charging_geo_distribution_info_s {
    struct OpenAPI_charging_user_location_s *loc;
    OpenAPI_list_t *supis;
    OpenAPI_list_t *gpsis;
};

OpenAPI_charging_geo_distribution_info_t *OpenAPI_charging_geo_distribution_info_create(
    OpenAPI_charging_user_location_t *loc,
    OpenAPI_list_t *supis,
    OpenAPI_list_t *gpsis
);
void OpenAPI_charging_geo_distribution_info_free(OpenAPI_charging_geo_distribution_info_t *charging_geo_distribution_info);
OpenAPI_charging_geo_distribution_info_t *OpenAPI_charging_geo_distribution_info_parseFromJSON(cJSON *charging_geo_distribution_infoJSON);
cJSON *OpenAPI_charging_geo_distribution_info_convertToJSON(OpenAPI_charging_geo_distribution_info_t *charging_geo_distribution_info);
OpenAPI_charging_geo_distribution_info_t *OpenAPI_charging_geo_distribution_info_copy(OpenAPI_charging_geo_distribution_info_t *dst, OpenAPI_charging_geo_distribution_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_charging_geo_distribution_info_H_ */

