/*
 * no_profile_match_reason_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_no_profile_match_reason_any_of_H_
#define _OpenAPI_no_profile_match_reason_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_no_profile_match_reason_any_of_NULL = 0, OpenAPI_no_profile_match_reason_any_of_REQUESTER_PLMN_NOT_ALLOWED, OpenAPI_no_profile_match_reason_any_of_TARGET_NF_SUSPENDED, OpenAPI_no_profile_match_reason_any_of_TARGET_NF_UNDISCOVERABLE, OpenAPI_no_profile_match_reason_any_of_QUERY_PARAMS_COMBINATION_NO_MATCH, OpenAPI_no_profile_match_reason_any_of_UNSPECIFIED } OpenAPI_no_profile_match_reason_any_of_e;

char* OpenAPI_no_profile_match_reason_any_of_ToString(OpenAPI_no_profile_match_reason_any_of_e no_profile_match_reason_any_of);

OpenAPI_no_profile_match_reason_any_of_e OpenAPI_no_profile_match_reason_any_of_FromString(char* no_profile_match_reason_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_no_profile_match_reason_any_of_H_ */

