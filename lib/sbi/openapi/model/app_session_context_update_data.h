/*
 * app_session_context_update_data.h
 *
 * Identifies the modifications to the \&quot;ascReqData\&quot; property of an Individual Application Session Context which may include the modifications to the sub-resource Events Subscription.
 */

#ifndef _OpenAPI_app_session_context_update_data_H_
#define _OpenAPI_app_session_context_update_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "af_routing_requirement_rm.h"
#include "bridge_management_container.h"
#include "events_subsc_req_data_rm.h"
#include "media_component_rm.h"
#include "port_management_container.h"
#include "preemption_control_information_rm.h"
#include "reserv_priority.h"
#include "service_info_status.h"
#include "sip_forking_indication.h"
#include "sponsoring_status.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_app_session_context_update_data_s OpenAPI_app_session_context_update_data_t;
typedef struct OpenAPI_app_session_context_update_data_s {
    char *af_app_id;
    struct OpenAPI_af_routing_requirement_rm_s *af_rout_req;
    char *asp_id;
    char *bdt_ref_id;
    struct OpenAPI_events_subsc_req_data_rm_s *ev_subsc;
    char *mcptt_id;
    char *mc_video_id;
    OpenAPI_list_t* med_components;
    char *mps_id;
    char *mcs_id;
    struct OpenAPI_preemption_control_information_rm_s *preempt_control_info;
    OpenAPI_reserv_priority_e res_prio;
    OpenAPI_service_info_status_e serv_inf_status;
    OpenAPI_sip_forking_indication_e sip_fork_ind;
    char *spon_id;
    OpenAPI_sponsoring_status_e spon_status;
    struct OpenAPI_bridge_management_container_s *tsn_bridge_man_cont;
    struct OpenAPI_port_management_container_s *tsn_port_man_cont_dstt;
    OpenAPI_list_t *tsn_port_man_cont_nwtts;
} OpenAPI_app_session_context_update_data_t;

OpenAPI_app_session_context_update_data_t *OpenAPI_app_session_context_update_data_create(
    char *af_app_id,
    OpenAPI_af_routing_requirement_rm_t *af_rout_req,
    char *asp_id,
    char *bdt_ref_id,
    OpenAPI_events_subsc_req_data_rm_t *ev_subsc,
    char *mcptt_id,
    char *mc_video_id,
    OpenAPI_list_t* med_components,
    char *mps_id,
    char *mcs_id,
    OpenAPI_preemption_control_information_rm_t *preempt_control_info,
    OpenAPI_reserv_priority_e res_prio,
    OpenAPI_service_info_status_e serv_inf_status,
    OpenAPI_sip_forking_indication_e sip_fork_ind,
    char *spon_id,
    OpenAPI_sponsoring_status_e spon_status,
    OpenAPI_bridge_management_container_t *tsn_bridge_man_cont,
    OpenAPI_port_management_container_t *tsn_port_man_cont_dstt,
    OpenAPI_list_t *tsn_port_man_cont_nwtts
);
void OpenAPI_app_session_context_update_data_free(OpenAPI_app_session_context_update_data_t *app_session_context_update_data);
OpenAPI_app_session_context_update_data_t *OpenAPI_app_session_context_update_data_parseFromJSON(cJSON *app_session_context_update_dataJSON);
cJSON *OpenAPI_app_session_context_update_data_convertToJSON(OpenAPI_app_session_context_update_data_t *app_session_context_update_data);
OpenAPI_app_session_context_update_data_t *OpenAPI_app_session_context_update_data_copy(OpenAPI_app_session_context_update_data_t *dst, OpenAPI_app_session_context_update_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_app_session_context_update_data_H_ */

