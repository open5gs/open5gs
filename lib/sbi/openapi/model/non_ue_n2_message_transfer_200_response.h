/*
 * non_ue_n2_message_transfer_200_response.h
 *
 * 
 */

#ifndef _OpenAPI_non_ue_n2_message_transfer_200_response_H_
#define _OpenAPI_non_ue_n2_message_transfer_200_response_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_non_ue_n2_message_transfer_200_response_s OpenAPI_non_ue_n2_message_transfer_200_response_t;
#include "n2_information_transfer_rsp_data.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_non_ue_n2_message_transfer_200_response_s {
    struct OpenAPI_n2_information_transfer_rsp_data_s *json_data;
    OpenAPI_binary_t* binary_data_n2_information1;
    OpenAPI_binary_t* binary_data_n2_information2;
    OpenAPI_binary_t* binary_data_n2_information3;
    OpenAPI_binary_t* binary_data_n2_information4;
    OpenAPI_binary_t* binary_data_n2_information5;
    OpenAPI_binary_t* binary_data_n2_information6;
    OpenAPI_binary_t* binary_data_n2_information7;
    OpenAPI_binary_t* binary_data_n2_information8;
    OpenAPI_binary_t* binary_data_n2_information9;
    OpenAPI_binary_t* binary_data_n2_information10;
};

OpenAPI_non_ue_n2_message_transfer_200_response_t *OpenAPI_non_ue_n2_message_transfer_200_response_create(
    OpenAPI_n2_information_transfer_rsp_data_t *json_data,
    OpenAPI_binary_t* binary_data_n2_information1,
    OpenAPI_binary_t* binary_data_n2_information2,
    OpenAPI_binary_t* binary_data_n2_information3,
    OpenAPI_binary_t* binary_data_n2_information4,
    OpenAPI_binary_t* binary_data_n2_information5,
    OpenAPI_binary_t* binary_data_n2_information6,
    OpenAPI_binary_t* binary_data_n2_information7,
    OpenAPI_binary_t* binary_data_n2_information8,
    OpenAPI_binary_t* binary_data_n2_information9,
    OpenAPI_binary_t* binary_data_n2_information10
);
void OpenAPI_non_ue_n2_message_transfer_200_response_free(OpenAPI_non_ue_n2_message_transfer_200_response_t *non_ue_n2_message_transfer_200_response);
OpenAPI_non_ue_n2_message_transfer_200_response_t *OpenAPI_non_ue_n2_message_transfer_200_response_parseFromJSON(cJSON *non_ue_n2_message_transfer_200_responseJSON);
cJSON *OpenAPI_non_ue_n2_message_transfer_200_response_convertToJSON(OpenAPI_non_ue_n2_message_transfer_200_response_t *non_ue_n2_message_transfer_200_response);
OpenAPI_non_ue_n2_message_transfer_200_response_t *OpenAPI_non_ue_n2_message_transfer_200_response_copy(OpenAPI_non_ue_n2_message_transfer_200_response_t *dst, OpenAPI_non_ue_n2_message_transfer_200_response_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_non_ue_n2_message_transfer_200_response_H_ */

