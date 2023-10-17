/*
 * media_component_rm.h
 *
 * This data type is defined in the same way as the MediaComponent data type, but with the OpenAPI nullable property set to true.
 */

#ifndef _OpenAPI_media_component_rm_H_
#define _OpenAPI_media_component_rm_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "af_routing_requirement_rm.h"
#include "alternative_service_requirements_data.h"
#include "flow_status.h"
#include "media_sub_component_rm.h"
#include "media_type.h"
#include "preemption_capability_rm.h"
#include "preemption_vulnerability_rm.h"
#include "priority_sharing_indicator.h"
#include "reserv_priority.h"
#include "tscai_input_container.h"
#include "tsn_qos_container_rm.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_media_component_rm_s OpenAPI_media_component_rm_t;
typedef struct OpenAPI_media_component_rm_s {
    char *af_app_id;
    bool is_af_rout_req_null;
    struct OpenAPI_af_routing_requirement_rm_s *af_rout_req;
    bool is_qos_reference_null;
    char *qos_reference;
    bool is_alt_ser_reqs_null;
    OpenAPI_list_t *alt_ser_reqs;
    bool is_alt_ser_reqs_data_null;
    OpenAPI_list_t *alt_ser_reqs_data;
    bool is_dis_ue_notif;
    int dis_ue_notif;
    bool is_cont_ver;
    int cont_ver;
    OpenAPI_list_t *codecs;
    bool is_des_max_latency_null;
    bool is_des_max_latency;
    float des_max_latency;
    bool is_des_max_loss_null;
    bool is_des_max_loss;
    float des_max_loss;
    bool is_flus_id_null;
    char *flus_id;
    OpenAPI_flow_status_e f_status;
    bool is_mar_bw_dl_null;
    char *mar_bw_dl;
    bool is_mar_bw_ul_null;
    char *mar_bw_ul;
    bool is_max_packet_loss_rate_dl_null;
    bool is_max_packet_loss_rate_dl;
    int max_packet_loss_rate_dl;
    bool is_max_packet_loss_rate_ul_null;
    bool is_max_packet_loss_rate_ul;
    int max_packet_loss_rate_ul;
    bool is_max_supp_bw_dl_null;
    char *max_supp_bw_dl;
    bool is_max_supp_bw_ul_null;
    char *max_supp_bw_ul;
    int med_comp_n;
    OpenAPI_list_t* med_sub_comps;
    OpenAPI_media_type_e med_type;
    bool is_min_des_bw_dl_null;
    char *min_des_bw_dl;
    bool is_min_des_bw_ul_null;
    char *min_des_bw_ul;
    bool is_mir_bw_dl_null;
    char *mir_bw_dl;
    bool is_mir_bw_ul_null;
    char *mir_bw_ul;
    struct OpenAPI_preemption_capability_rm_s *preempt_cap;
    struct OpenAPI_preemption_vulnerability_rm_s *preempt_vuln;
    OpenAPI_priority_sharing_indicator_e prio_sharing_ind;
    OpenAPI_reserv_priority_e res_prio;
    bool is_rr_bw_null;
    char *rr_bw;
    bool is_rs_bw_null;
    char *rs_bw;
    bool is_sharing_key_dl_null;
    bool is_sharing_key_dl;
    int sharing_key_dl;
    bool is_sharing_key_ul_null;
    bool is_sharing_key_ul;
    int sharing_key_ul;
    bool is_tsn_qos_null;
    struct OpenAPI_tsn_qos_container_rm_s *tsn_qos;
    bool is_tscai_input_dl_null;
    struct OpenAPI_tscai_input_container_s *tscai_input_dl;
    bool is_tscai_input_ul_null;
    struct OpenAPI_tscai_input_container_s *tscai_input_ul;
    bool is_tscai_time_dom;
    int tscai_time_dom;
} OpenAPI_media_component_rm_t;

OpenAPI_media_component_rm_t *OpenAPI_media_component_rm_create(
    char *af_app_id,
    bool is_af_rout_req_null,
    OpenAPI_af_routing_requirement_rm_t *af_rout_req,
    bool is_qos_reference_null,
    char *qos_reference,
    bool is_alt_ser_reqs_null,
    OpenAPI_list_t *alt_ser_reqs,
    bool is_alt_ser_reqs_data_null,
    OpenAPI_list_t *alt_ser_reqs_data,
    bool is_dis_ue_notif,
    int dis_ue_notif,
    bool is_cont_ver,
    int cont_ver,
    OpenAPI_list_t *codecs,
    bool is_des_max_latency_null,
    bool is_des_max_latency,
    float des_max_latency,
    bool is_des_max_loss_null,
    bool is_des_max_loss,
    float des_max_loss,
    bool is_flus_id_null,
    char *flus_id,
    OpenAPI_flow_status_e f_status,
    bool is_mar_bw_dl_null,
    char *mar_bw_dl,
    bool is_mar_bw_ul_null,
    char *mar_bw_ul,
    bool is_max_packet_loss_rate_dl_null,
    bool is_max_packet_loss_rate_dl,
    int max_packet_loss_rate_dl,
    bool is_max_packet_loss_rate_ul_null,
    bool is_max_packet_loss_rate_ul,
    int max_packet_loss_rate_ul,
    bool is_max_supp_bw_dl_null,
    char *max_supp_bw_dl,
    bool is_max_supp_bw_ul_null,
    char *max_supp_bw_ul,
    int med_comp_n,
    OpenAPI_list_t* med_sub_comps,
    OpenAPI_media_type_e med_type,
    bool is_min_des_bw_dl_null,
    char *min_des_bw_dl,
    bool is_min_des_bw_ul_null,
    char *min_des_bw_ul,
    bool is_mir_bw_dl_null,
    char *mir_bw_dl,
    bool is_mir_bw_ul_null,
    char *mir_bw_ul,
    OpenAPI_preemption_capability_rm_t *preempt_cap,
    OpenAPI_preemption_vulnerability_rm_t *preempt_vuln,
    OpenAPI_priority_sharing_indicator_e prio_sharing_ind,
    OpenAPI_reserv_priority_e res_prio,
    bool is_rr_bw_null,
    char *rr_bw,
    bool is_rs_bw_null,
    char *rs_bw,
    bool is_sharing_key_dl_null,
    bool is_sharing_key_dl,
    int sharing_key_dl,
    bool is_sharing_key_ul_null,
    bool is_sharing_key_ul,
    int sharing_key_ul,
    bool is_tsn_qos_null,
    OpenAPI_tsn_qos_container_rm_t *tsn_qos,
    bool is_tscai_input_dl_null,
    OpenAPI_tscai_input_container_t *tscai_input_dl,
    bool is_tscai_input_ul_null,
    OpenAPI_tscai_input_container_t *tscai_input_ul,
    bool is_tscai_time_dom,
    int tscai_time_dom
);
void OpenAPI_media_component_rm_free(OpenAPI_media_component_rm_t *media_component_rm);
OpenAPI_media_component_rm_t *OpenAPI_media_component_rm_parseFromJSON(cJSON *media_component_rmJSON);
cJSON *OpenAPI_media_component_rm_convertToJSON(OpenAPI_media_component_rm_t *media_component_rm);
OpenAPI_media_component_rm_t *OpenAPI_media_component_rm_copy(OpenAPI_media_component_rm_t *dst, OpenAPI_media_component_rm_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_media_component_rm_H_ */

