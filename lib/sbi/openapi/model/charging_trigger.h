/*
 * charging_trigger.h
 *
 * 
 */

#ifndef _OpenAPI_charging_trigger_H_
#define _OpenAPI_charging_trigger_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_charging_trigger_s OpenAPI_charging_trigger_t;
#include "trigger_category.h"
#include "trigger_type.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_charging_trigger_s {
    OpenAPI_trigger_type_e trigger_type;
    OpenAPI_trigger_category_e trigger_category;
    bool is_time_limit;
    int time_limit;
    bool is_volume_limit;
    int volume_limit;
    bool is_volume_limit64;
    long volume_limit64;
    bool is_event_limit;
    int event_limit;
    bool is_max_number_ofccc;
    int max_number_ofccc;
    char *tariff_time_change;
};

OpenAPI_charging_trigger_t *OpenAPI_charging_trigger_create(
    OpenAPI_trigger_type_e trigger_type,
    OpenAPI_trigger_category_e trigger_category,
    bool is_time_limit,
    int time_limit,
    bool is_volume_limit,
    int volume_limit,
    bool is_volume_limit64,
    long volume_limit64,
    bool is_event_limit,
    int event_limit,
    bool is_max_number_ofccc,
    int max_number_ofccc,
    char *tariff_time_change
);
void OpenAPI_charging_trigger_free(OpenAPI_charging_trigger_t *charging_trigger);
OpenAPI_charging_trigger_t *OpenAPI_charging_trigger_parseFromJSON(cJSON *charging_triggerJSON);
cJSON *OpenAPI_charging_trigger_convertToJSON(OpenAPI_charging_trigger_t *charging_trigger);
OpenAPI_charging_trigger_t *OpenAPI_charging_trigger_copy(OpenAPI_charging_trigger_t *dst, OpenAPI_charging_trigger_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_charging_trigger_H_ */

