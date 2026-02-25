/*
 * accuracy_fulfilment_indicator.h
 *
 * Indicates fulfilment of requested accuracy.
 */

#ifndef _OpenAPI_accuracy_fulfilment_indicator_H_
#define _OpenAPI_accuracy_fulfilment_indicator_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "accuracy_fulfilment_indicator_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_accuracy_fulfilment_indicator_s OpenAPI_accuracy_fulfilment_indicator_t;
typedef struct OpenAPI_accuracy_fulfilment_indicator_s {
} OpenAPI_accuracy_fulfilment_indicator_t;

OpenAPI_accuracy_fulfilment_indicator_t *OpenAPI_accuracy_fulfilment_indicator_create(void);
void OpenAPI_accuracy_fulfilment_indicator_free(OpenAPI_accuracy_fulfilment_indicator_t *accuracy_fulfilment_indicator);
OpenAPI_accuracy_fulfilment_indicator_t *OpenAPI_accuracy_fulfilment_indicator_parseFromJSON(cJSON *accuracy_fulfilment_indicatorJSON);
cJSON *OpenAPI_accuracy_fulfilment_indicator_convertToJSON(OpenAPI_accuracy_fulfilment_indicator_t *accuracy_fulfilment_indicator);
OpenAPI_accuracy_fulfilment_indicator_t *OpenAPI_accuracy_fulfilment_indicator_copy(OpenAPI_accuracy_fulfilment_indicator_t *dst, OpenAPI_accuracy_fulfilment_indicator_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_accuracy_fulfilment_indicator_H_ */

