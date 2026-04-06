/*
 * scheduled_area.h
 *
 * A scheduled area in the trajectory
 */

#ifndef _OpenAPI_scheduled_area_H_
#define _OpenAPI_scheduled_area_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_scheduled_area_s OpenAPI_scheduled_area_t;
#include "ecgi.h"
#include "global_ran_node_id.h"
#include "ncgi.h"
#include "tai.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_scheduled_area_s {
    OpenAPI_list_t *tai_list;
    char *enter_time;
    char *leave_time;
    bool is_altitude_low;
    double altitude_low;
    bool is_altitude_high;
    double altitude_high;
    OpenAPI_list_t *ran_node_id_list;
    OpenAPI_list_t *ncgi_list;
    OpenAPI_list_t *ecgi_list;
};

OpenAPI_scheduled_area_t *OpenAPI_scheduled_area_create(
    OpenAPI_list_t *tai_list,
    char *enter_time,
    char *leave_time,
    bool is_altitude_low,
    double altitude_low,
    bool is_altitude_high,
    double altitude_high,
    OpenAPI_list_t *ran_node_id_list,
    OpenAPI_list_t *ncgi_list,
    OpenAPI_list_t *ecgi_list
);
void OpenAPI_scheduled_area_free(OpenAPI_scheduled_area_t *scheduled_area);
OpenAPI_scheduled_area_t *OpenAPI_scheduled_area_parseFromJSON(cJSON *scheduled_areaJSON);
cJSON *OpenAPI_scheduled_area_convertToJSON(OpenAPI_scheduled_area_t *scheduled_area);
OpenAPI_scheduled_area_t *OpenAPI_scheduled_area_copy(OpenAPI_scheduled_area_t *dst, OpenAPI_scheduled_area_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_scheduled_area_H_ */

