/*
 * media_sub_component.h
 *
 * Identifies a media subcomponent.
 */

#ifndef _OpenAPI_media_sub_component_H_
#define _OpenAPI_media_sub_component_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_media_sub_component_s OpenAPI_media_sub_component_t;
#include "add_flow_description_info.h"
#include "af_sig_protocol.h"
#include "eth_flow_description.h"
#include "events_subsc_req_data.h"
#include "flow_status.h"
#include "flow_usage.h"
#include "mpx_media_info.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_media_sub_component_s {
    OpenAPI_af_sig_protocol_e af_sig_protocol;
    OpenAPI_list_t *ethf_descs;
    int f_num;
    OpenAPI_list_t *f_descs;
    OpenAPI_list_t *add_info_flow_descs;
    OpenAPI_flow_status_e f_status;
    char *mar_bw_dl;
    char *mar_bw_ul;
    char *tos_tr_cl;
    OpenAPI_flow_usage_e flow_usage;
    struct OpenAPI_events_subsc_req_data_s *ev_subsc;
    OpenAPI_list_t *mpx_media_ul_infos;
    OpenAPI_list_t *mpx_media_dl_infos;
};

OpenAPI_media_sub_component_t *OpenAPI_media_sub_component_create(
    OpenAPI_af_sig_protocol_e af_sig_protocol,
    OpenAPI_list_t *ethf_descs,
    int f_num,
    OpenAPI_list_t *f_descs,
    OpenAPI_list_t *add_info_flow_descs,
    OpenAPI_flow_status_e f_status,
    char *mar_bw_dl,
    char *mar_bw_ul,
    char *tos_tr_cl,
    OpenAPI_flow_usage_e flow_usage,
    OpenAPI_events_subsc_req_data_t *ev_subsc,
    OpenAPI_list_t *mpx_media_ul_infos,
    OpenAPI_list_t *mpx_media_dl_infos
);
void OpenAPI_media_sub_component_free(OpenAPI_media_sub_component_t *media_sub_component);
OpenAPI_media_sub_component_t *OpenAPI_media_sub_component_parseFromJSON(cJSON *media_sub_componentJSON);
cJSON *OpenAPI_media_sub_component_convertToJSON(OpenAPI_media_sub_component_t *media_sub_component);
OpenAPI_media_sub_component_t *OpenAPI_media_sub_component_copy(OpenAPI_media_sub_component_t *dst, OpenAPI_media_sub_component_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_media_sub_component_H_ */

