/*
 * domain_name_protocol.h
 *
 * Possible values are - DNS_QNAME: Identifies the DNS protocol and the question name in DNS query. - TLS_SNI: Identifies the Server Name Indication in TLS ClientHello message. - TLS_SAN: Identifies the Subject Alternative Name in TLS ServerCertificate message. - TLS_SCN: Identifies the Subject Common Name in TLS ServerCertificate message.
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

typedef struct OpenAPI_domain_name_protocol_s OpenAPI_domain_name_protocol_t;
typedef struct OpenAPI_domain_name_protocol_s {
} OpenAPI_domain_name_protocol_t;

OpenAPI_domain_name_protocol_t *OpenAPI_domain_name_protocol_create(
    );
void OpenAPI_domain_name_protocol_free(OpenAPI_domain_name_protocol_t *domain_name_protocol);
OpenAPI_domain_name_protocol_t *OpenAPI_domain_name_protocol_parseFromJSON(cJSON *domain_name_protocolJSON);
cJSON *OpenAPI_domain_name_protocol_convertToJSON(OpenAPI_domain_name_protocol_t *domain_name_protocol);
OpenAPI_domain_name_protocol_t *OpenAPI_domain_name_protocol_copy(OpenAPI_domain_name_protocol_t *dst, OpenAPI_domain_name_protocol_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_domain_name_protocol_H_ */

