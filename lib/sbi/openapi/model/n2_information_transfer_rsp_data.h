/*
 * n2_information_transfer_rsp_data.h
 *
 * Data within a successful response to the N2 Information Transfer request to transfer N2 Information to the AN
 */

#ifndef _OpenAPI_n2_information_transfer_rsp_data_H_
#define _OpenAPI_n2_information_transfer_rsp_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_n2_information_transfer_rsp_data_s OpenAPI_n2_information_transfer_rsp_data_t;
#include "n2_information_transfer_result.h"
#include "nrppa_rsp_per_ngran.h"
#include "pws_response_data.h"
#include "tss_rsp_per_ngran.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_n2_information_transfer_rsp_data_s {
    OpenAPI_n2_information_transfer_result_e result;
    OpenAPI_list_t *nrppa_rsp_per_ngran_list;
    struct OpenAPI_pws_response_data_s *pws_rsp_data;
    char *supported_features;
    OpenAPI_list_t *tss_rsp_per_ngran_list;
};

OpenAPI_n2_information_transfer_rsp_data_t *OpenAPI_n2_information_transfer_rsp_data_create(
    OpenAPI_n2_information_transfer_result_e result,
    OpenAPI_list_t *nrppa_rsp_per_ngran_list,
    OpenAPI_pws_response_data_t *pws_rsp_data,
    char *supported_features,
    OpenAPI_list_t *tss_rsp_per_ngran_list
);
void OpenAPI_n2_information_transfer_rsp_data_free(OpenAPI_n2_information_transfer_rsp_data_t *n2_information_transfer_rsp_data);
OpenAPI_n2_information_transfer_rsp_data_t *OpenAPI_n2_information_transfer_rsp_data_parseFromJSON(cJSON *n2_information_transfer_rsp_dataJSON);
cJSON *OpenAPI_n2_information_transfer_rsp_data_convertToJSON(OpenAPI_n2_information_transfer_rsp_data_t *n2_information_transfer_rsp_data);
OpenAPI_n2_information_transfer_rsp_data_t *OpenAPI_n2_information_transfer_rsp_data_copy(OpenAPI_n2_information_transfer_rsp_data_t *dst, OpenAPI_n2_information_transfer_rsp_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_n2_information_transfer_rsp_data_H_ */

