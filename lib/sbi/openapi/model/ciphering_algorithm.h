/*
 * ciphering_algorithm.h
 *
 *
 */

#ifndef _OpenAPI_ciphering_algorithm_H_
#define _OpenAPI_ciphering_algorithm_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_ciphering_algorithm_NULL = 0, OpenAPI_ciphering_algorithm_NEA0, OpenAPI_ciphering_algorithm_NEA1, OpenAPI_ciphering_algorithm_NEA2, OpenAPI_ciphering_algorithm_NEA3 } OpenAPI_ciphering_algorithm_e;

char* OpenAPI_ciphering_algorithm_ToString(OpenAPI_ciphering_algorithm_e ciphering_algorithm);

OpenAPI_ciphering_algorithm_e OpenAPI_ciphering_algorithm_FromString(char* ciphering_algorithm);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ciphering_algorithm_H_ */

