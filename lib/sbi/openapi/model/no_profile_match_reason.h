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
#include "no_profile_match_reason_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_no_profile_match_reason_s OpenAPI_no_profile_match_reason_t;
typedef struct OpenAPI_no_profile_match_reason_s {
} OpenAPI_no_profile_match_reason_t;

OpenAPI_no_profile_match_reason_t *OpenAPI_no_profile_match_reason_create(
);
void OpenAPI_no_profile_match_reason_free(OpenAPI_no_profile_match_reason_t *no_profile_match_reason);
OpenAPI_no_profile_match_reason_t *OpenAPI_no_profile_match_reason_parseFromJSON(cJSON *no_profile_match_reasonJSON);
cJSON *OpenAPI_no_profile_match_reason_convertToJSON(OpenAPI_no_profile_match_reason_t *no_profile_match_reason);
OpenAPI_no_profile_match_reason_t *OpenAPI_no_profile_match_reason_copy(OpenAPI_no_profile_match_reason_t *dst, OpenAPI_no_profile_match_reason_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_no_profile_match_reason_H_ */

