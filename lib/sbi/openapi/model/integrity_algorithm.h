/*
 * integrity_algorithm.h
 *
 * 
 */

#ifndef _OpenAPI_integrity_algorithm_H_
#define _OpenAPI_integrity_algorithm_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_integrity_algorithm_NULL = 0, OpenAPI_integrity_algorithm_NIA0, OpenAPI_integrity_algorithm_NIA1, OpenAPI_integrity_algorithm_NIA2, OpenAPI_integrity_algorithm_NIA3 } OpenAPI_integrity_algorithm_e;

char* OpenAPI_integrity_algorithm_ToString(OpenAPI_integrity_algorithm_e integrity_algorithm);

OpenAPI_integrity_algorithm_e OpenAPI_integrity_algorithm_FromString(char* integrity_algorithm);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_integrity_algorithm_H_ */

