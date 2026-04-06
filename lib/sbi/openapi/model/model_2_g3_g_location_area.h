/*
 * model_2_g3_g_location_area.h
 *
 * 2G/3G Location Area.
 */

#ifndef _OpenAPI_model_2_g3_g_location_area_H_
#define _OpenAPI_model_2_g3_g_location_area_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_model_2_g3_g_location_area_s OpenAPI_model_2_g3_g_location_area_t;
#include "location_area_id.h"
#include "routing_area_id.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_model_2_g3_g_location_area_s {
    struct OpenAPI_location_area_id_s *lai;
    struct OpenAPI_routing_area_id_s *rai;
};

OpenAPI_model_2_g3_g_location_area_t *OpenAPI_model_2_g3_g_location_area_create(
    OpenAPI_location_area_id_t *lai,
    OpenAPI_routing_area_id_t *rai
);
void OpenAPI_model_2_g3_g_location_area_free(OpenAPI_model_2_g3_g_location_area_t *model_2_g3_g_location_area);
OpenAPI_model_2_g3_g_location_area_t *OpenAPI_model_2_g3_g_location_area_parseFromJSON(cJSON *model_2_g3_g_location_areaJSON);
cJSON *OpenAPI_model_2_g3_g_location_area_convertToJSON(OpenAPI_model_2_g3_g_location_area_t *model_2_g3_g_location_area);
OpenAPI_model_2_g3_g_location_area_t *OpenAPI_model_2_g3_g_location_area_copy(OpenAPI_model_2_g3_g_location_area_t *dst, OpenAPI_model_2_g3_g_location_area_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_model_2_g3_g_location_area_H_ */

