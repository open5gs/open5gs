/*
 * trigger.h
 *
 * 
 */

#ifndef _OpenAPI_trigger_H_
#define _OpenAPI_trigger_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "trigger_category.h"
#include "trigger_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_trigger_s OpenAPI_trigger_t;
typedef struct OpenAPI_trigger_s {
    struct OpenAPI_trigger_type_s *trigger_type;
    struct OpenAPI_trigger_category_s *trigger_category;
    bool is_time_limit;
    int time_limit;
    bool is_volume_limit;
    int volume_limit;
    bool is_volume_limit64;
    int volume_limit64;
    bool is_event_limit;
    int event_limit;
    bool is_max_number_ofccc;
    int max_number_ofccc;
    char *tariff_time_change;
} OpenAPI_trigger_t;

OpenAPI_trigger_t *OpenAPI_trigger_create(
    OpenAPI_trigger_type_t *trigger_type,
    OpenAPI_trigger_category_t *trigger_category,
    bool is_time_limit,
    int time_limit,
    bool is_volume_limit,
    int volume_limit,
    bool is_volume_limit64,
    int volume_limit64,
    bool is_event_limit,
    int event_limit,
    bool is_max_number_ofccc,
    int max_number_ofccc,
    char *tariff_time_change
);
void OpenAPI_trigger_free(OpenAPI_trigger_t *trigger);
OpenAPI_trigger_t *OpenAPI_trigger_parseFromJSON(cJSON *triggerJSON);
cJSON *OpenAPI_trigger_convertToJSON(OpenAPI_trigger_t *trigger);
OpenAPI_trigger_t *OpenAPI_trigger_copy(OpenAPI_trigger_t *dst, OpenAPI_trigger_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_trigger_H_ */

