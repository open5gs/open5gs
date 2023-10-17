/*
 * dispersion_type.h
 *
 * Possible values are:   - DVDA: Data Volume Dispersion Analytics.   - TDA: Transactions Dispersion Analytics.   - DVDA_AND_TDA: Data Volume Dispersion Analytics and Transactions Dispersion Analytics. 
 */

#ifndef _OpenAPI_dispersion_type_H_
#define _OpenAPI_dispersion_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "dispersion_type_one_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_dispersion_type_s OpenAPI_dispersion_type_t;
typedef struct OpenAPI_dispersion_type_s {
} OpenAPI_dispersion_type_t;

OpenAPI_dispersion_type_t *OpenAPI_dispersion_type_create(
);
void OpenAPI_dispersion_type_free(OpenAPI_dispersion_type_t *dispersion_type);
OpenAPI_dispersion_type_t *OpenAPI_dispersion_type_parseFromJSON(cJSON *dispersion_typeJSON);
cJSON *OpenAPI_dispersion_type_convertToJSON(OpenAPI_dispersion_type_t *dispersion_type);
OpenAPI_dispersion_type_t *OpenAPI_dispersion_type_copy(OpenAPI_dispersion_type_t *dst, OpenAPI_dispersion_type_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dispersion_type_H_ */

