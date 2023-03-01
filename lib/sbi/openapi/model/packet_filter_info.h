/*
 * packet_filter_info.h
 *
 * Contains the information from a single packet filter sent from the SMF to the PCF.
 */

#ifndef _OpenAPI_packet_filter_info_H_
#define _OpenAPI_packet_filter_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "flow_direction.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_packet_filter_info_s OpenAPI_packet_filter_info_t;
typedef struct OpenAPI_packet_filter_info_s {
    char *pack_filt_id;
    char *pack_filt_cont;
    char *tos_traffic_class;
    char *spi;
    char *flow_label;
    OpenAPI_flow_direction_e flow_direction;
} OpenAPI_packet_filter_info_t;

OpenAPI_packet_filter_info_t *OpenAPI_packet_filter_info_create(
    char *pack_filt_id,
    char *pack_filt_cont,
    char *tos_traffic_class,
    char *spi,
    char *flow_label,
    OpenAPI_flow_direction_e flow_direction
);
void OpenAPI_packet_filter_info_free(OpenAPI_packet_filter_info_t *packet_filter_info);
OpenAPI_packet_filter_info_t *OpenAPI_packet_filter_info_parseFromJSON(cJSON *packet_filter_infoJSON);
cJSON *OpenAPI_packet_filter_info_convertToJSON(OpenAPI_packet_filter_info_t *packet_filter_info);
OpenAPI_packet_filter_info_t *OpenAPI_packet_filter_info_copy(OpenAPI_packet_filter_info_t *dst, OpenAPI_packet_filter_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_packet_filter_info_H_ */

