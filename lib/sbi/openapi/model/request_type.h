/*
 * request_type.h
 *
 * Possible values are - INITIAL_REQUEST - EXISTING_PDU_SESSION - INITIAL_EMERGENCY_REQUEST - EXISTING_EMERGENCY_PDU_SESSION
 */

#ifndef _OpenAPI_request_type_H_
#define _OpenAPI_request_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_request_type_s OpenAPI_request_type_t;
typedef struct OpenAPI_request_type_s {
} OpenAPI_request_type_t;

OpenAPI_request_type_t *OpenAPI_request_type_create(
    );
void OpenAPI_request_type_free(OpenAPI_request_type_t *request_type);
OpenAPI_request_type_t *OpenAPI_request_type_parseFromJSON(cJSON *request_typeJSON);
cJSON *OpenAPI_request_type_convertToJSON(OpenAPI_request_type_t *request_type);
OpenAPI_request_type_t *OpenAPI_request_type_copy(OpenAPI_request_type_t *dst, OpenAPI_request_type_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_request_type_H_ */

