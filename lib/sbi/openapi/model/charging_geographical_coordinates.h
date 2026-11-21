/*
 * charging_geographical_coordinates.h
 *
 * 
 */

#ifndef _OpenAPI_charging_geographical_coordinates_H_
#define _OpenAPI_charging_geographical_coordinates_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_charging_geographical_coordinates_s OpenAPI_charging_geographical_coordinates_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_charging_geographical_coordinates_s {
    bool is_latitude;
    int latitude;
    bool is_longitude;
    int longitude;
};

OpenAPI_charging_geographical_coordinates_t *OpenAPI_charging_geographical_coordinates_create(
    bool is_latitude,
    int latitude,
    bool is_longitude,
    int longitude
);
void OpenAPI_charging_geographical_coordinates_free(OpenAPI_charging_geographical_coordinates_t *charging_geographical_coordinates);
OpenAPI_charging_geographical_coordinates_t *OpenAPI_charging_geographical_coordinates_parseFromJSON(cJSON *charging_geographical_coordinatesJSON);
cJSON *OpenAPI_charging_geographical_coordinates_convertToJSON(OpenAPI_charging_geographical_coordinates_t *charging_geographical_coordinates);
OpenAPI_charging_geographical_coordinates_t *OpenAPI_charging_geographical_coordinates_copy(OpenAPI_charging_geographical_coordinates_t *dst, OpenAPI_charging_geographical_coordinates_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_charging_geographical_coordinates_H_ */

