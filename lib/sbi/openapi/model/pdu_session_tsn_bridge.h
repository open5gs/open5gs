/*
 * pdu_session_tsn_bridge.h
 *
 * Contains the new 5GS Bridge information and may contain the DS-TT port and/or NW-TT port management information.
 */

#ifndef _OpenAPI_pdu_session_tsn_bridge_H_
#define _OpenAPI_pdu_session_tsn_bridge_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "bridge_management_container.h"
#include "port_management_container.h"
#include "tsn_bridge_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pdu_session_tsn_bridge_s OpenAPI_pdu_session_tsn_bridge_t;
typedef struct OpenAPI_pdu_session_tsn_bridge_s {
    struct OpenAPI_tsn_bridge_info_s *tsn_bridge_info;
    struct OpenAPI_bridge_management_container_s *tsn_bridge_man_cont;
    struct OpenAPI_port_management_container_s *tsn_port_man_cont_dstt;
    OpenAPI_list_t *tsn_port_man_cont_nwtts;
} OpenAPI_pdu_session_tsn_bridge_t;

OpenAPI_pdu_session_tsn_bridge_t *OpenAPI_pdu_session_tsn_bridge_create(
    OpenAPI_tsn_bridge_info_t *tsn_bridge_info,
    OpenAPI_bridge_management_container_t *tsn_bridge_man_cont,
    OpenAPI_port_management_container_t *tsn_port_man_cont_dstt,
    OpenAPI_list_t *tsn_port_man_cont_nwtts
);
void OpenAPI_pdu_session_tsn_bridge_free(OpenAPI_pdu_session_tsn_bridge_t *pdu_session_tsn_bridge);
OpenAPI_pdu_session_tsn_bridge_t *OpenAPI_pdu_session_tsn_bridge_parseFromJSON(cJSON *pdu_session_tsn_bridgeJSON);
cJSON *OpenAPI_pdu_session_tsn_bridge_convertToJSON(OpenAPI_pdu_session_tsn_bridge_t *pdu_session_tsn_bridge);
OpenAPI_pdu_session_tsn_bridge_t *OpenAPI_pdu_session_tsn_bridge_copy(OpenAPI_pdu_session_tsn_bridge_t *dst, OpenAPI_pdu_session_tsn_bridge_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pdu_session_tsn_bridge_H_ */

