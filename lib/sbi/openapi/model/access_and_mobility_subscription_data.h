/*
 * access_and_mobility_subscription_data.h
 *
 *
 */

#ifndef _OpenAPI_access_and_mobility_subscription_data_H_
#define _OpenAPI_access_and_mobility_subscription_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ambr_rm.h"
#include "area.h"
#include "cag_data.h"
#include "core_network_type.h"
#include "ec_restriction_data.h"
#include "edrx_parameters.h"
#include "expected_ue_behaviour_data.h"
#include "lte_v2x_auth.h"
#include "maximum_latency.h"
#include "maximum_response_time.h"
#include "nr_v2x_auth.h"
#include "nssai.h"
#include "odb_packet_services.h"
#include "ptw_parameters.h"
#include "rat_type.h"
#include "service_area_restriction.h"
#include "sor_info.h"
#include "sor_update_indicator.h"
#include "tmbr.h"
#include "trace_data.h"
#include "upu_info.h"
#include "vn_group_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_access_and_mobility_subscription_data_s OpenAPI_access_and_mobility_subscription_data_t;
typedef struct OpenAPI_access_and_mobility_subscription_data_s {
    char *supported_features;
    OpenAPI_list_t *gpsis;
    OpenAPI_list_t *internal_group_ids;
    OpenAPI_list_t* vn_group_info;
    OpenAPI_list_t* shared_vn_group_data_ids;
    struct OpenAPI_ambr_rm_s *subscribed_ue_ambr;
    struct OpenAPI_nssai_s *nssai;
    OpenAPI_list_t *rat_restrictions;
    OpenAPI_list_t *forbidden_areas;
    struct OpenAPI_service_area_restriction_s *service_area_restriction;
    OpenAPI_list_t *core_network_type_restrictions;
    int rfsp_index;
    int subs_reg_timer;
    int ue_usage_type;
    int mps_priority;
    int mcs_priority;
    int active_time;
    int dl_packet_count;
    struct OpenAPI_sor_info_s *sor_info;
    int sor_info_expect_ind;
    int soraf_retrieval;
    OpenAPI_list_t *sor_update_indicator_list;
    struct OpenAPI_upu_info_s *upu_info;
    int mico_allowed;
    OpenAPI_list_t *shared_am_data_ids;
    struct OpenAPI_odb_packet_services_s *odb_packet_services;
    OpenAPI_list_t *subscribed_dnn_list;
    int service_gap_time;
    struct OpenAPI_trace_data_s *trace_data;
    struct OpenAPI_cag_data_s *cag_data;
    char *stn_sr;
    char *c_msisdn;
    int nb_io_t_ue_priority;
    int nssai_inclusion_allowed;
    char rg_wireline_characteristics;
    struct OpenAPI_tmbr_s *rg_tmbr;
    struct OpenAPI_ec_restriction_data_s *ec_restriction_data;
    struct OpenAPI_expected_ue_behaviour_data_s *expected_ue_behaviour_list;
    OpenAPI_list_t *maximum_response_time_list;
    OpenAPI_list_t *maximum_latency_list;
    OpenAPI_list_t *primary_rat_restrictions;
    OpenAPI_list_t *secondary_rat_restrictions;
    OpenAPI_list_t *edrx_parameters_list;
    OpenAPI_list_t *ptw_parameters_list;
    int iab_operation_allowed;
    struct OpenAPI_nr_v2x_auth_s *nr_v2x_services_auth;
    struct OpenAPI_lte_v2x_auth_s *lte_v2x_services_auth;
    char *nr_ue_pc5_ambr;
    char *lte_pc5_ambr;
} OpenAPI_access_and_mobility_subscription_data_t;

OpenAPI_access_and_mobility_subscription_data_t *OpenAPI_access_and_mobility_subscription_data_create(
    char *supported_features,
    OpenAPI_list_t *gpsis,
    OpenAPI_list_t *internal_group_ids,
    OpenAPI_list_t* vn_group_info,
    OpenAPI_list_t* shared_vn_group_data_ids,
    OpenAPI_ambr_rm_t *subscribed_ue_ambr,
    OpenAPI_nssai_t *nssai,
    OpenAPI_list_t *rat_restrictions,
    OpenAPI_list_t *forbidden_areas,
    OpenAPI_service_area_restriction_t *service_area_restriction,
    OpenAPI_list_t *core_network_type_restrictions,
    int rfsp_index,
    int subs_reg_timer,
    int ue_usage_type,
    int mps_priority,
    int mcs_priority,
    int active_time,
    int dl_packet_count,
    OpenAPI_sor_info_t *sor_info,
    int sor_info_expect_ind,
    int soraf_retrieval,
    OpenAPI_list_t *sor_update_indicator_list,
    OpenAPI_upu_info_t *upu_info,
    int mico_allowed,
    OpenAPI_list_t *shared_am_data_ids,
    OpenAPI_odb_packet_services_t *odb_packet_services,
    OpenAPI_list_t *subscribed_dnn_list,
    int service_gap_time,
    OpenAPI_trace_data_t *trace_data,
    OpenAPI_cag_data_t *cag_data,
    char *stn_sr,
    char *c_msisdn,
    int nb_io_t_ue_priority,
    int nssai_inclusion_allowed,
    char rg_wireline_characteristics,
    OpenAPI_tmbr_t *rg_tmbr,
    OpenAPI_ec_restriction_data_t *ec_restriction_data,
    OpenAPI_expected_ue_behaviour_data_t *expected_ue_behaviour_list,
    OpenAPI_list_t *maximum_response_time_list,
    OpenAPI_list_t *maximum_latency_list,
    OpenAPI_list_t *primary_rat_restrictions,
    OpenAPI_list_t *secondary_rat_restrictions,
    OpenAPI_list_t *edrx_parameters_list,
    OpenAPI_list_t *ptw_parameters_list,
    int iab_operation_allowed,
    OpenAPI_nr_v2x_auth_t *nr_v2x_services_auth,
    OpenAPI_lte_v2x_auth_t *lte_v2x_services_auth,
    char *nr_ue_pc5_ambr,
    char *lte_pc5_ambr
    );
void OpenAPI_access_and_mobility_subscription_data_free(OpenAPI_access_and_mobility_subscription_data_t *access_and_mobility_subscription_data);
OpenAPI_access_and_mobility_subscription_data_t *OpenAPI_access_and_mobility_subscription_data_parseFromJSON(cJSON *access_and_mobility_subscription_dataJSON);
cJSON *OpenAPI_access_and_mobility_subscription_data_convertToJSON(OpenAPI_access_and_mobility_subscription_data_t *access_and_mobility_subscription_data);
OpenAPI_access_and_mobility_subscription_data_t *OpenAPI_access_and_mobility_subscription_data_copy(OpenAPI_access_and_mobility_subscription_data_t *dst, OpenAPI_access_and_mobility_subscription_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_access_and_mobility_subscription_data_H_ */

