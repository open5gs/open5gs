/*
 * no_profile_match_info.h
 *
 * Provides the reason for not finding NF matching the search criteria
 */

#ifndef _OpenAPI_no_profile_match_info_H_
#define _OpenAPI_no_profile_match_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "no_profile_match_reason.h"
#include "query_param_combination.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_no_profile_match_info_s OpenAPI_no_profile_match_info_t;
typedef struct OpenAPI_no_profile_match_info_s {
    struct OpenAPI_no_profile_match_reason_s *reason;
    OpenAPI_list_t *query_param_combination_list;
} OpenAPI_no_profile_match_info_t;

OpenAPI_no_profile_match_info_t *OpenAPI_no_profile_match_info_create(
    OpenAPI_no_profile_match_reason_t *reason,
    OpenAPI_list_t *query_param_combination_list
);
void OpenAPI_no_profile_match_info_free(OpenAPI_no_profile_match_info_t *no_profile_match_info);
OpenAPI_no_profile_match_info_t *OpenAPI_no_profile_match_info_parseFromJSON(cJSON *no_profile_match_infoJSON);
cJSON *OpenAPI_no_profile_match_info_convertToJSON(OpenAPI_no_profile_match_info_t *no_profile_match_info);
OpenAPI_no_profile_match_info_t *OpenAPI_no_profile_match_info_copy(OpenAPI_no_profile_match_info_t *dst, OpenAPI_no_profile_match_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_no_profile_match_info_H_ */

