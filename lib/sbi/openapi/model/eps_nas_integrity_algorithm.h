/*
 * eps_nas_integrity_algorithm.h
 *
 * 
 */

#ifndef _OpenAPI_eps_nas_integrity_algorithm_H_
#define _OpenAPI_eps_nas_integrity_algorithm_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_eps_nas_integrity_algorithm_NULL = 0, OpenAPI_eps_nas_integrity_algorithm_EIA0, OpenAPI_eps_nas_integrity_algorithm_EIA1, OpenAPI_eps_nas_integrity_algorithm_EIA2, OpenAPI_eps_nas_integrity_algorithm_EIA3 } OpenAPI_eps_nas_integrity_algorithm_e;

char* OpenAPI_eps_nas_integrity_algorithm_ToString(OpenAPI_eps_nas_integrity_algorithm_e eps_nas_integrity_algorithm);

OpenAPI_eps_nas_integrity_algorithm_e OpenAPI_eps_nas_integrity_algorithm_FromString(char* eps_nas_integrity_algorithm);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_eps_nas_integrity_algorithm_H_ */

