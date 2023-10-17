/*
 * server_addressing_info.h
 *
 * Contains addressing information (IP addresses and/or FQDNs) of a server.
 */

#ifndef _OpenAPI_server_addressing_info_H_
#define _OpenAPI_server_addressing_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_server_addressing_info_s OpenAPI_server_addressing_info_t;
typedef struct OpenAPI_server_addressing_info_s {
    OpenAPI_list_t *ipv4_addresses;
    OpenAPI_list_t *ipv6_addresses;
    OpenAPI_list_t *fqdn_list;
} OpenAPI_server_addressing_info_t;

OpenAPI_server_addressing_info_t *OpenAPI_server_addressing_info_create(
    OpenAPI_list_t *ipv4_addresses,
    OpenAPI_list_t *ipv6_addresses,
    OpenAPI_list_t *fqdn_list
);
void OpenAPI_server_addressing_info_free(OpenAPI_server_addressing_info_t *server_addressing_info);
OpenAPI_server_addressing_info_t *OpenAPI_server_addressing_info_parseFromJSON(cJSON *server_addressing_infoJSON);
cJSON *OpenAPI_server_addressing_info_convertToJSON(OpenAPI_server_addressing_info_t *server_addressing_info);
OpenAPI_server_addressing_info_t *OpenAPI_server_addressing_info_copy(OpenAPI_server_addressing_info_t *dst, OpenAPI_server_addressing_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_server_addressing_info_H_ */

