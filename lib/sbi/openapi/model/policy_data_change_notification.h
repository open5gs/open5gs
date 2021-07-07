/*
 * policy_data_change_notification.h
 *
 * Contains changed policy data for which notification was requested.
 */

#ifndef _OpenAPI_policy_data_change_notification_H_
#define _OpenAPI_policy_data_change_notification_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "am_policy_data.h"
#include "bdt_data.h"
#include "notification_item.h"
#include "operator_specific_data_container.h"
#include "plmn_id_1.h"
#include "sm_policy_data.h"
#include "sponsor_connectivity_data.h"
#include "ue_policy_set.h"
#include "usage_mon_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_policy_data_change_notification_s OpenAPI_policy_data_change_notification_t;
typedef struct OpenAPI_policy_data_change_notification_s {
    struct OpenAPI_am_policy_data_s *am_policy_data;
    struct OpenAPI_ue_policy_set_s *ue_policy_set;
    struct OpenAPI_ue_policy_set_s *plmn_ue_policy_set;
    struct OpenAPI_sm_policy_data_s *sm_policy_data;
    struct OpenAPI_usage_mon_data_s *usage_mon_data;
    struct OpenAPI_sponsor_connectivity_data_s *sponsor_connectivity_data;
    struct OpenAPI_bdt_data_s *bdt_data;
    struct OpenAPI_operator_specific_data_container_s *op_spec_data;
    OpenAPI_list_t* op_spec_data_map;
    char *ue_id;
    char *sponsor_id;
    char *bdt_ref_id;
    char *usage_mon_id;
    struct OpenAPI_plmn_id_1_s *plmn_id;
    OpenAPI_list_t *del_resources;
    char *notif_id;
    OpenAPI_list_t *reported_fragments;
} OpenAPI_policy_data_change_notification_t;

OpenAPI_policy_data_change_notification_t *OpenAPI_policy_data_change_notification_create(
    OpenAPI_am_policy_data_t *am_policy_data,
    OpenAPI_ue_policy_set_t *ue_policy_set,
    OpenAPI_ue_policy_set_t *plmn_ue_policy_set,
    OpenAPI_sm_policy_data_t *sm_policy_data,
    OpenAPI_usage_mon_data_t *usage_mon_data,
    OpenAPI_sponsor_connectivity_data_t *sponsor_connectivity_data,
    OpenAPI_bdt_data_t *bdt_data,
    OpenAPI_operator_specific_data_container_t *op_spec_data,
    OpenAPI_list_t* op_spec_data_map,
    char *ue_id,
    char *sponsor_id,
    char *bdt_ref_id,
    char *usage_mon_id,
    OpenAPI_plmn_id_1_t *plmn_id,
    OpenAPI_list_t *del_resources,
    char *notif_id,
    OpenAPI_list_t *reported_fragments
);
void OpenAPI_policy_data_change_notification_free(OpenAPI_policy_data_change_notification_t *policy_data_change_notification);
OpenAPI_policy_data_change_notification_t *OpenAPI_policy_data_change_notification_parseFromJSON(cJSON *policy_data_change_notificationJSON);
cJSON *OpenAPI_policy_data_change_notification_convertToJSON(OpenAPI_policy_data_change_notification_t *policy_data_change_notification);
OpenAPI_policy_data_change_notification_t *OpenAPI_policy_data_change_notification_copy(OpenAPI_policy_data_change_notification_t *dst, OpenAPI_policy_data_change_notification_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_policy_data_change_notification_H_ */

