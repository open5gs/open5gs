/*
 * routing_area_id_range.h
 *
 * Routing Area ID Range.
 */

#ifndef _OpenAPI_routing_area_id_range_H_
#define _OpenAPI_routing_area_id_range_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_routing_area_id_range_s OpenAPI_routing_area_id_range_t;
#include "plmn_id.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_routing_area_id_range_s {
    struct OpenAPI_plmn_id_s *plmn_id;
    char *start_lac;
    char *end_lac;
    char *start_rac;
    char *end_rac;
};

OpenAPI_routing_area_id_range_t *OpenAPI_routing_area_id_range_create(
    OpenAPI_plmn_id_t *plmn_id,
    char *start_lac,
    char *end_lac,
    char *start_rac,
    char *end_rac
);
void OpenAPI_routing_area_id_range_free(OpenAPI_routing_area_id_range_t *routing_area_id_range);
OpenAPI_routing_area_id_range_t *OpenAPI_routing_area_id_range_parseFromJSON(cJSON *routing_area_id_rangeJSON);
cJSON *OpenAPI_routing_area_id_range_convertToJSON(OpenAPI_routing_area_id_range_t *routing_area_id_range);
OpenAPI_routing_area_id_range_t *OpenAPI_routing_area_id_range_copy(OpenAPI_routing_area_id_range_t *dst, OpenAPI_routing_area_id_range_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_routing_area_id_range_H_ */

