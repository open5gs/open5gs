/*
 * bridge_management_container.h
 *
 * Contains the UMIC.
 */

#ifndef _OpenAPI_bridge_management_container_H_
#define _OpenAPI_bridge_management_container_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_bridge_management_container_s OpenAPI_bridge_management_container_t;
typedef struct OpenAPI_bridge_management_container_s {
    char *bridge_man_cont;
} OpenAPI_bridge_management_container_t;

OpenAPI_bridge_management_container_t *OpenAPI_bridge_management_container_create(
    char *bridge_man_cont
);
void OpenAPI_bridge_management_container_free(OpenAPI_bridge_management_container_t *bridge_management_container);
OpenAPI_bridge_management_container_t *OpenAPI_bridge_management_container_parseFromJSON(cJSON *bridge_management_containerJSON);
cJSON *OpenAPI_bridge_management_container_convertToJSON(OpenAPI_bridge_management_container_t *bridge_management_container);
OpenAPI_bridge_management_container_t *OpenAPI_bridge_management_container_copy(OpenAPI_bridge_management_container_t *dst, OpenAPI_bridge_management_container_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_bridge_management_container_H_ */

