/*
 * eps_nas_ciphering_algorithm.h
 *
 * 
 */

#ifndef _OpenAPI_eps_nas_ciphering_algorithm_H_
#define _OpenAPI_eps_nas_ciphering_algorithm_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_eps_nas_ciphering_algorithm_NULL = 0, OpenAPI_eps_nas_ciphering_algorithm_EEA0, OpenAPI_eps_nas_ciphering_algorithm_EEA1, OpenAPI_eps_nas_ciphering_algorithm_EEA2, OpenAPI_eps_nas_ciphering_algorithm_EEA3 } OpenAPI_eps_nas_ciphering_algorithm_e;

char* OpenAPI_eps_nas_ciphering_algorithm_ToString(OpenAPI_eps_nas_ciphering_algorithm_e eps_nas_ciphering_algorithm);

OpenAPI_eps_nas_ciphering_algorithm_e OpenAPI_eps_nas_ciphering_algorithm_FromString(char* eps_nas_ciphering_algorithm);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_eps_nas_ciphering_algorithm_H_ */

