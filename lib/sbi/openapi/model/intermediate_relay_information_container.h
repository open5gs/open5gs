/*
 * intermediate_relay_information_container.h
 *
 * 
 */

#ifndef _OpenAPI_intermediate_relay_information_container_H_
#define _OpenAPI_intermediate_relay_information_container_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_intermediate_relay_information_container_s OpenAPI_intermediate_relay_information_container_t;
#include "ip_addr.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_intermediate_relay_information_container_s {
    struct OpenAPI_ip_addr_s *intermediate_relay_ip_address;
    char *prose_ueto_network_intermediate_relay_ueid;
};

OpenAPI_intermediate_relay_information_container_t *OpenAPI_intermediate_relay_information_container_create(
    OpenAPI_ip_addr_t *intermediate_relay_ip_address,
    char *prose_ueto_network_intermediate_relay_ueid
);
void OpenAPI_intermediate_relay_information_container_free(OpenAPI_intermediate_relay_information_container_t *intermediate_relay_information_container);
OpenAPI_intermediate_relay_information_container_t *OpenAPI_intermediate_relay_information_container_parseFromJSON(cJSON *intermediate_relay_information_containerJSON);
cJSON *OpenAPI_intermediate_relay_information_container_convertToJSON(OpenAPI_intermediate_relay_information_container_t *intermediate_relay_information_container);
OpenAPI_intermediate_relay_information_container_t *OpenAPI_intermediate_relay_information_container_copy(OpenAPI_intermediate_relay_information_container_t *dst, OpenAPI_intermediate_relay_information_container_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_intermediate_relay_information_container_H_ */

