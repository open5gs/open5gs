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
#include "af_sig_protocol.h"
#include "eth_flow_description.h"
#include "flow_status.h"
#include "flow_usage.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_media_sub_component_rm_s OpenAPI_media_sub_component_rm_t;
typedef struct OpenAPI_media_sub_component_rm_s {
    OpenAPI_af_sig_protocol_e af_sig_protocol;
    OpenAPI_list_t *ethf_descs;
    int f_num;
    OpenAPI_list_t *f_descs;
    OpenAPI_flow_status_e f_status;
    char *mar_bw_dl;
    char *mar_bw_ul;
    char *tos_tr_cl;
    OpenAPI_flow_usage_e flow_usage;
} OpenAPI_media_sub_component_rm_t;

OpenAPI_media_sub_component_rm_t *OpenAPI_media_sub_component_rm_create(
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
void OpenAPI_media_sub_component_rm_free(OpenAPI_media_sub_component_rm_t *media_sub_component_rm);
OpenAPI_media_sub_component_rm_t *OpenAPI_media_sub_component_rm_parseFromJSON(cJSON *media_sub_component_rmJSON);
cJSON *OpenAPI_media_sub_component_rm_convertToJSON(OpenAPI_media_sub_component_rm_t *media_sub_component_rm);
OpenAPI_media_sub_component_rm_t *OpenAPI_media_sub_component_rm_copy(OpenAPI_media_sub_component_rm_t *dst, OpenAPI_media_sub_component_rm_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_media_sub_component_rm_H_ */

