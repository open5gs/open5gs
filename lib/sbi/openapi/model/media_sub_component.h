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
#include "af_sig_protocol.h"
#include "eth_flow_description.h"
#include "flow_status.h"
#include "flow_usage.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_media_sub_component_s OpenAPI_media_sub_component_t;
typedef struct OpenAPI_media_sub_component_s {
    OpenAPI_af_sig_protocol_e af_sig_protocol;
    OpenAPI_list_t *ethf_descs;
    int f_num;
    OpenAPI_list_t *f_descs;
    OpenAPI_flow_status_e f_status;
    char *mar_bw_dl;
    char *mar_bw_ul;
    char *tos_tr_cl;
    OpenAPI_flow_usage_e flow_usage;
} OpenAPI_media_sub_component_t;

OpenAPI_media_sub_component_t *OpenAPI_media_sub_component_create(
    OpenAPI_af_sig_protocol_e af_sig_protocol,
    OpenAPI_list_t *ethf_descs,
    int f_num,
    OpenAPI_list_t *f_descs,
    OpenAPI_flow_status_e f_status,
    char *mar_bw_dl,
    char *mar_bw_ul,
    char *tos_tr_cl,
    OpenAPI_flow_usage_e flow_usage
);
void OpenAPI_media_sub_component_free(OpenAPI_media_sub_component_t *media_sub_component);
OpenAPI_media_sub_component_t *OpenAPI_media_sub_component_parseFromJSON(cJSON *media_sub_componentJSON);
cJSON *OpenAPI_media_sub_component_convertToJSON(OpenAPI_media_sub_component_t *media_sub_component);
OpenAPI_media_sub_component_t *OpenAPI_media_sub_component_copy(OpenAPI_media_sub_component_t *dst, OpenAPI_media_sub_component_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_media_sub_component_H_ */

