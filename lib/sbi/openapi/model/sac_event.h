/*
 * sac_event.h
 *
 * Describes an slice event to be subscribed
 */

#ifndef _OpenAPI_sac_event_H_
#define _OpenAPI_sac_event_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_sac_event_s OpenAPI_sac_event_t;
#include "access_type.h"
#include "plmn_id_nid.h"
#include "sac_event_trigger.h"
#include "sac_event_type.h"
#include "sac_info.h"
#include "snssai.h"
#include "var_rep_period.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_sac_event_s {
    OpenAPI_sac_event_type_e event_type;
    OpenAPI_sac_event_trigger_e event_trigger;
    OpenAPI_list_t *event_filter;
    bool is_notification_period;
    int notification_period;
    struct OpenAPI_sac_info_s *notif_threshold;
    bool is_immediate_flag;
    int immediate_flag;
    OpenAPI_list_t *var_rep_period_info;
    struct OpenAPI_plmn_id_nid_s *plmn_id_nid;
    OpenAPI_list_t *access_type_filter;
    OpenAPI_list_t *service_area_filter;
};

OpenAPI_sac_event_t *OpenAPI_sac_event_create(
    OpenAPI_sac_event_type_e event_type,
    OpenAPI_sac_event_trigger_e event_trigger,
    OpenAPI_list_t *event_filter,
    bool is_notification_period,
    int notification_period,
    OpenAPI_sac_info_t *notif_threshold,
    bool is_immediate_flag,
    int immediate_flag,
    OpenAPI_list_t *var_rep_period_info,
    OpenAPI_plmn_id_nid_t *plmn_id_nid,
    OpenAPI_list_t *access_type_filter,
    OpenAPI_list_t *service_area_filter
);
void OpenAPI_sac_event_free(OpenAPI_sac_event_t *sac_event);
OpenAPI_sac_event_t *OpenAPI_sac_event_parseFromJSON(cJSON *sac_eventJSON);
cJSON *OpenAPI_sac_event_convertToJSON(OpenAPI_sac_event_t *sac_event);
OpenAPI_sac_event_t *OpenAPI_sac_event_copy(OpenAPI_sac_event_t *dst, OpenAPI_sac_event_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sac_event_H_ */

