/*
 * tsn_bridge_info.h
 *
 *
 */

#ifndef _OpenAPI_tsn_bridge_info_H_
#define _OpenAPI_tsn_bridge_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "tsn_port_identifier.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_tsn_bridge_info_s OpenAPI_tsn_bridge_info_t;
typedef struct OpenAPI_tsn_bridge_info_s {
    char *bridge_name;
    char *bridge_mac;
    OpenAPI_list_t *nwtt_ports;
    struct OpenAPI_tsn_port_identifier_s *dstt_port;
    int dstt_resid_time;
} OpenAPI_tsn_bridge_info_t;

OpenAPI_tsn_bridge_info_t *OpenAPI_tsn_bridge_info_create(
    char *bridge_name,
    char *bridge_mac,
    OpenAPI_list_t *nwtt_ports,
    OpenAPI_tsn_port_identifier_t *dstt_port,
    int dstt_resid_time
    );
void OpenAPI_tsn_bridge_info_free(OpenAPI_tsn_bridge_info_t *tsn_bridge_info);
OpenAPI_tsn_bridge_info_t *OpenAPI_tsn_bridge_info_parseFromJSON(cJSON *tsn_bridge_infoJSON);
cJSON *OpenAPI_tsn_bridge_info_convertToJSON(OpenAPI_tsn_bridge_info_t *tsn_bridge_info);
OpenAPI_tsn_bridge_info_t *OpenAPI_tsn_bridge_info_copy(OpenAPI_tsn_bridge_info_t *dst, OpenAPI_tsn_bridge_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_tsn_bridge_info_H_ */

