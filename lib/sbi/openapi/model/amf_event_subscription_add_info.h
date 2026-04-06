/*
 * amf_event_subscription_add_info.h
 *
 * Additional information received for an AMF event subscription, e.g. binding indications, statistical information for UE access behavior trends report or UE location trends report. 
 */

#ifndef _OpenAPI_amf_event_subscription_add_info_H_
#define _OpenAPI_amf_event_subscription_add_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_amf_event_subscription_add_info_s OpenAPI_amf_event_subscription_add_info_t;
#include "area_of_interest_event_state.h"
#include "nf_type.h"
#include "ue_access_behavior_report_item.h"
#include "ue_location_trends_report_item.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_amf_event_subscription_add_info_s {
    OpenAPI_list_t *binding_info;
    OpenAPI_nf_type_e subscribing_nf_type;
    bool is_event_sync_ind;
    int event_sync_ind;
    OpenAPI_list_t *nf_consumer_info;
    OpenAPI_list_t* aoi_state_list;
    char *access_token;
    bool is_amf_set_level_bulk_subs;
    int amf_set_level_bulk_subs;
    OpenAPI_list_t *ue_access_behavior_trends;
    OpenAPI_list_t *ue_location_trends;
};

OpenAPI_amf_event_subscription_add_info_t *OpenAPI_amf_event_subscription_add_info_create(
    OpenAPI_list_t *binding_info,
    OpenAPI_nf_type_e subscribing_nf_type,
    bool is_event_sync_ind,
    int event_sync_ind,
    OpenAPI_list_t *nf_consumer_info,
    OpenAPI_list_t* aoi_state_list,
    char *access_token,
    bool is_amf_set_level_bulk_subs,
    int amf_set_level_bulk_subs,
    OpenAPI_list_t *ue_access_behavior_trends,
    OpenAPI_list_t *ue_location_trends
);
void OpenAPI_amf_event_subscription_add_info_free(OpenAPI_amf_event_subscription_add_info_t *amf_event_subscription_add_info);
OpenAPI_amf_event_subscription_add_info_t *OpenAPI_amf_event_subscription_add_info_parseFromJSON(cJSON *amf_event_subscription_add_infoJSON);
cJSON *OpenAPI_amf_event_subscription_add_info_convertToJSON(OpenAPI_amf_event_subscription_add_info_t *amf_event_subscription_add_info);
OpenAPI_amf_event_subscription_add_info_t *OpenAPI_amf_event_subscription_add_info_copy(OpenAPI_amf_event_subscription_add_info_t *dst, OpenAPI_amf_event_subscription_add_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_amf_event_subscription_add_info_H_ */

