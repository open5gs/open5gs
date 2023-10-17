/*
 * mbsfn_area.h
 *
 * Contains an MBSFN area information.
 */

#ifndef _OpenAPI_mbsfn_area_H_
#define _OpenAPI_mbsfn_area_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_mbsfn_area_s OpenAPI_mbsfn_area_t;
typedef struct OpenAPI_mbsfn_area_s {
    bool is_mbsfn_area_id;
    int mbsfn_area_id;
    bool is_carrier_frequency;
    int carrier_frequency;
} OpenAPI_mbsfn_area_t;

OpenAPI_mbsfn_area_t *OpenAPI_mbsfn_area_create(
    bool is_mbsfn_area_id,
    int mbsfn_area_id,
    bool is_carrier_frequency,
    int carrier_frequency
);
void OpenAPI_mbsfn_area_free(OpenAPI_mbsfn_area_t *mbsfn_area);
OpenAPI_mbsfn_area_t *OpenAPI_mbsfn_area_parseFromJSON(cJSON *mbsfn_areaJSON);
cJSON *OpenAPI_mbsfn_area_convertToJSON(OpenAPI_mbsfn_area_t *mbsfn_area);
OpenAPI_mbsfn_area_t *OpenAPI_mbsfn_area_copy(OpenAPI_mbsfn_area_t *dst, OpenAPI_mbsfn_area_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mbsfn_area_H_ */

