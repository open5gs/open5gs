/*
 * eth_flow_info.h
 *
 * Represents Ethernet flow information.
 */

#ifndef _OpenAPI_eth_flow_info_H_
#define _OpenAPI_eth_flow_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_eth_flow_info_s OpenAPI_eth_flow_info_t;
#include "eth_flow_description_1.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_eth_flow_info_s {
    int flow_id;
    OpenAPI_list_t *eth_flow_descriptions;
};

OpenAPI_eth_flow_info_t *OpenAPI_eth_flow_info_create(
    int flow_id,
    OpenAPI_list_t *eth_flow_descriptions
);
void OpenAPI_eth_flow_info_free(OpenAPI_eth_flow_info_t *eth_flow_info);
OpenAPI_eth_flow_info_t *OpenAPI_eth_flow_info_parseFromJSON(cJSON *eth_flow_infoJSON);
cJSON *OpenAPI_eth_flow_info_convertToJSON(OpenAPI_eth_flow_info_t *eth_flow_info);
OpenAPI_eth_flow_info_t *OpenAPI_eth_flow_info_copy(OpenAPI_eth_flow_info_t *dst, OpenAPI_eth_flow_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_eth_flow_info_H_ */

