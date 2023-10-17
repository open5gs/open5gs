/*
 * dispersion_class.h
 *
 * Possible values are: - FIXED: Dispersion class as fixed UE its data or transaction usage at a location or a slice, is higher than its class threshold set for its all data or transaction usage. - CAMPER: Dispersion class as camper UE, its data or transaction usage at a location or a slice, is higher than its class threshold and lower than the fixed class threshold set for its all data or transaction usage.. - TRAVELLER: Dispersion class as traveller UE, its data or transaction usage at a location or a slice, is lower than the camper class threshold set for its all data or transaction usage. - TOP_HEAVY: Dispersion class as Top_Heavy UE, who&#39;s dispersion percentile rating at a location or a slice, is higher than its class threshold. 
 */

#ifndef _OpenAPI_dispersion_class_H_
#define _OpenAPI_dispersion_class_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "dispersion_class_one_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_dispersion_class_s OpenAPI_dispersion_class_t;
typedef struct OpenAPI_dispersion_class_s {
} OpenAPI_dispersion_class_t;

OpenAPI_dispersion_class_t *OpenAPI_dispersion_class_create(
);
void OpenAPI_dispersion_class_free(OpenAPI_dispersion_class_t *dispersion_class);
OpenAPI_dispersion_class_t *OpenAPI_dispersion_class_parseFromJSON(cJSON *dispersion_classJSON);
cJSON *OpenAPI_dispersion_class_convertToJSON(OpenAPI_dispersion_class_t *dispersion_class);
OpenAPI_dispersion_class_t *OpenAPI_dispersion_class_copy(OpenAPI_dispersion_class_t *dst, OpenAPI_dispersion_class_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dispersion_class_H_ */

