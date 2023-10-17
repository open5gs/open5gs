/*
 * traffic_descriptor_components.h
 *
 * Traffic descriptor components for the requested URSP.
 */

#ifndef _OpenAPI_traffic_descriptor_components_H_
#define _OpenAPI_traffic_descriptor_components_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "app_descriptor_1.h"
#include "connection_capabilities.h"
#include "eth_flow_description.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_traffic_descriptor_components_s OpenAPI_traffic_descriptor_components_t;
typedef struct OpenAPI_traffic_descriptor_components_s {
    OpenAPI_list_t* app_descs;
    OpenAPI_list_t *flow_descs;
    OpenAPI_list_t *domain_descs;
    OpenAPI_list_t *eth_flow_descs;
    OpenAPI_list_t *dnns;
    OpenAPI_list_t *conn_caps;
} OpenAPI_traffic_descriptor_components_t;

OpenAPI_traffic_descriptor_components_t *OpenAPI_traffic_descriptor_components_create(
    OpenAPI_list_t* app_descs,
    OpenAPI_list_t *flow_descs,
    OpenAPI_list_t *domain_descs,
    OpenAPI_list_t *eth_flow_descs,
    OpenAPI_list_t *dnns,
    OpenAPI_list_t *conn_caps
);
void OpenAPI_traffic_descriptor_components_free(OpenAPI_traffic_descriptor_components_t *traffic_descriptor_components);
OpenAPI_traffic_descriptor_components_t *OpenAPI_traffic_descriptor_components_parseFromJSON(cJSON *traffic_descriptor_componentsJSON);
cJSON *OpenAPI_traffic_descriptor_components_convertToJSON(OpenAPI_traffic_descriptor_components_t *traffic_descriptor_components);
OpenAPI_traffic_descriptor_components_t *OpenAPI_traffic_descriptor_components_copy(OpenAPI_traffic_descriptor_components_t *dst, OpenAPI_traffic_descriptor_components_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_traffic_descriptor_components_H_ */

