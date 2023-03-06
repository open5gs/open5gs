/*
 * transfer_mt_data_ismf_request.h
 *
 * 
 */

#ifndef _OpenAPI_transfer_mt_data_ismf_request_H_
#define _OpenAPI_transfer_mt_data_ismf_request_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "transfer_mt_data_req_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_transfer_mt_data_ismf_request_s OpenAPI_transfer_mt_data_ismf_request_t;
typedef struct OpenAPI_transfer_mt_data_ismf_request_s {
    struct OpenAPI_transfer_mt_data_req_data_s *json_data;
    OpenAPI_binary_t* binary_mt_data;
} OpenAPI_transfer_mt_data_ismf_request_t;

OpenAPI_transfer_mt_data_ismf_request_t *OpenAPI_transfer_mt_data_ismf_request_create(
    OpenAPI_transfer_mt_data_req_data_t *json_data,
    OpenAPI_binary_t* binary_mt_data
);
void OpenAPI_transfer_mt_data_ismf_request_free(OpenAPI_transfer_mt_data_ismf_request_t *transfer_mt_data_ismf_request);
OpenAPI_transfer_mt_data_ismf_request_t *OpenAPI_transfer_mt_data_ismf_request_parseFromJSON(cJSON *transfer_mt_data_ismf_requestJSON);
cJSON *OpenAPI_transfer_mt_data_ismf_request_convertToJSON(OpenAPI_transfer_mt_data_ismf_request_t *transfer_mt_data_ismf_request);
OpenAPI_transfer_mt_data_ismf_request_t *OpenAPI_transfer_mt_data_ismf_request_copy(OpenAPI_transfer_mt_data_ismf_request_t *dst, OpenAPI_transfer_mt_data_ismf_request_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_transfer_mt_data_ismf_request_H_ */

