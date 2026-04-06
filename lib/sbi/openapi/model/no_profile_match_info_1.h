/*
 * no_profile_match_info_1.h
 *
 * Provides the reason for not finding NF matching the search criteria
 */

#ifndef _OpenAPI_no_profile_match_info_1_H_
#define _OpenAPI_no_profile_match_info_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_no_profile_match_info_1_s OpenAPI_no_profile_match_info_1_t;
#include "no_profile_match_reason.h"
#include "query_param_combination.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_no_profile_match_info_1_s {
    OpenAPI_no_profile_match_reason_e reason;
    OpenAPI_list_t *query_param_combination_list;
};

OpenAPI_no_profile_match_info_1_t *OpenAPI_no_profile_match_info_1_create(
    OpenAPI_no_profile_match_reason_e reason,
    OpenAPI_list_t *query_param_combination_list
);
void OpenAPI_no_profile_match_info_1_free(OpenAPI_no_profile_match_info_1_t *no_profile_match_info_1);
OpenAPI_no_profile_match_info_1_t *OpenAPI_no_profile_match_info_1_parseFromJSON(cJSON *no_profile_match_info_1JSON);
cJSON *OpenAPI_no_profile_match_info_1_convertToJSON(OpenAPI_no_profile_match_info_1_t *no_profile_match_info_1);
OpenAPI_no_profile_match_info_1_t *OpenAPI_no_profile_match_info_1_copy(OpenAPI_no_profile_match_info_1_t *dst, OpenAPI_no_profile_match_info_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_no_profile_match_info_1_H_ */

