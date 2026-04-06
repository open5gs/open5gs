/*
 * interface_upf_info_item.h
 *
 * Information of a given IP interface of an UPF
 */

#ifndef _OpenAPI_interface_upf_info_item_H_
#define _OpenAPI_interface_upf_info_item_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_interface_upf_info_item_s OpenAPI_interface_upf_info_item_t;
#include "port_range.h"
#include "up_interface_type.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_interface_upf_info_item_s {
    OpenAPI_up_interface_type_e interface_type;
    OpenAPI_list_t *ipv4_endpoint_addresses;
    OpenAPI_list_t *ipv6_endpoint_addresses;
    char *endpoint_fqdn;
    char *network_instance;
    OpenAPI_list_t *port_range_list;
};

OpenAPI_interface_upf_info_item_t *OpenAPI_interface_upf_info_item_create(
    OpenAPI_up_interface_type_e interface_type,
    OpenAPI_list_t *ipv4_endpoint_addresses,
    OpenAPI_list_t *ipv6_endpoint_addresses,
    char *endpoint_fqdn,
    char *network_instance,
    OpenAPI_list_t *port_range_list
);
void OpenAPI_interface_upf_info_item_free(OpenAPI_interface_upf_info_item_t *interface_upf_info_item);
OpenAPI_interface_upf_info_item_t *OpenAPI_interface_upf_info_item_parseFromJSON(cJSON *interface_upf_info_itemJSON);
cJSON *OpenAPI_interface_upf_info_item_convertToJSON(OpenAPI_interface_upf_info_item_t *interface_upf_info_item);
OpenAPI_interface_upf_info_item_t *OpenAPI_interface_upf_info_item_copy(OpenAPI_interface_upf_info_item_t *dst, OpenAPI_interface_upf_info_item_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_interface_upf_info_item_H_ */

