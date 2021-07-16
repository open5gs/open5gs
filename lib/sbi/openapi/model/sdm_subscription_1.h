/*
 * sdm_subscription_1.h
 *
 * 
 */

#ifndef _OpenAPI_sdm_subscription_1_H_
#define _OpenAPI_sdm_subscription_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "context_info.h"
#include "plmn_id_1.h"
#include "snssai.h"
#include "subscription_data_sets_1.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_sdm_subscription_1_s OpenAPI_sdm_subscription_1_t;
typedef struct OpenAPI_sdm_subscription_1_s {
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
    struct OpenAPI_plmn_id_1_s *plmn_id;
    bool is_immediate_report;
    int immediate_report;
    struct OpenAPI_subscription_data_sets_1_s *report;
    char *supported_features;
    struct OpenAPI_context_info_s *context_info;
} OpenAPI_sdm_subscription_1_t;

OpenAPI_sdm_subscription_1_t *OpenAPI_sdm_subscription_1_create(
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
    OpenAPI_plmn_id_1_t *plmn_id,
    bool is_immediate_report,
    int immediate_report,
    OpenAPI_subscription_data_sets_1_t *report,
    char *supported_features,
    OpenAPI_context_info_t *context_info
);
void OpenAPI_sdm_subscription_1_free(OpenAPI_sdm_subscription_1_t *sdm_subscription_1);
OpenAPI_sdm_subscription_1_t *OpenAPI_sdm_subscription_1_parseFromJSON(cJSON *sdm_subscription_1JSON);
cJSON *OpenAPI_sdm_subscription_1_convertToJSON(OpenAPI_sdm_subscription_1_t *sdm_subscription_1);
OpenAPI_sdm_subscription_1_t *OpenAPI_sdm_subscription_1_copy(OpenAPI_sdm_subscription_1_t *dst, OpenAPI_sdm_subscription_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sdm_subscription_1_H_ */

