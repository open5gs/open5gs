/*
 * dns_security_protocol.h
 *
 * DNS security protocol
 */

#ifndef _OpenAPI_dns_security_protocol_H_
#define _OpenAPI_dns_security_protocol_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_dns_security_protocol_NULL = 0, OpenAPI_dns_security_protocol_TLS, OpenAPI_dns_security_protocol_DTLS } OpenAPI_dns_security_protocol_e;

char* OpenAPI_dns_security_protocol_ToString(OpenAPI_dns_security_protocol_e dns_security_protocol);

OpenAPI_dns_security_protocol_e OpenAPI_dns_security_protocol_FromString(char* dns_security_protocol);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dns_security_protocol_H_ */

