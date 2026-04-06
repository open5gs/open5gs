/*
 * sdm_subs_modification.h
 *
 * Contains modification information
 */

#ifndef _OpenAPI_sdm_subs_modification_H_
#define _OpenAPI_sdm_subs_modification_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_sdm_subs_modification_s OpenAPI_sdm_subs_modification_t;
#include "expected_ue_behaviour_threshold.h"
#include "plmn_id.h"
#include "snssai.h"
#include "ue_context_in_smf_data_sub_filter.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_sdm_subs_modification_s {
    char *expires;
    OpenAPI_list_t *monitored_resource_uris;
    struct OpenAPI_snssai_s *single_nssai;
    char *dnn;
    struct OpenAPI_plmn_id_s *plmn_id;
    bool is_nf_change_filter;
    int nf_change_filter;
    struct OpenAPI_ue_context_in_smf_data_sub_filter_s *ue_con_smf_data_sub_filter;
    OpenAPI_list_t* expected_ue_behaviour_thresholds;
};

OpenAPI_sdm_subs_modification_t *OpenAPI_sdm_subs_modification_create(
    char *expires,
    OpenAPI_list_t *monitored_resource_uris,
    OpenAPI_snssai_t *single_nssai,
    char *dnn,
    OpenAPI_plmn_id_t *plmn_id,
    bool is_nf_change_filter,
    int nf_change_filter,
    OpenAPI_ue_context_in_smf_data_sub_filter_t *ue_con_smf_data_sub_filter,
    OpenAPI_list_t* expected_ue_behaviour_thresholds
);
void OpenAPI_sdm_subs_modification_free(OpenAPI_sdm_subs_modification_t *sdm_subs_modification);
OpenAPI_sdm_subs_modification_t *OpenAPI_sdm_subs_modification_parseFromJSON(cJSON *sdm_subs_modificationJSON);
cJSON *OpenAPI_sdm_subs_modification_convertToJSON(OpenAPI_sdm_subs_modification_t *sdm_subs_modification);
OpenAPI_sdm_subs_modification_t *OpenAPI_sdm_subs_modification_copy(OpenAPI_sdm_subs_modification_t *dst, OpenAPI_sdm_subs_modification_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sdm_subs_modification_H_ */

