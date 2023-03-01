/*
 * transport_protocol_1.h
 *
 * 
 */

#ifndef _OpenAPI_transport_protocol_1_H_
#define _OpenAPI_transport_protocol_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_transport_protocol_1_NULL = 0, OpenAPI_transport_protocol_1_TCP } OpenAPI_transport_protocol_1_e;

char* OpenAPI_transport_protocol_1_ToString(OpenAPI_transport_protocol_1_e transport_protocol_1);

OpenAPI_transport_protocol_1_e OpenAPI_transport_protocol_1_FromString(char* transport_protocol_1);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_transport_protocol_1_H_ */

