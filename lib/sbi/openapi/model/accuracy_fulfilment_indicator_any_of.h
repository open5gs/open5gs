/*
 * accuracy_fulfilment_indicator_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_accuracy_fulfilment_indicator_any_of_H_
#define _OpenAPI_accuracy_fulfilment_indicator_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_accuracy_fulfilment_indicator_any_of_NULL = 0, OpenAPI_accuracy_fulfilment_indicator_any_of_REQUESTED_ACCURACY_FULFILLED, OpenAPI_accuracy_fulfilment_indicator_any_of_REQUESTED_ACCURACY_NOT_FULFILLED } OpenAPI_accuracy_fulfilment_indicator_any_of_e;

char* OpenAPI_accuracy_fulfilment_indicator_any_of_ToString(OpenAPI_accuracy_fulfilment_indicator_any_of_e accuracy_fulfilment_indicator_any_of);

OpenAPI_accuracy_fulfilment_indicator_any_of_e OpenAPI_accuracy_fulfilment_indicator_any_of_FromString(char* accuracy_fulfilment_indicator_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_accuracy_fulfilment_indicator_any_of_H_ */

