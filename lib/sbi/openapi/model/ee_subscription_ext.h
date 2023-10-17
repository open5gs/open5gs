/*
 * ee_subscription_ext.h
 *
 * 
 */

#ifndef _OpenAPI_ee_subscription_ext_H_
#define _OpenAPI_ee_subscription_ext_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "amf_subscription_info.h"
#include "context_info.h"
#include "hss_subscription_info.h"
#include "monitoring_configuration_1.h"
#include "reporting_options_1.h"
#include "smf_subscription_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ee_subscription_ext_s OpenAPI_ee_subscription_ext_t;
typedef struct OpenAPI_ee_subscription_ext_s {
    char *callback_reference;
    OpenAPI_list_t* monitoring_configurations;
    struct OpenAPI_reporting_options_1_s *reporting_options;
    char *supported_features;
    char *subscription_id;
    struct OpenAPI_context_info_s *context_info;
    bool is_epc_applied_ind;
    int epc_applied_ind;
    char *scef_diam_host;
    char *scef_diam_realm;
    char *notify_correlation_id;
    char *second_callback_ref;
    char *gpsi;
    OpenAPI_list_t *exclude_gpsi_list;
    OpenAPI_list_t *include_gpsi_list;
    char *data_restoration_callback_uri;
    OpenAPI_list_t *amf_subscription_info_list;
    struct OpenAPI_smf_subscription_info_s *smf_subscription_info;
    struct OpenAPI_hss_subscription_info_s *hss_subscription_info;
} OpenAPI_ee_subscription_ext_t;

OpenAPI_ee_subscription_ext_t *OpenAPI_ee_subscription_ext_create(
    char *callback_reference,
    OpenAPI_list_t* monitoring_configurations,
    OpenAPI_reporting_options_1_t *reporting_options,
    char *supported_features,
    char *subscription_id,
    OpenAPI_context_info_t *context_info,
    bool is_epc_applied_ind,
    int epc_applied_ind,
    char *scef_diam_host,
    char *scef_diam_realm,
    char *notify_correlation_id,
    char *second_callback_ref,
    char *gpsi,
    OpenAPI_list_t *exclude_gpsi_list,
    OpenAPI_list_t *include_gpsi_list,
    char *data_restoration_callback_uri,
    OpenAPI_list_t *amf_subscription_info_list,
    OpenAPI_smf_subscription_info_t *smf_subscription_info,
    OpenAPI_hss_subscription_info_t *hss_subscription_info
);
void OpenAPI_ee_subscription_ext_free(OpenAPI_ee_subscription_ext_t *ee_subscription_ext);
OpenAPI_ee_subscription_ext_t *OpenAPI_ee_subscription_ext_parseFromJSON(cJSON *ee_subscription_extJSON);
cJSON *OpenAPI_ee_subscription_ext_convertToJSON(OpenAPI_ee_subscription_ext_t *ee_subscription_ext);
OpenAPI_ee_subscription_ext_t *OpenAPI_ee_subscription_ext_copy(OpenAPI_ee_subscription_ext_t *dst, OpenAPI_ee_subscription_ext_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ee_subscription_ext_H_ */

