/*
 * address_list.h
 *
 * Represents a list of IPv4 and/or IPv6 addresses.
 */

#ifndef _OpenAPI_address_list_H_
#define _OpenAPI_address_list_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_address_list_s OpenAPI_address_list_t;
typedef struct OpenAPI_address_list_s {
    OpenAPI_list_t *ipv4_addrs;
    OpenAPI_list_t *ipv6_addrs;
} OpenAPI_address_list_t;

OpenAPI_address_list_t *OpenAPI_address_list_create(
    OpenAPI_list_t *ipv4_addrs,
    OpenAPI_list_t *ipv6_addrs
);
void OpenAPI_address_list_free(OpenAPI_address_list_t *address_list);
OpenAPI_address_list_t *OpenAPI_address_list_parseFromJSON(cJSON *address_listJSON);
cJSON *OpenAPI_address_list_convertToJSON(OpenAPI_address_list_t *address_list);
OpenAPI_address_list_t *OpenAPI_address_list_copy(OpenAPI_address_list_t *dst, OpenAPI_address_list_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_address_list_H_ */

