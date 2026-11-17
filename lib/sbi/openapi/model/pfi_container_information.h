/*
 * pfi_container_information.h
 *
 * 
 */

#ifndef _OpenAPI_pfi_container_information_H_
#define _OpenAPI_pfi_container_information_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_pfi_container_information_s OpenAPI_pfi_container_information_t;
#include "charging_user_location.h"
#include "presence_info.h"
#include "qos_characteristics.h"
#include "qos_data.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_pfi_container_information_s {
    char *p_fi;
    char *report_time;
    char *timeof_first_usage;
    char *timeof_last_usage;
    bool is_qo_s_information_null;
    struct OpenAPI_qos_data_s *qo_s_information;
    struct OpenAPI_qos_characteristics_s *qo_s_characteristics;
    struct OpenAPI_charging_user_location_s *user_location_information;
    char *uetime_zone;
    OpenAPI_list_t* presence_reporting_area_information;
};

OpenAPI_pfi_container_information_t *OpenAPI_pfi_container_information_create(
    char *p_fi,
    char *report_time,
    char *timeof_first_usage,
    char *timeof_last_usage,
    bool is_qo_s_information_null,
    OpenAPI_qos_data_t *qo_s_information,
    OpenAPI_qos_characteristics_t *qo_s_characteristics,
    OpenAPI_charging_user_location_t *user_location_information,
    char *uetime_zone,
    OpenAPI_list_t* presence_reporting_area_information
);
void OpenAPI_pfi_container_information_free(OpenAPI_pfi_container_information_t *pfi_container_information);
OpenAPI_pfi_container_information_t *OpenAPI_pfi_container_information_parseFromJSON(cJSON *pfi_container_informationJSON);
cJSON *OpenAPI_pfi_container_information_convertToJSON(OpenAPI_pfi_container_information_t *pfi_container_information);
OpenAPI_pfi_container_information_t *OpenAPI_pfi_container_information_copy(OpenAPI_pfi_container_information_t *dst, OpenAPI_pfi_container_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pfi_container_information_H_ */

