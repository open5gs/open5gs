/*
 * ranging_sl_charging_information.h
 *
 * 
 */

#ifndef _OpenAPI_ranging_sl_charging_information_H_
#define _OpenAPI_ranging_sl_charging_information_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_ranging_sl_charging_information_s OpenAPI_ranging_sl_charging_information_t;
#include "location_estimate.h"
#include "location_type.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_ranging_sl_charging_information_s {
    char *target_ueid;
    char *s_l_reference_ueid;
    char *s_l_positioning_server_ueid;
    char *located_ueid;
    OpenAPI_location_type_e location_type;
    struct OpenAPI_location_estimate_s *location_estimate;
};

OpenAPI_ranging_sl_charging_information_t *OpenAPI_ranging_sl_charging_information_create(
    char *target_ueid,
    char *s_l_reference_ueid,
    char *s_l_positioning_server_ueid,
    char *located_ueid,
    OpenAPI_location_type_e location_type,
    OpenAPI_location_estimate_t *location_estimate
);
void OpenAPI_ranging_sl_charging_information_free(OpenAPI_ranging_sl_charging_information_t *ranging_sl_charging_information);
OpenAPI_ranging_sl_charging_information_t *OpenAPI_ranging_sl_charging_information_parseFromJSON(cJSON *ranging_sl_charging_informationJSON);
cJSON *OpenAPI_ranging_sl_charging_information_convertToJSON(OpenAPI_ranging_sl_charging_information_t *ranging_sl_charging_information);
OpenAPI_ranging_sl_charging_information_t *OpenAPI_ranging_sl_charging_information_copy(OpenAPI_ranging_sl_charging_information_t *dst, OpenAPI_ranging_sl_charging_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ranging_sl_charging_information_H_ */

