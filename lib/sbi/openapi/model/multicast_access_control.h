/*
 * multicast_access_control.h
 *
 * 
 */

#ifndef _OpenAPI_multicast_access_control_H_
#define _OpenAPI_multicast_access_control_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "multicast_access_control_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_multicast_access_control_s OpenAPI_multicast_access_control_t;
typedef struct OpenAPI_multicast_access_control_s {
} OpenAPI_multicast_access_control_t;

OpenAPI_multicast_access_control_t *OpenAPI_multicast_access_control_create(
);
void OpenAPI_multicast_access_control_free(OpenAPI_multicast_access_control_t *multicast_access_control);
OpenAPI_multicast_access_control_t *OpenAPI_multicast_access_control_parseFromJSON(cJSON *multicast_access_controlJSON);
cJSON *OpenAPI_multicast_access_control_convertToJSON(OpenAPI_multicast_access_control_t *multicast_access_control);
OpenAPI_multicast_access_control_t *OpenAPI_multicast_access_control_copy(OpenAPI_multicast_access_control_t *dst, OpenAPI_multicast_access_control_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_multicast_access_control_H_ */

