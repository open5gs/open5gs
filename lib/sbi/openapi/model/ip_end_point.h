/*
 * ip_end_point.h
 *
 * IP addressing information of a given NFService; it consists on, e.g. IP address, TCP port, transport protocol... 
 */

#ifndef _OpenAPI_ip_end_point_H_
#define _OpenAPI_ip_end_point_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "transport_protocol.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ip_end_point_s OpenAPI_ip_end_point_t;
typedef struct OpenAPI_ip_end_point_s {
    char *ipv4_address;
    char *ipv6_address;
    OpenAPI_transport_protocol_e transport;
    bool is_port;
    int port;
} OpenAPI_ip_end_point_t;

OpenAPI_ip_end_point_t *OpenAPI_ip_end_point_create(
    char *ipv4_address,
    char *ipv6_address,
    OpenAPI_transport_protocol_e transport,
    bool is_port,
    int port
);
void OpenAPI_ip_end_point_free(OpenAPI_ip_end_point_t *ip_end_point);
OpenAPI_ip_end_point_t *OpenAPI_ip_end_point_parseFromJSON(cJSON *ip_end_pointJSON);
cJSON *OpenAPI_ip_end_point_convertToJSON(OpenAPI_ip_end_point_t *ip_end_point);
OpenAPI_ip_end_point_t *OpenAPI_ip_end_point_copy(OpenAPI_ip_end_point_t *dst, OpenAPI_ip_end_point_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ip_end_point_H_ */

