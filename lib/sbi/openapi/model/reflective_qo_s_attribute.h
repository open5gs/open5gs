/*
 * reflective_qo_s_attribute.h
 *
 *
 */

#ifndef _OpenAPI_reflective_qo_s_attribute_H_
#define _OpenAPI_reflective_qo_s_attribute_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_reflective_qo_s_attribute_s OpenAPI_reflective_qo_s_attribute_t;
typedef struct OpenAPI_reflective_qo_s_attribute_s {
} OpenAPI_reflective_qo_s_attribute_t;

OpenAPI_reflective_qo_s_attribute_t *OpenAPI_reflective_qo_s_attribute_create(
    );
void OpenAPI_reflective_qo_s_attribute_free(OpenAPI_reflective_qo_s_attribute_t *reflective_qo_s_attribute);
OpenAPI_reflective_qo_s_attribute_t *OpenAPI_reflective_qo_s_attribute_parseFromJSON(cJSON *reflective_qo_s_attributeJSON);
cJSON *OpenAPI_reflective_qo_s_attribute_convertToJSON(OpenAPI_reflective_qo_s_attribute_t *reflective_qo_s_attribute);
OpenAPI_reflective_qo_s_attribute_t *OpenAPI_reflective_qo_s_attribute_copy(OpenAPI_reflective_qo_s_attribute_t *dst, OpenAPI_reflective_qo_s_attribute_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_reflective_qo_s_attribute_H_ */

