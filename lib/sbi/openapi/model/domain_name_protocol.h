/*
 * domain_name_protocol.h
 *
 * 
 */

#ifndef _OpenAPI_domain_name_protocol_H_
#define _OpenAPI_domain_name_protocol_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_domain_name_protocol_NULL = 0, OpenAPI_domain_name_protocol_DNS_QNAME, OpenAPI_domain_name_protocol_TLS_SNI, OpenAPI_domain_name_protocol_TLS_SAN, OpenAPI_domain_name_protocol_TSL_SCN } OpenAPI_domain_name_protocol_e;

char* OpenAPI_domain_name_protocol_ToString(OpenAPI_domain_name_protocol_e domain_name_protocol);

OpenAPI_domain_name_protocol_e OpenAPI_domain_name_protocol_FromString(char* domain_name_protocol);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_domain_name_protocol_H_ */

