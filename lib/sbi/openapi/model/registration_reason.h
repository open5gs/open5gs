/*
 * registration_reason.h
 *
 *
 */

#ifndef _OpenAPI_registration_reason_H_
#define _OpenAPI_registration_reason_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_registration_reason_s OpenAPI_registration_reason_t;
typedef struct OpenAPI_registration_reason_s {
} OpenAPI_registration_reason_t;

OpenAPI_registration_reason_t *OpenAPI_registration_reason_create(
    );
void OpenAPI_registration_reason_free(OpenAPI_registration_reason_t *registration_reason);
OpenAPI_registration_reason_t *OpenAPI_registration_reason_parseFromJSON(cJSON *registration_reasonJSON);
cJSON *OpenAPI_registration_reason_convertToJSON(OpenAPI_registration_reason_t *registration_reason);
OpenAPI_registration_reason_t *OpenAPI_registration_reason_copy(OpenAPI_registration_reason_t *dst, OpenAPI_registration_reason_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_registration_reason_H_ */

