/*
 * ue_context_transfer_rsp_data.h
 *
 * 
 */

#ifndef _OpenAPI_ue_context_transfer_rsp_data_H_
#define _OpenAPI_ue_context_transfer_rsp_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "n2_info_content.h"
#include "ue_context.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ue_context_transfer_rsp_data_s OpenAPI_ue_context_transfer_rsp_data_t;
typedef struct OpenAPI_ue_context_transfer_rsp_data_s {
    struct OpenAPI_ue_context_s *ue_context;
    struct OpenAPI_n2_info_content_s *ue_radio_capability;
    struct OpenAPI_n2_info_content_s *ue_nbiot_radio_capability;
    char *supported_features;
} OpenAPI_ue_context_transfer_rsp_data_t;

OpenAPI_ue_context_transfer_rsp_data_t *OpenAPI_ue_context_transfer_rsp_data_create(
    OpenAPI_ue_context_t *ue_context,
    OpenAPI_n2_info_content_t *ue_radio_capability,
    OpenAPI_n2_info_content_t *ue_nbiot_radio_capability,
    char *supported_features
);
void OpenAPI_ue_context_transfer_rsp_data_free(OpenAPI_ue_context_transfer_rsp_data_t *ue_context_transfer_rsp_data);
OpenAPI_ue_context_transfer_rsp_data_t *OpenAPI_ue_context_transfer_rsp_data_parseFromJSON(cJSON *ue_context_transfer_rsp_dataJSON);
cJSON *OpenAPI_ue_context_transfer_rsp_data_convertToJSON(OpenAPI_ue_context_transfer_rsp_data_t *ue_context_transfer_rsp_data);
OpenAPI_ue_context_transfer_rsp_data_t *OpenAPI_ue_context_transfer_rsp_data_copy(OpenAPI_ue_context_transfer_rsp_data_t *dst, OpenAPI_ue_context_transfer_rsp_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_context_transfer_rsp_data_H_ */

