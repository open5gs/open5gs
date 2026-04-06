/*
 * service_parameter_data_patch.h
 *
 * Represents the service parameter data that can be updated.
 */

#ifndef _OpenAPI_service_parameter_data_patch_H_
#define _OpenAPI_service_parameter_data_patch_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_service_parameter_data_patch_s OpenAPI_service_parameter_data_patch_t;
#include "event.h"
#include "tnap_id.h"
#include "ursp_rule_request.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_service_parameter_data_patch_s {
    bool is_param_over_pc5_null;
    char *param_over_pc5;
    bool is_param_over_uu_null;
    char *param_over_uu;
    bool is_a2x_params_pc5_null;
    char *a2x_params_pc5;
    bool is_a2x_params_uu_null;
    char *a2x_params_uu;
    bool is_param_for_pro_se_dd_null;
    char *param_for_pro_se_dd;
    bool is_param_for_pro_se_dc_null;
    char *param_for_pro_se_dc;
    bool is_param_for_pro_se_u2_n_rel_ue_null;
    char *param_for_pro_se_u2_n_rel_ue;
    bool is_param_for_pro_se_rem_ue_null;
    char *param_for_pro_se_rem_ue;
    bool is_param_for_pro_se_u2_u_rel_ue_null;
    char *param_for_pro_se_u2_u_rel_ue;
    bool is_param_for_pro_se_end_ue_null;
    char *param_for_pro_se_end_ue;
    bool is_multi_hop_u2_u_rel_ue_null;
    char *multi_hop_u2_u_rel_ue;
    bool is_multi_hop_u2_u_end_ue_null;
    char *multi_hop_u2_u_end_ue;
    bool is_multi_hop_u2_n_rel_ue_null;
    char *multi_hop_u2_n_rel_ue;
    bool is_multi_hop_u2_n_rem_ue_null;
    char *multi_hop_u2_n_rem_ue;
    bool is_multi_hop_u2_n_interm_ue_null;
    char *multi_hop_u2_n_interm_ue;
    OpenAPI_list_t *ursp_influence;
    bool is_ursp_guidance_null;
    OpenAPI_list_t *ursp_guidance;
    bool is_vps_ursp_guidance_null;
    OpenAPI_list_t *vps_ursp_guidance;
    bool is_tnaps_null;
    OpenAPI_list_t *tnaps;
    bool is_delivery_events_null;
    OpenAPI_list_t *delivery_events;
    char *polic_deliv_notif_uri;
    OpenAPI_list_t *headers;
    bool is_param_for_ranging_sl_pos_null;
    char *param_for_ranging_sl_pos;
};

OpenAPI_service_parameter_data_patch_t *OpenAPI_service_parameter_data_patch_create(
    bool is_param_over_pc5_null,
    char *param_over_pc5,
    bool is_param_over_uu_null,
    char *param_over_uu,
    bool is_a2x_params_pc5_null,
    char *a2x_params_pc5,
    bool is_a2x_params_uu_null,
    char *a2x_params_uu,
    bool is_param_for_pro_se_dd_null,
    char *param_for_pro_se_dd,
    bool is_param_for_pro_se_dc_null,
    char *param_for_pro_se_dc,
    bool is_param_for_pro_se_u2_n_rel_ue_null,
    char *param_for_pro_se_u2_n_rel_ue,
    bool is_param_for_pro_se_rem_ue_null,
    char *param_for_pro_se_rem_ue,
    bool is_param_for_pro_se_u2_u_rel_ue_null,
    char *param_for_pro_se_u2_u_rel_ue,
    bool is_param_for_pro_se_end_ue_null,
    char *param_for_pro_se_end_ue,
    bool is_multi_hop_u2_u_rel_ue_null,
    char *multi_hop_u2_u_rel_ue,
    bool is_multi_hop_u2_u_end_ue_null,
    char *multi_hop_u2_u_end_ue,
    bool is_multi_hop_u2_n_rel_ue_null,
    char *multi_hop_u2_n_rel_ue,
    bool is_multi_hop_u2_n_rem_ue_null,
    char *multi_hop_u2_n_rem_ue,
    bool is_multi_hop_u2_n_interm_ue_null,
    char *multi_hop_u2_n_interm_ue,
    OpenAPI_list_t *ursp_influence,
    bool is_ursp_guidance_null,
    OpenAPI_list_t *ursp_guidance,
    bool is_vps_ursp_guidance_null,
    OpenAPI_list_t *vps_ursp_guidance,
    bool is_tnaps_null,
    OpenAPI_list_t *tnaps,
    bool is_delivery_events_null,
    OpenAPI_list_t *delivery_events,
    char *polic_deliv_notif_uri,
    OpenAPI_list_t *headers,
    bool is_param_for_ranging_sl_pos_null,
    char *param_for_ranging_sl_pos
);
void OpenAPI_service_parameter_data_patch_free(OpenAPI_service_parameter_data_patch_t *service_parameter_data_patch);
OpenAPI_service_parameter_data_patch_t *OpenAPI_service_parameter_data_patch_parseFromJSON(cJSON *service_parameter_data_patchJSON);
cJSON *OpenAPI_service_parameter_data_patch_convertToJSON(OpenAPI_service_parameter_data_patch_t *service_parameter_data_patch);
OpenAPI_service_parameter_data_patch_t *OpenAPI_service_parameter_data_patch_copy(OpenAPI_service_parameter_data_patch_t *dst, OpenAPI_service_parameter_data_patch_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_service_parameter_data_patch_H_ */

