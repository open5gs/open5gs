/*
 * up_security.h
 *
 * 
 */

#ifndef _OpenAPI_up_security_H_
#define _OpenAPI_up_security_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "up_confidentiality.h"
#include "up_integrity.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_up_security_s OpenAPI_up_security_t;
typedef struct OpenAPI_up_security_s {
    OpenAPI_up_integrity_e up_integr;
    OpenAPI_up_confidentiality_e up_confid;
} OpenAPI_up_security_t;

OpenAPI_up_security_t *OpenAPI_up_security_create(
    OpenAPI_up_integrity_e up_integr,
    OpenAPI_up_confidentiality_e up_confid
);
void OpenAPI_up_security_free(OpenAPI_up_security_t *up_security);
OpenAPI_up_security_t *OpenAPI_up_security_parseFromJSON(cJSON *up_securityJSON);
cJSON *OpenAPI_up_security_convertToJSON(OpenAPI_up_security_t *up_security);
OpenAPI_up_security_t *OpenAPI_up_security_copy(OpenAPI_up_security_t *dst, OpenAPI_up_security_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_up_security_H_ */

