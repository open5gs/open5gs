/*
 * partial_record_method_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_partial_record_method_any_of_H_
#define _OpenAPI_partial_record_method_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_partial_record_method_any_of_NULL = 0, OpenAPI_partial_record_method_any_of__DEFAULT, OpenAPI_partial_record_method_any_of_INDIVIDUAL } OpenAPI_partial_record_method_any_of_e;

char* OpenAPI_partial_record_method_any_of_ToString(OpenAPI_partial_record_method_any_of_e partial_record_method_any_of);

OpenAPI_partial_record_method_any_of_e OpenAPI_partial_record_method_any_of_FromString(char* partial_record_method_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_partial_record_method_any_of_H_ */

