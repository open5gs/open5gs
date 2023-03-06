/*
 * an_gw_address.h
 *
 * Describes the address of the access network gateway control node.
 */

#ifndef _OpenAPI_an_gw_address_H_
#define _OpenAPI_an_gw_address_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_an_gw_address_s OpenAPI_an_gw_address_t;
typedef struct OpenAPI_an_gw_address_s {
    char *an_gw_ipv4_addr;
    char *an_gw_ipv6_addr;
} OpenAPI_an_gw_address_t;

OpenAPI_an_gw_address_t *OpenAPI_an_gw_address_create(
    char *an_gw_ipv4_addr,
    char *an_gw_ipv6_addr
);
void OpenAPI_an_gw_address_free(OpenAPI_an_gw_address_t *an_gw_address);
OpenAPI_an_gw_address_t *OpenAPI_an_gw_address_parseFromJSON(cJSON *an_gw_addressJSON);
cJSON *OpenAPI_an_gw_address_convertToJSON(OpenAPI_an_gw_address_t *an_gw_address);
OpenAPI_an_gw_address_t *OpenAPI_an_gw_address_copy(OpenAPI_an_gw_address_t *dst, OpenAPI_an_gw_address_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_an_gw_address_H_ */

