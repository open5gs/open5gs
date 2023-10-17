/*
 * search_result.h
 *
 * Contains the list of NF Profiles returned in a Discovery response
 */

#ifndef _OpenAPI_search_result_H_
#define _OpenAPI_search_result_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "nf_instance_info.h"
#include "nf_profile.h"
#include "no_profile_match_info.h"
#include "preferred_search.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_search_result_s OpenAPI_search_result_t;
typedef struct OpenAPI_search_result_s {
    bool is_validity_period;
    int validity_period;
    OpenAPI_list_t *nf_instances;
    char *search_id;
    bool is_num_nf_inst_complete;
    int num_nf_inst_complete;
    struct OpenAPI_preferred_search_s *preferred_search;
    char *nrf_supported_features;
    OpenAPI_list_t* nf_instance_list;
    bool is_altered_priority_ind;
    int altered_priority_ind;
    struct OpenAPI_no_profile_match_info_s *no_profile_match_info;
} OpenAPI_search_result_t;

OpenAPI_search_result_t *OpenAPI_search_result_create(
    bool is_validity_period,
    int validity_period,
    OpenAPI_list_t *nf_instances,
    char *search_id,
    bool is_num_nf_inst_complete,
    int num_nf_inst_complete,
    OpenAPI_preferred_search_t *preferred_search,
    char *nrf_supported_features,
    OpenAPI_list_t* nf_instance_list,
    bool is_altered_priority_ind,
    int altered_priority_ind,
    OpenAPI_no_profile_match_info_t *no_profile_match_info
);
void OpenAPI_search_result_free(OpenAPI_search_result_t *search_result);
OpenAPI_search_result_t *OpenAPI_search_result_parseFromJSON(cJSON *search_resultJSON);
cJSON *OpenAPI_search_result_convertToJSON(OpenAPI_search_result_t *search_result);
OpenAPI_search_result_t *OpenAPI_search_result_copy(OpenAPI_search_result_t *dst, OpenAPI_search_result_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_search_result_H_ */

