/*
 * steering_mode.h
 *
 * Contains the steering mode value and parameters determined by the PCF.
 */

#ifndef _OpenAPI_steering_mode_H_
#define _OpenAPI_steering_mode_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "access_type.h"
#include "access_type_rm.h"
#include "steer_mode_indicator.h"
#include "steer_mode_value.h"
#include "threshold_value.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_steering_mode_s OpenAPI_steering_mode_t;
typedef struct OpenAPI_steering_mode_s {
    OpenAPI_steer_mode_value_e steer_mode_value;
    OpenAPI_access_type_e active;
    struct OpenAPI_access_type_rm_s *standby;
    bool is__3g_load;
    int _3g_load;
    OpenAPI_access_type_e prio_acc;
    bool is_thres_value_null;
    struct OpenAPI_threshold_value_s *thres_value;
    OpenAPI_steer_mode_indicator_e steer_mode_ind;
} OpenAPI_steering_mode_t;

OpenAPI_steering_mode_t *OpenAPI_steering_mode_create(
    OpenAPI_steer_mode_value_e steer_mode_value,
    OpenAPI_access_type_e active,
    OpenAPI_access_type_rm_t *standby,
    bool is__3g_load,
    int _3g_load,
    OpenAPI_access_type_e prio_acc,
    bool is_thres_value_null,
    OpenAPI_threshold_value_t *thres_value,
    OpenAPI_steer_mode_indicator_e steer_mode_ind
);
void OpenAPI_steering_mode_free(OpenAPI_steering_mode_t *steering_mode);
OpenAPI_steering_mode_t *OpenAPI_steering_mode_parseFromJSON(cJSON *steering_modeJSON);
cJSON *OpenAPI_steering_mode_convertToJSON(OpenAPI_steering_mode_t *steering_mode);
OpenAPI_steering_mode_t *OpenAPI_steering_mode_copy(OpenAPI_steering_mode_t *dst, OpenAPI_steering_mode_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_steering_mode_H_ */

