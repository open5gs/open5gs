/*
 * steering_mode.h
 *
 *
 */

#ifndef _OpenAPI_steering_mode_H_
#define _OpenAPI_steering_mode_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "access_type.h"
#include "steer_mode_value.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_steering_mode_s OpenAPI_steering_mode_t;
typedef struct OpenAPI_steering_mode_s {
    struct OpenAPI_steer_mode_value_s *steer_mode_value;
    OpenAPI_access_type_e active;
    OpenAPI_access_type_e standby;
    int _3g_load;
    OpenAPI_access_type_e prio_acc;
} OpenAPI_steering_mode_t;

OpenAPI_steering_mode_t *OpenAPI_steering_mode_create(
    OpenAPI_steer_mode_value_t *steer_mode_value,
    OpenAPI_access_type_e active,
    OpenAPI_access_type_e standby,
    int _3g_load,
    OpenAPI_access_type_e prio_acc
    );
void OpenAPI_steering_mode_free(OpenAPI_steering_mode_t *steering_mode);
OpenAPI_steering_mode_t *OpenAPI_steering_mode_parseFromJSON(cJSON *steering_modeJSON);
cJSON *OpenAPI_steering_mode_convertToJSON(OpenAPI_steering_mode_t *steering_mode);
OpenAPI_steering_mode_t *OpenAPI_steering_mode_copy(OpenAPI_steering_mode_t *dst, OpenAPI_steering_mode_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_steering_mode_H_ */

