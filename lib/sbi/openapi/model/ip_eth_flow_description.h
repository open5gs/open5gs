/*
 * ip_eth_flow_description.h
 *
 * Contains the description of an Uplink and/or Downlink Ethernet flow.
 */

#ifndef _OpenAPI_ip_eth_flow_description_H_
#define _OpenAPI_ip_eth_flow_description_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "eth_flow_description.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ip_eth_flow_description_s OpenAPI_ip_eth_flow_description_t;
typedef struct OpenAPI_ip_eth_flow_description_s {
    char *ip_traffic_filter;
    struct OpenAPI_eth_flow_description_s *eth_traffic_filter;
} OpenAPI_ip_eth_flow_description_t;

OpenAPI_ip_eth_flow_description_t *OpenAPI_ip_eth_flow_description_create(
    char *ip_traffic_filter,
    OpenAPI_eth_flow_description_t *eth_traffic_filter
);
void OpenAPI_ip_eth_flow_description_free(OpenAPI_ip_eth_flow_description_t *ip_eth_flow_description);
OpenAPI_ip_eth_flow_description_t *OpenAPI_ip_eth_flow_description_parseFromJSON(cJSON *ip_eth_flow_descriptionJSON);
cJSON *OpenAPI_ip_eth_flow_description_convertToJSON(OpenAPI_ip_eth_flow_description_t *ip_eth_flow_description);
OpenAPI_ip_eth_flow_description_t *OpenAPI_ip_eth_flow_description_copy(OpenAPI_ip_eth_flow_description_t *dst, OpenAPI_ip_eth_flow_description_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ip_eth_flow_description_H_ */

