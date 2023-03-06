/*
 * geographical_area.h
 *
 * Contains geographical area information (e.g.a civic address or shapes).
 */

#ifndef _OpenAPI_geographical_area_H_
#define _OpenAPI_geographical_area_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "civic_address.h"
#include "geographic_area.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_geographical_area_s OpenAPI_geographical_area_t;
typedef struct OpenAPI_geographical_area_s {
    struct OpenAPI_civic_address_s *civic_address;
    struct OpenAPI_geographic_area_s *shapes;
} OpenAPI_geographical_area_t;

OpenAPI_geographical_area_t *OpenAPI_geographical_area_create(
    OpenAPI_civic_address_t *civic_address,
    OpenAPI_geographic_area_t *shapes
);
void OpenAPI_geographical_area_free(OpenAPI_geographical_area_t *geographical_area);
OpenAPI_geographical_area_t *OpenAPI_geographical_area_parseFromJSON(cJSON *geographical_areaJSON);
cJSON *OpenAPI_geographical_area_convertToJSON(OpenAPI_geographical_area_t *geographical_area);
OpenAPI_geographical_area_t *OpenAPI_geographical_area_copy(OpenAPI_geographical_area_t *dst, OpenAPI_geographical_area_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_geographical_area_H_ */

