/*
 * app_session_context_req_data.h
 *
 * Identifies the service requirements of an Individual Application Session Context.
 */

#ifndef _OpenAPI_app_session_context_req_data_H_
#define _OpenAPI_app_session_context_req_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "af_requested_data.h"
#include "af_routing_requirement.h"
#include "bridge_management_container.h"
#include "events_subsc_req_data.h"
#include "media_component.h"
#include "port_management_container.h"
#include "preemption_control_information.h"
#include "reserv_priority.h"
#include "service_info_status.h"
#include "snssai.h"
#include "sponsoring_status.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_app_session_context_req_data_s OpenAPI_app_session_context_req_data_t;
typedef struct OpenAPI_app_session_context_req_data_s {
    char *af_app_id;
    char *af_charg_id;
    OpenAPI_af_requested_data_e af_req_data;
    struct OpenAPI_af_routing_requirement_s *af_rout_req;
    char *asp_id;
    char *bdt_ref_id;
    char *dnn;
    struct OpenAPI_events_subsc_req_data_s *ev_subsc;
    char *mcptt_id;
    char *mc_video_id;
    OpenAPI_list_t* med_components;
    char *ip_domain;
    char *mps_id;
    char *mcs_id;
    OpenAPI_preemption_control_information_e preempt_control_info;
    OpenAPI_reserv_priority_e res_prio;
    OpenAPI_service_info_status_e serv_inf_status;
    char *notif_uri;
    char *serv_urn;
    struct OpenAPI_snssai_s *slice_info;
    char *spon_id;
    OpenAPI_sponsoring_status_e spon_status;
    char *supi;
    char *gpsi;
    char *supp_feat;
    char *ue_ipv4;
    char *ue_ipv6;
    char *ue_mac;
    struct OpenAPI_bridge_management_container_s *tsn_bridge_man_cont;
    struct OpenAPI_port_management_container_s *tsn_port_man_cont_dstt;
    OpenAPI_list_t *tsn_port_man_cont_nwtts;
} OpenAPI_app_session_context_req_data_t;

OpenAPI_app_session_context_req_data_t *OpenAPI_app_session_context_req_data_create(
    char *af_app_id,
    char *af_charg_id,
    OpenAPI_af_requested_data_e af_req_data,
    OpenAPI_af_routing_requirement_t *af_rout_req,
    char *asp_id,
    char *bdt_ref_id,
    char *dnn,
    OpenAPI_events_subsc_req_data_t *ev_subsc,
    char *mcptt_id,
    char *mc_video_id,
    OpenAPI_list_t* med_components,
    char *ip_domain,
    char *mps_id,
    char *mcs_id,
    OpenAPI_preemption_control_information_e preempt_control_info,
    OpenAPI_reserv_priority_e res_prio,
    OpenAPI_service_info_status_e serv_inf_status,
    char *notif_uri,
    char *serv_urn,
    OpenAPI_snssai_t *slice_info,
    char *spon_id,
    OpenAPI_sponsoring_status_e spon_status,
    char *supi,
    char *gpsi,
    char *supp_feat,
    char *ue_ipv4,
    char *ue_ipv6,
    char *ue_mac,
    OpenAPI_bridge_management_container_t *tsn_bridge_man_cont,
    OpenAPI_port_management_container_t *tsn_port_man_cont_dstt,
    OpenAPI_list_t *tsn_port_man_cont_nwtts
);
void OpenAPI_app_session_context_req_data_free(OpenAPI_app_session_context_req_data_t *app_session_context_req_data);
OpenAPI_app_session_context_req_data_t *OpenAPI_app_session_context_req_data_parseFromJSON(cJSON *app_session_context_req_dataJSON);
cJSON *OpenAPI_app_session_context_req_data_convertToJSON(OpenAPI_app_session_context_req_data_t *app_session_context_req_data);
OpenAPI_app_session_context_req_data_t *OpenAPI_app_session_context_req_data_copy(OpenAPI_app_session_context_req_data_t *dst, OpenAPI_app_session_context_req_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_app_session_context_req_data_H_ */

