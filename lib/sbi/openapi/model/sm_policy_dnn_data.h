/*
 * sm_policy_dnn_data.h
 *
 * Contains the SM policy data for a given DNN (and S-NSSAI).
 */

#ifndef _OpenAPI_sm_policy_dnn_data_H_
#define _OpenAPI_sm_policy_dnn_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "charging_information.h"
#include "limit_id_to_monitoring_key.h"
#include "presence_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_sm_policy_dnn_data_s OpenAPI_sm_policy_dnn_data_t;
typedef struct OpenAPI_sm_policy_dnn_data_s {
    char *dnn;
    OpenAPI_list_t *allowed_services;
    OpenAPI_list_t *subsc_cats;
    char *gbr_ul;
    char *gbr_dl;
    bool is_adc_support;
    int adc_support;
    bool is_subsc_spending_limits;
    int subsc_spending_limits;
    bool is_ipv4_index;
    int ipv4_index;
    bool is_ipv6_index;
    int ipv6_index;
    bool is_offline;
    int offline;
    bool is_online;
    int online;
    struct OpenAPI_charging_information_s *chf_info;
    OpenAPI_list_t* ref_um_data_limit_ids;
    bool is_mps_priority;
    int mps_priority;
    bool is_mcs_priority;
    int mcs_priority;
    bool is_ims_signalling_prio;
    int ims_signalling_prio;
    bool is_mps_priority_level;
    int mps_priority_level;
    bool is_mcs_priority_level;
    int mcs_priority_level;
    OpenAPI_list_t* pra_infos;
    bool is_bdt_ref_ids_null;
    OpenAPI_list_t* bdt_ref_ids;
    bool is_loc_rout_not_allowed;
    int loc_rout_not_allowed;
} OpenAPI_sm_policy_dnn_data_t;

OpenAPI_sm_policy_dnn_data_t *OpenAPI_sm_policy_dnn_data_create(
    char *dnn,
    OpenAPI_list_t *allowed_services,
    OpenAPI_list_t *subsc_cats,
    char *gbr_ul,
    char *gbr_dl,
    bool is_adc_support,
    int adc_support,
    bool is_subsc_spending_limits,
    int subsc_spending_limits,
    bool is_ipv4_index,
    int ipv4_index,
    bool is_ipv6_index,
    int ipv6_index,
    bool is_offline,
    int offline,
    bool is_online,
    int online,
    OpenAPI_charging_information_t *chf_info,
    OpenAPI_list_t* ref_um_data_limit_ids,
    bool is_mps_priority,
    int mps_priority,
    bool is_mcs_priority,
    int mcs_priority,
    bool is_ims_signalling_prio,
    int ims_signalling_prio,
    bool is_mps_priority_level,
    int mps_priority_level,
    bool is_mcs_priority_level,
    int mcs_priority_level,
    OpenAPI_list_t* pra_infos,
    bool is_bdt_ref_ids_null,
    OpenAPI_list_t* bdt_ref_ids,
    bool is_loc_rout_not_allowed,
    int loc_rout_not_allowed
);
void OpenAPI_sm_policy_dnn_data_free(OpenAPI_sm_policy_dnn_data_t *sm_policy_dnn_data);
OpenAPI_sm_policy_dnn_data_t *OpenAPI_sm_policy_dnn_data_parseFromJSON(cJSON *sm_policy_dnn_dataJSON);
cJSON *OpenAPI_sm_policy_dnn_data_convertToJSON(OpenAPI_sm_policy_dnn_data_t *sm_policy_dnn_data);
OpenAPI_sm_policy_dnn_data_t *OpenAPI_sm_policy_dnn_data_copy(OpenAPI_sm_policy_dnn_data_t *dst, OpenAPI_sm_policy_dnn_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sm_policy_dnn_data_H_ */

