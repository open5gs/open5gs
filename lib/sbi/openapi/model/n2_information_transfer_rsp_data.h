/*
 * n2_information_transfer_rsp_data.h
 *
 * 
 */

#ifndef _OpenAPI_n2_information_transfer_rsp_data_H_
#define _OpenAPI_n2_information_transfer_rsp_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "n2_information_transfer_result.h"
#include "pws_response_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_n2_information_transfer_rsp_data_s OpenAPI_n2_information_transfer_rsp_data_t;
typedef struct OpenAPI_n2_information_transfer_rsp_data_s {
    OpenAPI_n2_information_transfer_result_e result;
    struct OpenAPI_pws_response_data_s *pws_rsp_data;
    char *supported_features;
} OpenAPI_n2_information_transfer_rsp_data_t;

OpenAPI_n2_information_transfer_rsp_data_t *OpenAPI_n2_information_transfer_rsp_data_create(
    OpenAPI_n2_information_transfer_result_e result,
    OpenAPI_pws_response_data_t *pws_rsp_data,
    char *supported_features
);
void OpenAPI_n2_information_transfer_rsp_data_free(OpenAPI_n2_information_transfer_rsp_data_t *n2_information_transfer_rsp_data);
OpenAPI_n2_information_transfer_rsp_data_t *OpenAPI_n2_information_transfer_rsp_data_parseFromJSON(cJSON *n2_information_transfer_rsp_dataJSON);
cJSON *OpenAPI_n2_information_transfer_rsp_data_convertToJSON(OpenAPI_n2_information_transfer_rsp_data_t *n2_information_transfer_rsp_data);
OpenAPI_n2_information_transfer_rsp_data_t *OpenAPI_n2_information_transfer_rsp_data_copy(OpenAPI_n2_information_transfer_rsp_data_t *dst, OpenAPI_n2_information_transfer_rsp_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_n2_information_transfer_rsp_data_H_ */

