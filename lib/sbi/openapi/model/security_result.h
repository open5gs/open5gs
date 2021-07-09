/*
 * security_result.h
 *
 * 
 */

#ifndef _OpenAPI_security_result_H_
#define _OpenAPI_security_result_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "protection_result.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_security_result_s OpenAPI_security_result_t;
typedef struct OpenAPI_security_result_s {
    OpenAPI_protection_result_e integrity_protection_result;
    OpenAPI_protection_result_e confidentiality_protection_result;
} OpenAPI_security_result_t;

OpenAPI_security_result_t *OpenAPI_security_result_create(
    OpenAPI_protection_result_e integrity_protection_result,
    OpenAPI_protection_result_e confidentiality_protection_result
);
void OpenAPI_security_result_free(OpenAPI_security_result_t *security_result);
OpenAPI_security_result_t *OpenAPI_security_result_parseFromJSON(cJSON *security_resultJSON);
cJSON *OpenAPI_security_result_convertToJSON(OpenAPI_security_result_t *security_result);
OpenAPI_security_result_t *OpenAPI_security_result_copy(OpenAPI_security_result_t *dst, OpenAPI_security_result_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_security_result_H_ */

