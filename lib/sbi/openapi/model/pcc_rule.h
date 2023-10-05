/*
 * pcc_rule.h
 *
 * Contains a PCC rule information.
 */

#ifndef _OpenAPI_pcc_rule_H_
#define _OpenAPI_pcc_rule_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "af_sig_protocol.h"
#include "downlink_data_notification_control.h"
#include "downlink_data_notification_control_rm.h"
#include "flow_information.h"
#include "tscai_input_container.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pcc_rule_s OpenAPI_pcc_rule_t;
typedef struct OpenAPI_pcc_rule_s {
    OpenAPI_list_t *flow_infos;
    char *app_id;
    char *app_descriptor;
    bool is_cont_ver;
    int cont_ver;
    char *pcc_rule_id;
    bool is_precedence;
    int precedence;
    OpenAPI_af_sig_protocol_e af_sig_protocol;
    bool is_app_reloc;
    int app_reloc;
    bool is_eas_redis_ind;
    int eas_redis_ind;
    OpenAPI_list_t *ref_qos_data;
    OpenAPI_list_t *ref_alt_qos_params;
    OpenAPI_list_t *ref_tc_data;
    bool is_ref_chg_data_null;
    OpenAPI_list_t *ref_chg_data;
    bool is_ref_chg_n3g_data_null;
    OpenAPI_list_t *ref_chg_n3g_data;
    bool is_ref_um_data_null;
    OpenAPI_list_t *ref_um_data;
    bool is_ref_um_n3g_data_null;
    OpenAPI_list_t *ref_um_n3g_data;
    bool is_ref_cond_data_null;
    char *ref_cond_data;
    bool is_ref_qos_mon_null;
    OpenAPI_list_t *ref_qos_mon;
    bool is_addr_preser_ind_null;
    bool is_addr_preser_ind;
    int addr_preser_ind;
    bool is_tscai_input_dl_null;
    struct OpenAPI_tscai_input_container_s *tscai_input_dl;
    bool is_tscai_input_ul_null;
    struct OpenAPI_tscai_input_container_s *tscai_input_ul;
    bool is_tscai_time_dom;
    int tscai_time_dom;
    struct OpenAPI_downlink_data_notification_control_s *dd_notif_ctrl;
    bool is_dd_notif_ctrl2_null;
    struct OpenAPI_downlink_data_notification_control_rm_s *dd_notif_ctrl2;
    bool is_dis_ue_notif_null;
    bool is_dis_ue_notif;
    int dis_ue_notif;
    bool is_pack_filt_all_prec;
    int pack_filt_all_prec;
} OpenAPI_pcc_rule_t;

OpenAPI_pcc_rule_t *OpenAPI_pcc_rule_create(
    OpenAPI_list_t *flow_infos,
    char *app_id,
    char *app_descriptor,
    bool is_cont_ver,
    int cont_ver,
    char *pcc_rule_id,
    bool is_precedence,
    int precedence,
    OpenAPI_af_sig_protocol_e af_sig_protocol,
    bool is_app_reloc,
    int app_reloc,
    bool is_eas_redis_ind,
    int eas_redis_ind,
    OpenAPI_list_t *ref_qos_data,
    OpenAPI_list_t *ref_alt_qos_params,
    OpenAPI_list_t *ref_tc_data,
    bool is_ref_chg_data_null,
    OpenAPI_list_t *ref_chg_data,
    bool is_ref_chg_n3g_data_null,
    OpenAPI_list_t *ref_chg_n3g_data,
    bool is_ref_um_data_null,
    OpenAPI_list_t *ref_um_data,
    bool is_ref_um_n3g_data_null,
    OpenAPI_list_t *ref_um_n3g_data,
    bool is_ref_cond_data_null,
    char *ref_cond_data,
    bool is_ref_qos_mon_null,
    OpenAPI_list_t *ref_qos_mon,
    bool is_addr_preser_ind_null,
    bool is_addr_preser_ind,
    int addr_preser_ind,
    bool is_tscai_input_dl_null,
    OpenAPI_tscai_input_container_t *tscai_input_dl,
    bool is_tscai_input_ul_null,
    OpenAPI_tscai_input_container_t *tscai_input_ul,
    bool is_tscai_time_dom,
    int tscai_time_dom,
    OpenAPI_downlink_data_notification_control_t *dd_notif_ctrl,
    bool is_dd_notif_ctrl2_null,
    OpenAPI_downlink_data_notification_control_rm_t *dd_notif_ctrl2,
    bool is_dis_ue_notif_null,
    bool is_dis_ue_notif,
    int dis_ue_notif,
    bool is_pack_filt_all_prec,
    int pack_filt_all_prec
);
void OpenAPI_pcc_rule_free(OpenAPI_pcc_rule_t *pcc_rule);
OpenAPI_pcc_rule_t *OpenAPI_pcc_rule_parseFromJSON(cJSON *pcc_ruleJSON);
cJSON *OpenAPI_pcc_rule_convertToJSON(OpenAPI_pcc_rule_t *pcc_rule);
OpenAPI_pcc_rule_t *OpenAPI_pcc_rule_copy(OpenAPI_pcc_rule_t *dst, OpenAPI_pcc_rule_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pcc_rule_H_ */

