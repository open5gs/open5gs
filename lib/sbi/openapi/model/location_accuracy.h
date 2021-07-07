/*
 * location_accuracy.h
 *
 * 
 */

#ifndef _OpenAPI_location_accuracy_H_
#define _OpenAPI_location_accuracy_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "location_accuracy_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_location_accuracy_s OpenAPI_location_accuracy_t;
typedef struct OpenAPI_location_accuracy_s {
} OpenAPI_location_accuracy_t;

OpenAPI_location_accuracy_t *OpenAPI_location_accuracy_create(
);
void OpenAPI_location_accuracy_free(OpenAPI_location_accuracy_t *location_accuracy);
OpenAPI_location_accuracy_t *OpenAPI_location_accuracy_parseFromJSON(cJSON *location_accuracyJSON);
cJSON *OpenAPI_location_accuracy_convertToJSON(OpenAPI_location_accuracy_t *location_accuracy);
OpenAPI_location_accuracy_t *OpenAPI_location_accuracy_copy(OpenAPI_location_accuracy_t *dst, OpenAPI_location_accuracy_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_location_accuracy_H_ */

