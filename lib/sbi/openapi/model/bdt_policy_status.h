/*
 * bdt_policy_status.h
 *
 * Indicates the validation status of a negotiated BDT policy.
 */

#ifndef _OpenAPI_bdt_policy_status_H_
#define _OpenAPI_bdt_policy_status_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_bdt_policy_status_NULL = 0, OpenAPI_bdt_policy_status_INVALID, OpenAPI_bdt_policy_status_VALID } OpenAPI_bdt_policy_status_e;

char* OpenAPI_bdt_policy_status_ToString(OpenAPI_bdt_policy_status_e bdt_policy_status);

OpenAPI_bdt_policy_status_e OpenAPI_bdt_policy_status_FromString(char* bdt_policy_status);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_bdt_policy_status_H_ */

