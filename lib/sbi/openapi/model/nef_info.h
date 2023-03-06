/*
 * nef_info.h
 *
 * Information of an NEF NF Instance
 */

#ifndef _OpenAPI_nef_info_H_
#define _OpenAPI_nef_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "af_event_exposure_data.h"
#include "identity_range.h"
#include "pfd_data.h"
#include "tai.h"
#include "tai_range.h"
#include "un_trust_af_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nef_info_s OpenAPI_nef_info_t;
typedef struct OpenAPI_nef_info_s {
    char *nef_id;
    struct OpenAPI_pfd_data_s *pfd_data;
    struct OpenAPI_af_event_exposure_data_s *af_ee_data;
    OpenAPI_list_t *gpsi_ranges;
    OpenAPI_list_t *external_group_identifiers_ranges;
    OpenAPI_list_t *served_fqdn_list;
    OpenAPI_list_t *tai_list;
    OpenAPI_list_t *tai_range_list;
    OpenAPI_list_t *dnai_list;
    OpenAPI_list_t *un_trust_af_info_list;
    bool is_uas_nf_functionality_ind;
    int uas_nf_functionality_ind;
} OpenAPI_nef_info_t;

OpenAPI_nef_info_t *OpenAPI_nef_info_create(
    char *nef_id,
    OpenAPI_pfd_data_t *pfd_data,
    OpenAPI_af_event_exposure_data_t *af_ee_data,
    OpenAPI_list_t *gpsi_ranges,
    OpenAPI_list_t *external_group_identifiers_ranges,
    OpenAPI_list_t *served_fqdn_list,
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *tai_range_list,
    OpenAPI_list_t *dnai_list,
    OpenAPI_list_t *un_trust_af_info_list,
    bool is_uas_nf_functionality_ind,
    int uas_nf_functionality_ind
);
void OpenAPI_nef_info_free(OpenAPI_nef_info_t *nef_info);
OpenAPI_nef_info_t *OpenAPI_nef_info_parseFromJSON(cJSON *nef_infoJSON);
cJSON *OpenAPI_nef_info_convertToJSON(OpenAPI_nef_info_t *nef_info);
OpenAPI_nef_info_t *OpenAPI_nef_info_copy(OpenAPI_nef_info_t *dst, OpenAPI_nef_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nef_info_H_ */

