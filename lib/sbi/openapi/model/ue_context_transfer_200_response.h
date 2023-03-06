/*
 * ue_context_transfer_200_response.h
 *
 * 
 */

#ifndef _OpenAPI_ue_context_transfer_200_response_H_
#define _OpenAPI_ue_context_transfer_200_response_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ue_context_transfer_rsp_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ue_context_transfer_200_response_s OpenAPI_ue_context_transfer_200_response_t;
typedef struct OpenAPI_ue_context_transfer_200_response_s {
    struct OpenAPI_ue_context_transfer_rsp_data_s *json_data;
    OpenAPI_binary_t* binary_data_n2_information;
    OpenAPI_binary_t* binary_data_n2_information_ext1;
    OpenAPI_binary_t* binary_data_n2_information_ext2;
} OpenAPI_ue_context_transfer_200_response_t;

OpenAPI_ue_context_transfer_200_response_t *OpenAPI_ue_context_transfer_200_response_create(
    OpenAPI_ue_context_transfer_rsp_data_t *json_data,
    OpenAPI_binary_t* binary_data_n2_information,
    OpenAPI_binary_t* binary_data_n2_information_ext1,
    OpenAPI_binary_t* binary_data_n2_information_ext2
);
void OpenAPI_ue_context_transfer_200_response_free(OpenAPI_ue_context_transfer_200_response_t *ue_context_transfer_200_response);
OpenAPI_ue_context_transfer_200_response_t *OpenAPI_ue_context_transfer_200_response_parseFromJSON(cJSON *ue_context_transfer_200_responseJSON);
cJSON *OpenAPI_ue_context_transfer_200_response_convertToJSON(OpenAPI_ue_context_transfer_200_response_t *ue_context_transfer_200_response);
OpenAPI_ue_context_transfer_200_response_t *OpenAPI_ue_context_transfer_200_response_copy(OpenAPI_ue_context_transfer_200_response_t *dst, OpenAPI_ue_context_transfer_200_response_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_context_transfer_200_response_H_ */

