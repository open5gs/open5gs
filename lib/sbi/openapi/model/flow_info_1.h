/*
 * flow_info_1.h
 *
 * Represents IP flow information.
 */

#ifndef _OpenAPI_flow_info_1_H_
#define _OpenAPI_flow_info_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_flow_info_1_s OpenAPI_flow_info_1_t;
#include "mpx_media_info_1.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_flow_info_1_s {
    int flow_id;
    OpenAPI_list_t *flow_descriptions;
    char *tos_tc;
    OpenAPI_list_t *mpx_media_ul_infos;
    OpenAPI_list_t *mpx_media_dl_infos;
};

OpenAPI_flow_info_1_t *OpenAPI_flow_info_1_create(
    int flow_id,
    OpenAPI_list_t *flow_descriptions,
    char *tos_tc,
    OpenAPI_list_t *mpx_media_ul_infos,
    OpenAPI_list_t *mpx_media_dl_infos
);
void OpenAPI_flow_info_1_free(OpenAPI_flow_info_1_t *flow_info_1);
OpenAPI_flow_info_1_t *OpenAPI_flow_info_1_parseFromJSON(cJSON *flow_info_1JSON);
cJSON *OpenAPI_flow_info_1_convertToJSON(OpenAPI_flow_info_1_t *flow_info_1);
OpenAPI_flow_info_1_t *OpenAPI_flow_info_1_copy(OpenAPI_flow_info_1_t *dst, OpenAPI_flow_info_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_flow_info_1_H_ */

