/*
 * accuracy.h
 *
 * Possible values are: - LOW: Low accuracy.   - HIGH: High accuracy. 
 */

#ifndef _OpenAPI_accuracy_H_
#define _OpenAPI_accuracy_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "accuracy_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_accuracy_s OpenAPI_accuracy_t;
typedef struct OpenAPI_accuracy_s {
} OpenAPI_accuracy_t;

OpenAPI_accuracy_t *OpenAPI_accuracy_create(
);
void OpenAPI_accuracy_free(OpenAPI_accuracy_t *accuracy);
OpenAPI_accuracy_t *OpenAPI_accuracy_parseFromJSON(cJSON *accuracyJSON);
cJSON *OpenAPI_accuracy_convertToJSON(OpenAPI_accuracy_t *accuracy);
OpenAPI_accuracy_t *OpenAPI_accuracy_copy(OpenAPI_accuracy_t *dst, OpenAPI_accuracy_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_accuracy_H_ */

