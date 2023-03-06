/*
 * nrf_info_served_nef_info_value.h
 *
 * 
 */

#ifndef _OpenAPI_nrf_info_served_nef_info_value_H_
#define _OpenAPI_nrf_info_served_nef_info_value_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "af_event_exposure_data.h"
#include "identity_range.h"
#include "nef_info.h"
#include "pfd_data.h"
#include "tai.h"
#include "tai_range.h"
#include "un_trust_af_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nrf_info_served_nef_info_value_s OpenAPI_nrf_info_served_nef_info_value_t;
typedef struct OpenAPI_nrf_info_served_nef_info_value_s {
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
} OpenAPI_nrf_info_served_nef_info_value_t;

OpenAPI_nrf_info_served_nef_info_value_t *OpenAPI_nrf_info_served_nef_info_value_create(
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
void OpenAPI_nrf_info_served_nef_info_value_free(OpenAPI_nrf_info_served_nef_info_value_t *nrf_info_served_nef_info_value);
OpenAPI_nrf_info_served_nef_info_value_t *OpenAPI_nrf_info_served_nef_info_value_parseFromJSON(cJSON *nrf_info_served_nef_info_valueJSON);
cJSON *OpenAPI_nrf_info_served_nef_info_value_convertToJSON(OpenAPI_nrf_info_served_nef_info_value_t *nrf_info_served_nef_info_value);
OpenAPI_nrf_info_served_nef_info_value_t *OpenAPI_nrf_info_served_nef_info_value_copy(OpenAPI_nrf_info_served_nef_info_value_t *dst, OpenAPI_nrf_info_served_nef_info_value_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nrf_info_served_nef_info_value_H_ */

