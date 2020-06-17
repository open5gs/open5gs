/*
 * battery_indication_rm.h
 *
 *
 */

#ifndef _OpenAPI_battery_indication_rm_H_
#define _OpenAPI_battery_indication_rm_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "battery_indication.h"
#include "null_value.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_battery_indication_rm_s OpenAPI_battery_indication_rm_t;
typedef struct OpenAPI_battery_indication_rm_s {
    int battery_ind;
    int replaceable_ind;
    int rechargeable_ind;
} OpenAPI_battery_indication_rm_t;

OpenAPI_battery_indication_rm_t *OpenAPI_battery_indication_rm_create(
    int battery_ind,
    int replaceable_ind,
    int rechargeable_ind
    );
void OpenAPI_battery_indication_rm_free(OpenAPI_battery_indication_rm_t *battery_indication_rm);
OpenAPI_battery_indication_rm_t *OpenAPI_battery_indication_rm_parseFromJSON(cJSON *battery_indication_rmJSON);
cJSON *OpenAPI_battery_indication_rm_convertToJSON(OpenAPI_battery_indication_rm_t *battery_indication_rm);
OpenAPI_battery_indication_rm_t *OpenAPI_battery_indication_rm_copy(OpenAPI_battery_indication_rm_t *dst, OpenAPI_battery_indication_rm_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_battery_indication_rm_H_ */

