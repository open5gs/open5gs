/*
 * relocate_ue_context_request.h
 *
 * 
 */

#ifndef _OpenAPI_relocate_ue_context_request_H_
#define _OpenAPI_relocate_ue_context_request_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ue_context_relocate_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_relocate_ue_context_request_s OpenAPI_relocate_ue_context_request_t;
typedef struct OpenAPI_relocate_ue_context_request_s {
    struct OpenAPI_ue_context_relocate_data_s *json_data;
    OpenAPI_binary_t* binary_data_forward_relocation_request;
    OpenAPI_binary_t* binary_data_n2_information;
    OpenAPI_binary_t* binary_data_n2_information_ext1;
    OpenAPI_binary_t* binary_data_n2_information_ext2;
    OpenAPI_binary_t* binary_data_n2_information_ext3;
    OpenAPI_binary_t* binary_data_n2_information_ext4;
    OpenAPI_binary_t* binary_data_n2_information_ext5;
    OpenAPI_binary_t* binary_data_n2_information_ext6;
    OpenAPI_binary_t* binary_data_n2_information_ext7;
    OpenAPI_binary_t* binary_data_n2_information_ext8;
    OpenAPI_binary_t* binary_data_n2_information_ext9;
    OpenAPI_binary_t* binary_data_n2_information_ext10;
    OpenAPI_binary_t* binary_data_n2_information_ext11;
    OpenAPI_binary_t* binary_data_n2_information_ext12;
    OpenAPI_binary_t* binary_data_n2_information_ext13;
    OpenAPI_binary_t* binary_data_n2_information_ext14;
    OpenAPI_binary_t* binary_data_n2_information_ext15;
    OpenAPI_binary_t* binary_data_n2_information_ext16;
} OpenAPI_relocate_ue_context_request_t;

OpenAPI_relocate_ue_context_request_t *OpenAPI_relocate_ue_context_request_create(
    OpenAPI_ue_context_relocate_data_t *json_data,
    OpenAPI_binary_t* binary_data_forward_relocation_request,
    OpenAPI_binary_t* binary_data_n2_information,
    OpenAPI_binary_t* binary_data_n2_information_ext1,
    OpenAPI_binary_t* binary_data_n2_information_ext2,
    OpenAPI_binary_t* binary_data_n2_information_ext3,
    OpenAPI_binary_t* binary_data_n2_information_ext4,
    OpenAPI_binary_t* binary_data_n2_information_ext5,
    OpenAPI_binary_t* binary_data_n2_information_ext6,
    OpenAPI_binary_t* binary_data_n2_information_ext7,
    OpenAPI_binary_t* binary_data_n2_information_ext8,
    OpenAPI_binary_t* binary_data_n2_information_ext9,
    OpenAPI_binary_t* binary_data_n2_information_ext10,
    OpenAPI_binary_t* binary_data_n2_information_ext11,
    OpenAPI_binary_t* binary_data_n2_information_ext12,
    OpenAPI_binary_t* binary_data_n2_information_ext13,
    OpenAPI_binary_t* binary_data_n2_information_ext14,
    OpenAPI_binary_t* binary_data_n2_information_ext15,
    OpenAPI_binary_t* binary_data_n2_information_ext16
);
void OpenAPI_relocate_ue_context_request_free(OpenAPI_relocate_ue_context_request_t *relocate_ue_context_request);
OpenAPI_relocate_ue_context_request_t *OpenAPI_relocate_ue_context_request_parseFromJSON(cJSON *relocate_ue_context_requestJSON);
cJSON *OpenAPI_relocate_ue_context_request_convertToJSON(OpenAPI_relocate_ue_context_request_t *relocate_ue_context_request);
OpenAPI_relocate_ue_context_request_t *OpenAPI_relocate_ue_context_request_copy(OpenAPI_relocate_ue_context_request_t *dst, OpenAPI_relocate_ue_context_request_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_relocate_ue_context_request_H_ */

