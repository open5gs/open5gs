/*
 * pc5_qo_s_para.h
 *
 * Contains policy data on the PC5 QoS parameters.
 */

#ifndef _OpenAPI_pc5_qo_s_para_H_
#define _OpenAPI_pc5_qo_s_para_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "pc5_qos_flow_item.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pc5_qo_s_para_s OpenAPI_pc5_qo_s_para_t;
typedef struct OpenAPI_pc5_qo_s_para_s {
    OpenAPI_list_t *pc5_qos_flow_list;
    char *pc5_link_ambr;
} OpenAPI_pc5_qo_s_para_t;

OpenAPI_pc5_qo_s_para_t *OpenAPI_pc5_qo_s_para_create(
    OpenAPI_list_t *pc5_qos_flow_list,
    char *pc5_link_ambr
);
void OpenAPI_pc5_qo_s_para_free(OpenAPI_pc5_qo_s_para_t *pc5_qo_s_para);
OpenAPI_pc5_qo_s_para_t *OpenAPI_pc5_qo_s_para_parseFromJSON(cJSON *pc5_qo_s_paraJSON);
cJSON *OpenAPI_pc5_qo_s_para_convertToJSON(OpenAPI_pc5_qo_s_para_t *pc5_qo_s_para);
OpenAPI_pc5_qo_s_para_t *OpenAPI_pc5_qo_s_para_copy(OpenAPI_pc5_qo_s_para_t *dst, OpenAPI_pc5_qo_s_para_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pc5_qo_s_para_H_ */

