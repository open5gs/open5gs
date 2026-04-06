/*
 * eth_flow_description_1.h
 *
 * Identifies an Ethernet flow.
 */

#ifndef _OpenAPI_eth_flow_description_1_H_
#define _OpenAPI_eth_flow_description_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_eth_flow_description_1_s OpenAPI_eth_flow_description_1_t;
#include "flow_direction.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_eth_flow_description_1_s {
    char *dest_mac_addr;
    char *eth_type;
    char *f_desc;
    OpenAPI_flow_direction_e f_dir;
    char *source_mac_addr;
    OpenAPI_list_t *vlan_tags;
    char *src_mac_addr_end;
    char *dest_mac_addr_end;
};

OpenAPI_eth_flow_description_1_t *OpenAPI_eth_flow_description_1_create(
    char *dest_mac_addr,
    char *eth_type,
    char *f_desc,
    OpenAPI_flow_direction_e f_dir,
    char *source_mac_addr,
    OpenAPI_list_t *vlan_tags,
    char *src_mac_addr_end,
    char *dest_mac_addr_end
);
void OpenAPI_eth_flow_description_1_free(OpenAPI_eth_flow_description_1_t *eth_flow_description_1);
OpenAPI_eth_flow_description_1_t *OpenAPI_eth_flow_description_1_parseFromJSON(cJSON *eth_flow_description_1JSON);
cJSON *OpenAPI_eth_flow_description_1_convertToJSON(OpenAPI_eth_flow_description_1_t *eth_flow_description_1);
OpenAPI_eth_flow_description_1_t *OpenAPI_eth_flow_description_1_copy(OpenAPI_eth_flow_description_1_t *dst, OpenAPI_eth_flow_description_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_eth_flow_description_1_H_ */

