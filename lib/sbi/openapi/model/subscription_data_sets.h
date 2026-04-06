/*
 * subscription_data_sets.h
 *
 * Contains Subscription data sets
 */

#ifndef _OpenAPI_subscription_data_sets_H_
#define _OpenAPI_subscription_data_sets_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_subscription_data_sets_s OpenAPI_subscription_data_sets_t;
#include "a2x_subscription_data.h"
#include "access_and_mobility_subscription_data.h"
#include "lcs_broadcast_assistance_types_data.h"
#include "lcs_mo_data.h"
#include "lcs_privacy_data.h"
#include "lcs_subscription_data.h"
#include "mbs_subscription_data.h"
#include "prose_subscription_data.h"
#include "ranging_sl_privacy_data.h"
#include "sm_subs_data.h"
#include "smf_selection_subscription_data.h"
#include "sms_management_subscription_data.h"
#include "sms_subscription_data.h"
#include "trace_data.h"
#include "uc_subscription_data.h"
#include "ue_context_in_amf_data.h"
#include "ue_context_in_smf_data.h"
#include "ue_context_in_smsf_data.h"
#include "v2x_subscription_data.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_subscription_data_sets_s {
    struct OpenAPI_access_and_mobility_subscription_data_s *am_data;
    struct OpenAPI_smf_selection_subscription_data_s *smf_sel_data;
    struct OpenAPI_ue_context_in_amf_data_s *uec_amf_data;
    struct OpenAPI_ue_context_in_smf_data_s *uec_smf_data;
    struct OpenAPI_ue_context_in_smsf_data_s *uec_smsf_data;
    struct OpenAPI_sms_subscription_data_s *sms_subs_data;
    struct OpenAPI_sm_subs_data_s *sm_data;
    bool is_trace_data_null;
    struct OpenAPI_trace_data_s *trace_data;
    struct OpenAPI_sms_management_subscription_data_s *sms_mng_data;
    struct OpenAPI_lcs_privacy_data_s *lcs_privacy_data;
    struct OpenAPI_lcs_mo_data_s *lcs_mo_data;
    struct OpenAPI_lcs_subscription_data_s *lcs_subscription_data;
    struct OpenAPI_v2x_subscription_data_s *v2x_data;
    struct OpenAPI_lcs_broadcast_assistance_types_data_s *lcs_broadcast_assistance_types_data;
    struct OpenAPI_prose_subscription_data_s *prose_data;
    struct OpenAPI_mbs_subscription_data_s *mbs_data;
    struct OpenAPI_uc_subscription_data_s *uc_data;
    struct OpenAPI_a2x_subscription_data_s *a2x_data;
    struct OpenAPI_ranging_sl_privacy_data_s *ranging_sl_privacy_data;
};

OpenAPI_subscription_data_sets_t *OpenAPI_subscription_data_sets_create(
    OpenAPI_access_and_mobility_subscription_data_t *am_data,
    OpenAPI_smf_selection_subscription_data_t *smf_sel_data,
    OpenAPI_ue_context_in_amf_data_t *uec_amf_data,
    OpenAPI_ue_context_in_smf_data_t *uec_smf_data,
    OpenAPI_ue_context_in_smsf_data_t *uec_smsf_data,
    OpenAPI_sms_subscription_data_t *sms_subs_data,
    OpenAPI_sm_subs_data_t *sm_data,
    bool is_trace_data_null,
    OpenAPI_trace_data_t *trace_data,
    OpenAPI_sms_management_subscription_data_t *sms_mng_data,
    OpenAPI_lcs_privacy_data_t *lcs_privacy_data,
    OpenAPI_lcs_mo_data_t *lcs_mo_data,
    OpenAPI_lcs_subscription_data_t *lcs_subscription_data,
    OpenAPI_v2x_subscription_data_t *v2x_data,
    OpenAPI_lcs_broadcast_assistance_types_data_t *lcs_broadcast_assistance_types_data,
    OpenAPI_prose_subscription_data_t *prose_data,
    OpenAPI_mbs_subscription_data_t *mbs_data,
    OpenAPI_uc_subscription_data_t *uc_data,
    OpenAPI_a2x_subscription_data_t *a2x_data,
    OpenAPI_ranging_sl_privacy_data_t *ranging_sl_privacy_data
);
void OpenAPI_subscription_data_sets_free(OpenAPI_subscription_data_sets_t *subscription_data_sets);
OpenAPI_subscription_data_sets_t *OpenAPI_subscription_data_sets_parseFromJSON(cJSON *subscription_data_setsJSON);
cJSON *OpenAPI_subscription_data_sets_convertToJSON(OpenAPI_subscription_data_sets_t *subscription_data_sets);
OpenAPI_subscription_data_sets_t *OpenAPI_subscription_data_sets_copy(OpenAPI_subscription_data_sets_t *dst, OpenAPI_subscription_data_sets_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_subscription_data_sets_H_ */

