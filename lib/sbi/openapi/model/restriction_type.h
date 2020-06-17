/*
 * restriction_type.h
 *
 *
 */

#ifndef _OpenAPI_restriction_type_H_
#define _OpenAPI_restriction_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_restriction_type_s OpenAPI_restriction_type_t;
typedef struct OpenAPI_restriction_type_s {
} OpenAPI_restriction_type_t;

OpenAPI_restriction_type_t *OpenAPI_restriction_type_create(
    );
void OpenAPI_restriction_type_free(OpenAPI_restriction_type_t *restriction_type);
OpenAPI_restriction_type_t *OpenAPI_restriction_type_parseFromJSON(cJSON *restriction_typeJSON);
cJSON *OpenAPI_restriction_type_convertToJSON(OpenAPI_restriction_type_t *restriction_type);
OpenAPI_restriction_type_t *OpenAPI_restriction_type_copy(OpenAPI_restriction_type_t *dst, OpenAPI_restriction_type_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_restriction_type_H_ */

