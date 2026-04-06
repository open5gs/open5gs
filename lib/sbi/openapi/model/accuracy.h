/*
 * accuracy.h
 *
 * 
 */

#ifndef _OpenAPI_accuracy_H_
#define _OpenAPI_accuracy_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_accuracy_NULL = 0, OpenAPI_accuracy_LOW, OpenAPI_accuracy_MEDIUM, OpenAPI_accuracy_HIGH, OpenAPI_accuracy_HIGHEST } OpenAPI_accuracy_e;

char* OpenAPI_accuracy_ToString(OpenAPI_accuracy_e accuracy);

OpenAPI_accuracy_e OpenAPI_accuracy_FromString(char* accuracy);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_accuracy_H_ */

