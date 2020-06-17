/*
 * association_type.h
 *
 *
 */

#ifndef _OpenAPI_association_type_H_
#define _OpenAPI_association_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_association_type_s OpenAPI_association_type_t;
typedef struct OpenAPI_association_type_s {
} OpenAPI_association_type_t;

OpenAPI_association_type_t *OpenAPI_association_type_create(
    );
void OpenAPI_association_type_free(OpenAPI_association_type_t *association_type);
OpenAPI_association_type_t *OpenAPI_association_type_parseFromJSON(cJSON *association_typeJSON);
cJSON *OpenAPI_association_type_convertToJSON(OpenAPI_association_type_t *association_type);
OpenAPI_association_type_t *OpenAPI_association_type_copy(OpenAPI_association_type_t *dst, OpenAPI_association_type_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_association_type_H_ */

