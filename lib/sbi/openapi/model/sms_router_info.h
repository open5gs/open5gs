/*
 * sms_router_info.h
 *
 * Addressing information of the SMS Router configured at the UDM
 */

#ifndef _OpenAPI_sms_router_info_H_
#define _OpenAPI_sms_router_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "network_node_diameter_address.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_sms_router_info_s OpenAPI_sms_router_info_t;
typedef struct OpenAPI_sms_router_info_s {
    char *nf_instance_id;
    struct OpenAPI_network_node_diameter_address_s *diameter_address;
    char *map_address;
    char *router_ipv4;
    char *router_ipv6;
    char *router_fqdn;
} OpenAPI_sms_router_info_t;

OpenAPI_sms_router_info_t *OpenAPI_sms_router_info_create(
    char *nf_instance_id,
    OpenAPI_network_node_diameter_address_t *diameter_address,
    char *map_address,
    char *router_ipv4,
    char *router_ipv6,
    char *router_fqdn
);
void OpenAPI_sms_router_info_free(OpenAPI_sms_router_info_t *sms_router_info);
OpenAPI_sms_router_info_t *OpenAPI_sms_router_info_parseFromJSON(cJSON *sms_router_infoJSON);
cJSON *OpenAPI_sms_router_info_convertToJSON(OpenAPI_sms_router_info_t *sms_router_info);
OpenAPI_sms_router_info_t *OpenAPI_sms_router_info_copy(OpenAPI_sms_router_info_t *dst, OpenAPI_sms_router_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sms_router_info_H_ */

