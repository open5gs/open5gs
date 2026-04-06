/*
 * partial_record_method.h
 *
 * 
 */

#ifndef _OpenAPI_partial_record_method_H_
#define _OpenAPI_partial_record_method_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_partial_record_method_NULL = 0, OpenAPI_partial_record_method__DEFAULT, OpenAPI_partial_record_method_INDIVIDUAL } OpenAPI_partial_record_method_e;

char* OpenAPI_partial_record_method_ToString(OpenAPI_partial_record_method_e partial_record_method);

OpenAPI_partial_record_method_e OpenAPI_partial_record_method_FromString(char* partial_record_method);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_partial_record_method_H_ */

