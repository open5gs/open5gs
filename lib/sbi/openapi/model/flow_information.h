/*
 * flow_information.h
 *
 *
 */

#ifndef _OpenAPI_flow_information_H_
#define _OpenAPI_flow_information_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "eth_flow_description.h"
#include "flow_direction_rm.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_flow_information_s OpenAPI_flow_information_t;
typedef struct OpenAPI_flow_information_s {
    char *flow_description;
    struct OpenAPI_eth_flow_description_s *eth_flow_description;
    char *pack_filt_id;
    int packet_filter_usage;
    char *tos_traffic_class;
    char *spi;
    char *flow_label;
    struct OpenAPI_flow_direction_rm_s *flow_direction;
} OpenAPI_flow_information_t;

OpenAPI_flow_information_t *OpenAPI_flow_information_create(
    char *flow_description,
    OpenAPI_eth_flow_description_t *eth_flow_description,
    char *pack_filt_id,
    int packet_filter_usage,
    char *tos_traffic_class,
    char *spi,
    char *flow_label,
    OpenAPI_flow_direction_rm_t *flow_direction
    );
void OpenAPI_flow_information_free(OpenAPI_flow_information_t *flow_information);
OpenAPI_flow_information_t *OpenAPI_flow_information_parseFromJSON(cJSON *flow_informationJSON);
cJSON *OpenAPI_flow_information_convertToJSON(OpenAPI_flow_information_t *flow_information);
OpenAPI_flow_information_t *OpenAPI_flow_information_copy(OpenAPI_flow_information_t *dst, OpenAPI_flow_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_flow_information_H_ */

