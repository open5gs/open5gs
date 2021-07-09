/*
 * unrelated_class.h
 *
 * 
 */

#ifndef _OpenAPI_unrelated_class_H_
#define _OpenAPI_unrelated_class_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "default_unrelated_class.h"
#include "external_unrelated_class.h"
#include "service_type_unrelated_class.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_unrelated_class_s OpenAPI_unrelated_class_t;
typedef struct OpenAPI_unrelated_class_s {
    struct OpenAPI_default_unrelated_class_s *default_unrelated_class;
    struct OpenAPI_external_unrelated_class_s *external_unrelated_class;
    OpenAPI_list_t *service_type_unrelated_classes;
} OpenAPI_unrelated_class_t;

OpenAPI_unrelated_class_t *OpenAPI_unrelated_class_create(
    OpenAPI_default_unrelated_class_t *default_unrelated_class,
    OpenAPI_external_unrelated_class_t *external_unrelated_class,
    OpenAPI_list_t *service_type_unrelated_classes
);
void OpenAPI_unrelated_class_free(OpenAPI_unrelated_class_t *unrelated_class);
OpenAPI_unrelated_class_t *OpenAPI_unrelated_class_parseFromJSON(cJSON *unrelated_classJSON);
cJSON *OpenAPI_unrelated_class_convertToJSON(OpenAPI_unrelated_class_t *unrelated_class);
OpenAPI_unrelated_class_t *OpenAPI_unrelated_class_copy(OpenAPI_unrelated_class_t *dst, OpenAPI_unrelated_class_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_unrelated_class_H_ */

