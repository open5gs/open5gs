/*
 * qos_flow_profile.h
 *
 * QoS flow profile
 */

#ifndef _OpenAPI_qos_flow_profile_H_
#define _OpenAPI_qos_flow_profile_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_qos_flow_profile_s OpenAPI_qos_flow_profile_t;
#include "additional_qos_flow_info.h"
#include "arp.h"
#include "dynamic5_qi.h"
#include "gbr_qos_flow_information.h"
#include "non_dynamic5_qi.h"
#include "pdu_set_qos_para.h"
#include "qos_monitoring_req.h"
#include "reflective_qo_s_attribute.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_qos_flow_profile_s {
    int _5qi;
    struct OpenAPI_non_dynamic5_qi_s *non_dynamic5_qi;
    struct OpenAPI_dynamic5_qi_s *dynamic5_qi;
    struct OpenAPI_arp_s *arp;
    struct OpenAPI_gbr_qos_flow_information_s *gbr_qos_flow_info;
    OpenAPI_reflective_qo_s_attribute_e rqa;
    OpenAPI_additional_qos_flow_info_e additional_qos_flow_info;
    OpenAPI_qos_monitoring_req_e qos_monitoring_req;
    bool is_qos_rep_period;
    int qos_rep_period;
    bool is_pdu_set_qos_dl_null;
    struct OpenAPI_pdu_set_qos_para_s *pdu_set_qos_dl;
    bool is_pdu_set_qos_ul_null;
    struct OpenAPI_pdu_set_qos_para_s *pdu_set_qos_ul;
    bool is_dl_pdu_set_mark_sup_ind;
    int dl_pdu_set_mark_sup_ind;
    char *multi_modal_id;
    bool is_ul_bitrate_adapt_ind;
    int ul_bitrate_adapt_ind;
    bool is_no_qos_mon_pd_wo_n3_dl_teid;
    int no_qos_mon_pd_wo_n3_dl_teid;
};

OpenAPI_qos_flow_profile_t *OpenAPI_qos_flow_profile_create(
    int _5qi,
    OpenAPI_non_dynamic5_qi_t *non_dynamic5_qi,
    OpenAPI_dynamic5_qi_t *dynamic5_qi,
    OpenAPI_arp_t *arp,
    OpenAPI_gbr_qos_flow_information_t *gbr_qos_flow_info,
    OpenAPI_reflective_qo_s_attribute_e rqa,
    OpenAPI_additional_qos_flow_info_e additional_qos_flow_info,
    OpenAPI_qos_monitoring_req_e qos_monitoring_req,
    bool is_qos_rep_period,
    int qos_rep_period,
    bool is_pdu_set_qos_dl_null,
    OpenAPI_pdu_set_qos_para_t *pdu_set_qos_dl,
    bool is_pdu_set_qos_ul_null,
    OpenAPI_pdu_set_qos_para_t *pdu_set_qos_ul,
    bool is_dl_pdu_set_mark_sup_ind,
    int dl_pdu_set_mark_sup_ind,
    char *multi_modal_id,
    bool is_ul_bitrate_adapt_ind,
    int ul_bitrate_adapt_ind,
    bool is_no_qos_mon_pd_wo_n3_dl_teid,
    int no_qos_mon_pd_wo_n3_dl_teid
);
void OpenAPI_qos_flow_profile_free(OpenAPI_qos_flow_profile_t *qos_flow_profile);
OpenAPI_qos_flow_profile_t *OpenAPI_qos_flow_profile_parseFromJSON(cJSON *qos_flow_profileJSON);
cJSON *OpenAPI_qos_flow_profile_convertToJSON(OpenAPI_qos_flow_profile_t *qos_flow_profile);
OpenAPI_qos_flow_profile_t *OpenAPI_qos_flow_profile_copy(OpenAPI_qos_flow_profile_t *dst, OpenAPI_qos_flow_profile_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_qos_flow_profile_H_ */

