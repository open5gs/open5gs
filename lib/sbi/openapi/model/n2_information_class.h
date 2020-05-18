/*
 * n2_information_class.h
 *
 *
 */

#ifndef _OpenAPI_n2_information_class_H_
#define _OpenAPI_n2_information_class_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_n2_information_class_s OpenAPI_n2_information_class_t;
typedef struct OpenAPI_n2_information_class_s {
} OpenAPI_n2_information_class_t;

OpenAPI_n2_information_class_t *OpenAPI_n2_information_class_create(
    );
void OpenAPI_n2_information_class_free(OpenAPI_n2_information_class_t *n2_information_class);
OpenAPI_n2_information_class_t *OpenAPI_n2_information_class_parseFromJSON(cJSON *n2_information_classJSON);
cJSON *OpenAPI_n2_information_class_convertToJSON(OpenAPI_n2_information_class_t *n2_information_class);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_n2_information_class_H_ */

