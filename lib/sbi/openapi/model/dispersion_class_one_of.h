/*
 * dispersion_class_one_of.h
 *
 * 
 */

#ifndef _OpenAPI_dispersion_class_one_of_H_
#define _OpenAPI_dispersion_class_one_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_dispersion_class_one_of_NULL = 0, OpenAPI_dispersion_class_one_of_FIXED, OpenAPI_dispersion_class_one_of_CAMPER, OpenAPI_dispersion_class_one_of_TRAVELLER, OpenAPI_dispersion_class_one_of_TOP_HEAVY } OpenAPI_dispersion_class_one_of_e;

char* OpenAPI_dispersion_class_one_of_ToString(OpenAPI_dispersion_class_one_of_e dispersion_class_one_of);

OpenAPI_dispersion_class_one_of_e OpenAPI_dispersion_class_one_of_FromString(char* dispersion_class_one_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dispersion_class_one_of_H_ */

