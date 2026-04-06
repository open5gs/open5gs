/*
 * geo_distribution_info.h
 *
 * Represents the geographical distribution of the UEs.
 */

#ifndef _OpenAPI_geo_distribution_info_H_
#define _OpenAPI_geo_distribution_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_geo_distribution_info_s OpenAPI_geo_distribution_info_t;
#include "user_location.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_geo_distribution_info_s {
    struct OpenAPI_user_location_s *loc;
    OpenAPI_list_t *supis;
    OpenAPI_list_t *gpsis;
};

OpenAPI_geo_distribution_info_t *OpenAPI_geo_distribution_info_create(
    OpenAPI_user_location_t *loc,
    OpenAPI_list_t *supis,
    OpenAPI_list_t *gpsis
);
void OpenAPI_geo_distribution_info_free(OpenAPI_geo_distribution_info_t *geo_distribution_info);
OpenAPI_geo_distribution_info_t *OpenAPI_geo_distribution_info_parseFromJSON(cJSON *geo_distribution_infoJSON);
cJSON *OpenAPI_geo_distribution_info_convertToJSON(OpenAPI_geo_distribution_info_t *geo_distribution_info);
OpenAPI_geo_distribution_info_t *OpenAPI_geo_distribution_info_copy(OpenAPI_geo_distribution_info_t *dst, OpenAPI_geo_distribution_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_geo_distribution_info_H_ */

