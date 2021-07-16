/*
 * access_and_mobility_subscription_data_1.h
 *
 * 
 */

#ifndef _OpenAPI_access_and_mobility_subscription_data_1_H_
#define _OpenAPI_access_and_mobility_subscription_data_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ambr_rm.h"
#include "area_1.h"
#include "cag_data_1.h"
#include "core_network_type.h"
#include "ec_restriction_data_wb.h"
#include "edrx_parameters_1.h"
#include "expected_ue_behaviour_data_1.h"
#include "mdt_configuration_1.h"
#include "mdt_user_consent.h"
#include "nssai_1.h"
#include "odb_packet_services.h"
#include "ptw_parameters_1.h"
#include "rat_type.h"
#include "service_area_restriction_1.h"
#include "sor_info_1.h"
#include "sor_update_indicator.h"
#include "trace_data.h"
#include "upu_info_1.h"
#include "wireline_area_1.h"
#include "wireline_service_area_restriction_1.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_access_and_mobility_subscription_data_1_s OpenAPI_access_and_mobility_subscription_data_1_t;
typedef struct OpenAPI_access_and_mobility_subscription_data_1_s {
    char *supported_features;
    OpenAPI_list_t *gpsis;
    OpenAPI_list_t *internal_group_ids;
    OpenAPI_list_t* shared_vn_group_data_ids;
    struct OpenAPI_ambr_rm_s *subscribed_ue_ambr;
    struct OpenAPI_nssai_1_s *nssai;
    OpenAPI_list_t *rat_restrictions;
    OpenAPI_list_t *forbidden_areas;
    struct OpenAPI_service_area_restriction_1_s *service_area_restriction;
    OpenAPI_list_t *core_network_type_restrictions;
    bool is_rfsp_index;
    int rfsp_index;
    bool is_subs_reg_timer;
    int subs_reg_timer;
    bool is_ue_usage_type;
    int ue_usage_type;
    bool is_mps_priority;
    int mps_priority;
    bool is_mcs_priority;
    int mcs_priority;
    bool is_active_time;
    int active_time;
    struct OpenAPI_sor_info_1_s *sor_info;
    bool is_sor_info_expect_ind;
    int sor_info_expect_ind;
    bool is_soraf_retrieval;
    int soraf_retrieval;
    OpenAPI_list_t *sor_update_indicator_list;
    struct OpenAPI_upu_info_1_s *upu_info;
    bool is_mico_allowed;
    int mico_allowed;
    OpenAPI_list_t *shared_am_data_ids;
    OpenAPI_odb_packet_services_e odb_packet_services;
    OpenAPI_list_t *subscribed_dnn_list;
    bool is_service_gap_time;
    int service_gap_time;
    OpenAPI_mdt_user_consent_e mdt_user_consent;
    struct OpenAPI_mdt_configuration_1_s *mdt_configuration;
    struct OpenAPI_trace_data_s *trace_data;
    struct OpenAPI_cag_data_1_s *cag_data;
    char *stn_sr;
    char *c_msisdn;
    bool is_nb_io_tue_priority;
    int nb_io_tue_priority;
    bool is_nssai_inclusion_allowed;
    int nssai_inclusion_allowed;
    char *rg_wireline_characteristics;
    struct OpenAPI_ec_restriction_data_wb_s *ec_restriction_data_wb;
    bool is_ec_restriction_data_nb;
    int ec_restriction_data_nb;
    struct OpenAPI_expected_ue_behaviour_data_1_s *expected_ue_behaviour_list;
    OpenAPI_list_t *primary_rat_restrictions;
    OpenAPI_list_t *secondary_rat_restrictions;
    OpenAPI_list_t *edrx_parameters_list;
    OpenAPI_list_t *ptw_parameters_list;
    bool is_iab_operation_allowed;
    int iab_operation_allowed;
    OpenAPI_list_t *wireline_forbidden_areas;
    struct OpenAPI_wireline_service_area_restriction_1_s *wireline_service_area_restriction;
} OpenAPI_access_and_mobility_subscription_data_1_t;

OpenAPI_access_and_mobility_subscription_data_1_t *OpenAPI_access_and_mobility_subscription_data_1_create(
    char *supported_features,
    OpenAPI_list_t *gpsis,
    OpenAPI_list_t *internal_group_ids,
    OpenAPI_list_t* shared_vn_group_data_ids,
    OpenAPI_ambr_rm_t *subscribed_ue_ambr,
    OpenAPI_nssai_1_t *nssai,
    OpenAPI_list_t *rat_restrictions,
    OpenAPI_list_t *forbidden_areas,
    OpenAPI_service_area_restriction_1_t *service_area_restriction,
    OpenAPI_list_t *core_network_type_restrictions,
    bool is_rfsp_index,
    int rfsp_index,
    bool is_subs_reg_timer,
    int subs_reg_timer,
    bool is_ue_usage_type,
    int ue_usage_type,
    bool is_mps_priority,
    int mps_priority,
    bool is_mcs_priority,
    int mcs_priority,
    bool is_active_time,
    int active_time,
    OpenAPI_sor_info_1_t *sor_info,
    bool is_sor_info_expect_ind,
    int sor_info_expect_ind,
    bool is_soraf_retrieval,
    int soraf_retrieval,
    OpenAPI_list_t *sor_update_indicator_list,
    OpenAPI_upu_info_1_t *upu_info,
    bool is_mico_allowed,
    int mico_allowed,
    OpenAPI_list_t *shared_am_data_ids,
    OpenAPI_odb_packet_services_e odb_packet_services,
    OpenAPI_list_t *subscribed_dnn_list,
    bool is_service_gap_time,
    int service_gap_time,
    OpenAPI_mdt_user_consent_e mdt_user_consent,
    OpenAPI_mdt_configuration_1_t *mdt_configuration,
    OpenAPI_trace_data_t *trace_data,
    OpenAPI_cag_data_1_t *cag_data,
    char *stn_sr,
    char *c_msisdn,
    bool is_nb_io_tue_priority,
    int nb_io_tue_priority,
    bool is_nssai_inclusion_allowed,
    int nssai_inclusion_allowed,
    char *rg_wireline_characteristics,
    OpenAPI_ec_restriction_data_wb_t *ec_restriction_data_wb,
    bool is_ec_restriction_data_nb,
    int ec_restriction_data_nb,
    OpenAPI_expected_ue_behaviour_data_1_t *expected_ue_behaviour_list,
    OpenAPI_list_t *primary_rat_restrictions,
    OpenAPI_list_t *secondary_rat_restrictions,
    OpenAPI_list_t *edrx_parameters_list,
    OpenAPI_list_t *ptw_parameters_list,
    bool is_iab_operation_allowed,
    int iab_operation_allowed,
    OpenAPI_list_t *wireline_forbidden_areas,
    OpenAPI_wireline_service_area_restriction_1_t *wireline_service_area_restriction
);
void OpenAPI_access_and_mobility_subscription_data_1_free(OpenAPI_access_and_mobility_subscription_data_1_t *access_and_mobility_subscription_data_1);
OpenAPI_access_and_mobility_subscription_data_1_t *OpenAPI_access_and_mobility_subscription_data_1_parseFromJSON(cJSON *access_and_mobility_subscription_data_1JSON);
cJSON *OpenAPI_access_and_mobility_subscription_data_1_convertToJSON(OpenAPI_access_and_mobility_subscription_data_1_t *access_and_mobility_subscription_data_1);
OpenAPI_access_and_mobility_subscription_data_1_t *OpenAPI_access_and_mobility_subscription_data_1_copy(OpenAPI_access_and_mobility_subscription_data_1_t *dst, OpenAPI_access_and_mobility_subscription_data_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_access_and_mobility_subscription_data_1_H_ */

