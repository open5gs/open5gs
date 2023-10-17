/*
 * provisioned_data_sets.h
 *
 * Contains the provisioned data sets.
 */

#ifndef _OpenAPI_provisioned_data_sets_H_
#define _OpenAPI_provisioned_data_sets_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "access_and_mobility_subscription_data.h"
#include "authorization_data.h"
#include "ee_profile_data.h"
#include "lcs_broadcast_assistance_types_data.h"
#include "lcs_mo_data.h"
#include "lcs_privacy_data.h"
#include "mbs_subscription_data_1.h"
#include "odb_data.h"
#include "pp_profile_data.h"
#include "prose_subscription_data.h"
#include "sm_subs_data.h"
#include "smf_selection_subscription_data.h"
#include "sms_management_subscription_data.h"
#include "sms_subscription_data.h"
#include "trace_data.h"
#include "v2x_subscription_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_provisioned_data_sets_s OpenAPI_provisioned_data_sets_t;
typedef struct OpenAPI_provisioned_data_sets_s {
    struct OpenAPI_access_and_mobility_subscription_data_s *am_data;
    struct OpenAPI_smf_selection_subscription_data_s *smf_sel_data;
    struct OpenAPI_sms_subscription_data_s *sms_subs_data;
    struct OpenAPI_sm_subs_data_s *sm_data;
    bool is_trace_data_null;
    struct OpenAPI_trace_data_s *trace_data;
    struct OpenAPI_sms_management_subscription_data_s *sms_mng_data;
    struct OpenAPI_lcs_privacy_data_s *lcs_privacy_data;
    struct OpenAPI_lcs_mo_data_s *lcs_mo_data;
    struct OpenAPI_lcs_broadcast_assistance_types_data_s *lcs_bca_data;
    struct OpenAPI_v2x_subscription_data_s *v2x_data;
    struct OpenAPI_prose_subscription_data_s *prose_data;
    struct OpenAPI_odb_data_s *odb_data;
    struct OpenAPI_ee_profile_data_s *ee_profile_data;
    struct OpenAPI_pp_profile_data_s *pp_profile_data;
    struct OpenAPI_authorization_data_s *nidd_auth_data;
    struct OpenAPI_mbs_subscription_data_1_s *mbs_subscription_data;
} OpenAPI_provisioned_data_sets_t;

OpenAPI_provisioned_data_sets_t *OpenAPI_provisioned_data_sets_create(
    OpenAPI_access_and_mobility_subscription_data_t *am_data,
    OpenAPI_smf_selection_subscription_data_t *smf_sel_data,
    OpenAPI_sms_subscription_data_t *sms_subs_data,
    OpenAPI_sm_subs_data_t *sm_data,
    bool is_trace_data_null,
    OpenAPI_trace_data_t *trace_data,
    OpenAPI_sms_management_subscription_data_t *sms_mng_data,
    OpenAPI_lcs_privacy_data_t *lcs_privacy_data,
    OpenAPI_lcs_mo_data_t *lcs_mo_data,
    OpenAPI_lcs_broadcast_assistance_types_data_t *lcs_bca_data,
    OpenAPI_v2x_subscription_data_t *v2x_data,
    OpenAPI_prose_subscription_data_t *prose_data,
    OpenAPI_odb_data_t *odb_data,
    OpenAPI_ee_profile_data_t *ee_profile_data,
    OpenAPI_pp_profile_data_t *pp_profile_data,
    OpenAPI_authorization_data_t *nidd_auth_data,
    OpenAPI_mbs_subscription_data_1_t *mbs_subscription_data
);
void OpenAPI_provisioned_data_sets_free(OpenAPI_provisioned_data_sets_t *provisioned_data_sets);
OpenAPI_provisioned_data_sets_t *OpenAPI_provisioned_data_sets_parseFromJSON(cJSON *provisioned_data_setsJSON);
cJSON *OpenAPI_provisioned_data_sets_convertToJSON(OpenAPI_provisioned_data_sets_t *provisioned_data_sets);
OpenAPI_provisioned_data_sets_t *OpenAPI_provisioned_data_sets_copy(OpenAPI_provisioned_data_sets_t *dst, OpenAPI_provisioned_data_sets_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_provisioned_data_sets_H_ */

