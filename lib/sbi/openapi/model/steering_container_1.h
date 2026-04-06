/*
 * steering_container_1.h
 *
 * Contains Steering Info or a secured packet
 */

#ifndef _OpenAPI_steering_container_1_H_
#define _OpenAPI_steering_container_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_steering_container_1_s OpenAPI_steering_container_1_t;
#include "steering_info_1.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_steering_container_1_s {
    OpenAPI_list_t *steering_info_list;
    char *secured_packet;
};

OpenAPI_steering_container_1_t *OpenAPI_steering_container_1_create(
    OpenAPI_list_t *steering_info_list,
    char *secured_packet
);
void OpenAPI_steering_container_1_free(OpenAPI_steering_container_1_t *steering_container_1);
OpenAPI_steering_container_1_t *OpenAPI_steering_container_1_parseFromJSON(cJSON *steering_container_1JSON);
cJSON *OpenAPI_steering_container_1_convertToJSON(OpenAPI_steering_container_1_t *steering_container_1);
OpenAPI_steering_container_1_t *OpenAPI_steering_container_1_copy(OpenAPI_steering_container_1_t *dst, OpenAPI_steering_container_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_steering_container_1_H_ */

