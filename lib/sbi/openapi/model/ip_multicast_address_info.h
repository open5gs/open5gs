/*
 * ip_multicast_address_info.h
 *
 *
 */

#ifndef _OpenAPI_ip_multicast_address_info_H_
#define _OpenAPI_ip_multicast_address_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ip_multicast_address_info_s OpenAPI_ip_multicast_address_info_t;
typedef struct OpenAPI_ip_multicast_address_info_s {
    char *src_ipv4_addr;
    char *ipv4_mul_addr;
    char *src_ipv6_addr;
    char *ipv6_mul_addr;
} OpenAPI_ip_multicast_address_info_t;

OpenAPI_ip_multicast_address_info_t *OpenAPI_ip_multicast_address_info_create(
    char *src_ipv4_addr,
    char *ipv4_mul_addr,
    char *src_ipv6_addr,
    char *ipv6_mul_addr
    );
void OpenAPI_ip_multicast_address_info_free(OpenAPI_ip_multicast_address_info_t *ip_multicast_address_info);
OpenAPI_ip_multicast_address_info_t *OpenAPI_ip_multicast_address_info_parseFromJSON(cJSON *ip_multicast_address_infoJSON);
cJSON *OpenAPI_ip_multicast_address_info_convertToJSON(OpenAPI_ip_multicast_address_info_t *ip_multicast_address_info);
OpenAPI_ip_multicast_address_info_t *OpenAPI_ip_multicast_address_info_copy(OpenAPI_ip_multicast_address_info_t *dst, OpenAPI_ip_multicast_address_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ip_multicast_address_info_H_ */

