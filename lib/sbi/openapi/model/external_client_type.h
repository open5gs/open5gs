/*
 * external_client_type.h
 *
 *
 */

#ifndef _OpenAPI_external_client_type_H_
#define _OpenAPI_external_client_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_external_client_type_s OpenAPI_external_client_type_t;
typedef struct OpenAPI_external_client_type_s {
} OpenAPI_external_client_type_t;

OpenAPI_external_client_type_t *OpenAPI_external_client_type_create(
    );
void OpenAPI_external_client_type_free(OpenAPI_external_client_type_t *external_client_type);
OpenAPI_external_client_type_t *OpenAPI_external_client_type_parseFromJSON(cJSON *external_client_typeJSON);
cJSON *OpenAPI_external_client_type_convertToJSON(OpenAPI_external_client_type_t *external_client_type);
OpenAPI_external_client_type_t *OpenAPI_external_client_type_copy(OpenAPI_external_client_type_t *dst, OpenAPI_external_client_type_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_external_client_type_H_ */

