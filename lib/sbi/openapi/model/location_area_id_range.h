/*
 * location_area_id_range.h
 *
 * Location Area ID Range.
 */

#ifndef _OpenAPI_location_area_id_range_H_
#define _OpenAPI_location_area_id_range_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_location_area_id_range_s OpenAPI_location_area_id_range_t;
#include "plmn_id.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_location_area_id_range_s {
    struct OpenAPI_plmn_id_s *plmn_id;
    char *start_lac;
    char *end_lac;
};

OpenAPI_location_area_id_range_t *OpenAPI_location_area_id_range_create(
    OpenAPI_plmn_id_t *plmn_id,
    char *start_lac,
    char *end_lac
);
void OpenAPI_location_area_id_range_free(OpenAPI_location_area_id_range_t *location_area_id_range);
OpenAPI_location_area_id_range_t *OpenAPI_location_area_id_range_parseFromJSON(cJSON *location_area_id_rangeJSON);
cJSON *OpenAPI_location_area_id_range_convertToJSON(OpenAPI_location_area_id_range_t *location_area_id_range);
OpenAPI_location_area_id_range_t *OpenAPI_location_area_id_range_copy(OpenAPI_location_area_id_range_t *dst, OpenAPI_location_area_id_range_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_location_area_id_range_H_ */

