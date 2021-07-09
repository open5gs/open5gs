/*
 * subscription_data_sets_1.h
 *
 * 
 */

#ifndef _OpenAPI_subscription_data_sets_1_H_
#define _OpenAPI_subscription_data_sets_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "access_and_mobility_subscription_data_1.h"
#include "lcs_broadcast_assistance_types_data_1.h"
#include "lcs_mo_data.h"
#include "lcs_privacy_data_1.h"
#include "session_management_subscription_data_1.h"
#include "smf_selection_subscription_data_1.h"
#include "sms_management_subscription_data_1.h"
#include "sms_subscription_data_1.h"
#include "trace_data.h"
#include "ue_context_in_amf_data_1.h"
#include "ue_context_in_smf_data_1.h"
#include "ue_context_in_smsf_data_1.h"
#include "v2x_subscription_data_1.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_subscription_data_sets_1_s OpenAPI_subscription_data_sets_1_t;
typedef struct OpenAPI_subscription_data_sets_1_s {
    struct OpenAPI_access_and_mobility_subscription_data_1_s *am_data;
    struct OpenAPI_smf_selection_subscription_data_1_s *smf_sel_data;
    struct OpenAPI_ue_context_in_amf_data_1_s *uec_amf_data;
    struct OpenAPI_ue_context_in_smf_data_1_s *uec_smf_data;
    struct OpenAPI_ue_context_in_smsf_data_1_s *uec_smsf_data;
    struct OpenAPI_sms_subscription_data_1_s *sms_subs_data;
    OpenAPI_list_t *sm_data;
    struct OpenAPI_trace_data_s *trace_data;
    struct OpenAPI_sms_management_subscription_data_1_s *sms_mng_data;
    struct OpenAPI_lcs_privacy_data_1_s *lcs_privacy_data;
    struct OpenAPI_lcs_mo_data_s *lcs_mo_data;
    struct OpenAPI_v2x_subscription_data_1_s *v2x_data;
    struct OpenAPI_lcs_broadcast_assistance_types_data_1_s *lcs_broadcast_assistance_types_data;
} OpenAPI_subscription_data_sets_1_t;

OpenAPI_subscription_data_sets_1_t *OpenAPI_subscription_data_sets_1_create(
    OpenAPI_access_and_mobility_subscription_data_1_t *am_data,
    OpenAPI_smf_selection_subscription_data_1_t *smf_sel_data,
    OpenAPI_ue_context_in_amf_data_1_t *uec_amf_data,
    OpenAPI_ue_context_in_smf_data_1_t *uec_smf_data,
    OpenAPI_ue_context_in_smsf_data_1_t *uec_smsf_data,
    OpenAPI_sms_subscription_data_1_t *sms_subs_data,
    OpenAPI_list_t *sm_data,
    OpenAPI_trace_data_t *trace_data,
    OpenAPI_sms_management_subscription_data_1_t *sms_mng_data,
    OpenAPI_lcs_privacy_data_1_t *lcs_privacy_data,
    OpenAPI_lcs_mo_data_t *lcs_mo_data,
    OpenAPI_v2x_subscription_data_1_t *v2x_data,
    OpenAPI_lcs_broadcast_assistance_types_data_1_t *lcs_broadcast_assistance_types_data
);
void OpenAPI_subscription_data_sets_1_free(OpenAPI_subscription_data_sets_1_t *subscription_data_sets_1);
OpenAPI_subscription_data_sets_1_t *OpenAPI_subscription_data_sets_1_parseFromJSON(cJSON *subscription_data_sets_1JSON);
cJSON *OpenAPI_subscription_data_sets_1_convertToJSON(OpenAPI_subscription_data_sets_1_t *subscription_data_sets_1);
OpenAPI_subscription_data_sets_1_t *OpenAPI_subscription_data_sets_1_copy(OpenAPI_subscription_data_sets_1_t *dst, OpenAPI_subscription_data_sets_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_subscription_data_sets_1_H_ */

