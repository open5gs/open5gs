/*
 * dnn_smf_info_item_dnai_list_inner.h
 *
 * 
 */

#ifndef _OpenAPI_dnn_smf_info_item_dnai_list_inner_H_
#define _OpenAPI_dnn_smf_info_item_dnai_list_inner_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_dnn_smf_info_item_dnai_list_inner_s OpenAPI_dnn_smf_info_item_dnai_list_inner_t;
typedef struct OpenAPI_dnn_smf_info_item_dnai_list_inner_s {
} OpenAPI_dnn_smf_info_item_dnai_list_inner_t;

OpenAPI_dnn_smf_info_item_dnai_list_inner_t *OpenAPI_dnn_smf_info_item_dnai_list_inner_create(
);
void OpenAPI_dnn_smf_info_item_dnai_list_inner_free(OpenAPI_dnn_smf_info_item_dnai_list_inner_t *dnn_smf_info_item_dnai_list_inner);
OpenAPI_dnn_smf_info_item_dnai_list_inner_t *OpenAPI_dnn_smf_info_item_dnai_list_inner_parseFromJSON(cJSON *dnn_smf_info_item_dnai_list_innerJSON);
cJSON *OpenAPI_dnn_smf_info_item_dnai_list_inner_convertToJSON(OpenAPI_dnn_smf_info_item_dnai_list_inner_t *dnn_smf_info_item_dnai_list_inner);
OpenAPI_dnn_smf_info_item_dnai_list_inner_t *OpenAPI_dnn_smf_info_item_dnai_list_inner_copy(OpenAPI_dnn_smf_info_item_dnai_list_inner_t *dst, OpenAPI_dnn_smf_info_item_dnai_list_inner_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dnn_smf_info_item_dnai_list_inner_H_ */

