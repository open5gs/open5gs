/*
 * context_data_sets.h
 *
 *
 */

#ifndef _OpenAPI_context_data_sets_H_
#define _OpenAPI_context_data_sets_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "amf3_gpp_access_registration.h"
#include "amf_non3_gpp_access_registration.h"
#include "ee_subscription.h"
#include "sdm_subscription.h"
#include "smf_registration.h"
#include "smsf_registration.h"
#include "subscription_data_subscriptions.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_context_data_sets_s OpenAPI_context_data_sets_t;
typedef struct OpenAPI_context_data_sets_s {
    struct OpenAPI_amf3_gpp_access_registration_s *amf3_gpp;
    struct OpenAPI_amf_non3_gpp_access_registration_s *amf_non3_gpp;
    OpenAPI_list_t *sdm_subscriptions;
    OpenAPI_list_t *ee_subscriptions;
    struct OpenAPI_smsf_registration_s *smsf3_gpp_access;
    struct OpenAPI_smsf_registration_s *smsf_non3_gpp_access;
    OpenAPI_list_t *subscription_data_subscriptions;
    OpenAPI_list_t *smf_registrations;
} OpenAPI_context_data_sets_t;

OpenAPI_context_data_sets_t *OpenAPI_context_data_sets_create(
    OpenAPI_amf3_gpp_access_registration_t *amf3_gpp,
    OpenAPI_amf_non3_gpp_access_registration_t *amf_non3_gpp,
    OpenAPI_list_t *sdm_subscriptions,
    OpenAPI_list_t *ee_subscriptions,
    OpenAPI_smsf_registration_t *smsf3_gpp_access,
    OpenAPI_smsf_registration_t *smsf_non3_gpp_access,
    OpenAPI_list_t *subscription_data_subscriptions,
    OpenAPI_list_t *smf_registrations
    );
void OpenAPI_context_data_sets_free(OpenAPI_context_data_sets_t *context_data_sets);
OpenAPI_context_data_sets_t *OpenAPI_context_data_sets_parseFromJSON(cJSON *context_data_setsJSON);
cJSON *OpenAPI_context_data_sets_convertToJSON(OpenAPI_context_data_sets_t *context_data_sets);
OpenAPI_context_data_sets_t *OpenAPI_context_data_sets_copy(OpenAPI_context_data_sets_t *dst, OpenAPI_context_data_sets_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_context_data_sets_H_ */

