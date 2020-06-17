/*
 * deregistration_reason.h
 *
 *
 */

#ifndef _OpenAPI_deregistration_reason_H_
#define _OpenAPI_deregistration_reason_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_deregistration_reason_s OpenAPI_deregistration_reason_t;
typedef struct OpenAPI_deregistration_reason_s {
} OpenAPI_deregistration_reason_t;

OpenAPI_deregistration_reason_t *OpenAPI_deregistration_reason_create(
    );
void OpenAPI_deregistration_reason_free(OpenAPI_deregistration_reason_t *deregistration_reason);
OpenAPI_deregistration_reason_t *OpenAPI_deregistration_reason_parseFromJSON(cJSON *deregistration_reasonJSON);
cJSON *OpenAPI_deregistration_reason_convertToJSON(OpenAPI_deregistration_reason_t *deregistration_reason);
OpenAPI_deregistration_reason_t *OpenAPI_deregistration_reason_copy(OpenAPI_deregistration_reason_t *dst, OpenAPI_deregistration_reason_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_deregistration_reason_H_ */

