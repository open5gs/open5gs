/*
 * model_2_g3_g_location_area_range.h
 *
 * 2G/3G Location Area Range.
 */

#ifndef _OpenAPI_model_2_g3_g_location_area_range_H_
#define _OpenAPI_model_2_g3_g_location_area_range_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_model_2_g3_g_location_area_range_s OpenAPI_model_2_g3_g_location_area_range_t;
#include "location_area_id_range.h"
#include "routing_area_id_range.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_model_2_g3_g_location_area_range_s {
    struct OpenAPI_location_area_id_range_s *lai_range;
    struct OpenAPI_routing_area_id_range_s *rai_range;
};

OpenAPI_model_2_g3_g_location_area_range_t *OpenAPI_model_2_g3_g_location_area_range_create(
    OpenAPI_location_area_id_range_t *lai_range,
    OpenAPI_routing_area_id_range_t *rai_range
);
void OpenAPI_model_2_g3_g_location_area_range_free(OpenAPI_model_2_g3_g_location_area_range_t *model_2_g3_g_location_area_range);
OpenAPI_model_2_g3_g_location_area_range_t *OpenAPI_model_2_g3_g_location_area_range_parseFromJSON(cJSON *model_2_g3_g_location_area_rangeJSON);
cJSON *OpenAPI_model_2_g3_g_location_area_range_convertToJSON(OpenAPI_model_2_g3_g_location_area_range_t *model_2_g3_g_location_area_range);
OpenAPI_model_2_g3_g_location_area_range_t *OpenAPI_model_2_g3_g_location_area_range_copy(OpenAPI_model_2_g3_g_location_area_range_t *dst, OpenAPI_model_2_g3_g_location_area_range_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_model_2_g3_g_location_area_range_H_ */

