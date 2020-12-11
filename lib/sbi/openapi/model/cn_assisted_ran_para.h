/*
 * cn_assisted_ran_para.h
 *
 *
 */

#ifndef _OpenAPI_cn_assisted_ran_para_H_
#define _OpenAPI_cn_assisted_ran_para_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "battery_indication.h"
#include "scheduled_communication_time.h"
#include "scheduled_communication_type.h"
#include "stationary_indication.h"
#include "traffic_profile.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_cn_assisted_ran_para_s OpenAPI_cn_assisted_ran_para_t;
typedef struct OpenAPI_cn_assisted_ran_para_s {
    OpenAPI_stationary_indication_e stationary_indication;
    int communication_duration_time;
    int periodic_time;
    struct OpenAPI_scheduled_communication_time_s *scheduled_communication_time;
    OpenAPI_scheduled_communication_type_e scheduled_communication_type;
    OpenAPI_traffic_profile_e traffic_profile;
    struct OpenAPI_battery_indication_s *battery_indication;
} OpenAPI_cn_assisted_ran_para_t;

OpenAPI_cn_assisted_ran_para_t *OpenAPI_cn_assisted_ran_para_create(
    OpenAPI_stationary_indication_e stationary_indication,
    int communication_duration_time,
    int periodic_time,
    OpenAPI_scheduled_communication_time_t *scheduled_communication_time,
    OpenAPI_scheduled_communication_type_e scheduled_communication_type,
    OpenAPI_traffic_profile_e traffic_profile,
    OpenAPI_battery_indication_t *battery_indication
    );
void OpenAPI_cn_assisted_ran_para_free(OpenAPI_cn_assisted_ran_para_t *cn_assisted_ran_para);
OpenAPI_cn_assisted_ran_para_t *OpenAPI_cn_assisted_ran_para_parseFromJSON(cJSON *cn_assisted_ran_paraJSON);
cJSON *OpenAPI_cn_assisted_ran_para_convertToJSON(OpenAPI_cn_assisted_ran_para_t *cn_assisted_ran_para);
OpenAPI_cn_assisted_ran_para_t *OpenAPI_cn_assisted_ran_para_copy(OpenAPI_cn_assisted_ran_para_t *dst, OpenAPI_cn_assisted_ran_para_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_cn_assisted_ran_para_H_ */

