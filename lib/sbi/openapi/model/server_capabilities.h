/*
 * server_capabilities.h
 *
 * 
 */

#ifndef _OpenAPI_server_capabilities_H_
#define _OpenAPI_server_capabilities_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_server_capabilities_s OpenAPI_server_capabilities_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_server_capabilities_s {
    OpenAPI_list_t *mandatory_capability;
    OpenAPI_list_t *optional_capability;
    OpenAPI_list_t *server_name;
};

OpenAPI_server_capabilities_t *OpenAPI_server_capabilities_create(
    OpenAPI_list_t *mandatory_capability,
    OpenAPI_list_t *optional_capability,
    OpenAPI_list_t *server_name
);
void OpenAPI_server_capabilities_free(OpenAPI_server_capabilities_t *server_capabilities);
OpenAPI_server_capabilities_t *OpenAPI_server_capabilities_parseFromJSON(cJSON *server_capabilitiesJSON);
cJSON *OpenAPI_server_capabilities_convertToJSON(OpenAPI_server_capabilities_t *server_capabilities);
OpenAPI_server_capabilities_t *OpenAPI_server_capabilities_copy(OpenAPI_server_capabilities_t *dst, OpenAPI_server_capabilities_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_server_capabilities_H_ */

