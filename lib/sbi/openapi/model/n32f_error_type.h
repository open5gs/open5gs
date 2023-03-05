/*
 * n32f_error_type.h
 *
 * 
 */

#ifndef _OpenAPI_n32f_error_type_H_
#define _OpenAPI_n32f_error_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_n32f_error_type_NULL = 0, OpenAPI_n32f_error_type_INTEGRITY_CHECK_FAILED, OpenAPI_n32f_error_type_INTEGRITY_CHECK_ON_MODIFICATIONS_FAILED, OpenAPI_n32f_error_type_MODIFICATIONS_INSTRUCTIONS_FAILED, OpenAPI_n32f_error_type_DECIPHERING_FAILED, OpenAPI_n32f_error_type_MESSAGE_RECONSTRUCTION_FAILED, OpenAPI_n32f_error_type_CONTEXT_NOT_FOUND, OpenAPI_n32f_error_type_INTEGRITY_KEY_EXPIRED, OpenAPI_n32f_error_type_ENCRYPTION_KEY_EXPIRED, OpenAPI_n32f_error_type_POLICY_MISMATCH } OpenAPI_n32f_error_type_e;

char* OpenAPI_n32f_error_type_ToString(OpenAPI_n32f_error_type_e n32f_error_type);

OpenAPI_n32f_error_type_e OpenAPI_n32f_error_type_FromString(char* n32f_error_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_n32f_error_type_H_ */

