/*
 * sdm_subscription_1.h
 *
 * Contains SDM Subscriptions to Notifications
 */

#ifndef _OpenAPI_sdm_subscription_1_H_
#define _OpenAPI_sdm_subscription_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_sdm_subscription_1_s OpenAPI_sdm_subscription_1_t;
#include "context_info.h"
#include "expected_ue_behaviour_threshold_1.h"
#include "guami_1.h"
#include "immediate_report_2.h"
#include "plmn_id.h"
#include "service_name.h"
#include "snssai.h"
#include "ue_context_in_smf_data_sub_filter_1.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_sdm_subscription_1_s {
    char *nf_instance_id;
    bool is_implicit_unsubscribe;
    int implicit_unsubscribe;
    char *expires;
    char *callback_reference;
    OpenAPI_service_name_e amf_service_name;
    OpenAPI_list_t *monitored_resource_uris;
    struct OpenAPI_snssai_s *single_nssai;
    char *dnn;
    char *subscription_id;
    struct OpenAPI_plmn_id_s *plmn_id;
    bool is_immediate_report;
    int immediate_report;
    struct OpenAPI_immediate_report_2_s *report;
    char *supported_features;
    struct OpenAPI_context_info_s *context_info;
    bool is_nf_change_filter;
    int nf_change_filter;
    bool is_unique_subscription;
    int unique_subscription;
    OpenAPI_list_t *reset_ids;
    struct OpenAPI_ue_context_in_smf_data_sub_filter_1_s *ue_con_smf_data_sub_filter;
    OpenAPI_list_t *adjacent_plmns;
    bool is_disaster_roaming_ind;
    int disaster_roaming_ind;
    char *data_restoration_callback_uri;
    bool is_udr_restart_ind;
    int udr_restart_ind;
    char *last_synchronization_time;
    OpenAPI_list_t* expected_ue_behaviour_thresholds;
    struct OpenAPI_guami_1_s *guami;
};

OpenAPI_sdm_subscription_1_t *OpenAPI_sdm_subscription_1_create(
    char *nf_instance_id,
    bool is_implicit_unsubscribe,
    int implicit_unsubscribe,
    char *expires,
    char *callback_reference,
    OpenAPI_service_name_e amf_service_name,
    OpenAPI_list_t *monitored_resource_uris,
    OpenAPI_snssai_t *single_nssai,
    char *dnn,
    char *subscription_id,
    OpenAPI_plmn_id_t *plmn_id,
    bool is_immediate_report,
    int immediate_report,
    OpenAPI_immediate_report_2_t *report,
    char *supported_features,
    OpenAPI_context_info_t *context_info,
    bool is_nf_change_filter,
    int nf_change_filter,
    bool is_unique_subscription,
    int unique_subscription,
    OpenAPI_list_t *reset_ids,
    OpenAPI_ue_context_in_smf_data_sub_filter_1_t *ue_con_smf_data_sub_filter,
    OpenAPI_list_t *adjacent_plmns,
    bool is_disaster_roaming_ind,
    int disaster_roaming_ind,
    char *data_restoration_callback_uri,
    bool is_udr_restart_ind,
    int udr_restart_ind,
    char *last_synchronization_time,
    OpenAPI_list_t* expected_ue_behaviour_thresholds,
    OpenAPI_guami_1_t *guami
);
void OpenAPI_sdm_subscription_1_free(OpenAPI_sdm_subscription_1_t *sdm_subscription_1);
OpenAPI_sdm_subscription_1_t *OpenAPI_sdm_subscription_1_parseFromJSON(cJSON *sdm_subscription_1JSON);
cJSON *OpenAPI_sdm_subscription_1_convertToJSON(OpenAPI_sdm_subscription_1_t *sdm_subscription_1);
OpenAPI_sdm_subscription_1_t *OpenAPI_sdm_subscription_1_copy(OpenAPI_sdm_subscription_1_t *dst, OpenAPI_sdm_subscription_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sdm_subscription_1_H_ */

