/*
 * vsmf_update_error.h
 *
 * 
 */

#ifndef _OpenAPI_vsmf_update_error_H_
#define _OpenAPI_vsmf_update_error_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "arp.h"
#include "n4_information.h"
#include "ng_ap_cause.h"
#include "problem_details.h"
#include "ref_to_binary_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_vsmf_update_error_s OpenAPI_vsmf_update_error_t;
typedef struct OpenAPI_vsmf_update_error_s {
    struct OpenAPI_problem_details_s *error;
    bool is_pti;
    int pti;
    char *n1sm_cause;
    struct OpenAPI_ref_to_binary_data_s *n1_sm_info_from_ue;
    struct OpenAPI_ref_to_binary_data_s *unknown_n1_sm_info;
    OpenAPI_list_t *failed_to_assign_ebi_list;
    struct OpenAPI_ng_ap_cause_s *ng_ap_cause;
    bool is__5g_mm_cause_value;
    int _5g_mm_cause_value;
    char *recovery_time;
    struct OpenAPI_n4_information_s *n4_info;
    struct OpenAPI_n4_information_s *n4_info_ext1;
    struct OpenAPI_n4_information_s *n4_info_ext2;
} OpenAPI_vsmf_update_error_t;

OpenAPI_vsmf_update_error_t *OpenAPI_vsmf_update_error_create(
    OpenAPI_problem_details_t *error,
    bool is_pti,
    int pti,
    char *n1sm_cause,
    OpenAPI_ref_to_binary_data_t *n1_sm_info_from_ue,
    OpenAPI_ref_to_binary_data_t *unknown_n1_sm_info,
    OpenAPI_list_t *failed_to_assign_ebi_list,
    OpenAPI_ng_ap_cause_t *ng_ap_cause,
    bool is__5g_mm_cause_value,
    int _5g_mm_cause_value,
    char *recovery_time,
    OpenAPI_n4_information_t *n4_info,
    OpenAPI_n4_information_t *n4_info_ext1,
    OpenAPI_n4_information_t *n4_info_ext2
);
void OpenAPI_vsmf_update_error_free(OpenAPI_vsmf_update_error_t *vsmf_update_error);
OpenAPI_vsmf_update_error_t *OpenAPI_vsmf_update_error_parseFromJSON(cJSON *vsmf_update_errorJSON);
cJSON *OpenAPI_vsmf_update_error_convertToJSON(OpenAPI_vsmf_update_error_t *vsmf_update_error);
OpenAPI_vsmf_update_error_t *OpenAPI_vsmf_update_error_copy(OpenAPI_vsmf_update_error_t *dst, OpenAPI_vsmf_update_error_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_vsmf_update_error_H_ */

