/*
 * location_reporting_charging_information.h
 *
 * 
 */

#ifndef _OpenAPI_location_reporting_charging_information_H_
#define _OpenAPI_location_reporting_charging_information_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_location_reporting_charging_information_s OpenAPI_location_reporting_charging_information_t;
#include "charging_user_location.h"
#include "presence_info.h"
#include "ps_cell_information.h"
#include "rat_type.h"
#include "user_information.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_location_reporting_charging_information_s {
    int location_reporting_message_type;
    struct OpenAPI_user_information_s *user_information;
    struct OpenAPI_charging_user_location_s *user_locationinfo;
    struct OpenAPI_ps_cell_information_s *p_s_cell_information;
    char *uetime_zone;
    OpenAPI_rat_type_e r_at_type;
    OpenAPI_list_t* presence_reporting_area_information;
    bool is_satellite_access_indicator;
    int satellite_access_indicator;
};

OpenAPI_location_reporting_charging_information_t *OpenAPI_location_reporting_charging_information_create(
    int location_reporting_message_type,
    OpenAPI_user_information_t *user_information,
    OpenAPI_charging_user_location_t *user_locationinfo,
    OpenAPI_ps_cell_information_t *p_s_cell_information,
    char *uetime_zone,
    OpenAPI_rat_type_e r_at_type,
    OpenAPI_list_t* presence_reporting_area_information,
    bool is_satellite_access_indicator,
    int satellite_access_indicator
);
void OpenAPI_location_reporting_charging_information_free(OpenAPI_location_reporting_charging_information_t *location_reporting_charging_information);
OpenAPI_location_reporting_charging_information_t *OpenAPI_location_reporting_charging_information_parseFromJSON(cJSON *location_reporting_charging_informationJSON);
cJSON *OpenAPI_location_reporting_charging_information_convertToJSON(OpenAPI_location_reporting_charging_information_t *location_reporting_charging_information);
OpenAPI_location_reporting_charging_information_t *OpenAPI_location_reporting_charging_information_copy(OpenAPI_location_reporting_charging_information_t *dst, OpenAPI_location_reporting_charging_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_location_reporting_charging_information_H_ */

