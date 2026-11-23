/*
 * serving_node_address.h
 *
 * 
 */

#ifndef _OpenAPI_serving_node_address_H_
#define _OpenAPI_serving_node_address_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_serving_node_address_s OpenAPI_serving_node_address_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_serving_node_address_s {
    char *ipv4_addr;
    char *ipv6_addr;
};

OpenAPI_serving_node_address_t *OpenAPI_serving_node_address_create(
    char *ipv4_addr,
    char *ipv6_addr
);
void OpenAPI_serving_node_address_free(OpenAPI_serving_node_address_t *serving_node_address);
OpenAPI_serving_node_address_t *OpenAPI_serving_node_address_parseFromJSON(cJSON *serving_node_addressJSON);
cJSON *OpenAPI_serving_node_address_convertToJSON(OpenAPI_serving_node_address_t *serving_node_address);
OpenAPI_serving_node_address_t *OpenAPI_serving_node_address_copy(OpenAPI_serving_node_address_t *dst, OpenAPI_serving_node_address_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_serving_node_address_H_ */

