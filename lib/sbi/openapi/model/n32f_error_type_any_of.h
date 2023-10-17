/*
 * n32f_error_type_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_n32f_error_type_any_of_H_
#define _OpenAPI_n32f_error_type_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_n32f_error_type_any_of_NULL = 0, OpenAPI_n32f_error_type_any_of_INTEGRITY_CHECK_FAILED, OpenAPI_n32f_error_type_any_of_INTEGRITY_CHECK_ON_MODIFICATIONS_FAILED, OpenAPI_n32f_error_type_any_of_MODIFICATIONS_INSTRUCTIONS_FAILED, OpenAPI_n32f_error_type_any_of_DECIPHERING_FAILED, OpenAPI_n32f_error_type_any_of_MESSAGE_RECONSTRUCTION_FAILED, OpenAPI_n32f_error_type_any_of_CONTEXT_NOT_FOUND, OpenAPI_n32f_error_type_any_of_INTEGRITY_KEY_EXPIRED, OpenAPI_n32f_error_type_any_of_ENCRYPTION_KEY_EXPIRED, OpenAPI_n32f_error_type_any_of_POLICY_MISMATCH } OpenAPI_n32f_error_type_any_of_e;

char* OpenAPI_n32f_error_type_any_of_ToString(OpenAPI_n32f_error_type_any_of_e n32f_error_type_any_of);

OpenAPI_n32f_error_type_any_of_e OpenAPI_n32f_error_type_any_of_FromString(char* n32f_error_type_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_n32f_error_type_any_of_H_ */

