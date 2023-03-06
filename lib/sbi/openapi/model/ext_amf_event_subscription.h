/*
 * ext_amf_event_subscription.h
 *
 * AMF event subscription extended with additional information received for the subscription
 */

#ifndef _OpenAPI_ext_amf_event_subscription_H_
#define _OpenAPI_ext_amf_event_subscription_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "amf_event.h"
#include "amf_event_mode.h"
#include "area_of_interest_event_state.h"
#include "nf_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ext_amf_event_subscription_s OpenAPI_ext_amf_event_subscription_t;
typedef struct OpenAPI_ext_amf_event_subscription_s {
    OpenAPI_list_t *event_list;
    char *event_notify_uri;
    char *notify_correlation_id;
    char *nf_id;
    char *subs_change_notify_uri;
    char *subs_change_notify_correlation_id;
    char *supi;
    char *group_id;
    OpenAPI_list_t *exclude_supi_list;
    OpenAPI_list_t *exclude_gpsi_list;
    OpenAPI_list_t *include_supi_list;
    OpenAPI_list_t *include_gpsi_list;
    char *gpsi;
    char *pei;
    bool is_any_ue;
    int any_ue;
    struct OpenAPI_amf_event_mode_s *options;
    OpenAPI_nf_type_e source_nf_type;
    OpenAPI_list_t *binding_info;
    OpenAPI_nf_type_e subscribing_nf_type;
    bool is_event_sync_ind;
    int event_sync_ind;
    OpenAPI_list_t *nf_consumer_info;
    OpenAPI_list_t* aoi_state_list;
} OpenAPI_ext_amf_event_subscription_t;

OpenAPI_ext_amf_event_subscription_t *OpenAPI_ext_amf_event_subscription_create(
    OpenAPI_list_t *event_list,
    char *event_notify_uri,
    char *notify_correlation_id,
    char *nf_id,
    char *subs_change_notify_uri,
    char *subs_change_notify_correlation_id,
    char *supi,
    char *group_id,
    OpenAPI_list_t *exclude_supi_list,
    OpenAPI_list_t *exclude_gpsi_list,
    OpenAPI_list_t *include_supi_list,
    OpenAPI_list_t *include_gpsi_list,
    char *gpsi,
    char *pei,
    bool is_any_ue,
    int any_ue,
    OpenAPI_amf_event_mode_t *options,
    OpenAPI_nf_type_e source_nf_type,
    OpenAPI_list_t *binding_info,
    OpenAPI_nf_type_e subscribing_nf_type,
    bool is_event_sync_ind,
    int event_sync_ind,
    OpenAPI_list_t *nf_consumer_info,
    OpenAPI_list_t* aoi_state_list
);
void OpenAPI_ext_amf_event_subscription_free(OpenAPI_ext_amf_event_subscription_t *ext_amf_event_subscription);
OpenAPI_ext_amf_event_subscription_t *OpenAPI_ext_amf_event_subscription_parseFromJSON(cJSON *ext_amf_event_subscriptionJSON);
cJSON *OpenAPI_ext_amf_event_subscription_convertToJSON(OpenAPI_ext_amf_event_subscription_t *ext_amf_event_subscription);
OpenAPI_ext_amf_event_subscription_t *OpenAPI_ext_amf_event_subscription_copy(OpenAPI_ext_amf_event_subscription_t *dst, OpenAPI_ext_amf_event_subscription_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ext_amf_event_subscription_H_ */

