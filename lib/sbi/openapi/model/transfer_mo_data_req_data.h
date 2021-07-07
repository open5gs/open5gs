/*
 * transfer_mo_data_req_data.h
 *
 * 
 */

#ifndef _OpenAPI_transfer_mo_data_req_data_H_
#define _OpenAPI_transfer_mo_data_req_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "mo_exp_data_counter.h"
#include "ref_to_binary_data.h"
#include "user_location.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_transfer_mo_data_req_data_s OpenAPI_transfer_mo_data_req_data_t;
typedef struct OpenAPI_transfer_mo_data_req_data_s {
    struct OpenAPI_ref_to_binary_data_s *mo_data;
    struct OpenAPI_mo_exp_data_counter_s *mo_exp_data_counter;
    struct OpenAPI_user_location_s *ue_location;
} OpenAPI_transfer_mo_data_req_data_t;

OpenAPI_transfer_mo_data_req_data_t *OpenAPI_transfer_mo_data_req_data_create(
    OpenAPI_ref_to_binary_data_t *mo_data,
    OpenAPI_mo_exp_data_counter_t *mo_exp_data_counter,
    OpenAPI_user_location_t *ue_location
);
void OpenAPI_transfer_mo_data_req_data_free(OpenAPI_transfer_mo_data_req_data_t *transfer_mo_data_req_data);
OpenAPI_transfer_mo_data_req_data_t *OpenAPI_transfer_mo_data_req_data_parseFromJSON(cJSON *transfer_mo_data_req_dataJSON);
cJSON *OpenAPI_transfer_mo_data_req_data_convertToJSON(OpenAPI_transfer_mo_data_req_data_t *transfer_mo_data_req_data);
OpenAPI_transfer_mo_data_req_data_t *OpenAPI_transfer_mo_data_req_data_copy(OpenAPI_transfer_mo_data_req_data_t *dst, OpenAPI_transfer_mo_data_req_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_transfer_mo_data_req_data_H_ */

