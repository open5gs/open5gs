/*
 * sdm_subscription.h
 *
 * 
 */

#ifndef _OpenAPI_sdm_subscription_H_
#define _OpenAPI_sdm_subscription_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "context_info.h"
#include "immediate_report.h"
#include "plmn_id.h"
#include "snssai.h"
#include "ue_context_in_smf_data_sub_filter.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_sdm_subscription_s OpenAPI_sdm_subscription_t;
typedef struct OpenAPI_sdm_subscription_s {
    char *nf_instance_id;
    bool is_implicit_unsubscribe;
    int implicit_unsubscribe;
    char *expires;
    char *callback_reference;
    char *amf_service_name;
    OpenAPI_list_t *monitored_resource_uris;
    struct OpenAPI_snssai_s *single_nssai;
    char *dnn;
    char *subscription_id;
    struct OpenAPI_plmn_id_s *plmn_id;
    bool is_immediate_report;
    int immediate_report;
    struct OpenAPI_immediate_report_s *report;
    char *supported_features;
    struct OpenAPI_context_info_s *context_info;
    bool is_nf_change_filter;
    int nf_change_filter;
    bool is_unique_subscription;
    int unique_subscription;
    OpenAPI_list_t *reset_ids;
    struct OpenAPI_ue_context_in_smf_data_sub_filter_s *ue_con_smf_data_sub_filter;
} OpenAPI_sdm_subscription_t;

OpenAPI_sdm_subscription_t *OpenAPI_sdm_subscription_create(
    char *nf_instance_id,
    bool is_implicit_unsubscribe,
    int implicit_unsubscribe,
    char *expires,
    char *callback_reference,
    char *amf_service_name,
    OpenAPI_list_t *monitored_resource_uris,
    OpenAPI_snssai_t *single_nssai,
    char *dnn,
    char *subscription_id,
    OpenAPI_plmn_id_t *plmn_id,
    bool is_immediate_report,
    int immediate_report,
    OpenAPI_immediate_report_t *report,
    char *supported_features,
    OpenAPI_context_info_t *context_info,
    bool is_nf_change_filter,
    int nf_change_filter,
    bool is_unique_subscription,
    int unique_subscription,
    OpenAPI_list_t *reset_ids,
    OpenAPI_ue_context_in_smf_data_sub_filter_t *ue_con_smf_data_sub_filter
);
void OpenAPI_sdm_subscription_free(OpenAPI_sdm_subscription_t *sdm_subscription);
OpenAPI_sdm_subscription_t *OpenAPI_sdm_subscription_parseFromJSON(cJSON *sdm_subscriptionJSON);
cJSON *OpenAPI_sdm_subscription_convertToJSON(OpenAPI_sdm_subscription_t *sdm_subscription);
OpenAPI_sdm_subscription_t *OpenAPI_sdm_subscription_copy(OpenAPI_sdm_subscription_t *dst, OpenAPI_sdm_subscription_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sdm_subscription_H_ */

