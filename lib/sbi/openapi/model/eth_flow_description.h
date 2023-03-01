/*
 * eth_flow_description.h
 *
 * Identifies an Ethernet flow.
 */

#ifndef _OpenAPI_eth_flow_description_H_
#define _OpenAPI_eth_flow_description_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "flow_direction.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_eth_flow_description_s OpenAPI_eth_flow_description_t;
typedef struct OpenAPI_eth_flow_description_s {
    char *dest_mac_addr;
    char *eth_type;
    char *f_desc;
    OpenAPI_flow_direction_e f_dir;
    char *source_mac_addr;
    OpenAPI_list_t *vlan_tags;
    char *src_mac_addr_end;
    char *dest_mac_addr_end;
} OpenAPI_eth_flow_description_t;

OpenAPI_eth_flow_description_t *OpenAPI_eth_flow_description_create(
    char *dest_mac_addr,
    char *eth_type,
    char *f_desc,
    OpenAPI_flow_direction_e f_dir,
    char *source_mac_addr,
    OpenAPI_list_t *vlan_tags,
    char *src_mac_addr_end,
    char *dest_mac_addr_end
);
void OpenAPI_eth_flow_description_free(OpenAPI_eth_flow_description_t *eth_flow_description);
OpenAPI_eth_flow_description_t *OpenAPI_eth_flow_description_parseFromJSON(cJSON *eth_flow_descriptionJSON);
cJSON *OpenAPI_eth_flow_description_convertToJSON(OpenAPI_eth_flow_description_t *eth_flow_description);
OpenAPI_eth_flow_description_t *OpenAPI_eth_flow_description_copy(OpenAPI_eth_flow_description_t *dst, OpenAPI_eth_flow_description_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_eth_flow_description_H_ */

