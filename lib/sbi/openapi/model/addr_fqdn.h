/*
 * addr_fqdn.h
 *
 * IP address and/or FQDN.
 */

#ifndef _OpenAPI_addr_fqdn_H_
#define _OpenAPI_addr_fqdn_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ip_addr.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_addr_fqdn_s OpenAPI_addr_fqdn_t;
typedef struct OpenAPI_addr_fqdn_s {
    struct OpenAPI_ip_addr_s *ip_addr;
    char *fqdn;
} OpenAPI_addr_fqdn_t;

OpenAPI_addr_fqdn_t *OpenAPI_addr_fqdn_create(
    OpenAPI_ip_addr_t *ip_addr,
    char *fqdn
);
void OpenAPI_addr_fqdn_free(OpenAPI_addr_fqdn_t *addr_fqdn);
OpenAPI_addr_fqdn_t *OpenAPI_addr_fqdn_parseFromJSON(cJSON *addr_fqdnJSON);
cJSON *OpenAPI_addr_fqdn_convertToJSON(OpenAPI_addr_fqdn_t *addr_fqdn);
OpenAPI_addr_fqdn_t *OpenAPI_addr_fqdn_copy(OpenAPI_addr_fqdn_t *dst, OpenAPI_addr_fqdn_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_addr_fqdn_H_ */

