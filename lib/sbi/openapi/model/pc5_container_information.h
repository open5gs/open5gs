/*
 * pc5_container_information.h
 *
 * 
 */

#ifndef _OpenAPI_pc5_container_information_H_
#define _OpenAPI_pc5_container_information_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_pc5_container_information_s OpenAPI_pc5_container_information_t;
#include "coverage_info.h"
#include "radio_parameter_set_info.h"
#include "transmitter_info.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_pc5_container_information_s {
    OpenAPI_list_t *coverage_info_list;
    OpenAPI_list_t *radio_parameter_set_info_list;
    OpenAPI_list_t *transmitter_info_list;
    char *time_of_first_transmission;
    char *time_of_first_reception;
};

OpenAPI_pc5_container_information_t *OpenAPI_pc5_container_information_create(
    OpenAPI_list_t *coverage_info_list,
    OpenAPI_list_t *radio_parameter_set_info_list,
    OpenAPI_list_t *transmitter_info_list,
    char *time_of_first_transmission,
    char *time_of_first_reception
);
void OpenAPI_pc5_container_information_free(OpenAPI_pc5_container_information_t *pc5_container_information);
OpenAPI_pc5_container_information_t *OpenAPI_pc5_container_information_parseFromJSON(cJSON *pc5_container_informationJSON);
cJSON *OpenAPI_pc5_container_information_convertToJSON(OpenAPI_pc5_container_information_t *pc5_container_information);
OpenAPI_pc5_container_information_t *OpenAPI_pc5_container_information_copy(OpenAPI_pc5_container_information_t *dst, OpenAPI_pc5_container_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pc5_container_information_H_ */

