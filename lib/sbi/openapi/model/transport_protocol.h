/*
 * transport_protocol.h
 *
 * 
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

typedef enum { OpenAPI_transport_protocol_NULL = 0, OpenAPI_transport_protocol_TCP } OpenAPI_transport_protocol_e;

char* OpenAPI_transport_protocol_ToString(OpenAPI_transport_protocol_e transport_protocol);

OpenAPI_transport_protocol_e OpenAPI_transport_protocol_FromString(char* transport_protocol);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_transport_protocol_H_ */

