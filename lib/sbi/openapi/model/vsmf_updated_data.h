/*
 * vsmf_updated_data.h
 *
 * Data within Update Response from V-SMF, or from I-SMF to SMF
 */

#ifndef _OpenAPI_vsmf_updated_data_H_
#define _OpenAPI_vsmf_updated_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_vsmf_updated_data_s OpenAPI_vsmf_updated_data_t;
#include "arp.h"
#include "avail_bit_rate_mon_supported.h"
#include "ebi_arp_mapping.h"
#include "ecn_marking_congestion_info_status.h"
#include "n4_information.h"
#include "qos_flow_item.h"
#include "qos_monitoring_congestion_supported.h"
#include "qos_monitoring_pd_method.h"
#include "qos_monitoring_pd_supported.h"
#include "ref_to_binary_data.h"
#include "secondary_rat_usage_info.h"
#include "secondary_rat_usage_report.h"
#include "user_location.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_vsmf_updated_data_s {
    OpenAPI_list_t *qos_flows_add_mod_list;
    OpenAPI_list_t *qos_flows_rel_list;
    OpenAPI_list_t *qos_flows_failedto_add_mod_list;
    OpenAPI_list_t *qos_flows_vsmf_rejected_add_mod_list;
    OpenAPI_list_t *qos_flows_failedto_rel_list;
    struct OpenAPI_ref_to_binary_data_s *n1_sm_info_from_ue;
    struct OpenAPI_ref_to_binary_data_s *unknown_n1_sm_info;
    struct OpenAPI_user_location_s *ue_location;
    char *ue_time_zone;
    struct OpenAPI_user_location_s *add_ue_location;
    OpenAPI_list_t *assigned_ebi_list;
    OpenAPI_list_t *failed_to_assign_ebi_list;
    OpenAPI_list_t *released_ebi_list;
    OpenAPI_list_t *secondary_rat_usage_report;
    OpenAPI_list_t *secondary_rat_usage_info;
    struct OpenAPI_n4_information_s *n4_info;
    struct OpenAPI_n4_information_s *n4_info_ext1;
    struct OpenAPI_n4_information_s *n4_info_ext2;
    struct OpenAPI_n4_information_s *n4_info_ext3;
    bool is_modified_ebi_list_not_delivered;
    int modified_ebi_list_not_delivered;
    OpenAPI_list_t *ecn_marking_congestion_info_status;
    bool is_pdu_set_support_ind;
    int pdu_set_support_ind;
    OpenAPI_qos_monitoring_pd_supported_e qos_monitoring_pd_supported;
    OpenAPI_list_t *qos_monitoring_pd_methods;
    OpenAPI_qos_monitoring_congestion_supported_e qos_monitoring_congestion_supported;
    OpenAPI_avail_bit_rate_mon_supported_e avail_bit_rate_mon_supported;
    char *service_level_aa_container;
};

OpenAPI_vsmf_updated_data_t *OpenAPI_vsmf_updated_data_create(
    OpenAPI_list_t *qos_flows_add_mod_list,
    OpenAPI_list_t *qos_flows_rel_list,
    OpenAPI_list_t *qos_flows_failedto_add_mod_list,
    OpenAPI_list_t *qos_flows_vsmf_rejected_add_mod_list,
    OpenAPI_list_t *qos_flows_failedto_rel_list,
    OpenAPI_ref_to_binary_data_t *n1_sm_info_from_ue,
    OpenAPI_ref_to_binary_data_t *unknown_n1_sm_info,
    OpenAPI_user_location_t *ue_location,
    char *ue_time_zone,
    OpenAPI_user_location_t *add_ue_location,
    OpenAPI_list_t *assigned_ebi_list,
    OpenAPI_list_t *failed_to_assign_ebi_list,
    OpenAPI_list_t *released_ebi_list,
    OpenAPI_list_t *secondary_rat_usage_report,
    OpenAPI_list_t *secondary_rat_usage_info,
    OpenAPI_n4_information_t *n4_info,
    OpenAPI_n4_information_t *n4_info_ext1,
    OpenAPI_n4_information_t *n4_info_ext2,
    OpenAPI_n4_information_t *n4_info_ext3,
    bool is_modified_ebi_list_not_delivered,
    int modified_ebi_list_not_delivered,
    OpenAPI_list_t *ecn_marking_congestion_info_status,
    bool is_pdu_set_support_ind,
    int pdu_set_support_ind,
    OpenAPI_qos_monitoring_pd_supported_e qos_monitoring_pd_supported,
    OpenAPI_list_t *qos_monitoring_pd_methods,
    OpenAPI_qos_monitoring_congestion_supported_e qos_monitoring_congestion_supported,
    OpenAPI_avail_bit_rate_mon_supported_e avail_bit_rate_mon_supported,
    char *service_level_aa_container
);
void OpenAPI_vsmf_updated_data_free(OpenAPI_vsmf_updated_data_t *vsmf_updated_data);
OpenAPI_vsmf_updated_data_t *OpenAPI_vsmf_updated_data_parseFromJSON(cJSON *vsmf_updated_dataJSON);
cJSON *OpenAPI_vsmf_updated_data_convertToJSON(OpenAPI_vsmf_updated_data_t *vsmf_updated_data);
OpenAPI_vsmf_updated_data_t *OpenAPI_vsmf_updated_data_copy(OpenAPI_vsmf_updated_data_t *dst, OpenAPI_vsmf_updated_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_vsmf_updated_data_H_ */

