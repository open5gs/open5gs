/*
 * pp_data.h
 *
 * Contains PP Data
 */

#ifndef _OpenAPI_pp_data_H_
#define _OpenAPI_pp_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_pp_data_s OpenAPI_pp_data_t;
#include "acs_info_rm.h"
#include "app_specific_expected_ue_behaviour.h"
#include "cag_provision_information.h"
#include "communication_characteristics.h"
#include "dnn_snssai_specific_group.h"
#include "ec_restriction.h"
#include "expected_ue_behaviour.h"
#include "expected_ue_behaviour_extension.h"
#include "lcs_privacy.h"
#include "mbs_assistance_info.h"
#include "model_5_mbs_authorization_info.h"
#include "ranging_sl_privacy.h"
#include "slice_usage_control_info.h"
#include "sor_info.h"
#include "static_ue_ip_address_params.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_pp_data_s {
    bool is_communication_characteristics_null;
    struct OpenAPI_communication_characteristics_s *communication_characteristics;
    char *supported_features;
    bool is_expected_ue_behaviour_parameters_null;
    struct OpenAPI_expected_ue_behaviour_s *expected_ue_behaviour_parameters;
    bool is_expected_ue_behaviour_extension_null;
    struct OpenAPI_expected_ue_behaviour_extension_s *expected_ue_behaviour_extension;
    bool is_ec_restriction_null;
    struct OpenAPI_ec_restriction_s *ec_restriction;
    bool is_acs_info_null;
    struct OpenAPI_acs_info_rm_s *acs_info;
    bool is_stn_sr_null;
    char *stn_sr;
    bool is_lcs_privacy_null;
    struct OpenAPI_lcs_privacy_s *lcs_privacy;
    struct OpenAPI_sor_info_s *sor_info;
    bool is__5mbs_authorization_info_null;
    struct OpenAPI_model_5_mbs_authorization_info_s *_5mbs_authorization_info;
    bool is_dnn_snssai_specific_group_null;
    struct OpenAPI_dnn_snssai_specific_group_s *dnn_snssai_specific_group;
    struct OpenAPI_mbs_assistance_info_s *mbs_assistance_info;
    bool is_app_specific_expected_ue_behaviour_null;
    struct OpenAPI_app_specific_expected_ue_behaviour_s *app_specific_expected_ue_behaviour;
    OpenAPI_list_t *slice_usage_control_infos;
    bool is_ranging_sl_privacy_null;
    struct OpenAPI_ranging_sl_privacy_s *ranging_sl_privacy;
    struct OpenAPI_cag_provision_information_s *cag_provision_info;
    OpenAPI_list_t *static_ue_ip_address_params;
};

OpenAPI_pp_data_t *OpenAPI_pp_data_create(
    bool is_communication_characteristics_null,
    OpenAPI_communication_characteristics_t *communication_characteristics,
    char *supported_features,
    bool is_expected_ue_behaviour_parameters_null,
    OpenAPI_expected_ue_behaviour_t *expected_ue_behaviour_parameters,
    bool is_expected_ue_behaviour_extension_null,
    OpenAPI_expected_ue_behaviour_extension_t *expected_ue_behaviour_extension,
    bool is_ec_restriction_null,
    OpenAPI_ec_restriction_t *ec_restriction,
    bool is_acs_info_null,
    OpenAPI_acs_info_rm_t *acs_info,
    bool is_stn_sr_null,
    char *stn_sr,
    bool is_lcs_privacy_null,
    OpenAPI_lcs_privacy_t *lcs_privacy,
    OpenAPI_sor_info_t *sor_info,
    bool is__5mbs_authorization_info_null,
    OpenAPI_model_5_mbs_authorization_info_t *_5mbs_authorization_info,
    bool is_dnn_snssai_specific_group_null,
    OpenAPI_dnn_snssai_specific_group_t *dnn_snssai_specific_group,
    OpenAPI_mbs_assistance_info_t *mbs_assistance_info,
    bool is_app_specific_expected_ue_behaviour_null,
    OpenAPI_app_specific_expected_ue_behaviour_t *app_specific_expected_ue_behaviour,
    OpenAPI_list_t *slice_usage_control_infos,
    bool is_ranging_sl_privacy_null,
    OpenAPI_ranging_sl_privacy_t *ranging_sl_privacy,
    OpenAPI_cag_provision_information_t *cag_provision_info,
    OpenAPI_list_t *static_ue_ip_address_params
);
void OpenAPI_pp_data_free(OpenAPI_pp_data_t *pp_data);
OpenAPI_pp_data_t *OpenAPI_pp_data_parseFromJSON(cJSON *pp_dataJSON);
cJSON *OpenAPI_pp_data_convertToJSON(OpenAPI_pp_data_t *pp_data);
OpenAPI_pp_data_t *OpenAPI_pp_data_copy(OpenAPI_pp_data_t *dst, OpenAPI_pp_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pp_data_H_ */

