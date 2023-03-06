/*
 * target_area.h
 *
 * TA list or TAI range list or any TA
 */

#ifndef _OpenAPI_target_area_H_
#define _OpenAPI_target_area_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "tai.h"
#include "tai_range.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_target_area_s OpenAPI_target_area_t;
typedef struct OpenAPI_target_area_s {
    OpenAPI_list_t *ta_list;
    OpenAPI_list_t *tai_range_list;
    bool is_any_ta;
    int any_ta;
} OpenAPI_target_area_t;

OpenAPI_target_area_t *OpenAPI_target_area_create(
    OpenAPI_list_t *ta_list,
    OpenAPI_list_t *tai_range_list,
    bool is_any_ta,
    int any_ta
);
void OpenAPI_target_area_free(OpenAPI_target_area_t *target_area);
OpenAPI_target_area_t *OpenAPI_target_area_parseFromJSON(cJSON *target_areaJSON);
cJSON *OpenAPI_target_area_convertToJSON(OpenAPI_target_area_t *target_area);
OpenAPI_target_area_t *OpenAPI_target_area_copy(OpenAPI_target_area_t *dst, OpenAPI_target_area_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_target_area_H_ */

