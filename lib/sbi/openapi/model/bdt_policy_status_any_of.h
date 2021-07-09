/*
 * bdt_policy_status_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_bdt_policy_status_any_of_H_
#define _OpenAPI_bdt_policy_status_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_bdt_policy_status_any_of_NULL = 0, OpenAPI_bdt_policy_status_any_of_INVALID, OpenAPI_bdt_policy_status_any_of_VALID } OpenAPI_bdt_policy_status_any_of_e;

char* OpenAPI_bdt_policy_status_any_of_ToString(OpenAPI_bdt_policy_status_any_of_e bdt_policy_status_any_of);

OpenAPI_bdt_policy_status_any_of_e OpenAPI_bdt_policy_status_any_of_FromString(char* bdt_policy_status_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_bdt_policy_status_any_of_H_ */

