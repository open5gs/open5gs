/*
 * eps_nas_security_mode.h
 *
 * 
 */

#ifndef _OpenAPI_eps_nas_security_mode_H_
#define _OpenAPI_eps_nas_security_mode_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "eps_nas_ciphering_algorithm.h"
#include "eps_nas_integrity_algorithm.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_eps_nas_security_mode_s OpenAPI_eps_nas_security_mode_t;
typedef struct OpenAPI_eps_nas_security_mode_s {
    OpenAPI_eps_nas_integrity_algorithm_e integrity_algorithm;
    OpenAPI_eps_nas_ciphering_algorithm_e ciphering_algorithm;
} OpenAPI_eps_nas_security_mode_t;

OpenAPI_eps_nas_security_mode_t *OpenAPI_eps_nas_security_mode_create(
    OpenAPI_eps_nas_integrity_algorithm_e integrity_algorithm,
    OpenAPI_eps_nas_ciphering_algorithm_e ciphering_algorithm
);
void OpenAPI_eps_nas_security_mode_free(OpenAPI_eps_nas_security_mode_t *eps_nas_security_mode);
OpenAPI_eps_nas_security_mode_t *OpenAPI_eps_nas_security_mode_parseFromJSON(cJSON *eps_nas_security_modeJSON);
cJSON *OpenAPI_eps_nas_security_mode_convertToJSON(OpenAPI_eps_nas_security_mode_t *eps_nas_security_mode);
OpenAPI_eps_nas_security_mode_t *OpenAPI_eps_nas_security_mode_copy(OpenAPI_eps_nas_security_mode_t *dst, OpenAPI_eps_nas_security_mode_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_eps_nas_security_mode_H_ */

