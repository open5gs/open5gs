/*
 * an_node_type.h
 *
 *
 */

#ifndef _OpenAPI_an_node_type_H_
#define _OpenAPI_an_node_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_an_node_type_s OpenAPI_an_node_type_t;
typedef struct OpenAPI_an_node_type_s {
} OpenAPI_an_node_type_t;

OpenAPI_an_node_type_t *OpenAPI_an_node_type_create(
    );
void OpenAPI_an_node_type_free(OpenAPI_an_node_type_t *an_node_type);
OpenAPI_an_node_type_t *OpenAPI_an_node_type_parseFromJSON(cJSON *an_node_typeJSON);
cJSON *OpenAPI_an_node_type_convertToJSON(OpenAPI_an_node_type_t *an_node_type);
OpenAPI_an_node_type_t *OpenAPI_an_node_type_copy(OpenAPI_an_node_type_t *dst, OpenAPI_an_node_type_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_an_node_type_H_ */

