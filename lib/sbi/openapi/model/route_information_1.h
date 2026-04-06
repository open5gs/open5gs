/*
 * route_information_1.h
 *
 * At least one of the \&quot;ipv4Addr\&quot; attribute and the \&quot;ipv6Addr\&quot; attribute shall be included in the \&quot;RouteInformation\&quot; data type.  
 */

#ifndef _OpenAPI_route_information_1_H_
#define _OpenAPI_route_information_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_route_information_1_s OpenAPI_route_information_1_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_route_information_1_s {
    char *ipv4_addr;
    char *ipv6_addr;
    int port_number;
};

OpenAPI_route_information_1_t *OpenAPI_route_information_1_create(
    char *ipv4_addr,
    char *ipv6_addr,
    int port_number
);
void OpenAPI_route_information_1_free(OpenAPI_route_information_1_t *route_information_1);
OpenAPI_route_information_1_t *OpenAPI_route_information_1_parseFromJSON(cJSON *route_information_1JSON);
cJSON *OpenAPI_route_information_1_convertToJSON(OpenAPI_route_information_1_t *route_information_1);
OpenAPI_route_information_1_t *OpenAPI_route_information_1_copy(OpenAPI_route_information_1_t *dst, OpenAPI_route_information_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_route_information_1_H_ */

