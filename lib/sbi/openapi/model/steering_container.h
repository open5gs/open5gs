/*
 * steering_container.h
 *
 *
 */

#ifndef _OpenAPI_steering_container_H_
#define _OpenAPI_steering_container_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_steering_container_s OpenAPI_steering_container_t;
typedef struct OpenAPI_steering_container_s {
} OpenAPI_steering_container_t;

OpenAPI_steering_container_t *OpenAPI_steering_container_create(
    );
void OpenAPI_steering_container_free(OpenAPI_steering_container_t *steering_container);
OpenAPI_steering_container_t *OpenAPI_steering_container_parseFromJSON(cJSON *steering_containerJSON);
cJSON *OpenAPI_steering_container_convertToJSON(OpenAPI_steering_container_t *steering_container);
OpenAPI_steering_container_t *OpenAPI_steering_container_copy(OpenAPI_steering_container_t *dst, OpenAPI_steering_container_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_steering_container_H_ */

