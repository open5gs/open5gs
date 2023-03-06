/*
 * connection_capabilities.h
 *
 * Possible values are   - IMS: Indicates the connection capability to support IMS service.   - MMS: Indicates the connection capability to support MMS service.   - SUPL: Indicates the connection capability to support SUPL service.   - INTERNET: Indicates the connection capability to support Internet service. 
 */

#ifndef _OpenAPI_connection_capabilities_H_
#define _OpenAPI_connection_capabilities_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "connection_capabilities_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_connection_capabilities_s OpenAPI_connection_capabilities_t;
typedef struct OpenAPI_connection_capabilities_s {
} OpenAPI_connection_capabilities_t;

OpenAPI_connection_capabilities_t *OpenAPI_connection_capabilities_create(
);
void OpenAPI_connection_capabilities_free(OpenAPI_connection_capabilities_t *connection_capabilities);
OpenAPI_connection_capabilities_t *OpenAPI_connection_capabilities_parseFromJSON(cJSON *connection_capabilitiesJSON);
cJSON *OpenAPI_connection_capabilities_convertToJSON(OpenAPI_connection_capabilities_t *connection_capabilities);
OpenAPI_connection_capabilities_t *OpenAPI_connection_capabilities_copy(OpenAPI_connection_capabilities_t *dst, OpenAPI_connection_capabilities_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_connection_capabilities_H_ */

