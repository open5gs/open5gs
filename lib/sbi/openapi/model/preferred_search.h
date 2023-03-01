/*
 * preferred_search.h
 *
 * Contains information on whether the returned NFProfiles match the preferred query parameters 
 */

#ifndef _OpenAPI_preferred_search_H_
#define _OpenAPI_preferred_search_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_preferred_search_s OpenAPI_preferred_search_t;
typedef struct OpenAPI_preferred_search_s {
    bool is_preferred_tai_match_ind;
    int preferred_tai_match_ind;
    bool is_preferred_full_plmn_match_ind;
    int preferred_full_plmn_match_ind;
    bool is_preferred_api_versions_match_ind;
    int preferred_api_versions_match_ind;
    bool is_other_api_versions_ind;
    int other_api_versions_ind;
    bool is_preferred_locality_match_ind;
    int preferred_locality_match_ind;
    bool is_other_locality_ind;
    int other_locality_ind;
    bool is_preferred_vendor_specific_features_ind;
    int preferred_vendor_specific_features_ind;
    bool is_preferred_collocated_nf_type_ind;
    int preferred_collocated_nf_type_ind;
    bool is_preferred_pgw_match_ind;
    int preferred_pgw_match_ind;
    bool is_preferred_analytics_delays_ind;
    int preferred_analytics_delays_ind;
} OpenAPI_preferred_search_t;

OpenAPI_preferred_search_t *OpenAPI_preferred_search_create(
    bool is_preferred_tai_match_ind,
    int preferred_tai_match_ind,
    bool is_preferred_full_plmn_match_ind,
    int preferred_full_plmn_match_ind,
    bool is_preferred_api_versions_match_ind,
    int preferred_api_versions_match_ind,
    bool is_other_api_versions_ind,
    int other_api_versions_ind,
    bool is_preferred_locality_match_ind,
    int preferred_locality_match_ind,
    bool is_other_locality_ind,
    int other_locality_ind,
    bool is_preferred_vendor_specific_features_ind,
    int preferred_vendor_specific_features_ind,
    bool is_preferred_collocated_nf_type_ind,
    int preferred_collocated_nf_type_ind,
    bool is_preferred_pgw_match_ind,
    int preferred_pgw_match_ind,
    bool is_preferred_analytics_delays_ind,
    int preferred_analytics_delays_ind
);
void OpenAPI_preferred_search_free(OpenAPI_preferred_search_t *preferred_search);
OpenAPI_preferred_search_t *OpenAPI_preferred_search_parseFromJSON(cJSON *preferred_searchJSON);
cJSON *OpenAPI_preferred_search_convertToJSON(OpenAPI_preferred_search_t *preferred_search);
OpenAPI_preferred_search_t *OpenAPI_preferred_search_copy(OpenAPI_preferred_search_t *dst, OpenAPI_preferred_search_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_preferred_search_H_ */

