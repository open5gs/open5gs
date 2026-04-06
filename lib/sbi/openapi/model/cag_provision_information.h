/*
 * cag_provision_information.h
 *
 * CAG Provision Information
 */

#ifndef _OpenAPI_cag_provision_information_H_
#define _OpenAPI_cag_provision_information_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_cag_provision_information_s OpenAPI_cag_provision_information_t;
#include "cag_provision_operation.h"
#include "valid_time_period.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_cag_provision_information_s {
    OpenAPI_list_t *cag_info;
    OpenAPI_list_t *common_valid_time_period_list;
    OpenAPI_list_t* additional_valid_time_period_list;
    OpenAPI_cag_provision_operation_e provision_operation;
};

OpenAPI_cag_provision_information_t *OpenAPI_cag_provision_information_create(
    OpenAPI_list_t *cag_info,
    OpenAPI_list_t *common_valid_time_period_list,
    OpenAPI_list_t* additional_valid_time_period_list,
    OpenAPI_cag_provision_operation_e provision_operation
);
void OpenAPI_cag_provision_information_free(OpenAPI_cag_provision_information_t *cag_provision_information);
OpenAPI_cag_provision_information_t *OpenAPI_cag_provision_information_parseFromJSON(cJSON *cag_provision_informationJSON);
cJSON *OpenAPI_cag_provision_information_convertToJSON(OpenAPI_cag_provision_information_t *cag_provision_information);
OpenAPI_cag_provision_information_t *OpenAPI_cag_provision_information_copy(OpenAPI_cag_provision_information_t *dst, OpenAPI_cag_provision_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_cag_provision_information_H_ */

