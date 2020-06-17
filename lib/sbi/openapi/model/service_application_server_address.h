/*
 * service_application_server_address.h
 *
 *
 */

#ifndef _OpenAPI_service_application_server_address_H_
#define _OpenAPI_service_application_server_address_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "plmn_id.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_service_application_server_address_s OpenAPI_service_application_server_address_t;
typedef struct OpenAPI_service_application_server_address_s {
    OpenAPI_list_t *ser_ids;
    char *fqdn;
    OpenAPI_list_t *ipv4_addrs;
    OpenAPI_list_t *ipv6_addrs;
    OpenAPI_list_t *udp_port_numbers;
    OpenAPI_list_t *tcp_port_numbers;
    OpenAPI_list_t *plmns;
    char *geopraphical_area;
} OpenAPI_service_application_server_address_t;

OpenAPI_service_application_server_address_t *OpenAPI_service_application_server_address_create(
    OpenAPI_list_t *ser_ids,
    char *fqdn,
    OpenAPI_list_t *ipv4_addrs,
    OpenAPI_list_t *ipv6_addrs,
    OpenAPI_list_t *udp_port_numbers,
    OpenAPI_list_t *tcp_port_numbers,
    OpenAPI_list_t *plmns,
    char *geopraphical_area
    );
void OpenAPI_service_application_server_address_free(OpenAPI_service_application_server_address_t *service_application_server_address);
OpenAPI_service_application_server_address_t *OpenAPI_service_application_server_address_parseFromJSON(cJSON *service_application_server_addressJSON);
cJSON *OpenAPI_service_application_server_address_convertToJSON(OpenAPI_service_application_server_address_t *service_application_server_address);
OpenAPI_service_application_server_address_t *OpenAPI_service_application_server_address_copy(OpenAPI_service_application_server_address_t *dst, OpenAPI_service_application_server_address_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_service_application_server_address_H_ */

