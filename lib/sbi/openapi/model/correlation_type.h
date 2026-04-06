/*
 * correlation_type.h
 *
 * Indicates that a common DNAI or common EAS should be selected.
 */

#ifndef _OpenAPI_correlation_type_H_
#define _OpenAPI_correlation_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_correlation_type_NULL = 0, OpenAPI_correlation_type_COMMON_DNAI, OpenAPI_correlation_type_COMMON_EAS } OpenAPI_correlation_type_e;

char* OpenAPI_correlation_type_ToString(OpenAPI_correlation_type_e correlation_type);

OpenAPI_correlation_type_e OpenAPI_correlation_type_FromString(char* correlation_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_correlation_type_H_ */

