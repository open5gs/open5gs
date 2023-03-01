/*
 * accuracy_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_accuracy_any_of_H_
#define _OpenAPI_accuracy_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_accuracy_any_of_NULL = 0, OpenAPI_accuracy_any_of_LOW, OpenAPI_accuracy_any_of_HIGH } OpenAPI_accuracy_any_of_e;

char* OpenAPI_accuracy_any_of_ToString(OpenAPI_accuracy_any_of_e accuracy_any_of);

OpenAPI_accuracy_any_of_e OpenAPI_accuracy_any_of_FromString(char* accuracy_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_accuracy_any_of_H_ */

