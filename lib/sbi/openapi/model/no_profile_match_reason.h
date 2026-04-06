/*
 * no_profile_match_reason.h
 *
 * No Profile Match Reason
 */

#ifndef _OpenAPI_no_profile_match_reason_H_
#define _OpenAPI_no_profile_match_reason_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_no_profile_match_reason_NULL = 0, OpenAPI_no_profile_match_reason_REQUESTER_PLMN_NOT_ALLOWED, OpenAPI_no_profile_match_reason_TARGET_NF_SUSPENDED, OpenAPI_no_profile_match_reason_TARGET_NF_UNDISCOVERABLE, OpenAPI_no_profile_match_reason_QUERY_PARAMS_COMBINATION_NO_MATCH, OpenAPI_no_profile_match_reason_TARGET_NF_TYPE_NOT_SUPPORTED, OpenAPI_no_profile_match_reason_UNSPECIFIED } OpenAPI_no_profile_match_reason_e;

char* OpenAPI_no_profile_match_reason_ToString(OpenAPI_no_profile_match_reason_e no_profile_match_reason);

OpenAPI_no_profile_match_reason_e OpenAPI_no_profile_match_reason_FromString(char* no_profile_match_reason);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_no_profile_match_reason_H_ */

