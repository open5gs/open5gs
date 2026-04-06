/*
 * dispersion_type.h
 *
 * 
 */

#ifndef _OpenAPI_dispersion_type_H_
#define _OpenAPI_dispersion_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_dispersion_type_NULL = 0, OpenAPI_dispersion_type_DVDA, OpenAPI_dispersion_type_TDA, OpenAPI_dispersion_type_DVDA_AND_TDA } OpenAPI_dispersion_type_e;

char* OpenAPI_dispersion_type_ToString(OpenAPI_dispersion_type_e dispersion_type);

OpenAPI_dispersion_type_e OpenAPI_dispersion_type_FromString(char* dispersion_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dispersion_type_H_ */

