/*
 * n2_info_notification_rsp_data.h
 *
 * 
 */

#ifndef _OpenAPI_n2_info_notification_rsp_data_H_
#define _OpenAPI_n2_info_notification_rsp_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "n2_info_content.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_n2_info_notification_rsp_data_s OpenAPI_n2_info_notification_rsp_data_t;
typedef struct OpenAPI_n2_info_notification_rsp_data_s {
    struct OpenAPI_n2_info_content_s *n2_info_content;
} OpenAPI_n2_info_notification_rsp_data_t;

OpenAPI_n2_info_notification_rsp_data_t *OpenAPI_n2_info_notification_rsp_data_create(
    OpenAPI_n2_info_content_t *n2_info_content
);
void OpenAPI_n2_info_notification_rsp_data_free(OpenAPI_n2_info_notification_rsp_data_t *n2_info_notification_rsp_data);
OpenAPI_n2_info_notification_rsp_data_t *OpenAPI_n2_info_notification_rsp_data_parseFromJSON(cJSON *n2_info_notification_rsp_dataJSON);
cJSON *OpenAPI_n2_info_notification_rsp_data_convertToJSON(OpenAPI_n2_info_notification_rsp_data_t *n2_info_notification_rsp_data);
OpenAPI_n2_info_notification_rsp_data_t *OpenAPI_n2_info_notification_rsp_data_copy(OpenAPI_n2_info_notification_rsp_data_t *dst, OpenAPI_n2_info_notification_rsp_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_n2_info_notification_rsp_data_H_ */

