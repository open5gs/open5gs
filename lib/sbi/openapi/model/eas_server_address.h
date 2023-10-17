/*
 * eas_server_address.h
 *
 * Represents the IP address and port of an EAS server.
 */

#ifndef _OpenAPI_eas_server_address_H_
#define _OpenAPI_eas_server_address_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ip_addr.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_eas_server_address_s OpenAPI_eas_server_address_t;
typedef struct OpenAPI_eas_server_address_s {
    struct OpenAPI_ip_addr_s *ip;
    int port;
} OpenAPI_eas_server_address_t;

OpenAPI_eas_server_address_t *OpenAPI_eas_server_address_create(
    OpenAPI_ip_addr_t *ip,
    int port
);
void OpenAPI_eas_server_address_free(OpenAPI_eas_server_address_t *eas_server_address);
OpenAPI_eas_server_address_t *OpenAPI_eas_server_address_parseFromJSON(cJSON *eas_server_addressJSON);
cJSON *OpenAPI_eas_server_address_convertToJSON(OpenAPI_eas_server_address_t *eas_server_address);
OpenAPI_eas_server_address_t *OpenAPI_eas_server_address_copy(OpenAPI_eas_server_address_t *dst, OpenAPI_eas_server_address_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_eas_server_address_H_ */

