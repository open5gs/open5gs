/*
 * network_node_diameter_address.h
 *
 * 
 */

#ifndef _OpenAPI_network_node_diameter_address_H_
#define _OpenAPI_network_node_diameter_address_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_network_node_diameter_address_s OpenAPI_network_node_diameter_address_t;
typedef struct OpenAPI_network_node_diameter_address_s {
    char *name;
    char *realm;
} OpenAPI_network_node_diameter_address_t;

OpenAPI_network_node_diameter_address_t *OpenAPI_network_node_diameter_address_create(
    char *name,
    char *realm
);
void OpenAPI_network_node_diameter_address_free(OpenAPI_network_node_diameter_address_t *network_node_diameter_address);
OpenAPI_network_node_diameter_address_t *OpenAPI_network_node_diameter_address_parseFromJSON(cJSON *network_node_diameter_addressJSON);
cJSON *OpenAPI_network_node_diameter_address_convertToJSON(OpenAPI_network_node_diameter_address_t *network_node_diameter_address);
OpenAPI_network_node_diameter_address_t *OpenAPI_network_node_diameter_address_copy(OpenAPI_network_node_diameter_address_t *dst, OpenAPI_network_node_diameter_address_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_network_node_diameter_address_H_ */

