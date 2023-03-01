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
    OpenAPI_binary_t* binary_data_n2_information;
} OpenAPI_n2_info_notify_handover_complete_200_response_t;

OpenAPI_n2_info_notify_handover_complete_200_response_t *OpenAPI_n2_info_notify_handover_complete_200_response_create(
    OpenAPI_n2_info_notification_rsp_data_t *json_data,
    OpenAPI_binary_t* binary_data_n2_information
);
void OpenAPI_n2_info_notify_handover_complete_200_response_free(OpenAPI_n2_info_notify_handover_complete_200_response_t *n2_info_notify_handover_complete_200_response);
OpenAPI_n2_info_notify_handover_complete_200_response_t *OpenAPI_n2_info_notify_handover_complete_200_response_parseFromJSON(cJSON *n2_info_notify_handover_complete_200_responseJSON);
cJSON *OpenAPI_n2_info_notify_handover_complete_200_response_convertToJSON(OpenAPI_n2_info_notify_handover_complete_200_response_t *n2_info_notify_handover_complete_200_response);
OpenAPI_n2_info_notify_handover_complete_200_response_t *OpenAPI_n2_info_notify_handover_complete_200_response_copy(OpenAPI_n2_info_notify_handover_complete_200_response_t *dst, OpenAPI_n2_info_notify_handover_complete_200_response_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_n2_info_notify_handover_complete_200_response_H_ */

