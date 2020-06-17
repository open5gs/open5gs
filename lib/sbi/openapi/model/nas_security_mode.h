/*
 * nas_security_mode.h
 *
 *
 */

#ifndef _OpenAPI_nas_security_mode_H_
#define _OpenAPI_nas_security_mode_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ciphering_algorithm.h"
#include "integrity_algorithm.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nas_security_mode_s OpenAPI_nas_security_mode_t;
typedef struct OpenAPI_nas_security_mode_s {
    OpenAPI_integrity_algorithm_e integrity_algorithm;
    OpenAPI_ciphering_algorithm_e ciphering_algorithm;
} OpenAPI_nas_security_mode_t;

OpenAPI_nas_security_mode_t *OpenAPI_nas_security_mode_create(
    OpenAPI_integrity_algorithm_e integrity_algorithm,
    OpenAPI_ciphering_algorithm_e ciphering_algorithm
    );
void OpenAPI_nas_security_mode_free(OpenAPI_nas_security_mode_t *nas_security_mode);
OpenAPI_nas_security_mode_t *OpenAPI_nas_security_mode_parseFromJSON(cJSON *nas_security_modeJSON);
cJSON *OpenAPI_nas_security_mode_convertToJSON(OpenAPI_nas_security_mode_t *nas_security_mode);
OpenAPI_nas_security_mode_t *OpenAPI_nas_security_mode_copy(OpenAPI_nas_security_mode_t *dst, OpenAPI_nas_security_mode_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nas_security_mode_H_ */

