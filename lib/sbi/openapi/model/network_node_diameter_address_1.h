/*
 * network_node_diameter_address_1.h
 *
 * 
 */

#ifndef _OpenAPI_network_node_diameter_address_1_H_
#define _OpenAPI_network_node_diameter_address_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_network_node_diameter_address_1_s OpenAPI_network_node_diameter_address_1_t;
typedef struct OpenAPI_network_node_diameter_address_1_s {
    char *name;
    char *realm;
} OpenAPI_network_node_diameter_address_1_t;

OpenAPI_network_node_diameter_address_1_t *OpenAPI_network_node_diameter_address_1_create(
    char *name,
    char *realm
);
void OpenAPI_network_node_diameter_address_1_free(OpenAPI_network_node_diameter_address_1_t *network_node_diameter_address_1);
OpenAPI_network_node_diameter_address_1_t *OpenAPI_network_node_diameter_address_1_parseFromJSON(cJSON *network_node_diameter_address_1JSON);
cJSON *OpenAPI_network_node_diameter_address_1_convertToJSON(OpenAPI_network_node_diameter_address_1_t *network_node_diameter_address_1);
OpenAPI_network_node_diameter_address_1_t *OpenAPI_network_node_diameter_address_1_copy(OpenAPI_network_node_diameter_address_1_t *dst, OpenAPI_network_node_diameter_address_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_network_node_diameter_address_1_H_ */

