/*
 * n2_info_notify_handover_complete_200_response.h
 *
 * 
 */

#ifndef _OpenAPI_n2_info_notify_handover_complete_200_response_H_
#define _OpenAPI_n2_info_notify_handover_complete_200_response_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "n2_info_notification_rsp_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_n2_info_notify_handover_complete_200_response_s OpenAPI_n2_info_notify_handover_complete_200_response_t;
typedef struct OpenAPI_n2_info_notify_handover_complete_200_response_s {
    struct OpenAPI_n2_info_notification_rsp_data_s *json_data;
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
} OpenAPI_n2_info_notify_handover_complete_200_response_t;

OpenAPI_n2_info_notify_handover_complete_200_response_t *OpenAPI_n2_info_notify_handover_complete_200_response_create(
    OpenAPI_n2_info_notification_rsp_data_t *json_data,
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
void OpenAPI_n2_info_notify_handover_complete_200_response_free(OpenAPI_n2_info_notify_handover_complete_200_response_t *n2_info_notify_handover_complete_200_response);
OpenAPI_n2_info_notify_handover_complete_200_response_t *OpenAPI_n2_info_notify_handover_complete_200_response_parseFromJSON(cJSON *n2_info_notify_handover_complete_200_responseJSON);
cJSON *OpenAPI_n2_info_notify_handover_complete_200_response_convertToJSON(OpenAPI_n2_info_notify_handover_complete_200_response_t *n2_info_notify_handover_complete_200_response);
OpenAPI_n2_info_notify_handover_complete_200_response_t *OpenAPI_n2_info_notify_handover_complete_200_response_copy(OpenAPI_n2_info_notify_handover_complete_200_response_t *dst, OpenAPI_n2_info_notify_handover_complete_200_response_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_n2_info_notify_handover_complete_200_response_H_ */

