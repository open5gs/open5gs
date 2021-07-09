/*
 * ip_address_1.h
 *
 * 
 */

#ifndef _OpenAPI_ip_address_1_H_
#define _OpenAPI_ip_address_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ip_address_1_s OpenAPI_ip_address_1_t;
typedef struct OpenAPI_ip_address_1_s {
    char *ipv4_addr;
    char *ipv6_addr;
    char *ipv6_prefix;
} OpenAPI_ip_address_1_t;

OpenAPI_ip_address_1_t *OpenAPI_ip_address_1_create(
    char *ipv4_addr,
    char *ipv6_addr,
    char *ipv6_prefix
);
void OpenAPI_ip_address_1_free(OpenAPI_ip_address_1_t *ip_address_1);
OpenAPI_ip_address_1_t *OpenAPI_ip_address_1_parseFromJSON(cJSON *ip_address_1JSON);
cJSON *OpenAPI_ip_address_1_convertToJSON(OpenAPI_ip_address_1_t *ip_address_1);
OpenAPI_ip_address_1_t *OpenAPI_ip_address_1_copy(OpenAPI_ip_address_1_t *dst, OpenAPI_ip_address_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ip_address_1_H_ */

