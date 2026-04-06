/*
 * dnn_info.h
 *
 * Contains DNN Information
 */

#ifndef _OpenAPI_dnn_info_H_
#define _OpenAPI_dnn_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_dnn_info_s OpenAPI_dnn_info_t;
#include "additional_smf_selection_info.h"
#include "ip_index.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_dnn_info_s {
    char *dnn;
    bool is_default_dnn_indicator;
    int default_dnn_indicator;
    bool is_lbo_roaming_allowed;
    int lbo_roaming_allowed;
    bool is_iwk_eps_ind;
    int iwk_eps_ind;
    bool is_dnn_barred;
    int dnn_barred;
    bool is_invoke_nef_ind;
    int invoke_nef_ind;
    OpenAPI_list_t *smf_list;
    struct OpenAPI_ip_index_s *ipv4_index;
    struct OpenAPI_ip_index_s *ipv6_index;
    bool is_same_smf_ind;
    int same_smf_ind;
    bool is_hr_sbo_allowed;
    int hr_sbo_allowed;
    struct OpenAPI_additional_smf_selection_info_s *additional_smf_selection_info;
    bool is_local_offloading_mngt_ind;
    int local_offloading_mngt_ind;
};

OpenAPI_dnn_info_t *OpenAPI_dnn_info_create(
    char *dnn,
    bool is_default_dnn_indicator,
    int default_dnn_indicator,
    bool is_lbo_roaming_allowed,
    int lbo_roaming_allowed,
    bool is_iwk_eps_ind,
    int iwk_eps_ind,
    bool is_dnn_barred,
    int dnn_barred,
    bool is_invoke_nef_ind,
    int invoke_nef_ind,
    OpenAPI_list_t *smf_list,
    OpenAPI_ip_index_t *ipv4_index,
    OpenAPI_ip_index_t *ipv6_index,
    bool is_same_smf_ind,
    int same_smf_ind,
    bool is_hr_sbo_allowed,
    int hr_sbo_allowed,
    OpenAPI_additional_smf_selection_info_t *additional_smf_selection_info,
    bool is_local_offloading_mngt_ind,
    int local_offloading_mngt_ind
);
void OpenAPI_dnn_info_free(OpenAPI_dnn_info_t *dnn_info);
OpenAPI_dnn_info_t *OpenAPI_dnn_info_parseFromJSON(cJSON *dnn_infoJSON);
cJSON *OpenAPI_dnn_info_convertToJSON(OpenAPI_dnn_info_t *dnn_info);
OpenAPI_dnn_info_t *OpenAPI_dnn_info_copy(OpenAPI_dnn_info_t *dst, OpenAPI_dnn_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dnn_info_H_ */

