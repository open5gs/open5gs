/*
 * media_sub_component_rm.h
 *
 * This data type is defined in the same way as the MediaSubComponent data type, but with the OpenAPI nullable property set to true. Removable attributes marBwDl and marBwUl are defined with the corresponding removable data type. 
 */

#ifndef _OpenAPI_media_sub_component_rm_H_
#define _OpenAPI_media_sub_component_rm_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_media_sub_component_rm_s OpenAPI_media_sub_component_rm_t;
#include "add_flow_description_info.h"
#include "af_sig_protocol.h"
#include "eth_flow_description.h"
#include "events_subsc_req_data_rm.h"
#include "flow_status.h"
#include "flow_usage.h"
#include "mpx_media_info.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_media_sub_component_rm_s {
    OpenAPI_af_sig_protocol_e af_sig_protocol;
    bool is_ethf_descs_null;
    OpenAPI_list_t *ethf_descs;
    int f_num;
    bool is_f_descs_null;
    OpenAPI_list_t *f_descs;
    bool is_add_info_flow_descs_null;
    OpenAPI_list_t *add_info_flow_descs;
    OpenAPI_flow_status_e f_status;
    bool is_mar_bw_dl_null;
    char *mar_bw_dl;
    bool is_mar_bw_ul_null;
    char *mar_bw_ul;
    bool is_tos_tr_cl_null;
    char *tos_tr_cl;
    OpenAPI_flow_usage_e flow_usage;
    bool is_ev_subsc_null;
    struct OpenAPI_events_subsc_req_data_rm_s *ev_subsc;
    bool is_mpx_media_ul_infos_null;
    OpenAPI_list_t *mpx_media_ul_infos;
    bool is_mpx_media_dl_infos_null;
    OpenAPI_list_t *mpx_media_dl_infos;
};

OpenAPI_media_sub_component_rm_t *OpenAPI_media_sub_component_rm_create(
    OpenAPI_af_sig_protocol_e af_sig_protocol,
    bool is_ethf_descs_null,
    OpenAPI_list_t *ethf_descs,
    int f_num,
    bool is_f_descs_null,
    OpenAPI_list_t *f_descs,
    bool is_add_info_flow_descs_null,
    OpenAPI_list_t *add_info_flow_descs,
    OpenAPI_flow_status_e f_status,
    bool is_mar_bw_dl_null,
    char *mar_bw_dl,
    bool is_mar_bw_ul_null,
    char *mar_bw_ul,
    bool is_tos_tr_cl_null,
    char *tos_tr_cl,
    OpenAPI_flow_usage_e flow_usage,
    bool is_ev_subsc_null,
    OpenAPI_events_subsc_req_data_rm_t *ev_subsc,
    bool is_mpx_media_ul_infos_null,
    OpenAPI_list_t *mpx_media_ul_infos,
    bool is_mpx_media_dl_infos_null,
    OpenAPI_list_t *mpx_media_dl_infos
);
void OpenAPI_media_sub_component_rm_free(OpenAPI_media_sub_component_rm_t *media_sub_component_rm);
OpenAPI_media_sub_component_rm_t *OpenAPI_media_sub_component_rm_parseFromJSON(cJSON *media_sub_component_rmJSON);
cJSON *OpenAPI_media_sub_component_rm_convertToJSON(OpenAPI_media_sub_component_rm_t *media_sub_component_rm);
OpenAPI_media_sub_component_rm_t *OpenAPI_media_sub_component_rm_copy(OpenAPI_media_sub_component_rm_t *dst, OpenAPI_media_sub_component_rm_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_media_sub_component_rm_H_ */

