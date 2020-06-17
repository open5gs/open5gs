/*
 * transfer_mt_data_req_data.h
 *
 *
 */

#ifndef _OpenAPI_transfer_mt_data_req_data_H_
#define _OpenAPI_transfer_mt_data_req_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ref_to_binary_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_transfer_mt_data_req_data_s OpenAPI_transfer_mt_data_req_data_t;
typedef struct OpenAPI_transfer_mt_data_req_data_s {
    struct OpenAPI_ref_to_binary_data_s *mt_data;
} OpenAPI_transfer_mt_data_req_data_t;

OpenAPI_transfer_mt_data_req_data_t *OpenAPI_transfer_mt_data_req_data_create(
    OpenAPI_ref_to_binary_data_t *mt_data
    );
void OpenAPI_transfer_mt_data_req_data_free(OpenAPI_transfer_mt_data_req_data_t *transfer_mt_data_req_data);
OpenAPI_transfer_mt_data_req_data_t *OpenAPI_transfer_mt_data_req_data_parseFromJSON(cJSON *transfer_mt_data_req_dataJSON);
cJSON *OpenAPI_transfer_mt_data_req_data_convertToJSON(OpenAPI_transfer_mt_data_req_data_t *transfer_mt_data_req_data);
OpenAPI_transfer_mt_data_req_data_t *OpenAPI_transfer_mt_data_req_data_copy(OpenAPI_transfer_mt_data_req_data_t *dst, OpenAPI_transfer_mt_data_req_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_transfer_mt_data_req_data_H_ */

