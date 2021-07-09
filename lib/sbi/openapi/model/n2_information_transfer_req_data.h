/*
 * n2_information_transfer_req_data.h
 *
 * 
 */

#ifndef _OpenAPI_n2_information_transfer_req_data_H_
#define _OpenAPI_n2_information_transfer_req_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "global_ran_node_id.h"
#include "n2_info_container.h"
#include "rat_selector.h"
#include "tai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_n2_information_transfer_req_data_s OpenAPI_n2_information_transfer_req_data_t;
typedef struct OpenAPI_n2_information_transfer_req_data_s {
    OpenAPI_list_t *tai_list;
    OpenAPI_rat_selector_e rat_selector;
    OpenAPI_list_t *global_ran_node_list;
    struct OpenAPI_n2_info_container_s *n2_information;
    char *supported_features;
} OpenAPI_n2_information_transfer_req_data_t;

OpenAPI_n2_information_transfer_req_data_t *OpenAPI_n2_information_transfer_req_data_create(
    OpenAPI_list_t *tai_list,
    OpenAPI_rat_selector_e rat_selector,
    OpenAPI_list_t *global_ran_node_list,
    OpenAPI_n2_info_container_t *n2_information,
    char *supported_features
);
void OpenAPI_n2_information_transfer_req_data_free(OpenAPI_n2_information_transfer_req_data_t *n2_information_transfer_req_data);
OpenAPI_n2_information_transfer_req_data_t *OpenAPI_n2_information_transfer_req_data_parseFromJSON(cJSON *n2_information_transfer_req_dataJSON);
cJSON *OpenAPI_n2_information_transfer_req_data_convertToJSON(OpenAPI_n2_information_transfer_req_data_t *n2_information_transfer_req_data);
OpenAPI_n2_information_transfer_req_data_t *OpenAPI_n2_information_transfer_req_data_copy(OpenAPI_n2_information_transfer_req_data_t *dst, OpenAPI_n2_information_transfer_req_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_n2_information_transfer_req_data_H_ */

