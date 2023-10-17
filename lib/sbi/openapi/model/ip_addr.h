/*
 * ip_addr.h
 *
 * Contains an IP adresse.
 */

#ifndef _OpenAPI_ip_addr_H_
#define _OpenAPI_ip_addr_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ip_addr_s OpenAPI_ip_addr_t;
typedef struct OpenAPI_ip_addr_s {
    char *ipv4_addr;
    char *ipv6_addr;
    char *ipv6_prefix;
} OpenAPI_ip_addr_t;

OpenAPI_ip_addr_t *OpenAPI_ip_addr_create(
    char *ipv4_addr,
    char *ipv6_addr,
    char *ipv6_prefix
);
void OpenAPI_ip_addr_free(OpenAPI_ip_addr_t *ip_addr);
OpenAPI_ip_addr_t *OpenAPI_ip_addr_parseFromJSON(cJSON *ip_addrJSON);
cJSON *OpenAPI_ip_addr_convertToJSON(OpenAPI_ip_addr_t *ip_addr);
OpenAPI_ip_addr_t *OpenAPI_ip_addr_copy(OpenAPI_ip_addr_t *dst, OpenAPI_ip_addr_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ip_addr_H_ */

