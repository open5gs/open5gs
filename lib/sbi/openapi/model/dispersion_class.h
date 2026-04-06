/*
 * dispersion_class.h
 *
 * 
 */

#ifndef _OpenAPI_dispersion_class_H_
#define _OpenAPI_dispersion_class_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_dispersion_class_NULL = 0, OpenAPI_dispersion_class_FIXED, OpenAPI_dispersion_class_CAMPER, OpenAPI_dispersion_class_TRAVELLER, OpenAPI_dispersion_class_TOP_HEAVY } OpenAPI_dispersion_class_e;

char* OpenAPI_dispersion_class_ToString(OpenAPI_dispersion_class_e dispersion_class);

OpenAPI_dispersion_class_e OpenAPI_dispersion_class_FromString(char* dispersion_class);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dispersion_class_H_ */

