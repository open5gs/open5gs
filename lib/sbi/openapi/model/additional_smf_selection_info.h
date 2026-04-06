/*
 * additional_smf_selection_info.h
 *
 * Contains the additional parameters for SMF selection in target PLMN. 
 */

#ifndef _OpenAPI_additional_smf_selection_info_H_
#define _OpenAPI_additional_smf_selection_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_additional_smf_selection_info_s OpenAPI_additional_smf_selection_info_t;
#include "plmn_id_nid.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_additional_smf_selection_info_s {
    struct OpenAPI_plmn_id_nid_s *target_plmn;
};

OpenAPI_additional_smf_selection_info_t *OpenAPI_additional_smf_selection_info_create(
    OpenAPI_plmn_id_nid_t *target_plmn
);
void OpenAPI_additional_smf_selection_info_free(OpenAPI_additional_smf_selection_info_t *additional_smf_selection_info);
OpenAPI_additional_smf_selection_info_t *OpenAPI_additional_smf_selection_info_parseFromJSON(cJSON *additional_smf_selection_infoJSON);
cJSON *OpenAPI_additional_smf_selection_info_convertToJSON(OpenAPI_additional_smf_selection_info_t *additional_smf_selection_info);
OpenAPI_additional_smf_selection_info_t *OpenAPI_additional_smf_selection_info_copy(OpenAPI_additional_smf_selection_info_t *dst, OpenAPI_additional_smf_selection_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_additional_smf_selection_info_H_ */

