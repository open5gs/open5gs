/*
 * location_filter.h
 *
 * Describes the supported filters of LOCATION_REPORT event type
 */

#ifndef _OpenAPI_location_filter_H_
#define _OpenAPI_location_filter_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "location_filter_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_location_filter_s OpenAPI_location_filter_t;
typedef struct OpenAPI_location_filter_s {
} OpenAPI_location_filter_t;

OpenAPI_location_filter_t *OpenAPI_location_filter_create(
);
void OpenAPI_location_filter_free(OpenAPI_location_filter_t *location_filter);
OpenAPI_location_filter_t *OpenAPI_location_filter_parseFromJSON(cJSON *location_filterJSON);
cJSON *OpenAPI_location_filter_convertToJSON(OpenAPI_location_filter_t *location_filter);
OpenAPI_location_filter_t *OpenAPI_location_filter_copy(OpenAPI_location_filter_t *dst, OpenAPI_location_filter_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_location_filter_H_ */

