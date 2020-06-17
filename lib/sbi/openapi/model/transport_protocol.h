/*
 * transport_protocol.h
 *
 * Types of transport protocol used in a given IP endpoint of an NF Service Instance
 */

#ifndef _OpenAPI_transport_protocol_H_
#define _OpenAPI_transport_protocol_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_transport_protocol_s OpenAPI_transport_protocol_t;
typedef struct OpenAPI_transport_protocol_s {
} OpenAPI_transport_protocol_t;

OpenAPI_transport_protocol_t *OpenAPI_transport_protocol_create(
    );
void OpenAPI_transport_protocol_free(OpenAPI_transport_protocol_t *transport_protocol);
OpenAPI_transport_protocol_t *OpenAPI_transport_protocol_parseFromJSON(cJSON *transport_protocolJSON);
cJSON *OpenAPI_transport_protocol_convertToJSON(OpenAPI_transport_protocol_t *transport_protocol);
OpenAPI_transport_protocol_t *OpenAPI_transport_protocol_copy(OpenAPI_transport_protocol_t *dst, OpenAPI_transport_protocol_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_transport_protocol_H_ */

