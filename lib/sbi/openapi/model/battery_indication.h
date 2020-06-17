/*
 * battery_indication.h
 *
 *
 */

#ifndef _OpenAPI_battery_indication_H_
#define _OpenAPI_battery_indication_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_battery_indication_s OpenAPI_battery_indication_t;
typedef struct OpenAPI_battery_indication_s {
    int battery_ind;
    int replaceable_ind;
    int rechargeable_ind;
} OpenAPI_battery_indication_t;

OpenAPI_battery_indication_t *OpenAPI_battery_indication_create(
    int battery_ind,
    int replaceable_ind,
    int rechargeable_ind
    );
void OpenAPI_battery_indication_free(OpenAPI_battery_indication_t *battery_indication);
OpenAPI_battery_indication_t *OpenAPI_battery_indication_parseFromJSON(cJSON *battery_indicationJSON);
cJSON *OpenAPI_battery_indication_convertToJSON(OpenAPI_battery_indication_t *battery_indication);
OpenAPI_battery_indication_t *OpenAPI_battery_indication_copy(OpenAPI_battery_indication_t *dst, OpenAPI_battery_indication_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_battery_indication_H_ */

