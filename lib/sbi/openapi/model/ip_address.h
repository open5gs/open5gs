/*
 * ip_address.h
 *
 *
 */

#ifndef _OpenAPI_ip_address_H_
#define _OpenAPI_ip_address_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ip_address_s OpenAPI_ip_address_t;
typedef struct OpenAPI_ip_address_s {
    char *ipv4_addr;
    char *ipv6_addr;
    char *ipv6_prefix;
} OpenAPI_ip_address_t;

OpenAPI_ip_address_t *OpenAPI_ip_address_create(
    char *ipv4_addr,
    char *ipv6_addr,
    char *ipv6_prefix
    );
void OpenAPI_ip_address_free(OpenAPI_ip_address_t *ip_address);
OpenAPI_ip_address_t *OpenAPI_ip_address_parseFromJSON(cJSON *ip_addressJSON);
cJSON *OpenAPI_ip_address_convertToJSON(OpenAPI_ip_address_t *ip_address);
OpenAPI_ip_address_t *OpenAPI_ip_address_copy(OpenAPI_ip_address_t *dst, OpenAPI_ip_address_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ip_address_H_ */

