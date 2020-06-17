/*
 * core_network_type.h
 *
 *
 */

#ifndef _OpenAPI_core_network_type_H_
#define _OpenAPI_core_network_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_core_network_type_s OpenAPI_core_network_type_t;
typedef struct OpenAPI_core_network_type_s {
} OpenAPI_core_network_type_t;

OpenAPI_core_network_type_t *OpenAPI_core_network_type_create(
    );
void OpenAPI_core_network_type_free(OpenAPI_core_network_type_t *core_network_type);
OpenAPI_core_network_type_t *OpenAPI_core_network_type_parseFromJSON(cJSON *core_network_typeJSON);
cJSON *OpenAPI_core_network_type_convertToJSON(OpenAPI_core_network_type_t *core_network_type);
OpenAPI_core_network_type_t *OpenAPI_core_network_type_copy(OpenAPI_core_network_type_t *dst, OpenAPI_core_network_type_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_core_network_type_H_ */

