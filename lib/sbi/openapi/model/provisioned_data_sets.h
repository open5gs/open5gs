/*
 * provisioned_data_sets.h
 *
 *
 */

#ifndef _OpenAPI_provisioned_data_sets_H_
#define _OpenAPI_provisioned_data_sets_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "access_and_mobility_subscription_data.h"
#include "lcs_mo_data.h"
#include "lcs_privacy_data.h"
#include "session_management_subscription_data.h"
#include "smf_selection_subscription_data.h"
#include "sms_management_subscription_data.h"
#include "sms_subscription_data.h"
#include "trace_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_provisioned_data_sets_s OpenAPI_provisioned_data_sets_t;
typedef struct OpenAPI_provisioned_data_sets_s {
    struct OpenAPI_access_and_mobility_subscription_data_s *am_data;
    struct OpenAPI_smf_selection_subscription_data_s *smf_sel_data;
    struct OpenAPI_sms_subscription_data_s *sms_subs_data;
    OpenAPI_list_t *sm_data;
    struct OpenAPI_trace_data_s *trace_data;
    struct OpenAPI_sms_management_subscription_data_s *sms_mng_data;
    struct OpenAPI_lcs_privacy_data_s *lcs_privacy_data;
    struct OpenAPI_lcs_mo_data_s *lcs_mo_data;
} OpenAPI_provisioned_data_sets_t;

OpenAPI_provisioned_data_sets_t *OpenAPI_provisioned_data_sets_create(
    OpenAPI_access_and_mobility_subscription_data_t *am_data,
    OpenAPI_smf_selection_subscription_data_t *smf_sel_data,
    OpenAPI_sms_subscription_data_t *sms_subs_data,
    OpenAPI_list_t *sm_data,
    OpenAPI_trace_data_t *trace_data,
    OpenAPI_sms_management_subscription_data_t *sms_mng_data,
    OpenAPI_lcs_privacy_data_t *lcs_privacy_data,
    OpenAPI_lcs_mo_data_t *lcs_mo_data
    );
void OpenAPI_provisioned_data_sets_free(OpenAPI_provisioned_data_sets_t *provisioned_data_sets);
OpenAPI_provisioned_data_sets_t *OpenAPI_provisioned_data_sets_parseFromJSON(cJSON *provisioned_data_setsJSON);
cJSON *OpenAPI_provisioned_data_sets_convertToJSON(OpenAPI_provisioned_data_sets_t *provisioned_data_sets);
OpenAPI_provisioned_data_sets_t *OpenAPI_provisioned_data_sets_copy(OpenAPI_provisioned_data_sets_t *dst, OpenAPI_provisioned_data_sets_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_provisioned_data_sets_H_ */

