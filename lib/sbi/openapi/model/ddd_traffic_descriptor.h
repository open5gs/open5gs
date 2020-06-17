/*
 * ddd_traffic_descriptor.h
 *
 *
 */

#ifndef _OpenAPI_ddd_traffic_descriptor_H_
#define _OpenAPI_ddd_traffic_descriptor_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ddd_traffic_descriptor_s OpenAPI_ddd_traffic_descriptor_t;
typedef struct OpenAPI_ddd_traffic_descriptor_s {
    char *ipv4_addr;
    char *ipv6_addr;
    int port_number;
    char *mac_addr;
} OpenAPI_ddd_traffic_descriptor_t;

OpenAPI_ddd_traffic_descriptor_t *OpenAPI_ddd_traffic_descriptor_create(
    char *ipv4_addr,
    char *ipv6_addr,
    int port_number,
    char *mac_addr
    );
void OpenAPI_ddd_traffic_descriptor_free(OpenAPI_ddd_traffic_descriptor_t *ddd_traffic_descriptor);
OpenAPI_ddd_traffic_descriptor_t *OpenAPI_ddd_traffic_descriptor_parseFromJSON(cJSON *ddd_traffic_descriptorJSON);
cJSON *OpenAPI_ddd_traffic_descriptor_convertToJSON(OpenAPI_ddd_traffic_descriptor_t *ddd_traffic_descriptor);
OpenAPI_ddd_traffic_descriptor_t *OpenAPI_ddd_traffic_descriptor_copy(OpenAPI_ddd_traffic_descriptor_t *dst, OpenAPI_ddd_traffic_descriptor_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ddd_traffic_descriptor_H_ */

