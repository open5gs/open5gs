/*
 * qfi_container_information.h
 *
 * 
 */

#ifndef _OpenAPI_qfi_container_information_H_
#define _OpenAPI_qfi_container_information_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_qfi_container_information_s OpenAPI_qfi_container_information_t;
#include "charging_user_location.h"
#include "model_3_gppps_data_off_status.h"
#include "presence_info.h"
#include "qos_characteristics.h"
#include "qos_data.h"
#include "rat_type.h"
#include "serving_network_function_id.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_qfi_container_information_s {
    bool is_q_fi;
    int q_fi;
    char *report_time;
    char *timeof_first_usage;
    char *timeof_last_usage;
    bool is_qo_s_information_null;
    struct OpenAPI_qos_data_s *qo_s_information;
    struct OpenAPI_qos_characteristics_s *qo_s_characteristics;
    struct OpenAPI_charging_user_location_s *user_location_information;
    char *uetime_zone;
    OpenAPI_list_t* presence_reporting_area_information;
    OpenAPI_rat_type_e r_at_type;
    OpenAPI_list_t *serving_network_function_id;
    OpenAPI_model_3_gppps_data_off_status_e _3gpp_ps_data_off_status;
    bool is__3gpp_charging_id;
    int _3gpp_charging_id;
    bool is_diagnostics;
    int diagnostics;
    OpenAPI_list_t *enhanced_diagnostics;
};

OpenAPI_qfi_container_information_t *OpenAPI_qfi_container_information_create(
    bool is_q_fi,
    int q_fi,
    char *report_time,
    char *timeof_first_usage,
    char *timeof_last_usage,
    bool is_qo_s_information_null,
    OpenAPI_qos_data_t *qo_s_information,
    OpenAPI_qos_characteristics_t *qo_s_characteristics,
    OpenAPI_charging_user_location_t *user_location_information,
    char *uetime_zone,
    OpenAPI_list_t* presence_reporting_area_information,
    OpenAPI_rat_type_e r_at_type,
    OpenAPI_list_t *serving_network_function_id,
    OpenAPI_model_3_gppps_data_off_status_e _3gpp_ps_data_off_status,
    bool is__3gpp_charging_id,
    int _3gpp_charging_id,
    bool is_diagnostics,
    int diagnostics,
    OpenAPI_list_t *enhanced_diagnostics
);
void OpenAPI_qfi_container_information_free(OpenAPI_qfi_container_information_t *qfi_container_information);
OpenAPI_qfi_container_information_t *OpenAPI_qfi_container_information_parseFromJSON(cJSON *qfi_container_informationJSON);
cJSON *OpenAPI_qfi_container_information_convertToJSON(OpenAPI_qfi_container_information_t *qfi_container_information);
OpenAPI_qfi_container_information_t *OpenAPI_qfi_container_information_copy(OpenAPI_qfi_container_information_t *dst, OpenAPI_qfi_container_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_qfi_container_information_H_ */

