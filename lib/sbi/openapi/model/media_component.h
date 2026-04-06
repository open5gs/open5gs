/*
 * media_component.h
 *
 * 
 */

#ifndef _OpenAPI_media_component_H_
#define _OpenAPI_media_component_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_media_component_s OpenAPI_media_component_t;
#include "af_header_handling_control_info.h"
#include "af_routing_requirement.h"
#include "af_sfc_requirement.h"
#include "alternative_service_requirements_data.h"
#include "flow_status.h"
#include "media_sub_component.h"
#include "media_type.h"
#include "on_path_n6_sig_info.h"
#include "pdu_set_qos_para.h"
#include "preemption_capability.h"
#include "preemption_vulnerability.h"
#include "priority_sharing_indicator.h"
#include "protocol_description.h"
#include "reserv_priority.h"
#include "rtt_flow_reference.h"
#include "tscai_input_container.h"
#include "tsn_qos_container.h"
#include "uplink_downlink_support.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_media_component_s {
    char *af_app_id;
    struct OpenAPI_af_routing_requirement_s *af_rout_req;
    bool is_af_sfc_req_null;
    struct OpenAPI_af_sfc_requirement_s *af_sfc_req;
    bool is_af_hdr_req_null;
    struct OpenAPI_af_header_handling_control_info_s *af_hdr_req;
    char *qos_reference;
    bool is_dis_ue_notif;
    int dis_ue_notif;
    OpenAPI_list_t *alt_ser_reqs;
    OpenAPI_list_t *alt_ser_reqs_data;
    bool is_cont_ver;
    int cont_ver;
    OpenAPI_list_t *codecs;
    bool is_des_max_latency;
    float des_max_latency;
    bool is_des_max_loss;
    float des_max_loss;
    char *flus_id;
    OpenAPI_flow_status_e f_status;
    char *mar_bw_dl;
    char *mar_bw_ul;
    bool is_max_packet_loss_rate_dl_null;
    bool is_max_packet_loss_rate_dl;
    int max_packet_loss_rate_dl;
    bool is_max_packet_loss_rate_ul_null;
    bool is_max_packet_loss_rate_ul;
    int max_packet_loss_rate_ul;
    char *max_supp_bw_dl;
    char *max_supp_bw_ul;
    int med_comp_n;
    OpenAPI_list_t* med_sub_comps;
    OpenAPI_media_type_e med_type;
    char *min_des_bw_dl;
    char *min_des_bw_ul;
    char *mir_bw_dl;
    char *mir_bw_ul;
    OpenAPI_preemption_capability_e preempt_cap;
    OpenAPI_preemption_vulnerability_e preempt_vuln;
    OpenAPI_priority_sharing_indicator_e prio_sharing_ind;
    OpenAPI_reserv_priority_e res_prio;
    char *rr_bw;
    char *rs_bw;
    bool is_sharing_key_dl;
    int sharing_key_dl;
    bool is_sharing_key_ul;
    int sharing_key_ul;
    struct OpenAPI_tsn_qos_container_s *tsn_qos;
    bool is_tscai_input_dl_null;
    struct OpenAPI_tscai_input_container_s *tscai_input_dl;
    bool is_tscai_input_ul_null;
    struct OpenAPI_tscai_input_container_s *tscai_input_ul;
    bool is_tscai_time_dom;
    int tscai_time_dom;
    bool is_cap_bat_adaptation;
    int cap_bat_adaptation;
    bool is_r_t_latency_ind;
    int r_t_latency_ind;
    bool is_pdb;
    int pdb;
    struct OpenAPI_rtt_flow_reference_s *r_t_latency_ind_corre_id;
    bool is_pdu_set_qos_dl_null;
    struct OpenAPI_pdu_set_qos_para_s *pdu_set_qos_dl;
    bool is_pdu_set_qos_ul_null;
    struct OpenAPI_pdu_set_qos_para_s *pdu_set_qos_ul;
    struct OpenAPI_protocol_description_s *proto_desc_dl;
    struct OpenAPI_protocol_description_s *proto_desc_ul;
    bool is_period_ul;
    int period_ul;
    bool is_period_dl;
    int period_dl;
    OpenAPI_uplink_downlink_support_e l4s_ind;
    bool is_dat_burst_size_ind;
    int dat_burst_size_ind;
    bool is_timeto_next_burst_ind;
    int timeto_next_burst_ind;
    bool is_on_path_n6_sig_info_null;
    struct OpenAPI_on_path_n6_sig_info_s *on_path_n6_sig_info;
    bool is_exp_tran_ind;
    int exp_tran_ind;
    bool is_ul_br_rec_ind;
    int ul_br_rec_ind;
};

OpenAPI_media_component_t *OpenAPI_media_component_create(
    char *af_app_id,
    OpenAPI_af_routing_requirement_t *af_rout_req,
    bool is_af_sfc_req_null,
    OpenAPI_af_sfc_requirement_t *af_sfc_req,
    bool is_af_hdr_req_null,
    OpenAPI_af_header_handling_control_info_t *af_hdr_req,
    char *qos_reference,
    bool is_dis_ue_notif,
    int dis_ue_notif,
    OpenAPI_list_t *alt_ser_reqs,
    OpenAPI_list_t *alt_ser_reqs_data,
    bool is_cont_ver,
    int cont_ver,
    OpenAPI_list_t *codecs,
    bool is_des_max_latency,
    float des_max_latency,
    bool is_des_max_loss,
    float des_max_loss,
    char *flus_id,
    OpenAPI_flow_status_e f_status,
    char *mar_bw_dl,
    char *mar_bw_ul,
    bool is_max_packet_loss_rate_dl_null,
    bool is_max_packet_loss_rate_dl,
    int max_packet_loss_rate_dl,
    bool is_max_packet_loss_rate_ul_null,
    bool is_max_packet_loss_rate_ul,
    int max_packet_loss_rate_ul,
    char *max_supp_bw_dl,
    char *max_supp_bw_ul,
    int med_comp_n,
    OpenAPI_list_t* med_sub_comps,
    OpenAPI_media_type_e med_type,
    char *min_des_bw_dl,
    char *min_des_bw_ul,
    char *mir_bw_dl,
    char *mir_bw_ul,
    OpenAPI_preemption_capability_e preempt_cap,
    OpenAPI_preemption_vulnerability_e preempt_vuln,
    OpenAPI_priority_sharing_indicator_e prio_sharing_ind,
    OpenAPI_reserv_priority_e res_prio,
    char *rr_bw,
    char *rs_bw,
    bool is_sharing_key_dl,
    int sharing_key_dl,
    bool is_sharing_key_ul,
    int sharing_key_ul,
    OpenAPI_tsn_qos_container_t *tsn_qos,
    bool is_tscai_input_dl_null,
    OpenAPI_tscai_input_container_t *tscai_input_dl,
    bool is_tscai_input_ul_null,
    OpenAPI_tscai_input_container_t *tscai_input_ul,
    bool is_tscai_time_dom,
    int tscai_time_dom,
    bool is_cap_bat_adaptation,
    int cap_bat_adaptation,
    bool is_r_t_latency_ind,
    int r_t_latency_ind,
    bool is_pdb,
    int pdb,
    OpenAPI_rtt_flow_reference_t *r_t_latency_ind_corre_id,
    bool is_pdu_set_qos_dl_null,
    OpenAPI_pdu_set_qos_para_t *pdu_set_qos_dl,
    bool is_pdu_set_qos_ul_null,
    OpenAPI_pdu_set_qos_para_t *pdu_set_qos_ul,
    OpenAPI_protocol_description_t *proto_desc_dl,
    OpenAPI_protocol_description_t *proto_desc_ul,
    bool is_period_ul,
    int period_ul,
    bool is_period_dl,
    int period_dl,
    OpenAPI_uplink_downlink_support_e l4s_ind,
    bool is_dat_burst_size_ind,
    int dat_burst_size_ind,
    bool is_timeto_next_burst_ind,
    int timeto_next_burst_ind,
    bool is_on_path_n6_sig_info_null,
    OpenAPI_on_path_n6_sig_info_t *on_path_n6_sig_info,
    bool is_exp_tran_ind,
    int exp_tran_ind,
    bool is_ul_br_rec_ind,
    int ul_br_rec_ind
);
void OpenAPI_media_component_free(OpenAPI_media_component_t *media_component);
OpenAPI_media_component_t *OpenAPI_media_component_parseFromJSON(cJSON *media_componentJSON);
cJSON *OpenAPI_media_component_convertToJSON(OpenAPI_media_component_t *media_component);
OpenAPI_media_component_t *OpenAPI_media_component_copy(OpenAPI_media_component_t *dst, OpenAPI_media_component_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_media_component_H_ */

