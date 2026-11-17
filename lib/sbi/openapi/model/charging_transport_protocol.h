/*
 * charging_transport_protocol.h
 *
 * 
 */

#ifndef _OpenAPI_charging_transport_protocol_H_
#define _OpenAPI_charging_transport_protocol_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_charging_transport_protocol_NULL = 0, OpenAPI_charging_transport_protocol_UDP, OpenAPI_charging_transport_protocol_TCP } OpenAPI_charging_transport_protocol_e;

char* OpenAPI_charging_transport_protocol_ToString(OpenAPI_charging_transport_protocol_e charging_transport_protocol);

OpenAPI_charging_transport_protocol_e OpenAPI_charging_transport_protocol_FromString(char* charging_transport_protocol);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_charging_transport_protocol_H_ */

