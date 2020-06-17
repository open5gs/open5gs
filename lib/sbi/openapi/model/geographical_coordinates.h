/*
 * geographical_coordinates.h
 *
 *
 */

#ifndef _OpenAPI_geographical_coordinates_H_
#define _OpenAPI_geographical_coordinates_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_geographical_coordinates_s OpenAPI_geographical_coordinates_t;
typedef struct OpenAPI_geographical_coordinates_s {
    double lon;
    double lat;
} OpenAPI_geographical_coordinates_t;

OpenAPI_geographical_coordinates_t *OpenAPI_geographical_coordinates_create(
    double lon,
    double lat
    );
void OpenAPI_geographical_coordinates_free(OpenAPI_geographical_coordinates_t *geographical_coordinates);
OpenAPI_geographical_coordinates_t *OpenAPI_geographical_coordinates_parseFromJSON(cJSON *geographical_coordinatesJSON);
cJSON *OpenAPI_geographical_coordinates_convertToJSON(OpenAPI_geographical_coordinates_t *geographical_coordinates);
OpenAPI_geographical_coordinates_t *OpenAPI_geographical_coordinates_copy(OpenAPI_geographical_coordinates_t *dst, OpenAPI_geographical_coordinates_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_geographical_coordinates_H_ */

