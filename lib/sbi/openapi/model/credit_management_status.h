/*
 * credit_management_status.h
 *
 * 
 */

#ifndef _OpenAPI_credit_management_status_H_
#define _OpenAPI_credit_management_status_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_credit_management_status_NULL = 0, OpenAPI_credit_management_status_END_USER_SER_DENIED, OpenAPI_credit_management_status_CREDIT_CTRL_NOT_APP, OpenAPI_credit_management_status_AUTH_REJECTED, OpenAPI_credit_management_status_USER_UNKNOWN, OpenAPI_credit_management_status_RATING_FAILED } OpenAPI_credit_management_status_e;

char* OpenAPI_credit_management_status_ToString(OpenAPI_credit_management_status_e credit_management_status);

OpenAPI_credit_management_status_e OpenAPI_credit_management_status_FromString(char* credit_management_status);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_credit_management_status_H_ */

