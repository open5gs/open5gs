/*
 * vsmf_updated_data.h
 *
 * 
 */

#ifndef _OpenAPI_vsmf_updated_data_H_
#define _OpenAPI_vsmf_updated_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "arp.h"
#include "ebi_arp_mapping.h"
#include "n4_information.h"
#include "qos_flow_item.h"
#include "ref_to_binary_data.h"
#include "secondary_rat_usage_info.h"
#include "secondary_rat_usage_report.h"
#include "user_location.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_vsmf_updated_data_s OpenAPI_vsmf_updated_data_t;
typedef struct OpenAPI_vsmf_updated_data_s {
    OpenAPI_list_t *qos_flows_add_mod_list;
    OpenAPI_list_t *qos_flows_rel_list;
    OpenAPI_list_t *qos_flows_failedto_add_mod_list;
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
} OpenAPI_vsmf_updated_data_t;

OpenAPI_vsmf_updated_data_t *OpenAPI_vsmf_updated_data_create(
    OpenAPI_list_t *qos_flows_add_mod_list,
    OpenAPI_list_t *qos_flows_rel_list,
    OpenAPI_list_t *qos_flows_failedto_add_mod_list,
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
    OpenAPI_n4_information_t *n4_info_ext2
);
void OpenAPI_vsmf_updated_data_free(OpenAPI_vsmf_updated_data_t *vsmf_updated_data);
OpenAPI_vsmf_updated_data_t *OpenAPI_vsmf_updated_data_parseFromJSON(cJSON *vsmf_updated_dataJSON);
cJSON *OpenAPI_vsmf_updated_data_convertToJSON(OpenAPI_vsmf_updated_data_t *vsmf_updated_data);
OpenAPI_vsmf_updated_data_t *OpenAPI_vsmf_updated_data_copy(OpenAPI_vsmf_updated_data_t *dst, OpenAPI_vsmf_updated_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_vsmf_updated_data_H_ */

