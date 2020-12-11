/*
 * steer_mode_value.h
 *
 *
 */

#ifndef _OpenAPI_steer_mode_value_H_
#define _OpenAPI_steer_mode_value_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_steer_mode_value_s OpenAPI_steer_mode_value_t;
typedef struct OpenAPI_steer_mode_value_s {
} OpenAPI_steer_mode_value_t;

OpenAPI_steer_mode_value_t *OpenAPI_steer_mode_value_create(
    );
void OpenAPI_steer_mode_value_free(OpenAPI_steer_mode_value_t *steer_mode_value);
OpenAPI_steer_mode_value_t *OpenAPI_steer_mode_value_parseFromJSON(cJSON *steer_mode_valueJSON);
cJSON *OpenAPI_steer_mode_value_convertToJSON(OpenAPI_steer_mode_value_t *steer_mode_value);
OpenAPI_steer_mode_value_t *OpenAPI_steer_mode_value_copy(OpenAPI_steer_mode_value_t *dst, OpenAPI_steer_mode_value_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_steer_mode_value_H_ */

