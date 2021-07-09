/*
 * route_information.h
 *
 * 
 */

#ifndef _OpenAPI_route_information_H_
#define _OpenAPI_route_information_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_route_information_s OpenAPI_route_information_t;
typedef struct OpenAPI_route_information_s {
    char *ipv4_addr;
    char *ipv6_addr;
    int port_number;
} OpenAPI_route_information_t;

OpenAPI_route_information_t *OpenAPI_route_information_create(
    char *ipv4_addr,
    char *ipv6_addr,
    int port_number
);
void OpenAPI_route_information_free(OpenAPI_route_information_t *route_information);
OpenAPI_route_information_t *OpenAPI_route_information_parseFromJSON(cJSON *route_informationJSON);
cJSON *OpenAPI_route_information_convertToJSON(OpenAPI_route_information_t *route_information);
OpenAPI_route_information_t *OpenAPI_route_information_copy(OpenAPI_route_information_t *dst, OpenAPI_route_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_route_information_H_ */

