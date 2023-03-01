/*
 * ddd_traffic_descriptor_1.h
 *
 * Contains a Traffic Descriptor.
 */

#ifndef _OpenAPI_ddd_traffic_descriptor_1_H_
#define _OpenAPI_ddd_traffic_descriptor_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ddd_traffic_descriptor_1_s OpenAPI_ddd_traffic_descriptor_1_t;
typedef struct OpenAPI_ddd_traffic_descriptor_1_s {
    char *ipv4_addr;
    char *ipv6_addr;
    bool is_port_number;
    int port_number;
    char *mac_addr;
} OpenAPI_ddd_traffic_descriptor_1_t;

OpenAPI_ddd_traffic_descriptor_1_t *OpenAPI_ddd_traffic_descriptor_1_create(
    char *ipv4_addr,
    char *ipv6_addr,
    bool is_port_number,
    int port_number,
    char *mac_addr
);
void OpenAPI_ddd_traffic_descriptor_1_free(OpenAPI_ddd_traffic_descriptor_1_t *ddd_traffic_descriptor_1);
OpenAPI_ddd_traffic_descriptor_1_t *OpenAPI_ddd_traffic_descriptor_1_parseFromJSON(cJSON *ddd_traffic_descriptor_1JSON);
cJSON *OpenAPI_ddd_traffic_descriptor_1_convertToJSON(OpenAPI_ddd_traffic_descriptor_1_t *ddd_traffic_descriptor_1);
OpenAPI_ddd_traffic_descriptor_1_t *OpenAPI_ddd_traffic_descriptor_1_copy(OpenAPI_ddd_traffic_descriptor_1_t *dst, OpenAPI_ddd_traffic_descriptor_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ddd_traffic_descriptor_1_H_ */

