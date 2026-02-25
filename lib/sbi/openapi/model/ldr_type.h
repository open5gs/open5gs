/*
 * ldr_type.h
 *
 * Indicates LDR types.
 */

#ifndef _OpenAPI_ldr_type_H_
#define _OpenAPI_ldr_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ldr_type_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ldr_type_s OpenAPI_ldr_type_t;
typedef struct OpenAPI_ldr_type_s {
} OpenAPI_ldr_type_t;

OpenAPI_ldr_type_t *OpenAPI_ldr_type_create(void);
void OpenAPI_ldr_type_free(OpenAPI_ldr_type_t *ldr_type);
OpenAPI_ldr_type_t *OpenAPI_ldr_type_parseFromJSON(cJSON *ldr_typeJSON);
cJSON *OpenAPI_ldr_type_convertToJSON(OpenAPI_ldr_type_t *ldr_type);
OpenAPI_ldr_type_t *OpenAPI_ldr_type_copy(OpenAPI_ldr_type_t *dst, OpenAPI_ldr_type_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ldr_type_H_ */

