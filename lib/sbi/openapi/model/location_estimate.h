/*
 * location_estimate.h
 *
 * 
 */

#ifndef _OpenAPI_location_estimate_H_
#define _OpenAPI_location_estimate_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_location_estimate_s OpenAPI_location_estimate_t;
#include "charging_user_location.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_location_estimate_s {
    struct OpenAPI_charging_user_location_s *user_location_information;
    char *horizontal_accuracy;
    char *vertical_accuracy;
};

OpenAPI_location_estimate_t *OpenAPI_location_estimate_create(
    OpenAPI_charging_user_location_t *user_location_information,
    char *horizontal_accuracy,
    char *vertical_accuracy
);
void OpenAPI_location_estimate_free(OpenAPI_location_estimate_t *location_estimate);
OpenAPI_location_estimate_t *OpenAPI_location_estimate_parseFromJSON(cJSON *location_estimateJSON);
cJSON *OpenAPI_location_estimate_convertToJSON(OpenAPI_location_estimate_t *location_estimate);
OpenAPI_location_estimate_t *OpenAPI_location_estimate_copy(OpenAPI_location_estimate_t *dst, OpenAPI_location_estimate_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_location_estimate_H_ */

