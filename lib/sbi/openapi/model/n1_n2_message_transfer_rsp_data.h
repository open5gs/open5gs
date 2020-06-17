/*
 * n1_n2_message_transfer_rsp_data.h
 *
 *
 */

#ifndef _OpenAPI_n1_n2_message_transfer_rsp_data_H_
#define _OpenAPI_n1_n2_message_transfer_rsp_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "n1_n2_message_transfer_cause.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_n1_n2_message_transfer_rsp_data_s OpenAPI_n1_n2_message_transfer_rsp_data_t;
typedef struct OpenAPI_n1_n2_message_transfer_rsp_data_s {
    OpenAPI_n1_n2_message_transfer_cause_e cause;
    char *supported_features;
} OpenAPI_n1_n2_message_transfer_rsp_data_t;

OpenAPI_n1_n2_message_transfer_rsp_data_t *OpenAPI_n1_n2_message_transfer_rsp_data_create(
    OpenAPI_n1_n2_message_transfer_cause_e cause,
    char *supported_features
    );
void OpenAPI_n1_n2_message_transfer_rsp_data_free(OpenAPI_n1_n2_message_transfer_rsp_data_t *n1_n2_message_transfer_rsp_data);
OpenAPI_n1_n2_message_transfer_rsp_data_t *OpenAPI_n1_n2_message_transfer_rsp_data_parseFromJSON(cJSON *n1_n2_message_transfer_rsp_dataJSON);
cJSON *OpenAPI_n1_n2_message_transfer_rsp_data_convertToJSON(OpenAPI_n1_n2_message_transfer_rsp_data_t *n1_n2_message_transfer_rsp_data);
OpenAPI_n1_n2_message_transfer_rsp_data_t *OpenAPI_n1_n2_message_transfer_rsp_data_copy(OpenAPI_n1_n2_message_transfer_rsp_data_t *dst, OpenAPI_n1_n2_message_transfer_rsp_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_n1_n2_message_transfer_rsp_data_H_ */

