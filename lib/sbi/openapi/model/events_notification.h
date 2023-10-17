/*
 * events_notification.h
 *
 * Describes the notification of a matched event.
 */

#ifndef _OpenAPI_events_notification_H_
#define _OpenAPI_events_notification_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "acc_net_charging_address.h"
#include "access_net_charging_identifier.h"
#include "access_type.h"
#include "accumulated_usage.h"
#include "additional_access_info.h"
#include "af_event_notification.h"
#include "an_gw_address.h"
#include "app_detection_report.h"
#include "bridge_management_container.h"
#include "net_loc_access_support.h"
#include "out_of_credit_information.h"
#include "plmn_id_nid.h"
#include "port_management_container.h"
#include "qos_monitoring_report.h"
#include "qos_notification_control_info.h"
#include "ran_nas_rel_cause.h"
#include "rat_type.h"
#include "resources_allocation_info.h"
#include "satellite_backhaul_category.h"
#include "user_location.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_events_notification_s OpenAPI_events_notification_t;
typedef struct OpenAPI_events_notification_s {
    OpenAPI_list_t *ad_reports;
    OpenAPI_access_type_e access_type;
    struct OpenAPI_additional_access_info_s *add_access_info;
    struct OpenAPI_additional_access_info_s *rel_access_info;
    struct OpenAPI_acc_net_charging_address_s *an_charg_addr;
    OpenAPI_list_t *an_charg_ids;
    struct OpenAPI_an_gw_address_s *an_gw_addr;
    char *ev_subs_uri;
    OpenAPI_list_t *ev_notifs;
    OpenAPI_list_t *failed_resourc_alloc_reports;
    OpenAPI_list_t *succ_resourc_alloc_reports;
    OpenAPI_net_loc_access_support_e no_net_loc_supp;
    OpenAPI_list_t *out_of_cred_reports;
    struct OpenAPI_plmn_id_nid_s *plmn_id;
    OpenAPI_list_t *qnc_reports;
    OpenAPI_list_t *qos_mon_reports;
    OpenAPI_list_t *ran_nas_rel_causes;
    OpenAPI_rat_type_e rat_type;
    OpenAPI_satellite_backhaul_category_e sat_backhaul_category;
    struct OpenAPI_user_location_s *ue_loc;
    char *ue_loc_time;
    char *ue_time_zone;
    struct OpenAPI_accumulated_usage_s *usg_rep;
    struct OpenAPI_bridge_management_container_s *tsn_bridge_man_cont;
    struct OpenAPI_port_management_container_s *tsn_port_man_cont_dstt;
    OpenAPI_list_t *tsn_port_man_cont_nwtts;
} OpenAPI_events_notification_t;

OpenAPI_events_notification_t *OpenAPI_events_notification_create(
    OpenAPI_list_t *ad_reports,
    OpenAPI_access_type_e access_type,
    OpenAPI_additional_access_info_t *add_access_info,
    OpenAPI_additional_access_info_t *rel_access_info,
    OpenAPI_acc_net_charging_address_t *an_charg_addr,
    OpenAPI_list_t *an_charg_ids,
    OpenAPI_an_gw_address_t *an_gw_addr,
    char *ev_subs_uri,
    OpenAPI_list_t *ev_notifs,
    OpenAPI_list_t *failed_resourc_alloc_reports,
    OpenAPI_list_t *succ_resourc_alloc_reports,
    OpenAPI_net_loc_access_support_e no_net_loc_supp,
    OpenAPI_list_t *out_of_cred_reports,
    OpenAPI_plmn_id_nid_t *plmn_id,
    OpenAPI_list_t *qnc_reports,
    OpenAPI_list_t *qos_mon_reports,
    OpenAPI_list_t *ran_nas_rel_causes,
    OpenAPI_rat_type_e rat_type,
    OpenAPI_satellite_backhaul_category_e sat_backhaul_category,
    OpenAPI_user_location_t *ue_loc,
    char *ue_loc_time,
    char *ue_time_zone,
    OpenAPI_accumulated_usage_t *usg_rep,
    OpenAPI_bridge_management_container_t *tsn_bridge_man_cont,
    OpenAPI_port_management_container_t *tsn_port_man_cont_dstt,
    OpenAPI_list_t *tsn_port_man_cont_nwtts
);
void OpenAPI_events_notification_free(OpenAPI_events_notification_t *events_notification);
OpenAPI_events_notification_t *OpenAPI_events_notification_parseFromJSON(cJSON *events_notificationJSON);
cJSON *OpenAPI_events_notification_convertToJSON(OpenAPI_events_notification_t *events_notification);
OpenAPI_events_notification_t *OpenAPI_events_notification_copy(OpenAPI_events_notification_t *dst, OpenAPI_events_notification_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_events_notification_H_ */

