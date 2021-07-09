/*
 * n1_n2_message_transfer_error.h
 *
 * 
 */

#ifndef _OpenAPI_n1_n2_message_transfer_error_H_
#define _OpenAPI_n1_n2_message_transfer_error_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "n1_n2_msg_txfr_err_detail.h"
#include "problem_details.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_n1_n2_message_transfer_error_s OpenAPI_n1_n2_message_transfer_error_t;
typedef struct OpenAPI_n1_n2_message_transfer_error_s {
    struct OpenAPI_problem_details_s *error;
    struct OpenAPI_n1_n2_msg_txfr_err_detail_s *err_info;
} OpenAPI_n1_n2_message_transfer_error_t;

OpenAPI_n1_n2_message_transfer_error_t *OpenAPI_n1_n2_message_transfer_error_create(
    OpenAPI_problem_details_t *error,
    OpenAPI_n1_n2_msg_txfr_err_detail_t *err_info
);
void OpenAPI_n1_n2_message_transfer_error_free(OpenAPI_n1_n2_message_transfer_error_t *n1_n2_message_transfer_error);
OpenAPI_n1_n2_message_transfer_error_t *OpenAPI_n1_n2_message_transfer_error_parseFromJSON(cJSON *n1_n2_message_transfer_errorJSON);
cJSON *OpenAPI_n1_n2_message_transfer_error_convertToJSON(OpenAPI_n1_n2_message_transfer_error_t *n1_n2_message_transfer_error);
OpenAPI_n1_n2_message_transfer_error_t *OpenAPI_n1_n2_message_transfer_error_copy(OpenAPI_n1_n2_message_transfer_error_t *dst, OpenAPI_n1_n2_message_transfer_error_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_n1_n2_message_transfer_error_H_ */

