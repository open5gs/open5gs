/*
 * protection_result.h
 *
 * 
 */

#ifndef _OpenAPI_protection_result_H_
#define _OpenAPI_protection_result_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_protection_result_NULL = 0, OpenAPI_protection_result_PERFORMED, OpenAPI_protection_result_NOT_PERFORMED } OpenAPI_protection_result_e;

char* OpenAPI_protection_result_ToString(OpenAPI_protection_result_e protection_result);

OpenAPI_protection_result_e OpenAPI_protection_result_FromString(char* protection_result);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_protection_result_H_ */

