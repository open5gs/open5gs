/*
 * domain_name_protocol_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_domain_name_protocol_any_of_H_
#define _OpenAPI_domain_name_protocol_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_domain_name_protocol_any_of_NULL = 0, OpenAPI_domain_name_protocol_any_of_DNS_QNAME, OpenAPI_domain_name_protocol_any_of_TLS_SNI, OpenAPI_domain_name_protocol_any_of_TLS_SAN, OpenAPI_domain_name_protocol_any_of_TSL_SCN } OpenAPI_domain_name_protocol_any_of_e;

char* OpenAPI_domain_name_protocol_any_of_ToString(OpenAPI_domain_name_protocol_any_of_e domain_name_protocol_any_of);

OpenAPI_domain_name_protocol_any_of_e OpenAPI_domain_name_protocol_any_of_FromString(char* domain_name_protocol_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_domain_name_protocol_any_of_H_ */

