/*
 * cag_provision_information_1.h
 *
 * CAG Provision Information
 */

#ifndef _OpenAPI_cag_provision_information_1_H_
#define _OpenAPI_cag_provision_information_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_cag_provision_information_1_s OpenAPI_cag_provision_information_1_t;
#include "cag_provision_operation.h"
#include "valid_time_period_1.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_cag_provision_information_1_s {
    OpenAPI_list_t *cag_info;
    OpenAPI_list_t *common_valid_time_period_list;
    OpenAPI_list_t* additional_valid_time_period_list;
    OpenAPI_cag_provision_operation_e provision_operation;
};

OpenAPI_cag_provision_information_1_t *OpenAPI_cag_provision_information_1_create(
    OpenAPI_list_t *cag_info,
    OpenAPI_list_t *common_valid_time_period_list,
    OpenAPI_list_t* additional_valid_time_period_list,
    OpenAPI_cag_provision_operation_e provision_operation
);
void OpenAPI_cag_provision_information_1_free(OpenAPI_cag_provision_information_1_t *cag_provision_information_1);
OpenAPI_cag_provision_information_1_t *OpenAPI_cag_provision_information_1_parseFromJSON(cJSON *cag_provision_information_1JSON);
cJSON *OpenAPI_cag_provision_information_1_convertToJSON(OpenAPI_cag_provision_information_1_t *cag_provision_information_1);
OpenAPI_cag_provision_information_1_t *OpenAPI_cag_provision_information_1_copy(OpenAPI_cag_provision_information_1_t *dst, OpenAPI_cag_provision_information_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_cag_provision_information_1_H_ */

